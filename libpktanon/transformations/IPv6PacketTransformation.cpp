/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 *
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence.
 * See LICENSE file found in the top-level directory of this distribution.
 */

#include "IPv6PacketTransformation.h"
#include "PseudoheaderAwareTransformation.h"
#include "ErrorCodes.h"

#include "optmacros.h"
#include "debug.h"

using namespace pktanon;

IPv6PacketTransformation::~IPv6PacketTransformation()
{
  delete anon_traffic_class;
  delete anon_flow_label;
  delete anon_hop_limit;
  delete anon_source_addr;
  delete anon_destination_addr;
}

IPv6PacketTransformation::IPv6PacketTransformation (
  AnonPrimitive* anon_traffic_class,
  AnonPrimitive* anon_flow_label,
  AnonPrimitive* anon_next_header,
  AnonPrimitive* anon_hop_limit,
  AnonPrimitive* anon_source_addr,
  AnonPrimitive* anon_destination_addr,
  bool recalculate_payload_checksums
) :
  anon_traffic_class ( anon_traffic_class ),
  anon_flow_label ( anon_flow_label ),
  anon_next_header ( anon_next_header ),
  anon_hop_limit ( anon_hop_limit ),
  anon_source_addr ( anon_source_addr ),
  anon_destination_addr ( anon_destination_addr ),
  recalculate_payload_checksums ( recalculate_payload_checksums )
{
}

int IPv6PacketTransformation::transform ( const uint8_t* source_buffer, uint8_t* destination_buffer, unsigned int max_packet_length ) const noexcept
{
  const size_t ipv6_header_length = sizeof ( IPv6_HEADER );
  if ( unlikely ( max_packet_length < ipv6_header_length ) ) return error_codes::ip6_header_too_short;

  IPv6_HEADER* input_header  = ( IPv6_HEADER* ) source_buffer;
  IPv6_HEADER* output_header = ( IPv6_HEADER* ) destination_buffer;

  const uint8_t next_header = input_header->next_header;

  ntoh32 ( input_header->version__traffic_class__flow_label );
  ntoh16 ( input_header->payload_length );

  //version is always 6
  set_version ( 6u,  output_header );

  //anonymize traffic class
  uint32_t input_traffic_class = get_traffic_class ( input_header );
  uint32_t output_traffic_class = 0;
  transform_field ( anon_traffic_class, &input_traffic_class,  &output_traffic_class,  sizeof ( uint32_t ) );
  set_traffic_class ( output_traffic_class,  output_header );

  //anonymize flow label
  uint32_t input_flow_label = get_flow_label ( input_header );
  uint32_t output_flow_label = 0;
  transform_field ( anon_flow_label, &input_flow_label,  &output_flow_label,  sizeof ( uint32_t ) );
  set_flow_label ( output_flow_label, output_header );

  //payload_length is recalculated below

  //rest of the fields
  transform_field ( anon_next_header,    &input_header->next_header,     &output_header->next_header,    sizeof ( uint8_t ) );
  transform_field ( anon_hop_limit,    &input_header->hop_limit,     &output_header->hop_limit,    sizeof ( uint8_t ) );
  transform_field ( anon_source_addr,    &input_header->source_addr,     &output_header->source_addr,    sizeof ( in6_addr ) );
  transform_field ( anon_destination_addr, &input_header->destination_addr,  &output_header->destination_addr, sizeof ( in6_addr ) );

  // payload
  size_t payload_length;
  const Transformation* next_transformation;
  next_transformation = Transformation::getTransformationProtocol ( next_header );
  payload_length = next_transformation->transform ( source_buffer + ipv6_header_length,  destination_buffer + ipv6_header_length, input_header->payload_length );

  if ( payload_length < 0 )
  {
    output_header->payload_length = GET_LENGTH_VALUE ( payload_length );
    hton32 ( output_header->version__traffic_class__flow_label );
    hton16 ( output_header->payload_length );
    return ADD_LENGTH_VALUE ( payload_length,  ipv6_header_length );
  };

  output_header->payload_length = payload_length;


  // if the payload needs to calculate checksum with pseudoheader - feed ip information into it
  if ( recalculate_payload_checksums && Transformation::hasPseudoHeader ( next_header ) )
  {
    const PseudoheaderAwareTransformation* p_trans = static_cast<const PseudoheaderAwareTransformation*> ( next_transformation );
    p_trans->calculate_checksum (
      ( uint8_t* ) &output_header->source_addr, ( uint8_t* ) &output_header->destination_addr,  sizeof ( in6_addr ),
      output_header->payload_length, destination_buffer + ipv6_header_length );
  }

  hton32 ( output_header->version__traffic_class__flow_label );
  hton16 ( output_header->payload_length );
  return ipv6_header_length + payload_length;
}


