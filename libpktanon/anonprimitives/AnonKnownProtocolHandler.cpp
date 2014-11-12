/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#include "AnonKnownProtocolHandler.h"
#include "AnonFactory.h"
#include "transformations/Transformation.h"

using namespace pktanon;

AnonKnownProtocolHandler::~AnonKnownProtocolHandler() = default;
AnonKnownProtocolHandler::AnonKnownProtocolHandler()
{}

AnonPrimitive::ANON_RESULT AnonKnownProtocolHandler::anonymize_internal ( const void*const src_buff, void* dst_buff, unsigned int len ) const
{
  memcpy(dst_buff, src_buff, len);
  return anonymize_chain(dst_buff, len);
}

AnonPrimitive::ANON_RESULT AnonKnownProtocolHandler::anonymize_chain ( void* buf, unsigned int len ) const
{
  uint8_t protocol = ((uint8_t*)buf)[0];
  if(Transformation::getTransformationProtocol(protocol) == Transformation::getPayloadTransformation())
  {
    return ANON_RESULT(len, false);
  }
  else 
  {
    return ANON_RESULT(len);
  }
}

// AnonPrimitive::ANON_RESULT AnonKnownProtocolHandler::anonymize ( void* buf, unsigned int len ) const
// {
//   uint8_t protocol = ((uint8_t*)buf)[0];
//   if(Transformation::getTransformationProtocol(protocol) == Transformation::getPayloadTransformation())
//   {
//     return ANON_RESULT(len, false);
//   }
//   else 
//   {
//     return ANON_RESULT(len);
//   }
// }

REGISTER_ANON(AnonKnownProtocolHandler);


