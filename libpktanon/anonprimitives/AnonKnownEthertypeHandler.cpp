/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#include "AnonKnownEthertypeHandler.h"
#include "AnonFactory.h"
#include "transformations/Transformation.h"

namespace pktanon {

AnonKnownEthertypeHandler::~AnonKnownEthertypeHandler() = default;
AnonKnownEthertypeHandler::AnonKnownEthertypeHandler()
{}

AnonPrimitive::ANON_RESULT AnonKnownEthertypeHandler::anonymize_internal ( const void*const src_buff, void* dst_buff, unsigned int len ) const
{
  memcpy(dst_buff, src_buff, len);
  return anonymize_chain(dst_buff, len);
}

AnonPrimitive::ANON_RESULT AnonKnownEthertypeHandler::anonymize_chain ( void* buf, unsigned int len ) const
{
  uint16_t* ethertype = ( uint16_t* ) buf;
  if ( Transformation::getTransformationEthertype ( *ethertype ) == Transformation::getPayloadTransformation() )
  {
    return ANON_RESULT ( len, false );
  }
  else
  {
    return ANON_RESULT ( len );
  }
}


REGISTER_ANON(AnonKnownEthertypeHandler)

}