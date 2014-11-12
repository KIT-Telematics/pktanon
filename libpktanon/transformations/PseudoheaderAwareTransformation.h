/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#ifndef PKTANON_PSEUDOHEADER_AWARETRANSFORMATION_H
# define PKTANON_PSEUDOHEADER_AWARETRANSFORMATION_H

# include "Transformation.h"

namespace pktanon
{

class PseudoheaderAwareTransformation : public Transformation
{
public:
  virtual ~PseudoheaderAwareTransformation();

public:
//   virtual size_t verify_checksum(uint8_t protocol,  uint16_t length, uint8_t* source_address,  uint8_t* destination_address, size_t address_length, uint8_t* source_next_header);
  
  virtual int calculate_checksum(uint8_t* source_address,  uint8_t* destination_address, unsigned address_length, uint16_t ip_length, uint8_t* sink_next_header) const noexcept = 0;

};

inline PseudoheaderAwareTransformation::~PseudoheaderAwareTransformation()
{}


}

#endif // PKTANON_TRANSFORMATION_H
