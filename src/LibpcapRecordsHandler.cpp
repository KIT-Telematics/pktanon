/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#include "LibpcapRecordsHandler.h"
#include "includes.h"
#include "LibpcapDumpOutput.h"
#include "LibpcapInjectOutput.h"
#include "RuntimeFactory.h"
#include "OutputSource.h"
#include "Utils.h"

#include <string.h>


using namespace pktanon;

pcap_pkt::pcap_pkt ( unsigned int snaplen ) :
  snaplen ( snaplen )
{
  packet = new u_char[snaplen];
  memset ( packet, '\0', snaplen );
}

//--------------------------------------------------------------------------

LibpcapRecordsHandler::~LibpcapRecordsHandler()
{
  pcap_close ( input_handle );
}

LibpcapRecordsHandler::LibpcapRecordsHandler ( Stats& stats ) :
  RecordsHandler ( stats ), input_handle ( nullptr ), libpcap_output_source ( nullptr )
{
  int timeout_ms = -1;
  char errbuf[PCAP_ERRBUF_SIZE];
  unsigned snaplen;

  if ( runtime_config.getInputType() == InputType::LIBPCAP_LIVE )
  {

    //
    // open interface for capturing
    //

    const InputNetworkConfig* cfg = runtime_config.getInputConfig<InputNetworkConfig>();
    snaplen = cfg->snaplen;
    int promisc = cfg->promisc ? 1 : 0;

    input_handle = pcap_open_live ( cfg->interface.c_str(), snaplen , promisc, timeout_ms, errbuf );
    if ( input_handle == nullptr )
    {
      throw std::runtime_error ( std::string ( "failed to open interface: " ) + errbuf );
    }

    _plg_info ( "libpcap: capturing on interface " <<   cfg->interface );

    // compile filter
    if ( cfg->pcap_filter.length() > 0 )
    {
      const char* filter_string = cfg->pcap_filter.c_str();

      struct bpf_program bpfilter;
      if ( pcap_compile ( input_handle, &bpfilter, filter_string, 1,  PCAP_NETMASK_UNKNOWN ) < 0 )
      {
        throw std::runtime_error ( "error compiling bpf filter" );
      }

      if ( pcap_setfilter ( input_handle , &bpfilter ) < 0 )
      {
        throw std::runtime_error ( "error setting bpf filter" );
      }

      _plg_info ( "libpcap: set filter \"" << filter_string <<  "\" on interface" );
    }

  }
  else if ( runtime_config.getInputType() == InputType::LIBPCAP_DUMP || runtime_config.getInputType() == InputType::ISTREAM )
  {
    const InputFileConfig* cfg = runtime_config.getInputConfig<InputFileConfig>();
    snaplen = cfg->snaplen;

    if ( ( input_handle = pcap_open_offline ( cfg->file_name.c_str(), errbuf ) ) == nullptr )
    {
      throw std::runtime_error ( std::string ( "failed to open input file: " ) + errbuf );
    }
    _plg_info ( "libpcap: opened input file " <<   cfg->file_name );
  }
  else
  {
    throw std::logic_error ( std::string ( "this should never happen (" ) + __FILE__ + "," + std::to_string ( __LINE__ ) + ")" );
  }

  //
  // initialize this and pktanon
  //

  transformed_packet = new pcap_pkt ( snaplen );

  auto link_type = pcap_datalink ( input_handle );

  if ( !PktAnon::set_link_type ( link_type ) )
  {
    throw std::runtime_error ( "unsupported link type: " + std::to_string ( link_type ) );
  }

  //
  // create appropriate output source
  //
  
  switch ( runtime_config.getOutputType() )
  {
    case OutputType::LIBPCAP_DUMP:
    case OutputType::OSTREAM:
      libpcap_output_source = new LibpcapDumpOutput ( input_handle );
      break;

    case OutputType::LIBPCAP_INJECT:
      libpcap_output_source = new LibpcapInjectOutput ();
      break;

    default:
      output_source = RuntimeFactory::createOutputSource(); 
  };

}

void LibpcapRecordsHandler::read_packets()
{
//   int captured_packets;
//
//   do
//   {
//     captured_packets = pcap_loop ( input_handle, -1,  LibpcapCapture::pcap_receive_packet, ( u_char* ) this );
//     TRACEV ( captured_packets );
//   }
//   while ( captured_packets > 0 );

  int ret;
  struct pcap_pkthdr *pkt_header;
  const u_char *pkt_data;

  while ( ( ret = pcap_next_ex ( input_handle, &pkt_header, &pkt_data ) ) >= 0 )
  {
    if ( ret == 0 ) continue; // timeout

    memcpy ( &transformed_packet->pkt_header, pkt_header, sizeof ( struct pcap_pkthdr ) );
    transform_packet ( &transformed_packet->pkt_header, pkt_data, transformed_packet->packet, stats );

    int pkt_sent;

    if ( libpcap_output_source )
    {
      pkt_sent = libpcap_output_source->write_packet ( &transformed_packet->pkt_header, transformed_packet->packet );
    }
    else
    {
      PCAP_REC_HEADER header;
      header.tv_sec = transformed_packet->pkt_header.ts.tv_sec;
      header.tv_usec = transformed_packet->pkt_header.ts.tv_usec;
      header.incl_len = transformed_packet->pkt_header.caplen;
      header.orig_len = transformed_packet->pkt_header.caplen;
      pkt_sent = output_source->write_packet ( &header, transformed_packet->packet );
    }

    if ( pkt_sent < 0 )
    {
      TRACEV ( pkt_sent );
      break;
    }

    memset ( transformed_packet->packet, '\0', transformed_packet->snaplen );
  }

  if ( ret == -1 )
  {
    _plg_error ( "Error reading the packets: " << pcap_geterr ( input_handle ) );
  }

}


// void LibpcapCapture::pcap_receive_packet ( u_char* args, const struct pcap_pkthdr* pkt_header, const u_char* packet )
// {
//   LibpcapCapture* _this = ( LibpcapCapture* ) args;
//
//   memcpy ( &_this->transformed_packet->pkt_header, pkt_header, sizeof ( struct pcap_pkthdr ) );
//
//   transform_packet ( &_this->transformed_packet->pkt_header, packet, _this->transformed_packet->packet,  _this->stats );
//
//   int pkt_sent;
//
//   if ( _this->libpcap_output_source )
//   {
//     pkt_sent = _this->libpcap_output_source->write_packet ( &_this->transformed_packet->pkt_header, _this->transformed_packet->packet );
//   }
//   else
//   {
//     PCAP_REC_HEADER header;
//     header.tv_sec = _this->transformed_packet->pkt_header.ts.tv_sec;
//     header.tv_usec = _this->transformed_packet->pkt_header.ts.tv_usec;
//     header.incl_len = _this->transformed_packet->pkt_header.caplen;
//     header.orig_len = _this->transformed_packet->pkt_header.caplen;
//     pkt_sent = _this->output_source->write_packet ( &header, _this->transformed_packet->packet );
//   }
//
//   if ( pkt_sent < 0 )
//   {
//     TRACEV ( pkt_sent );
//     pcap_breakloop ( _this->input_handle );
//   }
//
//   memset ( _this->transformed_packet->packet, '\0', _this->transformed_packet->snaplen );
// }
//




