/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#include "LibpcapInjectOutput.h"
#include "includes.h"

using namespace pktanon;

LibpcapInjectOutput::~LibpcapInjectOutput()
{
  pcap_close ( output_handle );
}

LibpcapInjectOutput::LibpcapInjectOutput()
{
  char errbuf[PCAP_ERRBUF_SIZE];

  const std::string& ifname = runtime_config.getOutputConfig<OutputNetworkConfig>()->interface;

  if ( ( output_handle = pcap_open_live ( ifname.c_str(), 0, 0, 0, errbuf ) ) == nullptr )
  {
    throw std::runtime_error ( std::string ( "error opening interface for sending: " ) + errbuf );
  }
  
  _plg_info("libpcap: sending packets to interface " << ifname);
}

int LibpcapInjectOutput::write_packet (const struct pcap_pkthdr* pkt_header, const u_char* packet )
{
  return pcap_sendpacket (output_handle, packet, pkt_header->caplen );
}


