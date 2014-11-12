/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 *
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence.
 * See LICENSE file found in the top-level directory of this distribution.
 */

#include "AnonShorten.h"
#include "AnonFactory.h"
#include "log.h"
#include "debug.h"
#include <string.h>

namespace pktanon
{



AnonShorten* AnonShorten::construct ( const Params& params )
{
  if ( !params.has_param ( "newlen" ) )
    throw std::runtime_error ( "incomplete configuration for AnonShorten: missing newlen attribute" );

  unsigned int len = strtoul ( params.get_param ( "newlen" ).c_str(),  nullptr,  10 );

  _plg_verbose ( "\t\t\t" << "newlen: " << len );

  return new AnonShorten ( len );

}

AnonShorten::AnonShorten ( unsigned int length ) :
  newlen ( length )
{
}

AnonShorten::~AnonShorten()
{
}

AnonPrimitive::ANON_RESULT AnonShorten::anonymize_chain ( void* buf, unsigned int len ) const
{
//   HB();
  return ANON_RESULT ( std::min ( newlen, len ) );
}

AnonPrimitive::ANON_RESULT AnonShorten::anonymize_internal ( const void*const src_buff, void* dst_buff, unsigned int len ) const
{
//   HB();
  if ( newlen == 0 ) return ANON_RESULT ( 0 );

  memcpy ( dst_buff, src_buff, std::min ( newlen, len ) );
  return ANON_RESULT ( std::min ( newlen, len ) );
}


REGISTER_ANON_PARAM ( AnonShorten, AnonShorten::construct );

}
