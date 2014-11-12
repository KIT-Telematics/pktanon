/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#ifndef PKTANON__ANON_BYTEWISE_H
#define PKTANON__ANON_BYTEWISE_H

#include "anonprimitives/AnonPrimitive.h"

namespace pktanon
{
class AnonBytewise: public AnonPrimitive
{
public:
  AnonBytewise();
  virtual ~AnonBytewise();

protected:
  ANON_RESULT anonymize (void* buf, unsigned int len);
  unsigned char anonbytes[256];
};

}

#endif // __ANON_BYTEWISE_H
