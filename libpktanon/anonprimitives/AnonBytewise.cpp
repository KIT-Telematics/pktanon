/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 *
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence.
 * See LICENSE file found in the top-level directory of this distribution.
 */

#include "AnonBytewise.h"
#include <cassert>
#include <cstring>

namespace pktanon
{

AnonBytewise::AnonBytewise()
{
}

AnonBytewise::AnonBytewise ( const AnonBytewise& other ) : AnonPrimitive ( other )
{
  memcpy ( anonbytes, other.anonbytes, 256*sizeof ( unsigned char ) );
}

AnonBytewise::~AnonBytewise()
{
}

AnonPrimitive::ANON_RESULT AnonBytewise::anonymize ( void* buf, unsigned int len )
{
  unsigned char* pnt;

  for ( unsigned int i = 0; i < len; i++ )
  {
    pnt = ( unsigned char* ) buf + i;
    memset ( pnt, anonbytes[*pnt], 1 );

  } // for (unsigned int i=0; i<len; i++)

  return ANON_RESULT ( len );
}

}
