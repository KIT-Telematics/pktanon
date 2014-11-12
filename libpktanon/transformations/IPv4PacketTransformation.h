/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#ifndef PKTANON_IPV4TRANSFORMATION_H
#define PKTANON_IPV4TRANSFORMATION_H
# include "Transformation.h"

namespace pktanon
{

# pragma pack (1)
struct IPv4_HEADER
{
  uint8_t version__header_len;
  uint8_t tos;
  uint16_t total_length;
  uint16_t identification;
  uint16_t flags__fragment_offset;
  uint8_t ttl;
  uint8_t protocol;
  uint16_t header_checksum;
  in_addr source_ip;
  in_addr destination_ip;
};
# pragma pack ()


class IPv4PacketTransformation : public Transformation
{
public:
  ~IPv4PacketTransformation();
  /**
   * If you have a procedure with 10 parameters, you probably missed some.
   */
  IPv4PacketTransformation (
    AnonPrimitive* anon_tos,
    AnonPrimitive* anon_identification,
    AnonPrimitive* anon_flags,
    AnonPrimitive* anon_fragment_offset,
    AnonPrimitive* anon_ttl,
    AnonPrimitive* anon_protocol,
    AnonPrimitive* anon_source_ip,
    AnonPrimitive* anon_destination_ip,
    AnonPrimitive* anon_options,
    bool recalculate_ipv4_header_checksum,
    bool recalculate_payload_checksums
  );

  virtual int transform (const uint8_t* source_buffer, uint8_t* destination_buffer, unsigned max_packet_length) const noexcept;



  static uint8_t get_version (IPv4_HEADER* header)
  {
    return ( (header->version__header_len &	0xF0) >>  4);
  }

  static void set_version (uint8_t version, IPv4_HEADER* header)
  {
    header->version__header_len = ( (version & 0xF) << 4) | (get_header_len (header) / 4);
  }

  static uint8_t get_header_len (IPv4_HEADER* header)
  {
    return ( (header->version__header_len &	0x0F) <<  2);
  }

  static void set_header_len (uint16_t len, IPv4_HEADER* header)
  {
    header->version__header_len = (get_version (header) << 4) | ( ( (len / 4) & 0xF));
  }

  static uint16_t get_flags (IPv4_HEADER* header)
  {
    return ( (header->flags__fragment_offset &	0xE000) >> 13);
  }

  static void set_flags (uint16_t flags,  IPv4_HEADER* header)
  {
    header->flags__fragment_offset = ( (uint16_t) (flags & 0x07) << 13) | get_fragment_offset (header);
  }

  static uint16_t get_fragment_offset (IPv4_HEADER* header)
  {
    return ( (header->flags__fragment_offset &	0x1FFF)) ;
  }

  static void set_fragment_offset (uint16_t fragment_offset,  IPv4_HEADER* header)
  {
    header->flags__fragment_offset = ( (get_flags (header) & 0x07) << 13) | (0xD & fragment_offset);
  }

private:
  const AnonPrimitive* const anon_tos;
  const AnonPrimitive* const anon_identification;
  const AnonPrimitive* const anon_flags;
  const AnonPrimitive* const anon_fragment_offset;
  const AnonPrimitive* const anon_ttl;
  const AnonPrimitive* const anon_protocol;
  const AnonPrimitive* const anon_source_ip;
  const AnonPrimitive* const anon_destination_ip;
  const AnonPrimitive* const anon_options;
  const bool recalculate_ipv4_header_checksum;
  const bool recalculate_payload_checksums;
};
}

#endif // PKTANON_IPV4TRANSFORMATION_H
