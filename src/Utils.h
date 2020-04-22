/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 *
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence.
 * See LICENSE file found in the top-level directory of this distribution.
 */

#ifndef PKTANON_IO_UTILS_H
#define PKTANON_IO_UTILS_H

#include <Stats.h>
#include <PcapHeaders.h>
#include <PktAnon.h>
// #include <transformations/ErrorCodes.h>

# ifdef HAVE_LIBPCAP

# include <pcap.h>
# endif

namespace pktanon
{

inline void transform_packet ( PCAP_REC_HEADER& record_header, const uint8_t* original_packet, uint8_t* transformed_packet, unsigned packet_len, Stats& stats )
{
  auto new_pkt_len = PktAnon::transform_packet ( original_packet,  transformed_packet,  record_header.incl_len );
  if ( new_pkt_len >= 0 )
  {
    record_header.incl_len = new_pkt_len;
  }
  else
  {
    record_header.incl_len = PktAnon::get_erroneus_packet_length ( new_pkt_len );
    _plg_warn("error in packet " << stats.getProcessedPackets() + 1 << ": " << PktAnon::get_error_string ( new_pkt_len ));
    stats.add_error_in_packet();
  }

  stats.add_processed_packet ( record_header.orig_len );
}

# ifdef HAVE_LIBPCAP

inline void transform_packet ( struct pcap_pkthdr* pkt_header, const uint8_t* original_packet, uint8_t* transformed_packet, Stats& stats )
{
  auto new_pkt_len = PktAnon::transform_packet ( original_packet, transformed_packet,  pkt_header->caplen );
  if ( new_pkt_len < 0 )
  {
    stats.add_error_in_packet();
    pkt_header->caplen = PktAnon::get_erroneus_packet_length ( new_pkt_len );
    _plg_warn("error in packet " << stats.getProcessedPackets() + 1 << ": " << PktAnon::get_error_string ( new_pkt_len ));
  }
  else
  {
    pkt_header->caplen = new_pkt_len;
  }

  stats.add_processed_packet ( pkt_header->len );
}

# endif

}

#endif
