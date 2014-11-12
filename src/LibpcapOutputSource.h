/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#ifndef PKTANON_LIBPCAPOUTPUTSOURCE_H
#define PKTANON_LIBPCAPOUTPUTSOURCE_H

#include <cstdint>
#include <pcap.h>

namespace pktanon
{

class LibpcapOutputSource
{
public:
  ~LibpcapOutputSource();
  LibpcapOutputSource () {};
  
  virtual int write_packet (const struct pcap_pkthdr* pkt_header, const u_char* packet) = 0;
};

inline LibpcapOutputSource::~LibpcapOutputSource() = default;

}

#endif // PKTANON_LIBPCAPOUTPUTSOURCE_H
