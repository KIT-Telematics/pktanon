/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#include "AnonCryptoPan.h"

#include <cstring>
#include <cassert>
#include <AnonFactory.h>

namespace pktanon
{

AnonCryptoPan* AnonCryptoPan::construct(pktanon::Params& params)
{
  if (!params.has_param("key"))
    throw std::runtime_error("incomplete configuration for AnonCryptoPan: missing key attribute");

  return new AnonCryptoPan(params.get_param("key"));
}


AnonCryptoPan::AnonCryptoPan(string _key) :
  key(_key), cryptopan((const UINT8*) key.c_str())
{
}

AnonCryptoPan::~AnonCryptoPan()
{
}

AnonPrimitive::ANON_RESULT AnonCryptoPan::anonymize_chain(void* buf, unsigned int len) const
{
  assert(len == sizeof (UINT32));
  UINT32 orig = 0;
  memcpy(&orig, buf, sizeof(UINT32));

  orig = const_cast<AnonCryptoPan*>(this)->cryptopan.anonymize(orig);
  memcpy(buf, &orig, sizeof(UINT32));

  return ANON_RESULT(len);
}

AnonPrimitive::ANON_RESULT AnonCryptoPan::anonymize_internal ( const void*const src_buff, void* dst_buff, unsigned int len ) const
{
  assert(len == sizeof (UINT32));
  UINT32 orig = * (UINT32*) src_buff;

  orig = const_cast<AnonCryptoPan*>(this)->cryptopan.anonymize(orig);
  * (UINT32*) dst_buff = orig;
  
  return ANON_RESULT(len);  
}


}
