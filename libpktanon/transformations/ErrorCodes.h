/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 *
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence.
 * See LICENSE file found in the top-level directory of this distribution.
 */

# ifndef PKTANON__ERRORCODES_H
# define PKTANON__ERRORCODES_H

namespace pktanon
{

/**
 * error code structure: left-most bit '1' to indicate negative value, next 7 bits are packet number, next 8 bits
 * is error code within the packet, next 2 / 6 bytes is length of the transformed value.
 *
 * this is a preparation for dynamically adding error codes in TransformationConfigurator
 */

extern const char* const error_strings[][3];

enum error_codes : int
{
  ethernet_header_too_short = 0x8000 << sizeof ( int ) * 8 - 16,
  
  arp_packet_too_short = 0x8100 << sizeof ( int ) * 8 - 16,
  
  ip4_header_too_short = 0x8200 << sizeof ( int ) * 8 - 16,
  ip4_header_lenght_value_too_short = 0x8201 << sizeof ( int ) * 8 - 16,
  ip4_packet_fragmented = 0x8202 << sizeof ( int ) * 8 - 16,
  
  ip6_header_too_short = 0x8300 << sizeof ( int ) * 8 - 16,
  
  tcp_header_too_short = 0x8400 << sizeof ( int ) * 8 - 16,
  
  udp_header_too_short = 0x8500 << sizeof ( int ) * 8 - 16,
  udp_header_length_too_long = 0x8501 << sizeof ( int ) * 8 - 16,
  
  linux_llc_header_too_short = 0x8600 << sizeof ( int ) * 8 - 16,
};

inline long int GET_ERROR_CODE ( long int error_value )
{
  return error_value >> sizeof ( int ) * 8 - 16;
}

inline const char* GET_ERROR_STRING ( int error_value )
{
  auto pkt_error_code = error_value >> sizeof ( int ) * 8 - 16;
  auto pkt = ( pkt_error_code >> 8 ) & 0x6f;
  auto error_code = pkt_error_code & 0xff;
  return error_strings[pkt][error_code];
}

inline int GET_LENGTH_VALUE ( int error_value )
{
  return error_value & 0xffff;
}

inline int ADD_LENGTH_VALUE ( int error_value,  unsigned int length_value )
{
  // assume that length_value < max_short 
  return error_value + length_value;
}


}
# endif
