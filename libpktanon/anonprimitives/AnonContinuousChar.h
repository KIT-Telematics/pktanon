/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 *
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence.
 * See LICENSE file found in the top-level directory of this distribution.
 */

#ifndef PKTANON__ANON_CONTINUOUS_CHAR_H
#define PKTANON__ANON_CONTINUOUS_CHAR_H

#include "AnonPrimitive.h"

namespace pktanon
{

class AnonContinuousChar: public AnonPrimitive
{
public:
  AnonContinuousChar();
  virtual ~AnonContinuousChar();
protected:
  virtual ANON_RESULT anonymize_chain ( void* buf, unsigned int len ) const;

private:
  unsigned char current;
};

}

#endif // __ANON_CONTINUOUS_CHAR_H
