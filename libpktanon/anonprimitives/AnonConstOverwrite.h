/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 *
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence.
 * See LICENSE file found in the top-level directory of this distribution.
 */

#ifndef PKTANON__ANON_CONST_OVERWRITE_H
#define PKTANON__ANON_CONST_OVERWRITE_H

#include "AnonPrimitive.h"

namespace pktanon
{

class Params;

class AnonConstOverwrite: public AnonPrimitive
{
public:
  static AnonConstOverwrite* construct ( const Params& params );

  AnonConstOverwrite ( unsigned char byte );
  virtual ~AnonConstOverwrite();
protected:
  virtual ANON_RESULT anonymize_chain ( void* buf, unsigned int len ) const;
  virtual ANON_RESULT anonymize_internal ( const void*const src_buff, void* dst_buff, unsigned int len ) const;

private:
  unsigned char byteval;
};

}

#endif // __ANON_CONST_OVERWRITE_H
