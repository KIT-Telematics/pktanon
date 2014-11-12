/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#ifndef PKTANON_HARDCODEDCONFIG_H
# define PKTANON_HARDCODEDCONFIG_H

namespace pktanon
{

class PktAnonConfig;

class HardCodedConfig
{
public:
  static PktAnonConfig* fill_pktanon_config();
};
}

#endif // PKTANON_TESTCONFIG_H
