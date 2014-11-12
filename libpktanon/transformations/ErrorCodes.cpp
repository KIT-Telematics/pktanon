/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#include "ErrorCodes.h"

namespace pktanon
{

const char* const error_strings[][3] =
{
  {
    "ethernet header too short", 0, 0
  },
  {
    "arp header too short", 0 , 0
  },
  {
    "ip4 header too short", "bogus ip4 header length", "packet is fragmented"
  }, 
  {
    "ip6 header too short", 0, 0
  },
  {
    "tcp header too short", 0, 0
  },
  {
    "udp header too short", "bad udp length", 0
  }
};

}
