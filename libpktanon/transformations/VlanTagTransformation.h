/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */
#ifndef PKTANON_VLANTAGTRANSFORMATION_H
#define PKTANON_VLANTAGTRANSFORMATION_H
# include "Transformation.h"

namespace pktanon
{

# pragma pack (1)
struct VLAN_TAG
{
  uint16_t priority_cfi_id;// priority, canonical format identifier, vlan identifier
  uint16_t type;// next header, based on ethernet protocol fields
};
# pragma pack ()

class VlanTagTransformation : public Transformation
{
public:
  ~VlanTagTransformation();
  VlanTagTransformation (
    AnonPrimitive* anon_priority, AnonPrimitive* anon_cfi,
    AnonPrimitive* anon_vlan_id, AnonPrimitive* anon_type
  );

  virtual int transform (const uint8_t* source_buffer, uint8_t* destination_buffer, unsigned max_packet_length) const noexcept;

private:

  static uint16_t get_priority (VLAN_TAG* header)
  {
    return ( (header->priority_cfi_id & 0xE000) >> 13);
  }

  static void set_priority (VLAN_TAG* header, uint16_t priority)
  {
    header->priority_cfi_id =
      (priority & 0xE000) | (header->priority_cfi_id & 0x1FFF);
  }

  static uint16_t get_cfi (VLAN_TAG* header)
  {
    return ( (header->priority_cfi_id & 0x1000) != 0);
  }

  static void set_cfi (VLAN_TAG* header, uint16_t cfi)
  {
    header->priority_cfi_id =
      (cfi & 0x1000) | (header->priority_cfi_id & 0xEFFF);
  }

  static uint16_t get_vlan_id (VLAN_TAG* header)
  {
    return (header->priority_cfi_id & 0x0FFF);

  }

  static void set_vlan_id (VLAN_TAG* header, uint16_t vlan_id)
  {
    header->priority_cfi_id =
      (vlan_id & 0x0FFF) | (header->priority_cfi_id & 0xF000);
  }

  AnonPrimitive* anon_priority;
  AnonPrimitive* anon_cfi;
  AnonPrimitive* anon_vlan_id;
  AnonPrimitive* anon_type;

};
}

#endif // PKTANON_VLANTAGTRANSFORMATION_H
