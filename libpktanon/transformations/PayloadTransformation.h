/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#ifndef PKTANON_PAYLOADTRANSFORMATION_H
#define PKTANON_PAYLOADTRANSFORMATION_H
# include "Transformation.h"

namespace pktanon
{

class PayloadTransformation : public Transformation
{
public:
  ~PayloadTransformation();
  PayloadTransformation(AnonPrimitive* payload_anon);
    
  virtual int transform(const uint8_t* source_buffer, uint8_t* destination_buffer, unsigned max_packet_length) const noexcept;
  
private:
  AnonPrimitive* payload_anon;
};
}

#endif // PKTANON_PAYLOADTRANSFORMATION_H
