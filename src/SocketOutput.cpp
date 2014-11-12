/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#include "SocketOutput.h"
#include "includes.h"
#include <PcapHeaders.h>

# include <sys/socket.h>
# include <arpa/inet.h>
# include <netinet/in.h>
# include <unistd.h>
# include <string.h>

namespace pktanon
{

union inet_addr_t
{
  in_addr ip4_addr;
  in6_addr ip6_addr;
};

SocketOutput::~SocketOutput()
{
  if ( socket_descriptor > 0 );
  close ( socket_descriptor );
}

SocketOutput::SocketOutput()  : socket_descriptor ( -1 )
{
  const OutputSocketConfig* cfg = runtime_config.getOutputConfig<OutputSocketConfig>();

  int sock_domain = cfg->use_ipv6 ? AF_INET6 : AF_INET;
  int sock_type = cfg->use_udp ? SOCK_DGRAM : SOCK_STREAM;
  inet_addr_t sock_addr;

  //  parse text address
  if ( inet_pton ( sock_domain, cfg->address.c_str(),  &sock_addr ) <= 0 )
  {
    throw std::runtime_error ( "failed to parse destinaton address. Is it valid?" );
  }

  // open socket
  socket_descriptor = ::socket ( sock_domain , sock_type, 0 );
  if ( socket_descriptor < 0 )
  {
    throw std::runtime_error ( "failed to open socket: " + std::to_string ( errno ) );
  }
  _plg_info ( "socket-output: opened socket " <<  socket_descriptor );

  //
  // connect to remote host
  //

  struct sockaddr_in6 s_addr;
  memset ( &s_addr, 0,  sizeof ( s_addr ) );
  socklen_t sockaddr_len;

  if ( sock_domain ==  AF_INET )
  {
    struct sockaddr_in* addr = ( struct sockaddr_in* ) &s_addr;
    addr->sin_family = sock_domain;
    addr->sin_addr = sock_addr.ip4_addr;
    addr->sin_port = htons ( cfg->port );
    sockaddr_len = sizeof ( struct sockaddr_in );
  }
  else
  {
    s_addr.sin6_family = sock_domain;
    memcpy ( &s_addr.sin6_addr,  &sock_addr.ip6_addr,  sizeof ( struct in6_addr ) );
    s_addr.sin6_port = htons ( cfg->port );
    sockaddr_len = sizeof ( struct sockaddr_in6 );
  }

  if ( connect ( socket_descriptor, ( struct sockaddr* ) &s_addr,sockaddr_len ) < 0 )
  {
    throw std::runtime_error ( "socket-output: failed to connect: " + std::to_string ( errno ) + ": " + strerror ( errno ) );
  }
  _plg_info ( "socket-output: connected" );

}

void SocketOutput::write_file_header ( PCAP_FILE_HEADER* file_header )
{
  // on tcp socket - send file header
  // on udp socket - don't send header since it is impossible to know whether it was delivered or not
  const OutputSocketConfig* cfg = runtime_config.getOutputConfig<OutputSocketConfig>();
  if ( !cfg->use_udp )
  {
    if ( ::send ( socket_descriptor, ( const void * ) file_header, sizeof ( PCAP_FILE_HEADER ), MSG_NOSIGNAL ) < 0 )
    {
      switch ( errno )
      {
        case ECONNRESET:
        case EPIPE:
        case ECONNREFUSED:
          throw std::runtime_error ( std::string ( "socket-output: unrecoverable error " ) + strerror ( errno ) + ",  exiting..." );
        case EMSGSIZE:
          _plg_error ( "socket-output:message too long" );
          break;
        default:
          _plg_error ( "socket-output: " <<  errno <<  " " <<   strerror ( errno ) );
      }
    }
  }

}

int SocketOutput::write_packet ( PCAP_REC_HEADER* record_header, uint8_t* transformed_packet_buffer )
{
  struct msghdr message;
  memset ( &message,  0,  sizeof ( msghdr ) );
 
  struct iovec iov[2];
  iov[0].iov_len = sizeof ( PCAP_REC_HEADER );

  message.msg_iov = ( iov );
  message.msg_iovlen = 2;

  iov[0].iov_base = record_header;
  iov[1].iov_base = transformed_packet_buffer;
  iov[1].iov_len =  record_header->incl_len;
  if ( sendmsg ( socket_descriptor,  &message,  MSG_NOSIGNAL ) < 0 )
  {
    switch ( errno )
    {
      case ECONNRESET:
      case EPIPE:
      case ECONNREFUSED:
        _plg_error ( "socket-output: unrecoverable error " <<  strerror ( errno ) <<  ",  exiting..." );
        return -1;
      case EMSGSIZE:
        _plg_error ( "socket-output:message too long" );
        break;
      default:
        _plg_error ( "socket-output: " <<  errno <<  " " <<   strerror ( errno ) );
    };
  }
  
  return 1;
}

}

