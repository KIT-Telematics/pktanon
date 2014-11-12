/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#include "PktAnonRuntime.h"

#include <iomanip>

#include "log.h"
#include "globals.h"
#include "PktAnon.h"
#include "PktAnonConfig.h"
#include "RuntimeConfig.h"
#include "Stats.h"
#include "RecordsHandler.h"
#include "RuntimeFactory.h"

namespace pktanon
{
  
const RuntimeConfig runtime_config; // from globals.h
static Stats stats;
static RecordsHandler* records_handler;

void PktAnonRuntime::initialize ()
{
  PktAnon::initialize ( runtime_config.getAnonProfile() );
  records_handler = RuntimeFactory::createRecordsHandler(stats);
}

void PktAnonRuntime::run()
{
    records_handler->start_collecting_stats();
    records_handler->read_packets();
    records_handler->stop_collecting_stats();
}

void PktAnonRuntime::cleanup()
{
  delete records_handler;
}

void PktAnonRuntime::interrupt()
{
  records_handler->stop_collecting_stats();
  delete records_handler;
}

void PktAnonRuntime::print_statistics()
{
//   HB();
// Statistics for network device: lo
//         Attempted packets:         100
//         Successful packets:        100
//         Failed packets:            0
//         Truncated packets:         0
//         Retried packets (ENOBUFS): 0
//         Retried packets (EAGAIN):  0

  _plg_info ( "" );
  switch ( runtime_config.getInputType() )
  {
    case InputType::ISTREAM:
    case InputType::LIBPCAP_DUMP:
      _plg_info ( "statistics for input file '" << runtime_config.getInputConfig<InputFileConfig>()->file_name << "'" );
      break;
  }

  _plg_info ( "  processed packets: " << stats.getProcessedPackets() );
  _plg_info ( "  errors in packets: " << stats.getErrorsInPackets() );
  switch ( runtime_config.getInputType() )
  {
    case InputType::ISTREAM:
    case InputType::LIBPCAP_DUMP:
      _plg_info ( "  elapsed time:      " << stats.getElapsedTimeUS() << "us" );
      break;
    case InputType::LIBPCAP_LIVE:
    case InputType::RAW_SOCKETS:
      _plg_info ( "  elapsed time:      " << std::setprecision ( 3 ) << stats.getElapsedTimeS() << "s" );
      break;
  }
  _plg_info ( "  Mpps:              " << std::setprecision ( 3 ) << stats.getMPPS() );
}


}

