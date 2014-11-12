/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 *
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence.
 * See LICENSE file found in the top-level directory of this distribution.
 */

#include "AnonShuffle.h"

#include <algorithm>
#include <string.h>
#include "AnonFactory.h"

namespace pktanon
{

AnonShuffle::~AnonShuffle() = default;
AnonShuffle::AnonShuffle() { }


AnonPrimitive::ANON_RESULT AnonShuffle::anonymize_chain ( void* buf, unsigned int len ) const
{
  unsigned char* buf1 = reinterpret_cast<unsigned char*> ( buf );
  std::random_shuffle ( buf1, buf1+len );

  return ANON_RESULT ( len );
}

AnonPrimitive::ANON_RESULT AnonShuffle::anonymize_internal ( const void*const src_buff, void* dst_buff, unsigned int len ) const
{
  memcpy ( dst_buff, src_buff, len );

  unsigned char* buff = reinterpret_cast<unsigned char*> ( dst_buff );
  std::random_shuffle ( buff, buff+len );

  return ANON_RESULT ( len );
}


REGISTER_ANON ( AnonShuffle );
}
