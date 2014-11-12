/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#ifndef PKTANON_OSTREAMOUTPUT_H
#define PKTANON_OSTREAMOUTPUT_H

#include "OutputSource.h"
#include <iosfwd>

namespace pktanon
{

class OstreamOutput : public OutputSource
{
public:
  virtual ~OstreamOutput();
  OstreamOutput();
  
  virtual void write_file_header ( PCAP_FILE_HEADER* file_header );
  virtual int write_packet ( PCAP_REC_HEADER* metadata, uint8_t* transformed_packet_buffer );
private:
  std::ostream* output_stream;
  bool packet_buffered;
  
};
}

#endif // PKTANON_OSTREAMOUTPUT_H
