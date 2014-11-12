/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#include "AnonWhitenoise.h"
#include "AnonFactory.h"
#include <string.h>
#include "RandomNumberGenerator.h"

namespace pktanon
{

AnonWhitenoise* AnonWhitenoise::construct ( const Params& params )
{
    if ( !params.has_param ( "strength" ) )
        throw std::runtime_error ( "incomplete configuration for AnonWhitenoise: missing strength attribute" );

    unsigned char istrength = std::strtoul ( params.get_param ( "strength" ).c_str(), nullptr, 10 );

    return new AnonWhitenoise ( istrength );
}

AnonWhitenoise::AnonWhitenoise ( unsigned char strengthval ) :
    strength ( strengthval )
{
}

AnonWhitenoise::~AnonWhitenoise()
{
}

AnonPrimitive::ANON_RESULT AnonWhitenoise::anonymize_chain ( void* buf, unsigned int len ) const
{
    //  how many bits are in the buffer?
    unsigned int bits = len * 8;

    // how many bits of these will we flip? Generate a random number and apply the given strength.
    unsigned int randbits = RandomNumberGenerator::generate ( 0, bits );
    randbits = ( unsigned int ) ceil (
                   ( ( double ) randbits / 100.0 ) * ( strength * 10 ) );

    // in any case we will flip at least one bit, if we have any bits
    if ( bits > 0 && randbits == 0 )
        randbits++;

    //
    // now we flip randbits in the buffer at random positions
    //

    unsigned int flipbit;
    unsigned char* byte;
    unsigned char mask;

    for ( unsigned int i = 0; i < randbits; i++ )
    {

        flipbit = RandomNumberGenerator::generate ( 0, bits - 1 );
        byte = ( ( unsigned char* ) buf ) + ( flipbit / 8 );
        mask = 1 << ( flipbit % 8 );

        if ( ( *byte & mask ) == 0 )
            *byte |= mask; // set bit to 1
        else
            *byte &= ~mask; // set bit to 0

    } // for (unsigned int i=0; i<randbits; i++)

    return ANON_RESULT ( len );
}

AnonPrimitive::ANON_RESULT AnonWhitenoise::anonymize_internal ( const void*const src_buff, void* dst_buff, unsigned int len ) const
{
  memcpy(dst_buff, src_buff, len);
  return anonymize_chain(dst_buff, len);
}


REGISTER_ANON_PARAM ( AnonWhitenoise, AnonWhitenoise::construct );

}
