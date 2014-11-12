/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#include "VlanTagTransformation.h"

using namespace pktanon;

VlanTagTransformation::~VlanTagTransformation()
{
  delete anon_priority;
  delete anon_cfi;
  delete anon_vlan_id;
  delete anon_type;
}

VlanTagTransformation::VlanTagTransformation (
  AnonPrimitive* anon_priority, AnonPrimitive* anon_cfi,
  AnonPrimitive* anon_vlan_id, AnonPrimitive* anon_type
) :
  anon_priority (anon_priority), anon_cfi (anon_cfi),
  anon_vlan_id (anon_vlan_id), anon_type (anon_type)
{}

int VlanTagTransformation::transform ( const uint8_t* source_buffer, uint8_t* destination_buffer, unsigned int max_packet_length ) const noexcept
{
  size_t vlan_tag_length = sizeof (VLAN_TAG);

  VLAN_TAG* input_header = (VLAN_TAG*) source_buffer;
  VLAN_TAG* output_header = (VLAN_TAG*) destination_buffer;


  ntoh16 (input_header->priority_cfi_id);
  ntoh16 (input_header->type);

  uint16_t ethertype = input_header->type;

  // priority
  uint16_t priority = get_priority (input_header);
  uint16_t priority_anon = 0;
  transform_field (anon_priority, &priority, &priority_anon, sizeof (uint16_t));
  set_priority (output_header, priority_anon);

  // cfi
  uint16_t cfi = get_cfi (input_header);
  uint16_t cfi_anon = 0;
  transform_field (anon_cfi, &cfi, &cfi_anon, sizeof (uint16_t));
  set_cfi (output_header, cfi_anon);

  // vlan_id
  uint16_t vlan_id = get_vlan_id (input_header);
  uint16_t vlan_id_anon = 0;
  transform_field (anon_vlan_id, &vlan_id, &vlan_id_anon, sizeof (uint16_t));
  set_vlan_id (output_header, vlan_id);

  // (ether)type
  transform_field (anon_type, &input_header->type, &output_header->type, sizeof (uint16_t));

  const Transformation* next_transformation = Transformation::getTransformationEthertype (ethertype);
  vlan_tag_length += next_transformation->transform (source_buffer + vlan_tag_length,  destination_buffer + vlan_tag_length, max_packet_length - vlan_tag_length);

  return vlan_tag_length;
}

