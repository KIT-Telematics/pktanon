/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#ifndef PKTANON_LIBPCAPCAPTURE_H
#define PKTANON_LIBPCAPCAPTURE_H

#include "RecordsHandler.h"
#include "pcap.h"

namespace pktanon
{

class OutputSource;
class LibpcapOutputSource;

struct pcap_pkt
{
  ~pcap_pkt() { delete[] packet;};
  pcap_pkt ( unsigned snaplen );

  unsigned snaplen;
  struct pcap_pkthdr pkt_header;
  u_char* packet;
};

class LibpcapRecordsHandler : public RecordsHandler
{
public:
  ~LibpcapRecordsHandler();
  LibpcapRecordsHandler ( Stats& stats );

  virtual void read_packets();

private:
//   static void pcap_receive_packet ( u_char* args, const struct pcap_pkthdr* pkt_header, const u_char* packet );

  pcap_t* input_handle;
  pcap_pkt* transformed_packet;
  LibpcapOutputSource* libpcap_output_source;
  OutputSource* output_source;
};

}

#endif // PKTANON_LIBPCAPCAPTURE_H
