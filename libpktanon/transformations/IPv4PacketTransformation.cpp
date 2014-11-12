/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#include "IPv4PacketTransformation.h"
#include "PseudoheaderAwareTransformation.h"
#include "ErrorCodes.h"
//------------------------------------------------------------
#include "debug.h"
#include <Checksum.h>

using namespace pktanon;

IPv4PacketTransformation::~IPv4PacketTransformation()
{
  delete anon_tos;
  delete anon_identification;
  delete anon_flags;
  delete anon_fragment_offset;
  delete anon_ttl;
  delete anon_protocol;
  delete anon_source_ip;
  delete anon_destination_ip;
  delete anon_options;
}

IPv4PacketTransformation::IPv4PacketTransformation (
  AnonPrimitive* anon_tos,
  AnonPrimitive* anon_identification,
  AnonPrimitive* anon_flags,
  AnonPrimitive* anon_fragment_offset,
  AnonPrimitive* anon_ttl,
  AnonPrimitive* anon_protocol,
  AnonPrimitive* anon_source_ip,
  AnonPrimitive* anon_destination_ip,
  AnonPrimitive* anon_options,
  bool recalculate_ipv4_header_checksum,
  bool recalculate_payload_checksums
) :
  anon_tos (anon_tos),
  anon_identification (anon_identification),
  anon_flags (anon_flags),
  anon_fragment_offset (anon_fragment_offset),
  anon_ttl (anon_ttl),
  anon_protocol (anon_protocol),
  anon_source_ip (anon_source_ip),
  anon_destination_ip (anon_destination_ip),
  anon_options (anon_options),
  recalculate_ipv4_header_checksum(recalculate_ipv4_header_checksum),
  recalculate_payload_checksums(recalculate_payload_checksums)
{ }

/// convert fields to host byte order
static inline void prepare_input_header (IPv4_HEADER* input_header)
{
  Transformation::ntoh16 (input_header->total_length);
  Transformation::ntoh16 (input_header->identification);
  Transformation::ntoh16 (input_header->flags__fragment_offset);
  Transformation::ntoh16 (input_header->header_checksum);
}

/// convert fields back to network byte order and recalculate header_checksum
static inline void prepare_output_header (IPv4_HEADER* output_header,  bool recalculate_checksum = true)
{
  Transformation::hton16 (output_header->total_length);
  Transformation::hton16 (output_header->identification);
  Transformation::hton16 (output_header->flags__fragment_offset);

  // TODO set to random value on error?
  output_header->header_checksum = 0;
  if (recalculate_checksum)
  {
    // recalculate header checksum
    Checksum checksum;
    checksum.update (output_header, sizeof (IPv4_HEADER));
    output_header->header_checksum = checksum.final();
    Transformation::hton16(output_header->header_checksum);
    //   TRACE(std::hex <<  output_header->header_checksum);
  }

  Transformation::hton16 (output_header->header_checksum);
}

int IPv4PacketTransformation::transform ( const uint8_t* source_buffer, uint8_t* destination_buffer, unsigned int max_packet_length ) const noexcept
{
  size_t ip_header_length = sizeof (IPv4_HEADER);
  if (max_packet_length < ip_header_length)
  {
    return error_codes::ip4_header_too_short;
  }
  IPv4_HEADER* input_header  = (IPv4_HEADER*) source_buffer;
  IPv4_HEADER* output_header = (IPv4_HEADER*) destination_buffer;

  prepare_input_header (input_header);

  // save required fields
  size_t original_header_length = get_header_len (input_header);
  uint8_t transport_protocol = input_header->protocol;

  // transform packet
  set_version (4, output_header);
  transform_field (anon_tos,		&input_header->tos,		&output_header->tos,		sizeof (uint8_t));
  transform_field (anon_identification,	&input_header->identification,	&output_header->identification,	sizeof (uint16_t));

  uint16_t flags = get_flags (input_header);
  uint16_t flags_output = 0;
  transform_field (anon_flags,		&flags,				&flags_output,		sizeof (uint16_t));
//   TRACEV(std::hex << flags_output);
  set_flags (flags_output, output_header);
//   TRACEV(std::hex << get_flags (output_header));
//   TRACEV(std::hex << output_header->flags__fragment_offset);
//   TRACEV(std::hex << ((uint16_t) (flags & 0x07) << 13));

  transform_field (anon_ttl,		&input_header->ttl,		&output_header->ttl,		sizeof (uint8_t));
  transform_field (anon_protocol,	&input_header->protocol,	&output_header->protocol,	sizeof (uint8_t));
  transform_field (anon_source_ip,	&input_header->source_ip,	&output_header->source_ip,	sizeof (in_addr));
  transform_field (anon_destination_ip,	&input_header->destination_ip,	&output_header->destination_ip,	sizeof (in_addr));

  // if the header length field of packet has too small value, we can't go any further
  // TODO anonymize checksum ?!?
  if (original_header_length < ip_header_length)
  {
    set_header_len (original_header_length,  output_header);
    prepare_output_header (output_header,  false);
    return ADD_LENGTH_VALUE (error_codes::ip4_header_lenght_value_too_short,  ip_header_length);
  }

  // transform options and set header length
  size_t options_length = get_header_len (input_header) - ip_header_length;
  size_t new_options_length = transform_field (anon_options, source_buffer + ip_header_length, destination_buffer + ip_header_length,  options_length);
  uint16_t new_header_length = (uint16_t) (ip_header_length + new_options_length);
  set_header_len (new_header_length,  output_header);


  //TODO fragment offset
  // check if this is a fragment,  if this is a fragment -> stop transformation
  uint16_t fragment_offset = get_fragment_offset(input_header);
  set_fragment_offset(fragment_offset,  output_header);
  if (fragment_offset != 0)
  {
    prepare_output_header (output_header,  true);
    return ADD_LENGTH_VALUE(error_codes::ip4_packet_fragmented,  ip_header_length);
    // TODO call payload transformation?
  }

  size_t original_payload_length = input_header->total_length - original_header_length;

  // transform next layer
  const Transformation* next_transformation = Transformation::getTransformationProtocol (transport_protocol);
  ssize_t transformed_payload_length = next_transformation->transform (source_buffer + original_header_length,  destination_buffer + ip_header_length, original_payload_length);

  if (transformed_payload_length < 0)
  {
    output_header->total_length = (uint16_t (GET_LENGTH_VALUE (transformed_payload_length) + get_header_len (output_header)));
    prepare_output_header (output_header,  false);
    return ADD_LENGTH_VALUE (transformed_payload_length,  new_header_length);
  }

  // set payload length
  output_header->total_length = (uint16_t (transformed_payload_length + get_header_len (output_header)));

  // if the payload needs to calculate checksum with pseudoheader - feed ip information into it
  if (recalculate_payload_checksums && Transformation::hasPseudoHeader (transport_protocol))
  {
    const PseudoheaderAwareTransformation* p_trans = static_cast<const PseudoheaderAwareTransformation*> (next_transformation);
    p_trans->calculate_checksum (
      (uint8_t*) &output_header->source_ip, (uint8_t*) &output_header->destination_ip,  sizeof (in_addr),
      output_header->total_length - ip_header_length, destination_buffer + ip_header_length);
  }

  prepare_output_header (output_header, recalculate_ipv4_header_checksum);

  return new_header_length + transformed_payload_length;
}

