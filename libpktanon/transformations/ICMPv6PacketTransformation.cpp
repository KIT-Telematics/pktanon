/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#include "ICMPv6PacketTransformation.h"
#include <Checksum.h>

#include <debug.h>

using namespace pktanon;

ICMPv6PacketTransformation::~ICMPv6PacketTransformation()
{
  delete anon_type;
  delete anon_code;
  delete anon_misc;
  delete anon_target_address;
}

ICMPv6PacketTransformation::ICMPv6PacketTransformation (
  AnonPrimitive* anon_type, AnonPrimitive* anon_code, AnonPrimitive* anon_misc, AnonPrimitive* anon_target_address
) :
  anon_type (anon_type),  anon_code (anon_code), anon_misc (anon_misc),
  anon_target_address (anon_misc)
{ }

int ICMPv6PacketTransformation::transform ( const uint8_t* source_buffer, uint8_t* destination_buffer, unsigned int max_packet_length ) const noexcept
{
  if (max_packet_length < sizeof (ICMPv6_HEADER)) return -1;

  ICMPv6_HEADER* input_header = (ICMPv6_HEADER*) source_buffer;
  ICMPv6_HEADER* output_header = (ICMPv6_HEADER*) destination_buffer;

  uint8_t type = input_header->type;

  transform_field (anon_type, &input_header->type, &output_header->type, sizeof (uint8_t));
  transform_field (anon_code, &input_header->code, &output_header->code, sizeof (uint8_t));

  size_t message_length = 0;

  if (type == ICMPv6_Types::NDP_NSM)
  {
    message_length += nsm_transform (source_buffer + sizeof (uint32_t),  destination_buffer + sizeof (uint32_t),  max_packet_length - sizeof (uint32_t));
  }
  else if (type ==  ICMPv6_Types::NDP_NAM)
  {
    message_length +=  nam_transform (source_buffer + sizeof (uint32_t),  destination_buffer + sizeof (uint32_t),  max_packet_length - sizeof (uint32_t));
  }
  else
  {
    transform_field (anon_misc, &input_header->misc, &output_header->misc, sizeof (uint32_t));
    message_length = sizeof (uint32_t);
  }

  if (message_length < 0) return message_length;

//   Checksum checksum;
//   output_header->checksum = 0;
//   checksum.update(destination_buffer, sizeof(uint32_t) + message_length);
//   output_header->checksum = checksum.final();
//
//   hton16(output_header->checksum);

  return sizeof (ICMPv6_HEADER) + message_length;
}


int ICMPv6PacketTransformation::nam_transform ( const uint8_t* source_buffer, uint8_t* destination_buffer, unsigned int max_packet_length ) const
{
  if (max_packet_length < sizeof (ICMPv6_NAM_HEADER)) return -1;

  ICMPv6_NAM_HEADER* input_header = (ICMPv6_NAM_HEADER*) source_buffer;
  ICMPv6_NAM_HEADER* output_header = (ICMPv6_NAM_HEADER*) destination_buffer;

  // TODO what to do with RSO?
  output_header->rso__reserved = 0;
  transform_field (anon_target_address, &input_header->target_address, &output_header->target_address, sizeof (in6_addr));

  return sizeof (ICMPv6_NAM_HEADER);
}

int ICMPv6PacketTransformation::nsm_transform ( const uint8_t* source_buffer, uint8_t* destination_buffer, unsigned int max_packet_length ) const
{
  if (max_packet_length < sizeof (ICMPv6_NSM_HEADER)) return -1;

  ICMPv6_NSM_HEADER* input_header = (ICMPv6_NSM_HEADER*) source_buffer;
  ICMPv6_NSM_HEADER* output_header = (ICMPv6_NSM_HEADER*) destination_buffer;

  output_header->reserved = 0;
  transform_field (anon_target_address, &input_header->target_address, &output_header->target_address, sizeof (in6_addr));

  return sizeof (ICMPv6_NSM_HEADER);
}

int ICMPv6PacketTransformation::calculate_checksum ( uint8_t* source_address, uint8_t* destination_address, unsigned int address_length, uint16_t ip_length, uint8_t* sink_next_header ) const noexcept
{
  ICMPv6_HEADER* output_header = (ICMPv6_HEADER*) sink_next_header;

  // set checksum to 0
  output_header->checksum = 0;
  Checksum checksum;
  
  // IPv6 pseudoheader
  checksum.update (source_address, address_length);
  checksum.update (destination_address, address_length);
  checksum.update (&ip_length, sizeof (uint16_t));
  uint16_t padded_protocol = htons (IPPROTO_ICMPV6 & 0x00ff);
  checksum.update (&padded_protocol, sizeof (uint16_t));
  
  // icmp6 packet of length as in ipv6 header
  checksum.update(sink_next_header,  ip_length);

  // get final checksum
  output_header->checksum = checksum.final();
  hton16 (output_header->checksum);

  return 0;
}

