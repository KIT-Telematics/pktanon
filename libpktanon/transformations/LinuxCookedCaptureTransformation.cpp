/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#include "LinuxCookedCaptureTransformation.h"
#include "optmacros.h"
#include "transformations/ErrorCodes.h"
#include <ProtocolUtils.h>

using namespace pktanon;

LinuxCookedCaptureTransformation::LinuxCookedCaptureTransformation(
  AnonPrimitive* anon_packet_type, AnonPrimitive* anon_hardware_type,
  AnonPrimitive* anon_link_layer_addr, AnonPrimitive* anon_protocol_type
) :
  anon_packet_type(anon_packet_type), anon_hardware_type(anon_hardware_type),
  anon_link_layer_addr(anon_link_layer_addr),  anon_protocol_type(anon_protocol_type)
{ }

int LinuxCookedCaptureTransformation::transform( const uint8_t* source_buffer, uint8_t* destination_buffer, unsigned int max_packet_length ) const noexcept
{
  const size_t linux_sll_header_len = sizeof (LINUX_SLL_HEADER);
  if (unlikely (max_packet_length < linux_sll_header_len))
  {
    return error_codes::linux_llc_header_too_short;
  }

  LINUX_SLL_HEADER* input_header = (LINUX_SLL_HEADER*) source_buffer;
  LINUX_SLL_HEADER* output_header = (LINUX_SLL_HEADER*) destination_buffer;

  // network->host byteorder

  ntoh16(input_header->packet_type);
  ntoh16(input_header->hardware_type);
  ntoh16(input_header->link_layer_addr_len);
  ntoh16(input_header->protocol_type);

  const uint16_t ether_type = input_header->protocol_type;

  // anonymize packet

  transform_field (anon_packet_type, &input_header->packet_type,	&output_header->packet_type,       	sizeof (uint16_t));
  transform_field (anon_hardware_type,&input_header->hardware_type,	&output_header->hardware_type,       	sizeof (uint16_t));

  output_header->link_layer_addr_len = transform_field (anon_link_layer_addr,&input_header->link_layer_addr,&output_header->link_layer_addr, std::min((uint16_t)8,  input_header->link_layer_addr_len));

  transform_field (anon_protocol_type,  &input_header->protocol_type,	&output_header->protocol_type,       	sizeof (uint16_t));

  // host->network byteorder

  hton16(output_header->packet_type);
  hton16(output_header->hardware_type);
  hton16(output_header->link_layer_addr_len);
  hton16(output_header->protocol_type);

  // call next transformation

  const Transformation* next_transformation = Transformation::getTransformationEthertype (ether_type);

  ssize_t payload_len = next_transformation->transform (
                          source_buffer + linux_sll_header_len,
                          destination_buffer + linux_sll_header_len,
                          max_packet_length - linux_sll_header_len);
                          
  if (unlikely (payload_len < 0)) return ADD_LENGTH_VALUE (payload_len,  linux_sll_header_len);

  return linux_sll_header_len + payload_len;

}
