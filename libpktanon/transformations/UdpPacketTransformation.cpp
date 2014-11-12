/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#include "UdpPacketTransformation.h"
#include "ErrorCodes.h"
#include <Checksum.h>
#include <optmacros.h>
#include <debug.h>
#include <assert.h>
 
using namespace pktanon;

UdpPacketTransformation::~UdpPacketTransformation()
{
  delete anon_source_port;
  delete anon_destination_port;
}

UdpPacketTransformation::UdpPacketTransformation(AnonPrimitive* anon_source_port, AnonPrimitive* anon_destination_port):
  anon_source_port(anon_source_port),
  anon_destination_port(anon_destination_port)
{}

static inline void prepare_input_header(UDP_HEADER* input_header) 
{
  Transformation::ntoh16(input_header->source_port);
  Transformation::ntoh16(input_header->destination_port);
  Transformation::ntoh16(input_header->length);
  Transformation::ntoh16(input_header->checksum);
}

static inline void prepare_output_header(UDP_HEADER* output_header)
{
  UdpPacketTransformation::hton16(output_header->source_port);
  UdpPacketTransformation::hton16(output_header->destination_port);
  UdpPacketTransformation::hton16(output_header->length);
  output_header->checksum = 0;
}

int UdpPacketTransformation::transform( const uint8_t* source_buffer, uint8_t* destination_buffer, unsigned int max_packet_length ) const noexcept
{  
  const size_t udp_header_length = sizeof(UDP_HEADER);
  if(unlikely(max_packet_length - udp_header_length < 0)) return error_codes::udp_header_too_short;
  
  UDP_HEADER* input_header = (UDP_HEADER*) source_buffer;
  UDP_HEADER* output_header = (UDP_HEADER*) destination_buffer;
  
  prepare_input_header(input_header);
  
  //TODO what to do if max_packet_length < length?
  
  transform_field(anon_source_port,	  &input_header->source_port,	    &output_header->source_port,  sizeof(uint16_t));
  transform_field(anon_destination_port,  &input_header->destination_port,  &output_header->destination_port,  sizeof(uint16_t));

  // if input_header->length is bigger than ip payload length than packet is malicious 
  // don't anonymize further
  if (unlikely(input_header->length > max_packet_length))             //  length field is length of both header and data!
  {
    output_header->length = input_header->length;
    prepare_output_header(output_header);
    return ADD_LENGTH_VALUE(error_codes::udp_header_length_too_long,  udp_header_length);
  }
  
  const auto payload_transformation = Transformation::getPayloadTransformation();
  const size_t new_payload_length = payload_transformation->transform(source_buffer+udp_header_length, destination_buffer+udp_header_length, input_header->length);
  
  output_header->length = udp_header_length + new_payload_length;
  
  prepare_output_header(output_header);
  return udp_header_length + new_payload_length;
}



int UdpPacketTransformation::calculate_checksum( uint8_t* source_address, uint8_t* destination_address, unsigned int address_length, uint16_t ip_length, uint8_t* sink_next_header ) const noexcept
{
  UDP_HEADER* udp_header = (UDP_HEADER*) sink_next_header;
  size_t header_length = sizeof(UDP_HEADER);
  size_t udp_length = ntohs(udp_header->length);
  size_t payload_length = udp_length - header_length;
  
  assert(udp_header->checksum == 0);
  uint16_t padded_protocol = htons(IPPROTO_UDP & 0x00ff);
  
  Checksum checksum;
  
  // source address
  checksum.update(source_address,  address_length);
  
  // destination address
  checksum.update(destination_address,  address_length);
  
  // zero,  protocol and udp length
  checksum.update(&udp_header->length,  2);
  checksum.update(&padded_protocol, 2);
  
  // add udp header
  checksum.update(udp_header,  header_length);
  
  // add packet payload
  checksum.update(sink_next_header+header_length, payload_length);
    
  // complete
  udp_header->checksum = checksum.final();
//   hton16(udp_header->checksum);
  
  return 0;
}

// udp checksum for ipv4:
// pseudoheader:
// 0      7 8     15 16    23 24    31 
// +--------+--------+--------+--------+
// |          source address           |
// +--------+--------+--------+--------+
// |        destination address        |
// +--------+--------+--------+--------+
// |  zero  |protocol|   UDP length    |
// +--------+--------+--------+--------+
// udp packet
// +--------+--------+--------+--------+ 
// |     Source      |   Destination   | 
// |      Port       |      Port       | 
// +--------+--------+--------+--------+ 
// |                 |                 | 
// |     Length      |    Checksum     | 
// +--------+--------+--------+--------+ 
// |                                     
// |          data octets ...            
// +---------------- ...                 
