/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#include "AnonHashHmacSha1.h"
#include "AnonFactory.h"
#include <cstring>
#include <nettle/hmac.h>

namespace pktanon
{

AnonHashHmacSha1* AnonHashHmacSha1::construct ( const Params& params )
{
    if ( !params.has_param ( "key" ) )
        throw std::runtime_error ( "incomplete configuration for AnonHashHmacSha1: missing key attribute" );

    return new AnonHashHmacSha1 ( params.get_param ( "key" ) );
}

AnonHashHmacSha1::AnonHashHmacSha1 ( string hmackey )
{
    keylen = ( unsigned int ) hmackey.length();
    key = new uint8_t[keylen + 1];

#ifdef WIN32
    strcpy ( ( char* ) key, hmackey.c_str () );
#else
    strcpy ( ( char* ) key, hmackey.c_str() );
#endif

}

AnonHashHmacSha1::~AnonHashHmacSha1()
{
    delete[] key;
}

AnonPrimitive::ANON_RESULT AnonHashHmacSha1::anonymize_chain ( void* buf, unsigned int len ) const
{
    struct hmac_sha1_ctx hmac_ctx;
    hmac_sha1_set_key ( &hmac_ctx, keylen, key );
    hmac_sha1_update ( &hmac_ctx, len, ( unsigned char* ) buf );
    hmac_sha1_digest ( &hmac_ctx,  std::min ( ( int ) len,  SHA1_DIGEST_SIZE ), ( unsigned char* ) buf );

    if ( SHA1_DIGEST_SIZE >= len )
    {
        return ANON_RESULT ( std::min ( ( int ) len,  SHA1_DIGEST_SIZE ) );
    }


//   HmacSha1::sha1_hmac ( (uint8_t*) &digest, key, keylen, (uint8_t*) buf, len);

    int remaining = len;
    int pos = 0;

    while ( remaining > 0 )
    {
        memcpy ( ( unsigned char* ) buf + pos, buf, std::min ( remaining, SHA1_DIGEST_SIZE ) );
        remaining -= SHA1_DIGEST_SIZE;
        pos += SHA1_DIGEST_SIZE;
    } // while (remaining != 0)

    return ANON_RESULT ( len );
}

AnonPrimitive::ANON_RESULT AnonHashHmacSha1::anonymize_internal ( const void*const src_buff, void* dst_buff, unsigned int len ) const
{
    struct hmac_sha1_ctx hmac_ctx;
    nettle_hmac_sha1_set_key ( &hmac_ctx, keylen, key );
    nettle_hmac_sha1_update  ( &hmac_ctx, len, ( unsigned char* ) src_buff );
    nettle_hmac_sha1_digest  ( &hmac_ctx,  std::min ( ( int ) len,  SHA1_DIGEST_SIZE ), ( unsigned char* ) dst_buff );

    if ( SHA1_DIGEST_SIZE >= len )
    {
        return ANON_RESULT ( std::min ( ( int ) len,  SHA1_DIGEST_SIZE ) );
    }


//   HmacSha1::sha1_hmac ( (uint8_t*) &digest, key, keylen, (uint8_t*) buf, len);

    int remaining = len;
    int pos = 0;

    while ( remaining > 0 )
    {
        memcpy ( ( unsigned char* ) dst_buff + pos, dst_buff, std::min ( remaining, SHA1_DIGEST_SIZE ) );
        remaining -= SHA1_DIGEST_SIZE;
        pos += SHA1_DIGEST_SIZE;
    } // while (remaining != 0)

    return ANON_RESULT ( len );
}



REGISTER_ANON_PARAM ( AnonHashHmacSha1, AnonHashHmacSha1::construct );

}
