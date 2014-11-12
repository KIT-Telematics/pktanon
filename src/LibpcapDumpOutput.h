/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#ifndef PKTANON_LIBPCAPDUMP_H
#define PKTANON_LIBPCAPDUMP_H

#include "LibpcapOutputSource.h"

namespace pktanon
{

class LibpcapDumpOutput : public LibpcapOutputSource
{
public:
  ~LibpcapDumpOutput();
  LibpcapDumpOutput(pcap_t* input_handle);
  
  virtual int write_packet (const struct pcap_pkthdr* pkt_header, const u_char* packet);
  
private:
  pcap_dumper_t* output_handle;
  bool packet_buffered;
};

}

#endif // PKTANON_LIBPCAPDUMP_H
