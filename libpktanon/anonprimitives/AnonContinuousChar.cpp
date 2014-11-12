/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#include "AnonContinuousChar.h"
#include <cstring>
#include "AnonFactory.h"

namespace pktanon
{

AnonContinuousChar::AnonContinuousChar() : current(0)
{
}

AnonContinuousChar::~AnonContinuousChar()
{
}

AnonPrimitive::ANON_RESULT AnonContinuousChar::anonymize_chain(void* buf, unsigned int len) const
{
  memset(buf, current++, len);
  return 0;
}

REGISTER_ANON(AnonContinuousChar);

}
