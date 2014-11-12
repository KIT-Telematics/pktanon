/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#include "AnonBytewiseHashHmacSha1.h"
#include "AnonHashHmacSha1.h"
#include <stdexcept>
#include "AnonFactory.h"

namespace pktanon
{

AnonBytewiseHashHmacSha1* AnonBytewiseHashHmacSha1::construct (std::unordered_map<string, string> attrmap)
{
  auto it = attrmap.find ("key");
  string key;

  if (it == attrmap.end())
  {
    throw std::runtime_error ("incomplete configuration for AnonBytewiseHashHmacSha1: missing key attribute");
  }

  key = it->second;

  return new AnonBytewiseHashHmacSha1 (key);
}

AnonBytewiseHashHmacSha1::AnonBytewiseHashHmacSha1 (string key)
{
  fillTable (key);
}

AnonBytewiseHashHmacSha1::~AnonBytewiseHashHmacSha1()
{
}

void AnonBytewiseHashHmacSha1::fillTable (string& hmackey)
{
  unsigned char data;
  AnonHashHmacSha1 hashobj (hmackey);

  for (unsigned short i = 0; i < 256; i++)
  {

    data = (unsigned char) i;
    hashobj.anonimyze (&data, 1);
    anonbytes[i] = data;

  } // for (unsigned short i = 0; i<256; i++)
}

REGISTER_ANON_PARAM (AnonBytewiseHashHmacSha1, AnonBytewiseHashHmacSha1::construct);

}
