/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#ifndef PKTANON_INPUTSOURCE_H
#define PKTANON_INPUTSOURCE_H

#include "Stats.h"

namespace pktanon
{

class RecordsHandler
{
public:
  virtual ~RecordsHandler();
  RecordsHandler ( Stats& stats ) : stats ( stats ) {};

  void start_collecting_stats() { stats.start_collecting(); };
  void stop_collecting_stats() { stats.stop_collecting(); };

  virtual void read_packets() = 0;

protected:
  Stats& stats;
};

inline RecordsHandler::~RecordsHandler() = default;

}

#endif // INPUTSOURCE_H
