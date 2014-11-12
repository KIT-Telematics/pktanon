/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#ifndef PKTANON_UDPPACKETTRANSFORMATION_H
#define PKTANON_UDPPACKETTRANSFORMATION_H

# include "transformations/PseudoheaderAwareTransformation.h"

namespace pktanon
{

# pragma pack (1)
struct UDP_HEADER
{
  uint16_t source_port;
  uint16_t destination_port;
  uint16_t length;
  uint16_t checksum;
};
# pragma pack ()

class UdpPacketTransformation : public PseudoheaderAwareTransformation
{
public:
  virtual ~UdpPacketTransformation();
  UdpPacketTransformation(AnonPrimitive* anon_source_port,  AnonPrimitive* anon_destination_port);
  
  virtual int transform(const uint8_t* source_buffer, uint8_t* destination_buffer, unsigned max_packet_length) const noexcept;
  
  virtual int calculate_checksum(uint8_t* source_address, uint8_t* destination_address, unsigned address_length, uint16_t ip_length, uint8_t* sink_next_header) const noexcept;  
private:
  AnonPrimitive* anon_source_port;
  AnonPrimitive* anon_destination_port;
};
}

#endif // PKTANON_UDPPACKETTRANSFORMATION_H
