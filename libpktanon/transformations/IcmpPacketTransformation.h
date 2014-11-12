/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#ifndef PKTANON_ICMPPACKETTRANSFORMATION_H
#define PKTANON_ICMPPACKETTRANSFORMATION_H
# include "Transformation.h"

namespace pktanon
{
# pragma pack (1)
struct ICMP_HEADER
{
  uint8_t type;	
  uint8_t code;	
  uint16_t checksum; 
  uint32_t misc; 
} ;
# pragma pack ()


class IcmpPacketTransformation : public Transformation
{
public:
  virtual ~IcmpPacketTransformation();
  IcmpPacketTransformation(
    AnonPrimitive* anon_type,
    AnonPrimitive* anon_code,
    AnonPrimitive* anon_misc
  );

  virtual int transform(const uint8_t* source_buffer, uint8_t* destination_buffer, unsigned max_packet_length) const noexcept;
private:

  AnonPrimitive* anon_type;
  AnonPrimitive* anon_code;
  AnonPrimitive* anon_misc;

};
}

#endif // PKTANON_ICMPPACKETTRANSFORMATION_H
