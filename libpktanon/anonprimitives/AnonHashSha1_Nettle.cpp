/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#include "AnonHashSha1.h"
#include "AnonFactory.h"
#include <cstring>
#include <nettle/sha1.h>

namespace pktanon
{

AnonHashSha1::AnonHashSha1(bool truncate)
{
}

AnonHashSha1::~AnonHashSha1()
{
}

AnonPrimitive::ANON_RESULT AnonHashSha1::anonymize_chain (void* buf, unsigned int len) const
{
  thread_local static struct sha1_ctx ctx;

  sha1_init (&ctx);
  sha1_update (&ctx,  len, (unsigned char*) buf);
  sha1_digest (&ctx,  std::min ( (int) len,  SHA1_DIGEST_SIZE), (unsigned char*) buf);

  if (SHA1_DIGEST_SIZE >= len || truncate)
  {
    return ANON_RESULT (std::min ( (int) len,  SHA1_DIGEST_SIZE));
  }

  int pos = SHA1_DIGEST_SIZE;
  int remaining = len - SHA1_DIGEST_SIZE;

  while (remaining > 0)
  {
    memcpy ( (unsigned char*) buf + pos, buf, std::min (remaining, SHA1_DIGEST_SIZE));
    pos += SHA1_DIGEST_SIZE;
    remaining -= SHA1_DIGEST_SIZE;
  }

  return ANON_RESULT (len);
}

AnonPrimitive::ANON_RESULT AnonHashSha1::anonymize_internal ( const void* const src_buff, void* dst_buff, unsigned int len ) const
{
  thread_local static struct sha1_ctx ctx;

  nettle_sha1_init (&ctx);
  nettle_sha1_update (&ctx,  len, (unsigned char*) src_buff);
  nettle_sha1_digest (&ctx,  std::min ( (int) len,  SHA1_DIGEST_SIZE), (unsigned char*) dst_buff);

  if (SHA1_DIGEST_SIZE >= len || truncate)
  {
    return ANON_RESULT (std::min ( (int) len,  SHA1_DIGEST_SIZE));
  }

  int pos = SHA1_DIGEST_SIZE;
  int remaining = len - SHA1_DIGEST_SIZE;

  while (remaining > 0)
  {
    memcpy ( (unsigned char*) dst_buff + pos, dst_buff, std::min (remaining, SHA1_DIGEST_SIZE));
    pos += SHA1_DIGEST_SIZE;
    remaining -= SHA1_DIGEST_SIZE;
  }

  return ANON_RESULT (len);  
}


REGISTER_ANON (AnonHashSha1)

}
