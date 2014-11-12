/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#include "AnonBytewiseHashSha1.h"
#include "AnonHashSha1.h"
#include "AnonFactory.h"

namespace pktanon
{

AnonBytewiseHashSha1::AnonBytewiseHashSha1()
{
  fillTable();
}

AnonBytewiseHashSha1::~AnonBytewiseHashSha1()
{
}

void AnonBytewiseHashSha1::fillTable()
{
  unsigned char data;
  AnonHashSha1 hashobj;

  for (unsigned short i = 0; i < 256; i++)
  {
    data = (unsigned char) i;
    hashobj.anonimyze (&data, 1);
    anonbytes[i] = data;
  } // for (unsigned short i = 0; i<256; i++)

}

REGISTER_ANON (AnonBytewiseHashSha1);

}

