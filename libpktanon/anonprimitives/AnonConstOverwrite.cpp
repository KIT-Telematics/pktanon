/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#include "AnonConstOverwrite.h"
#include "AnonFactory.h"
#include <cstring>
#include <stdexcept>

namespace pktanon
{

AnonConstOverwrite* AnonConstOverwrite::construct(const Params& params)
{
  if (!params.has_param("value"))
    throw std::runtime_error("incomplete configuration for AnonConstOverwrite: missing  attribute 'value'");

  unsigned char byte = strtoul(params.get_param("value").c_str(),  nullptr,  16);
  return new AnonConstOverwrite(byte);
}

AnonConstOverwrite::AnonConstOverwrite(unsigned char byte) :
  byteval(byte)
{
}

AnonConstOverwrite::~AnonConstOverwrite()
{
}

AnonPrimitive::ANON_RESULT AnonConstOverwrite::anonymize_chain(void* buf, unsigned int len) const
{
  memset(buf, byteval, len);
  return ANON_RESULT(len);
}

AnonPrimitive::ANON_RESULT AnonConstOverwrite::anonymize_internal ( const void*const src_buff, void* dst_buff, unsigned int len ) const
{
//   HB();
  memset(dst_buff, byteval, len);
  return ANON_RESULT(len);
}

REGISTER_ANON_PARAM(AnonConstOverwrite, AnonConstOverwrite::construct);

}
