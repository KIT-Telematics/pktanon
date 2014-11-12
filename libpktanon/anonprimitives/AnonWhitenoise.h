/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 *
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence.
 * See LICENSE file found in the top-level directory of this distribution.
 */

#ifndef __ANON_WHITENOISE_H
#define __ANON_WHITENOISE_H

#include "AnonPrimitive.h"

namespace pktanon
{

class AnonWhitenoise: public AnonPrimitive
{
public:
  static AnonWhitenoise* construct ( const pktanon::Params& params );

  AnonWhitenoise ( unsigned char strengthval );
  virtual ~AnonWhitenoise();
protected:
  virtual ANON_RESULT anonymize_chain ( void* buf, unsigned int len ) const;
  virtual ANON_RESULT anonymize_internal ( const void*const src_buff, void* dst_buff, unsigned int len ) const;

private:
  unsigned char strength;
};
}

#endif // __ANON_WHITENOISE_H
