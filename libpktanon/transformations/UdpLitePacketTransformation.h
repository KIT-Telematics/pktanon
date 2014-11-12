/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#ifndef PKTANON_UDPLITEPACKETTRANSFORMATION_H
#define PKTANON_UDPLITEPACKETTRANSFORMATION_H

# include <Transformation.h>

namespace pktanon
{

# pragma pack (1)
  struct UDPLITE_HEADER
  {
    uint16_t source_port;
    uint16_t destination_port;
    uint16_t checksum_coverage;
    uint16_t checksum;
  };
# pragma pack ()

class UdpLitePacketTransformation : public Transformation
{
public:
  virtual ~UdpLitePacketTransformation();
  UdpLitePacketTransformation(
    AnonPrimitive* anon_source_port,
    AnonPrimitive* anon_destination_port,
    AnonPrimitive* anon_checksum_coverage
  );

  size_t transform(uint8_t* source_buffer, uint8_t* destination_buffer) const;

private:
  AnonPrimitive* anon_source_port;
  AnonPrimitive* anon_destination_port;
  AnonPrimitive* anon_checksum_coverage;
};

}

#endif // PKTANON_UDPLITEPACKETTRANSFORMATION_H
