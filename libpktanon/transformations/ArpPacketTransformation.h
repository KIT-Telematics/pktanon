/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 *
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence.
 * See LICENSE file found in the top-level directory of this distribution.
 */

#ifndef PKTANON__ARPPACKETTRANSFORMATION_H
#define PKTANON__ARPPACKETTRANSFORMATION_H

#include "Transformation.h"
#include <transformations/Addresses.h>
#include <netinet/in.h>

namespace pktanon
{

# pragma pack (1)
struct ARP_HEADER
{
  uint16_t hardware_type;                                     // 0x0001 for ethernet. configuration item ArpHardwaretp
  uint16_t protocol_type;                                     // 0x0800 for ip. configuration item ArpPrototp
  uint8_t hardware_size;                                         // 0x6 for ethernet mac addressess. configuration item ArpHardwareaddrlen
  uint8_t protocol_size;                                      // 0x4 for ip addresses. configuration item ArpProtoaddrlen
  uint16_t opcode;                                           // see defines below. configuration item ArpOpcode
  MAC_ADDR sender_mac;                                        // configuration item ArpSourcemac
  in_addr sender_ip;                                          // configuration item ArpSourceip
  MAC_ADDR target_mac;                                          // configuration item ArpDestmac
  in_addr target_ip;                                            // configuration item ArpDestip
};
# pragma pack ()

class ArpPacketTransformation : public Transformation
{
public:
  ~ArpPacketTransformation();
  ArpPacketTransformation (
    AnonPrimitive* anon_hardware_type,  AnonPrimitive* anon_protocol_type,
    AnonPrimitive* anon_hardware_size,  AnonPrimitive* anon_protocol_size,
    AnonPrimitive* anon_opcode,
    AnonPrimitive* anon_sender_mac,  AnonPrimitive* anon_sender_ip,
    AnonPrimitive* anon_target_mac,  AnonPrimitive* anon_target_ip
  );

  virtual int transform ( const uint8_t* source_buffer, uint8_t* destination_buffer, unsigned max_packet_length ) const noexcept;

private:
  const AnonPrimitive* const anon_hardware_type;
  const AnonPrimitive* const anon_protocol_type;
  const AnonPrimitive* const anon_hardware_size;
  const AnonPrimitive* const anon_protocol_size;
  const AnonPrimitive* const anon_opcode;
  const AnonPrimitive* const anon_sender_mac;
  const AnonPrimitive* const anon_sender_ip;
  const AnonPrimitive* const anon_target_mac;
  const AnonPrimitive* const anon_target_ip;

};
}

#endif // PKTANON_ARPPACKETTRANSFORMATION_H
