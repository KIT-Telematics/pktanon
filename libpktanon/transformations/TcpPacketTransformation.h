/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#ifndef PKTANON_TCPPACKETTRANSFORMATION_H
#define PKTANON_TCPPACKETTRANSFORMATION_H

# include "transformations/PseudoheaderAwareTransformation.h"

namespace pktanon
{

# pragma pack (1)
struct TCP_HEADER
{
  uint16_t source_port;                                       // source port. configuration item TcpSourceport
  uint16_t dest_port;                                         // destination port. configuration item TcpDestport
  uint32_t seq_num;                                      // sequence number. configuration item TcpSeqnum
  uint32_t ack_num;                                           // acknowledgement number. configuration item TcpAcknum
  uint16_t header_len__flags;                                  //header length 4 bit, 6 bit reserved, 6 bit flags. configuration item TcpFlags. none for header length
  uint16_t window_size;                                       // window size. configuration item TcpWindowsize
  uint16_t checksum;                                         // checksum
  uint16_t urgent_pointer;                                    // urgent pointer. configuration item TcpUrgentpnt
};
# pragma pack ()

class TcpPacketTransformation : public PseudoheaderAwareTransformation
{
public:
  virtual ~TcpPacketTransformation();
  TcpPacketTransformation(
    AnonPrimitive* anon_source_port, AnonPrimitive* anon_dest_port,
    AnonPrimitive* anon_seq_num,
    AnonPrimitive* anon_ack_num,
    AnonPrimitive* anon_flags, AnonPrimitive* anon_window_size,
    AnonPrimitive* anon_urgent_pointer,
    AnonPrimitive* anon_options
  );

  virtual int transform(const uint8_t* source_buffer, uint8_t* destination_buffer, unsigned max_packet_length) const noexcept;

  virtual int calculate_checksum(uint8_t* source_address, uint8_t* destination_address, unsigned address_length, uint16_t length, uint8_t* sink_next_header) const noexcept;
private:

  static uint16_t get_header_len(TCP_HEADER* header)
  {
    return (((header->header_len__flags & 0xF000) >> 12) << 2);
  }

  static void set_header_len(uint16_t header_len,  TCP_HEADER* header)
  {
    header->header_len__flags = (((header_len / 4) & 0xF) << 12) | (get_flags(header) & 0x3F);
  }

  static uint16_t get_flags(TCP_HEADER* header)
  {
    return ((header->header_len__flags & 0x3F ));

  }

  static void set_flags(uint16_t flags,  TCP_HEADER* header)
  {
    header->header_len__flags =
      (((get_header_len(header) / 4) & 0xF) << 12) | (flags & 0x3F);
  }



  AnonPrimitive* anon_source_port;
  AnonPrimitive* anon_dest_port;
  AnonPrimitive* anon_seq_num;
  AnonPrimitive* anon_ack_num;
  AnonPrimitive* anon_flags;
  AnonPrimitive* anon_window_size;
  AnonPrimitive* anon_urgent_pointer;
  AnonPrimitive* anon_options;
};
}

#endif // PKTANON_TCPPACKETTRANSFORMATION_H
