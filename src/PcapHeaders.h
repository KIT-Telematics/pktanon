/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#ifndef PKTANON_PCAPHEADERS_H
#define PKTANON_PCAPHEADERS_H

# include <cstdint>

using std::uint32_t;
using std::uint16_t;

namespace pktanon
{
static const uint32_t pcap_magic_number =  0xa1b2c3d4;
static const uint32_t pcap_swapped_magic_number = 0xd4c3b2a1;
static const uint32_t link_type_unknown = 0xffff;

# pragma pack (1)
struct PCAP_FILE_HEADER
{
  uint32_t magic_number; // always 0xa1b2c3d4
  uint16_t version_major;// 2 for now
  uint16_t version_minor;// 4 for now
  uint32_t thiszone;// GMT to local correction
  uint32_t sigfigs;// accuracy of timestamps, 0 is fine
  uint32_t snaplen;// max length of captured packets
  uint32_t network;// data link type
};
# pragma pack ()

# pragma pack (1)
struct PCAP_REC_HEADER
{
  uint32_t tv_sec;// timestamp
  uint32_t tv_usec;
  uint32_t incl_len;// number of octets of packet saved in file
  uint32_t orig_len;// actual length of packet

};
# pragma pack ()

}

#endif
