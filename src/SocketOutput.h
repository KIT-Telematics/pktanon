/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#ifndef PKTANON_SOCKETOUTPUT_H
#define PKTANON_SOCKETOUTPUT_H

#include "OutputSource.h"

namespace pktanon
{

class SocketOutput : public OutputSource
{
public:
  virtual ~SocketOutput();
  SocketOutput();

  virtual void write_file_header ( PCAP_FILE_HEADER* file_header );
  virtual int write_packet ( PCAP_REC_HEADER* record_header, uint8_t* transformed_packet_buffer );

private:
  int socket_descriptor;
};
}

#endif // PKTANON_SOCKETOUTPUT_H
