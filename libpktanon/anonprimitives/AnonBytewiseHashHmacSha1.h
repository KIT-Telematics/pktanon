/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#ifndef __ANON_BYTEWISE_HASH_HMAC_SHA1_H
#define __ANON_BYTEWISE_HASH_HMAC_SHA1_H

#include "AnonBytewise.h"
# include "unordered_map"

namespace pktanon
{

class AnonBytewiseHashHmacSha1: public AnonBytewise
{
public:
  static AnonBytewiseHashHmacSha1* construct(std::unordered_map<string, string> attrmap);

  AnonBytewiseHashHmacSha1(string key);
  virtual ~AnonBytewiseHashHmacSha1();
private:
  void fillTable(string& hmackey);
};

}

#endif // __ANON_BYTEWISE_HASH_HMAC_SHA1_H
