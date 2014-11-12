/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#ifndef PKTANON_ICMPV6PACKETTRANSFORMATION_H
#define PKTANON_ICMPV6PACKETTRANSFORMATION_H

# include "Transformation.h"
#include "PseudoheaderAwareTransformation.h"
namespace pktanon
{

# pragma pack (1)
struct ICMPv6_HEADER
{
  uint8_t type;
  uint8_t code;
  uint16_t checksum;
  uint32_t misc;
} ;
# pragma pack ()

# pragma pack (1)
struct ICMPv6_NSM_HEADER                                    //NSM = neighbor solicitation message
{
  uint32_t reserved;
  in6_addr target_address;
} ;
# pragma pack ()

# pragma pack (1)
struct ICMPv6_NAM_HEADER                                    //NAM = neighbor advertisement message
{
  uint32_t rso__reserved;
  in6_addr target_address;
} ;
# pragma pack ()

enum ICMPv6_Types{
  NDP_NSM = 135,                                                //  Neighbor Solicitation (NDP)
  NDP_NAM = 136,                                                // Neighbor Advertisement (NDP)
};

class ICMPv6PacketTransformation : public PseudoheaderAwareTransformation
{
public:
  virtual ~ICMPv6PacketTransformation();
  ICMPv6PacketTransformation (
    AnonPrimitive* anon_type,
    AnonPrimitive* anon_code,
    AnonPrimitive* anon_misc,
    AnonPrimitive* anon_target_address
  );

  virtual int transform (const uint8_t* source_buffer, uint8_t* destination_buffer, unsigned max_packet_length) const noexcept;
  virtual int calculate_checksum (uint8_t* source_address, uint8_t* destination_address, unsigned address_length, uint16_t ip_length, uint8_t* sink_next_header) const noexcept;

private:
  
  int nsm_transform(const uint8_t* source_buffer, uint8_t* destination_buffer, unsigned max_packet_length) const;
  int nam_transform(const uint8_t* source_buffer, uint8_t* destination_buffer, unsigned max_packet_length) const;
  
  AnonPrimitive* anon_type;
  AnonPrimitive* anon_code;
  AnonPrimitive* anon_misc;
  AnonPrimitive* anon_target_address;

};
}

#endif // PKTANON_ICMPV6PACKETTRANSFORMATION_H
