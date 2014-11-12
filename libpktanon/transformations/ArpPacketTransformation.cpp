/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#include "transformations/ArpPacketTransformation.h"
#include "ErrorCodes.h"
#include "debug.h"

namespace pktanon
{

ArpPacketTransformation::~ArpPacketTransformation()
{
  delete anon_hardware_type;
  delete anon_protocol_type;
  delete anon_hardware_size;
  delete anon_protocol_size;
  delete anon_opcode;
  delete anon_sender_mac;
  delete anon_sender_ip;
  delete anon_target_mac;
  delete anon_target_ip;
}

ArpPacketTransformation::ArpPacketTransformation(
  AnonPrimitive* anon_hardware_type, AnonPrimitive* anon_protocol_type,
  AnonPrimitive* anon_hardware_size, AnonPrimitive* anon_protocol_size,
  AnonPrimitive* anon_opcode,
  AnonPrimitive* anon_sender_mac, AnonPrimitive* anon_sender_ip,
  AnonPrimitive* anon_target_mac, AnonPrimitive* anon_target_ip):
  anon_hardware_type(anon_hardware_type),  anon_protocol_type(anon_protocol_type),
  anon_hardware_size(anon_hardware_size),  anon_protocol_size(anon_protocol_size),
  anon_opcode(anon_opcode),
  anon_sender_mac(anon_sender_mac),  anon_sender_ip(anon_sender_ip),
  anon_target_mac(anon_target_mac),  anon_target_ip(anon_target_ip)
{ }

int ArpPacketTransformation::transform( const uint8_t* source_buffer, uint8_t* destination_buffer, unsigned int max_packet_length ) const noexcept
{
  if (max_packet_length < sizeof(ARP_HEADER))
  {
    HB();
    return error_codes::arp_packet_too_short;
  };

  // TODO check hardware_type && hardware_size?

  ARP_HEADER* input_header  = (ARP_HEADER*) source_buffer;
  ARP_HEADER* output_header = (ARP_HEADER*) destination_buffer;

  transform_field(anon_hardware_type,	&input_header->hardware_type,	&output_header->hardware_type,	sizeof(uint16_t));
  transform_field(anon_protocol_type,	&input_header->protocol_type,	&output_header->protocol_type,	sizeof(uint16_t));
  transform_field(anon_hardware_size,	&input_header->hardware_size,	&output_header->hardware_size,	sizeof(uint8_t));
  transform_field(anon_protocol_size,	&input_header->protocol_size,	&output_header->protocol_size,	sizeof(uint8_t));
  transform_field(anon_opcode,		&input_header->opcode,		&output_header->opcode,		sizeof(uint16_t));
  transform_field(anon_sender_mac,	&input_header->sender_mac,	&output_header->sender_mac,	sizeof(MAC_ADDR));
  transform_field(anon_sender_ip, 	&input_header->sender_ip,	&output_header->sender_ip,	sizeof(in_addr));
  transform_field(anon_target_mac,	&input_header->target_mac,	&output_header->target_mac,	sizeof(MAC_ADDR));
  transform_field(anon_target_ip,	&input_header->target_ip,	&output_header->target_ip,	sizeof(in_addr));

  return sizeof(ARP_HEADER);
}

}
