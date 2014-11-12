/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#ifndef PKTANON_ISTREAMINPUT_H
#define PKTANON_ISTREAMINPUT_H

#include "RecordsHandler.h"
#include <iosfwd>


namespace pktanon
{

class OutputSource;

class IstreamInput : public RecordsHandler
{
public:
  ~IstreamInput();
  IstreamInput ( Stats& stats );
  
  virtual void read_packets();

private:
  void process_file_header();
  
  std::istream* input_stream;
  OutputSource* output_source;
  std::uint32_t snaplen;
  
};
}

#endif // PKTANON_ISTREAMINPUT_H
