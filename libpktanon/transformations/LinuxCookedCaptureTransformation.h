/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#ifndef PKTANON_LINUXCOOKEDCAPTURETRANSFORMATION_H
#define PKTANON_LINUXCOOKEDCAPTURETRANSFORMATION_H
# include "Transformation.h"

namespace pktanon
{

# pragma pack (1)
struct LINUX_SLL_HEADER
{
  uint16_t packet_type;
  uint16_t hardware_type;
  uint16_t link_layer_addr_len;
  uint8_t link_layer_addr[8];                               
  uint16_t protocol_type;
};
# pragma pack ()

class LinuxCookedCaptureTransformation : public Transformation
{
public:
  LinuxCookedCaptureTransformation(
    AnonPrimitive*  anon_packet_type,
    AnonPrimitive*  anon_hardware_type,
    AnonPrimitive*  anon_link_layer_addr,
    AnonPrimitive*  anon_protocol_type
  );

  virtual int transform(const uint8_t* source_buffer, uint8_t* destination_buffer, unsigned max_packet_length) const noexcept;

private:

  const AnonPrimitive* const anon_packet_type;
  const AnonPrimitive* const anon_hardware_type;
  const AnonPrimitive* const anon_link_layer_addr;
  const AnonPrimitive* const anon_protocol_type;
};
}

#endif // PKTANON_LINUXCOOKEDCAPTURETRANSFORMATION_H
