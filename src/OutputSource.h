/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#ifndef PKTANON_OUTPUTSOURCE_H
#define PKTANON_OUTPUTSOURCE_H

#include <cstdint>

namespace pktanon
{

struct PCAP_FILE_HEADER;
struct PCAP_REC_HEADER;

class OutputSource
{
public:
  virtual ~OutputSource();
  OutputSource () {};

  virtual void write_file_header(PCAP_FILE_HEADER* file_header) noexcept(false) = 0;
  virtual int write_packet ( PCAP_REC_HEADER* record_header, uint8_t* transformed_packet_buffer) = 0;

protected:
};

inline OutputSource::~OutputSource() = default;

}

#endif // INPUTSOURCE_H
