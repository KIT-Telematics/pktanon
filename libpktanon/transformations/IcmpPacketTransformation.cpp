/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#include "IcmpPacketTransformation.h"
#include <Checksum.h>

using namespace pktanon;

IcmpPacketTransformation::~IcmpPacketTransformation()
{
  delete anon_type;
  delete anon_code;
  delete anon_misc;
}

IcmpPacketTransformation::IcmpPacketTransformation(
  AnonPrimitive* anon_type,
  AnonPrimitive* anon_code,
  AnonPrimitive* anon_misc
) :
  anon_type(anon_type),  anon_code(anon_code),
  anon_misc(anon_misc)
{ }

int IcmpPacketTransformation::transform( const uint8_t* source_buffer, uint8_t* destination_buffer, unsigned int max_packet_length ) const noexcept 
{
  ICMP_HEADER* input_header  = (ICMP_HEADER*) source_buffer;
  ICMP_HEADER* output_header = (ICMP_HEADER*) destination_buffer;

  ntoh32(input_header->misc);

  transform_field(anon_type,&input_header->type,&output_header->type,sizeof(uint8_t));
  transform_field(anon_code,&input_header->code,&output_header->code,sizeof(uint8_t));
  transform_field(anon_misc,&input_header->misc,&output_header->misc,sizeof(uint32_t));

  hton32(output_header->misc);

  Checksum checksum;
  checksum.update(output_header,  sizeof(ICMP_HEADER));
  output_header->checksum = checksum.final();

//   hton16(output_header->checksum);

  return sizeof(ICMP_HEADER);
}
