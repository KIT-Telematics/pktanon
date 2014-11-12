/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#include "LibpcapDumpOutput.h"
#include "includes.h"

using namespace pktanon;

LibpcapDumpOutput::~LibpcapDumpOutput()
{
  pcap_dump_flush ( output_handle );
  pcap_dump_close ( output_handle );
}

LibpcapDumpOutput::LibpcapDumpOutput ( pcap_t* input_handle ) :
  output_handle ( nullptr )
{
  char errbuf[PCAP_ERRBUF_SIZE];

  const OutputFileConfig* cfg = runtime_config.getOutputConfig<OutputFileConfig>();
  if ( ( output_handle = pcap_dump_open ( input_handle, cfg->file_name.c_str() ) ) == nullptr )
  {
    throw std::runtime_error ( std::string ( "Error opening savefile for writing: " ) + errbuf );
  }
  _plg_info ( "libpcap: opened output file " <<   cfg->file_name );

  packet_buffered = cfg->packet_buffered;
}

int LibpcapDumpOutput::write_packet (const struct pcap_pkthdr* pkt_header, const u_char* packet)
{
  pcap_dump ( ( u_char* ) output_handle, pkt_header, packet );
  if ( packet_buffered )
    pcap_dump_flush ( output_handle );
  
  return 1;
}


