/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#ifndef PKTANON_RAWSOCKETCAPTURE_H
#define PKTANON_RAWSOCKETCAPTURE_H

#include "RecordsHandler.h"

namespace pktanon
{

class OutputSource;

class RawSocketRecordsHandler : public RecordsHandler
{
public:
  ~RawSocketRecordsHandler();
  RawSocketRecordsHandler ( Stats& stats );

  virtual void read_packets();

private:
  int socket_descriptor;
  std::size_t snaplen;
  OutputSource* output_source;

};

}
#endif // PKTANON_RAWSOCKETCAPTURE_H
