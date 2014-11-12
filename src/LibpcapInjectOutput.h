/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#ifndef PKTANON_LIBPCAPINJECT_H
#define PKTANON_LIBPCAPINJECT_H
#include "LibpcapOutputSource.h"

namespace pktanon
{

class LibpcapInjectOutput : public LibpcapOutputSource
{
public:
  ~LibpcapInjectOutput();
  LibpcapInjectOutput();
  
  virtual int write_packet (const struct pcap_pkthdr* pkt_header, const u_char* packet );
  
private:
  pcap_t* output_handle;
  
};
}

#endif // PKTANON_LIBPCAPINJECT_H
