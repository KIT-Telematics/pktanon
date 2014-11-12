/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#ifndef PKTANON_PROTOCOLUTILS_H
#define PKTANON_PROTOCOLUTILS_H

# include <stdint.h>

namespace pktanon
{

static const unsigned int ETHERNET_MTU = 1500;

enum EtherTypes : uint16_t
{
  ETHERTYPE_IP = 0x0800,
  ETHERTYPE_ARP = 0x0806,
  ETHERTYPE_IPV6 = 0x86DD,
  ETHERTYPE_VLAN = 0x8100
};

}

#endif // PKTANON_PROTOCOLUTILS_H
