/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#include "AnonConstOverwriteRange.h"
#include "AnonFactory.h"
#include <string.h>
#include <stdexcept>
#include <debug.h>

using namespace pktanon;

AnonConstOverwriteRange* AnonConstOverwriteRange::construct(const pktanon::Params& params)
{
  if (!params.has_param("range-begin"))
    throw std::runtime_error("incomplete configuration for AnonConstOverwriteRange - missing attribute: range-begin");

  if (!params.has_param("range-length"))
    throw std::runtime_error("incomplete configuration for AnonConstOverwriteRange - missing attribute: range-length");

  if (!params.has_param("value"))
    throw std::runtime_error("incomplete configuration for AnonConstOverwriteRange - missing attribute: value");

  int range_begin = std::stoi(params.get_param("range-begin"));
  int range_length = std::stoi(params.get_param("range-length"));
  uint8_t value = std::stoul(params.get_param("value"),  nullptr, 16);

//   TRACE(range_begin);
//   TRACE(range_length);
//   TRACE(value);

  return new AnonConstOverwriteRange(range_begin, range_length, value);
}


AnonConstOverwriteRange::AnonConstOverwriteRange(int range_begin, int range_length, uint8_t value):
  range_begin(range_begin), range_length(range_length),  value(value)
{}

AnonConstOverwriteRange::~AnonConstOverwriteRange() = default;

AnonPrimitive::ANON_RESULT AnonConstOverwriteRange::anonymize_chain(void* buf, unsigned int len) const
{
  memset(((uint8_t*)buf)+range_begin, value, std::min(range_length, (int) len-range_begin));
  return ANON_RESULT(len);
}

AnonPrimitive::ANON_RESULT AnonConstOverwriteRange::anonymize_internal ( const void*const src_buff, void* dst_buff, unsigned int len ) const
{
  memcpy(dst_buff, src_buff, len);
  memset(((uint8_t*)dst_buff)+range_begin, value, std::min(range_length, (int) len-range_begin));
  return ANON_RESULT(len);  
}


REGISTER_ANON_PARAM(AnonConstOverwriteRange, AnonConstOverwriteRange::construct);
