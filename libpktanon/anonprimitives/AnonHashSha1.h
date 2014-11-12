/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 *
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence.
 * See LICENSE file found in the top-level directory of this distribution.
 */

#ifndef PKTANON__ANON_HASH_SHA1_H
#define PKTANON__ANON_HASH_SHA1_H

#include "AnonPrimitive.h"

namespace pktanon
{

class AnonHashSha1: public AnonPrimitive
{
public:
  AnonHashSha1 ( bool truncate = false );
  virtual ~AnonHashSha1();
protected:
  virtual ANON_RESULT anonymize_chain ( void* buf, unsigned int len ) const;
  virtual ANON_RESULT anonymize_internal ( const void* const src_buff, void* dst_buff, unsigned int len ) const;
private:
  bool truncate;
};

}

#endif // __ANON_HASH_SHA1_H
