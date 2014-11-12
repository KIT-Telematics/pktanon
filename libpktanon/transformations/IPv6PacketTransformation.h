/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#ifndef PKTANON_IPV6PACKETTRANSFORMATION_H
#define PKTANON_IPV6PACKETTRANSFORMATION_H

# include "Transformation.h"

namespace pktanon
{

struct IPv6_HEADER
{
  uint32_t version__traffic_class__flow_label;                   // configuration item Ipv6Trafficclass and Ipv6Flowlabel
  uint16_t payload_length;
  uint8_t next_header;
  uint8_t hop_limit;                                          // configuration item Ipv6Hoplimit
  in6_addr source_addr;                                       // configuration item Ipv6Sourceaddr
  in6_addr destination_addr;                                         // configuration item Ipv6Destaddr


};

class IPv6PacketTransformation : public Transformation
{
public:
  ~IPv6PacketTransformation();
  IPv6PacketTransformation(
    AnonPrimitive* anon_traffic_class,
    AnonPrimitive* anon_flow_label,
    AnonPrimitive* anon_next_header,
    AnonPrimitive* anon_hop_limit,
    AnonPrimitive* anon_source_addr,
    AnonPrimitive* anon_destination_addr,
    bool recalculate_payload_checksums
  );

  virtual int transform(const uint8_t* source_buffer, uint8_t* destination_buffer, unsigned max_packet_length) const noexcept;

private:

  static uint32_t get_version(IPv6_HEADER* header)
  {
    return (header->version__traffic_class__flow_label & 0xF0000000) >> 28;
  }

  static void set_version(uint32_t version,  IPv6_HEADER* header)
  {
    header->version__traffic_class__flow_label =
      ((version & 0xF) << 28) | (header->version__traffic_class__flow_label & 0x0FFFFFFF);
  }

  static uint32_t get_traffic_class(IPv6_HEADER* header)
  {
    return (header->version__traffic_class__flow_label & 0x0FF00000) >> 20;
  };

  static void set_traffic_class(uint32_t traffic_class, IPv6_HEADER* header)
  {
    header->version__traffic_class__flow_label =
      ((traffic_class << 20) & 0x0FF00000)  | (header->version__traffic_class__flow_label & 0xF00FFFFF);
  }

  static uint32_t get_flow_label(IPv6_HEADER* header)
  {
    return (header->version__traffic_class__flow_label & 0x000FFFFF);
  };

  static void set_flow_label(uint32_t flow_label, IPv6_HEADER* header)
  {
    header->version__traffic_class__flow_label =
      (flow_label & 0x000FFFFF) | (header->version__traffic_class__flow_label & 0xFFF00000);
  };

  const AnonPrimitive* const anon_traffic_class;
  const AnonPrimitive* const anon_flow_label;
  const AnonPrimitive* const anon_next_header;
  const AnonPrimitive* const anon_hop_limit;
  const AnonPrimitive* const anon_source_addr;
  const AnonPrimitive* const anon_destination_addr;
  const bool recalculate_payload_checksums;
};
}

#endif // PKTANON_IPV6PACKETTRANSFORMATION_H
