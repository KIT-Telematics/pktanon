/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#ifndef PKTANON_ADDRESSES_H
#define PKTANON_ADDRESSES_H

namespace pktanon
{
  
static const unsigned int ETHER_ADDR_LEN = 6;

struct MAC_ADDR
{
  uint8_t addr[ETHER_ADDR_LEN];
};
}

#endif
