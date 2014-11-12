/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 *
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence.
 * See LICENSE file found in the top-level directory of this distribution.
 */

#ifndef PKTANON__ANON_SHORTEN_H
# define PKTANON__ANON_SHORTEN_H

#include "AnonPrimitive.h"

namespace pktanon
{

/**
 * shortens the buffer.
 *
 * if the parameter 'length' is 0,  buffer is completely removed
 * if the parameter 'length' is larger than the size of the buffer,  buffer is unchanged
 */
class AnonShorten: public AnonPrimitive
{
public:
  static AnonShorten* construct ( const pktanon::Params& params );

  AnonShorten ( unsigned int length );
  virtual ~AnonShorten();
protected:
  virtual ANON_RESULT anonymize_chain ( void* buf, unsigned int len ) const;
  virtual ANON_RESULT anonymize_internal ( const void*const src_buff, void* dst_buff, unsigned int len ) const;

private:
  unsigned int newlen;
};

}

#endif // __ANON_SHORTEN_H
