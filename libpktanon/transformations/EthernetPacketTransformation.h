/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#ifndef PKTANON__ETHERNETPACKETTRANSFORMATION_H
#define PKTANON__ETHERNETPACKETTRANSFORMATION_H

#include "Transformation.h"
#include "transformations/Addresses.h"

namespace pktanon
{

# pragma pack (1)
struct ETHERNET_HEADER
{
  ETHERNET_HEADER() : type (0) {};

  MAC_ADDR destination;
  MAC_ADDR source;
  uint16_t type;
};
# pragma pack ()

class EthernetPacketTransformation :  public Transformation
{
public:
  enum class PAD
  {
    ALWAYS,
    KEEP_LEN,
    NEVER,
  };

  ~EthernetPacketTransformation();
  EthernetPacketTransformation (AnonPrimitive* anon_dest_mac,  AnonPrimitive* anon_source_mac,  AnonPrimitive* anon_ethertype, PAD pad_mode);

  virtual int transform (const uint8_t* source_buffer, uint8_t* destination_buffer, unsigned max_packet_length) const noexcept;

private:
  const AnonPrimitive* const anon_dest_mac;
  const AnonPrimitive* const anon_source_mac;
  const AnonPrimitive* const anon_ethertype;
  const PAD pad_mode;


};
}

#endif // PKTANON_ETHERNETPACKETTRANSFORMATION_H
