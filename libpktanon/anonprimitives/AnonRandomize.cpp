/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#include "AnonRandomize.h"

#include <AnonFactory.h>
#include <RandomNumberGenerator.h>

namespace pktanon
{

AnonRandomize::~AnonRandomize() = default;
AnonRandomize::AnonRandomize() { }

AnonPrimitive::ANON_RESULT AnonRandomize::anonymize_chain(void* buf, unsigned int len) const
{
  RandomNumberGenerator::generate_random_bytes (reinterpret_cast<uint8_t*>(buf), len);
  return ANON_RESULT (len);
}

AnonPrimitive::ANON_RESULT AnonRandomize::anonymize_internal ( const void*const src_buff, void* dst_buff, unsigned int len ) const
{
  RandomNumberGenerator::generate_random_bytes (reinterpret_cast<uint8_t*>(dst_buff), len);
  return ANON_RESULT (len);
}

REGISTER_ANON (AnonRandomize);
}
