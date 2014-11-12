/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#include "EthernetPacketTransformation.h"
#include "ErrorCodes.h"
//-----------------------------------------------------------------------------
#include "optmacros.h"
#include "debug.h"

namespace pktanon
{

EthernetPacketTransformation::~EthernetPacketTransformation()
{
  delete anon_dest_mac;
  delete anon_source_mac;
  delete anon_ethertype;
};

EthernetPacketTransformation::EthernetPacketTransformation (pktanon::AnonPrimitive* anon_dest_mac, pktanon::AnonPrimitive* anon_source_mac, pktanon::AnonPrimitive* anon_ethertype, pktanon::EthernetPacketTransformation::PAD pad_mode) :
  anon_dest_mac (anon_dest_mac),
  anon_source_mac (anon_source_mac),
  anon_ethertype (anon_ethertype),
  pad_mode (pad_mode)
{}

  int EthernetPacketTransformation::transform ( const uint8_t* source_buffer, uint8_t* destination_buffer, unsigned int max_packet_length ) const noexcept
{
  const size_t ether_header_len = sizeof (ETHERNET_HEADER);
  if (unlikely (max_packet_length < ether_header_len))
  {
    return error_codes::ethernet_header_too_short;
  }
  
  ETHERNET_HEADER* input_header = (ETHERNET_HEADER*) source_buffer;
  ETHERNET_HEADER* output_header = (ETHERNET_HEADER*) destination_buffer;

  ntoh16 (input_header->type);
  const uint16_t ether_type = input_header->type;

  transform_field (anon_dest_mac,    &input_header->destination, &output_header->destination,    sizeof (MAC_ADDR));
  transform_field (anon_source_mac,  &input_header->source,  	&output_header->source,  	sizeof (MAC_ADDR));
  transform_field (anon_ethertype,   &input_header->type,	&output_header->type,        	sizeof (uint16_t));

  hton16 (output_header->type);

  const Transformation* next_transformation = Transformation::getTransformationEthertype (ether_type);

  ssize_t payload_len = next_transformation->transform (
                          source_buffer + ether_header_len,
                          destination_buffer + ether_header_len,
                          max_packet_length - ether_header_len);

  if (unlikely (payload_len < 0)) return ADD_LENGTH_VALUE (payload_len,  ether_header_len);

  if (pad_mode != PAD::NEVER)
  {
      
    // TODO? here we should pad to the minimum 46 length, but if the original
    // packet is smaller than 46, than padding can create overflow in the
    // destination_buffer.
    if (payload_len < 46 && ( pad_mode == PAD::ALWAYS || (max_packet_length >= ether_header_len + 46)))
    {
      payload_len = 46;
    }
  }

  return ether_header_len + payload_len;
}

}
