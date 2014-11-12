/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#include "TcpPacketTransformation.h"
#include "ErrorCodes.h"
#include "Checksum.h"
#include "debug.h"

using namespace pktanon;

TcpPacketTransformation::~TcpPacketTransformation()
{
  delete anon_source_port;
  delete anon_dest_port;
  delete anon_seq_num;
  delete anon_ack_num;
  delete anon_flags;
  delete anon_window_size;
  delete anon_urgent_pointer;
  delete anon_options;
}

TcpPacketTransformation::TcpPacketTransformation (
  AnonPrimitive* anon_source_port, AnonPrimitive* anon_dest_port,
  AnonPrimitive* anon_seq_num,
  AnonPrimitive* anon_ack_num,
  AnonPrimitive* anon_flags, AnonPrimitive* anon_window_size,
  AnonPrimitive* anon_urgent_pointer,
  AnonPrimitive* anon_options
) :
  anon_source_port (anon_source_port),  anon_dest_port (anon_dest_port),
  anon_seq_num (anon_seq_num),
  anon_ack_num (anon_ack_num),
  anon_flags (anon_flags),  anon_window_size (anon_window_size),
  anon_urgent_pointer (anon_urgent_pointer),
  anon_options (anon_options)
{ }

int TcpPacketTransformation::transform ( const uint8_t* source_buffer, uint8_t* destination_buffer, unsigned int max_packet_length ) const noexcept
{
  size_t tcp_header_length = sizeof (TCP_HEADER);
  if (max_packet_length < tcp_header_length) return error_codes::tcp_header_too_short;

  TCP_HEADER* input_header = (TCP_HEADER*) source_buffer;
  TCP_HEADER* output_header = (TCP_HEADER*) destination_buffer;


  ntoh16 (input_header->source_port);
  ntoh16 (input_header->dest_port);
  ntoh32 (input_header->seq_num);
  ntoh32 (input_header->ack_num);
  ntoh16 (input_header->header_len__flags);
  ntoh16 (input_header->window_size);
  ntoh16 (input_header->checksum);
  ntoh16 (input_header->urgent_pointer);

//   anon_source_port->anonimyze(&input_header->source_port, &output_header->source_port, sizeof(uint16_t));
  
  transform_field (anon_source_port,&input_header->source_port,&output_header->source_port,    sizeof (uint16_t));
  transform_field (anon_dest_port,  &input_header->dest_port,	&output_header->dest_port,    sizeof (uint16_t));
  transform_field (anon_seq_num,    &input_header->seq_num,	&output_header->seq_num,    sizeof (uint32_t));
  transform_field (anon_ack_num,    &input_header->ack_num,	&output_header->ack_num,    sizeof (uint32_t));

  uint16_t flags_original = get_flags (input_header);
  uint16_t flags_transformed = 0;
  transform_field (anon_flags,  &flags_original,  &flags_transformed,  sizeof (uint16_t));
  set_flags (flags_transformed, output_header);

  transform_field (anon_window_size,    &input_header->window_size,	&output_header->window_size,    sizeof (uint16_t));
  transform_field (anon_urgent_pointer, &input_header->urgent_pointer,	&output_header->urgent_pointer,    sizeof (uint16_t));

  size_t options_length = get_header_len (input_header) - tcp_header_length;
  size_t new_options_length = transform_field (anon_options, source_buffer + tcp_header_length, destination_buffer + tcp_header_length,  options_length);

  set_header_len (tcp_header_length + new_options_length,  output_header);

  // transform payload packet
  auto payload_transformation = Transformation::getPayloadTransformation();
  size_t new_payload_length = payload_transformation->transform (source_buffer + get_header_len(input_header), destination_buffer + get_header_len(output_header), max_packet_length - get_header_len(input_header) );

  hton16 (output_header->source_port);
  hton16 (output_header->dest_port);
  hton32 (output_header->seq_num);
  hton32 (output_header->ack_num);
  hton16 (output_header->header_len__flags);
  hton16 (output_header->window_size);
  hton16 (output_header->checksum);
  hton16 (output_header->urgent_pointer);

  return tcp_header_length + new_options_length + new_payload_length;
}

int TcpPacketTransformation::calculate_checksum ( uint8_t* source_address, uint8_t* destination_address, unsigned int address_length, uint16_t length, uint8_t* sink_next_header ) const noexcept
{
  TCP_HEADER* tcp_header = (TCP_HEADER*) sink_next_header;
  tcp_header->checksum = 1;

  size_t header_length = sizeof (TCP_HEADER);
  size_t payload_length = length - header_length;

//   TRACE(header_length);
//   TRACE(payload_length);
//   TRACE(length);

  hton16 (length);

  tcp_header->checksum = 0;
  uint16_t padded_protocol = htons (IPPROTO_TCP & 0x00ff);

  Checksum checksum;

  // source address
  checksum.update (source_address,  address_length);

  // destination address
  checksum.update (destination_address,  address_length);

  // zero,  protocol and  length
  checksum.update (&length,  2);
  checksum.update (&padded_protocol, 2);

  // add udp header
  checksum.update (tcp_header,  header_length);

  // add packet payload
  checksum.update (sink_next_header + header_length, payload_length);

  // complete
  tcp_header->checksum = checksum.final();
//   hton16 (tcp_header->checksum);

  return 0;
}

// TCP Checksum Calculation
// pseudoheader
// +--------+--------+--------+--------+
// |           Source Address          |
// +--------+--------+--------+--------+
// |         Destination Address       |
// +--------+--------+--------+--------+
// |  zero  |  PTCL  |    TCP Length   |
// +--------+--------+--------+--------+
// tcp header
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |          Source Port          |       Destination Port        |
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |                        Sequence Number                        |
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |                    Acknowledgment Number                      |
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |  Data |           |U|A|P|R|S|F|                               |
// | Offset| Reserved  |R|C|S|S|Y|I|            Window             |
// |       |           |G|K|H|T|N|N|                               |
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |           Checksum            |         Urgent Pointer        |
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |                    Options                    |    Padding    |
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |                             data                              |
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
