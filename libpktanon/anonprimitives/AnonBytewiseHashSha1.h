/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 *
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence.
 * See LICENSE file found in the top-level directory of this distribution.
 */

#ifndef PKTANON__ANON_BYTEWISE_HASH_SHA1_H
# define PKTANON__ANON_BYTEWISE_HASH_SHA1_H

#include "AnonBytewise.h"

namespace pktanon
{

class AnonBytewiseHashSha1: public AnonBytewise
{
public:
  AnonBytewiseHashSha1();
  virtual ~AnonBytewiseHashSha1();
private:
  void fillTable();
};

}

#endif // __ANON_BYTEWISE_HASH_SHA1_H
