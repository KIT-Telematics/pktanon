/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#ifndef PKTANON_CONFIGSAXPARSER_H
#define PKTANON_CONFIGSAXPARSER_H

#include "PktAnonConfig.h"

namespace pktanon
{

class ConfigSAXParser
{
public:
  static ConfigSAXParser& instance()
  { return *pInstance; }

  static void destroy()
  { delete pInstance;  }

  void parseConfigFromFile(const std::string& filename, PktAnonConfig& config);

private:
  ~ConfigSAXParser();
  ConfigSAXParser();

  static ConfigSAXParser* pInstance;
};

}

#endif // CONFIGPARSER_H
