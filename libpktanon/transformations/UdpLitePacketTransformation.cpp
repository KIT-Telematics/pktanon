/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#include "UdpLitePacketTransformation.h"

using namespace pktanon;

UdpLitePacketTransformation::~UdpLitePacketTransformation()
{
  delete anon_source_port;
  delete anon_destination_port;
  delete anon_checksum_coverage;
}

UdpLitePacketTransformation::UdpLitePacketTransformation(
  AnonPrimitive* anon_source_port,
  AnonPrimitive* anon_destination_port,
  AnonPrimitive* anon_checksum_coverage):
  anon_source_port(anon_source_port),
  anon_destination_port(anon_destination_port),
  anon_checksum_coverage(anon_checksum_coverage)
{ }

size_t UdpLitePacketTransformation::transform(uint8_t* source_buffer, uint8_t* destination_buffer) const
{
  UDPLITE_HEADER* input_header = (UDPLITE_HEADER*) source_buffer;
  UDPLITE_HEADER* output_header = (UDPLITE_HEADER*) destination_buffer;
  
  ntoh16(input_header->source_port);
  ntoh16(input_header->destination_port);
  ntoh16(input_header->checksum_coverage);

  transform_field(anon_source_port,	  &input_header->source_port,	    &output_header->source_port,  sizeof(uint16_t));
  transform_field(anon_destination_port,  &input_header->destination_port,  &output_header->destination_port,  sizeof(uint16_t));
  transform_field(anon_checksum_coverage,  &input_header->checksum_coverage,  &output_header->checksum_coverage,  sizeof(uint16_t));
  output_header->checksum = 0;

  auto payload_transformation = Transformation::getPayloadTransformation();
  size_t new_payload_length = payload_transformation->transform(source_buffer+sizeof(UDPLITE_HEADER), destination_buffer+sizeof(UDPLITE_HEADER));

  uint16_t new_packet_length = sizeof(UDPLITE_HEADER) + new_payload_length;
  //checksum coverage must be at least header
  output_header->checksum_coverage = std::max (output_header->checksum_coverage, sizeof(UDPLITE_HEADER));
  // checksum_coverage must be smaller than packet length
  output_header->checksum_coverage = std::min(output_header->checksum_coverage,  new_packet_length);

  hton16(output_header->source_port);
  hton16(output_header->destination_port);
  hton16(output_header->checksum_coverage);

  return sizeof(UDPLITE_HEADER) + new_payload_length;
}

