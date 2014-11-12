/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#include "AnonPrimitive.h"
#include "string.h"
#include "optmacros.h"
#include "debug.h"

namespace pktanon
{

AnonPrimitive::AnonPrimitive() :
  next ( NULL )
{
}
 
AnonPrimitive::~AnonPrimitive()
{
}

AnonPrimitive* AnonPrimitive::getNext() const
{
  return next;
}

void AnonPrimitive::setNext ( AnonPrimitive* nextprim )
{
  next = nextprim;
}

unsigned int AnonPrimitive::anonimyze ( const void*const src_buff, void* dst_buff, unsigned int len ) const
{
  ANON_RESULT result = anonymize_internal ( src_buff, dst_buff, len );
  
  auto next = this->next;
  while ( unlikely ( next && result.call_next_anon ) )
  {
    result &=  next->anonymize_chain ( dst_buff,  result.newlength );
    next = next->getNext();
  }

  return result.newlength;
}

AnonPrimitive::ANON_RESULT AnonPrimitive::anonymize_internal ( const void*const src_buff, void* dst_buff, unsigned int len ) const
{
    memcpy(dst_buff, src_buff, len);
    return anonymize_chain(dst_buff, len);
}


}
