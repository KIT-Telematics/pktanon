/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#include "anonprimitives/AnonHashSha256.h"
#include <cstring>
#include <AnonFactory.h>
#include <nettle/sha2.h>

namespace pktanon
{

AnonHashSha256::AnonHashSha256 ( bool truncate ) :
    truncate ( truncate )
{ }

AnonPrimitive::ANON_RESULT AnonHashSha256::anonymize_chain ( void* buf, unsigned int len ) const
{
    struct sha256_ctx ctx;

    sha256_init ( &ctx );
    sha256_update ( &ctx,  len, ( unsigned char* ) buf );
    sha256_digest ( &ctx,  std::min ( ( int ) len,  SHA256_DIGEST_SIZE ), ( unsigned char* ) buf );

    if ( SHA256_DIGEST_SIZE >= len || truncate )
    {
        return ANON_RESULT ( std::min ( ( int ) len,  SHA256_DIGEST_SIZE ) );
    }

    int pos = SHA256_DIGEST_SIZE;
    int remaining = len - SHA256_DIGEST_SIZE;

    while ( remaining > 0 )
    {
        memcpy ( ( unsigned char* ) buf + pos, buf, std::min ( remaining, SHA256_DIGEST_SIZE ) );
        pos+= SHA256_DIGEST_SIZE;
        remaining-=SHA256_DIGEST_SIZE;
    }

    return ANON_RESULT ( len );
}

AnonPrimitive::ANON_RESULT AnonHashSha256::anonymize_internal ( const void*const src_buff, void* dst_buff, unsigned int len ) const
{
    struct sha256_ctx ctx;

    nettle_sha256_init ( &ctx );
    nettle_sha256_update ( &ctx,  len, ( unsigned char* ) src_buff );
    nettle_sha256_digest ( &ctx,  std::min ( ( int ) len,  SHA256_DIGEST_SIZE ), ( unsigned char* ) dst_buff );

    if ( SHA256_DIGEST_SIZE >= len || truncate )
    {
        return ANON_RESULT ( std::min ( ( int ) len,  SHA256_DIGEST_SIZE ) );
    }

    int pos = SHA256_DIGEST_SIZE;
    int remaining = len - SHA256_DIGEST_SIZE;

    while ( remaining > 0 )
    {
        memcpy ( ( unsigned char* ) dst_buff + pos, dst_buff, std::min ( remaining, SHA256_DIGEST_SIZE ) );
        pos+= SHA256_DIGEST_SIZE;
        remaining-=SHA256_DIGEST_SIZE;
    }

    return ANON_RESULT ( len );
}



REGISTER_ANON ( AnonHashSha256 );

}
