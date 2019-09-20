/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#include "RuntimeFactory.h"

#include <stdexcept>

#include <globals.h>
#include <RuntimeConfig.h>

#include "IstreamRecordsHandler.h"
#include "LibpcapRecordsHandler.h"
#include "RawSocketRecordsHandler.h"
#include "OstreamOutput.h"
#include "SocketOutput.h"

using namespace pktanon;

RecordsHandler* RuntimeFactory::createRecordsHandler( Stats& stats )
{
  switch (runtime_config.getInputType())
  {
    case InputType::ISTREAM:
      return new IstreamInput(stats);
      
    case InputType::LIBPCAP_DUMP:
    case InputType::LIBPCAP_LIVE:
      return new LibpcapRecordsHandler(stats);
      
    case InputType::RAW_SOCKETS:
      return new RawSocketRecordsHandler(stats);
      
    default:
      throw std::runtime_error("internal error: input source not found");
  }  
}

OutputSource* RuntimeFactory::createOutputSource()
{
  switch (runtime_config.getOutputType())
  {
    case OutputType::OSTREAM:
      return new OstreamOutput();
      
    case OutputType::OSOCKET:
      return new SocketOutput();
      
    default:
      throw std::runtime_error("internal error: output source not found");
  }
  
}
