/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#include "AnonIdentity.h"
#include <AnonFactory.h>
#include <string.h>
//--------------------------------------------

namespace pktanon
{

AnonIdentity::AnonIdentity() {}
AnonIdentity::~AnonIdentity() = default;

AnonPrimitive::ANON_RESULT AnonIdentity::anonymize_chain(void* buf, unsigned int len) const
{
  return ANON_RESULT(len);
}

AnonPrimitive::ANON_RESULT AnonIdentity::anonymize_internal ( const void*const src_buff, void* dst_buff, unsigned int len ) const
{
  memcpy(dst_buff, src_buff, len);
  return ANON_RESULT(len);
}


REGISTER_ANON(AnonIdentity)
}

