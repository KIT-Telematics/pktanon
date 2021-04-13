/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#include "RawSocketRecordsHandler.h"
#include "includes.h"
#include "RuntimeFactory.h"
#include "OutputSource.h"
#include "PcapUtils.h"
#include "PcapHeaders.h"
#include "Utils.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <linux/if_ether.h>
#include <netpacket/packet.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

namespace pktanon
{

RawSocketRecordsHandler::~RawSocketRecordsHandler()
{
  if ( socket_descriptor >= 0 )
    close ( socket_descriptor );

  delete output_source;
}

RawSocketRecordsHandler::RawSocketRecordsHandler ( Stats& stats ) :
  RecordsHandler ( stats ) ,
  socket_descriptor ( -1 ), output_source ( nullptr )
{
  const InputNetworkConfig* cfg = runtime_config.getInputConfig<InputNetworkConfig>();
  snaplen = cfg->snaplen;
  auto ifname = cfg->interface.c_str();

  // TODO support 'any' interface
  if ( strcmp ( ifname, "any" ) == 0 )
  {
    throw std::logic_error ( "raw sockets: capturing on 'any' interface is currently not supported" );
  }

  // TODO if interface is 'any' start capture in linux_sll
  socket_descriptor = ::socket ( AF_PACKET , SOCK_RAW , htons ( ETH_P_ALL ) );
  if ( socket_descriptor < 0 )
  {
    throw std::runtime_error ( "failed to open socket: (" + std::to_string ( errno ) + ") " + strerror ( errno ) );
  }

  _plg_verbose ( "socket-raw: opened socket " <<  socket_descriptor );

//   if ( strcmp ( ifname,  "any" ) !=  0 )
//   {
  auto interface_index = if_nametoindex ( ifname );
  if ( interface_index ==  0 )
  {
    throw std::runtime_error ( "unknown interface " + ( cfg->interface ) + " " + strerror ( errno ) );
  }

  struct ::sockaddr_ll bind_addr;
  memset ( &bind_addr,  '\0',  sizeof ( struct sockaddr_ll ) );

  bind_addr.sll_family = AF_PACKET;
  bind_addr.sll_ifindex = interface_index;
  bind_addr.sll_protocol = htons ( ETH_P_ALL );

  if ( bind ( socket_descriptor, ( struct ::sockaddr* ) &bind_addr,  sizeof ( struct sockaddr_ll ) ) < 0 )
  {
    throw std::runtime_error ( "cannot bind socket to interface " + ( cfg->interface ) + " " + strerror ( errno ) );
  }
  _plg_verbose ( "socket-raw: (not) bound socket to interface " << ( cfg->interface ) );

  //set device in the promiscuous mode
  // TODO if if_name == any set all ifs in promiscuous mode?
  if ( cfg->promisc )
  {
    struct ifreq ethreq;
    strcpy ( ethreq.ifr_name, ifname );
    ioctl ( socket_descriptor, SIOCGIFFLAGS, &ethreq );
    ethreq.ifr_flags |= IFF_PROMISC;
    ioctl ( socket_descriptor, SIOCSIFFLAGS, &ethreq );
    _plg_verbose ( "socket-raw: set interface into promiscuous mode" );
  }

  _plg_info ( "socket-raw: listening on " << cfg->interface );

  // TODO ethernet or linux_sll
  PktAnon::set_link_type ( LINKTYPE_ETHERNET );

  output_source = RuntimeFactory::createOutputSource();

}

void RawSocketRecordsHandler::read_packets()
{
  // create and write pcap file header
  PCAP_FILE_HEADER* pcap_file_header = create_pcap_file_header ( snaplen, LINKTYPE_ETHERNET );
  output_source->write_file_header ( pcap_file_header );

  //---------------------------------------------------------------------------------------------

  PCAP_REC_HEADER record_header;
  uint8_t* original_packet = ( uint8_t* ) alloca ( snaplen );
  uint8_t* transformed_packet = ( uint8_t* ) alloca ( snaplen );

  struct sockaddr_ll sa;
  socklen_t sa_len = sizeof ( struct sockaddr_ll );

  while ( 1 )
  {
    auto rcv_bytes_count = recvfrom ( socket_descriptor, original_packet, snaplen, MSG_TRUNC, ( struct ::sockaddr* ) &sa, &sa_len );
//     TRACEV ( rcvfrom_exit++ );
//     TRACEV ( sa.sll_ifindex );
//     TRACEV ( sa.sll_pkttype );

    if ( rcv_bytes_count < 0 )
    {
      if ( errno ==  EAGAIN  || errno ==  EWOULDBLOCK )
      {
        continue;
      }
      _plg_error ( "socket-raw: failed to receive data from socket" );
      return;
    }

    auto pkt_caplen = std::min ( ( std::size_t ) rcv_bytes_count,  snaplen );
    fill_pcap_record_header ( &record_header, rcv_bytes_count, pkt_caplen );

    transform_packet ( record_header, original_packet, transformed_packet, pkt_caplen, stats );

    if ( output_source->write_packet ( &record_header, transformed_packet ) < 0 )
    {
      return;
    }
  }
}


}
