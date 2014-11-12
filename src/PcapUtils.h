/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

# ifndef PKTANON_PCAPUTILS_H
# define PKTANON_PCAPUTILS_H

# include <byteswap.h>
# include <cstdlib>
# include <chrono>

# include "PcapHeaders.h"
# include "log.h"
# include "debug.h"

namespace pktanon {

/**
 * parse pcap header and return link type,  or link_type_unknown on error
 */
inline uint32_t parse_pcap_file_header(char* buffer)
{
  PCAP_FILE_HEADER* file_header = (PCAP_FILE_HEADER*) buffer;

  uint32_t version_major = 2;
  uint32_t version_minor = 4;
  uint32_t link_type = file_header->network;

  // check magic_number
  if (file_header->magic_number == pcap_magic_number)
  {
//     TRACE("magic number ok!");
  }
  else if (file_header->magic_number ==  pcap_swapped_magic_number)
  {
//     TRACE("magic number is swapped");
    bswap_32(version_major);
    bswap_32(version_minor);
    bswap_32(link_type);
  }
  else
  {
    _plg_error("wrong magic number: " <<  file_header->magic_number);
    return link_type_unknown;
  }

  if (file_header->version_major !=  version_major && file_header->version_minor != version_minor)
  {
    _plg_error("wrong pcap file version: should be 2.4");
    return link_type_unknown;
  };

  return link_type;
};

const uint32_t LINKTYPE_ETHERNET = 1;
const uint32_t LINKTYPE_LINUX_SLL = 113;

inline PCAP_FILE_HEADER* create_pcap_file_header(uint32_t snaplen,  uint32_t network)
{
  PCAP_FILE_HEADER* header = (PCAP_FILE_HEADER*)std::calloc(1, sizeof(PCAP_FILE_HEADER));
  header->magic_number = pcap_magic_number;
  header->version_major = 2;
  header->version_minor = 4;
  header->thiszone = 0;                                     // TODO
  header->sigfigs = 0;                                      // TODO
  header->snaplen = snaplen;
  header->network = network;
  return header;
}

inline void fill_pcap_record_header(PCAP_REC_HEADER* header,  uint32_t orig_len,  uint32_t incl_len)
{
  auto duration = std::chrono::system_clock::now().time_since_epoch();
  auto sec = std::chrono::duration_cast<std::chrono::seconds>(duration).count();
  auto usec = std::chrono::duration_cast<std::chrono::microseconds>(duration).count() % 1000000;

  //     TRACEV(sec <<  "." <<  usec);
  //     TRACEV((int)rcv_addr.sll_pkttype);
  //     TRACEV((int)rcv_addr.sll_ifindex);

  header->tv_sec = sec;
  header->tv_usec = usec;
  header->incl_len = incl_len;
  header->orig_len = orig_len;
}

}
#endif //  ifndef PKTANON_PCAPUTILS_H
