/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#ifndef PKTANON_CHECKSUM_H
#define PKTANON_CHECKSUM_H

#include <stdint.h>
#include <stddef.h>

namespace pktanon
{

class Checksum
{
public:
  Checksum() : sum(0) {};
  
  void update(void* buffer,  size_t length);
  
  uint16_t final();
  
private:
  uint32_t sum;
};
}

#endif // PKTANON_CHECKSUM_H
