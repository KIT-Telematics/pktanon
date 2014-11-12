/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#ifndef PKTANON__DEFAULTTRANSFORMATIONSCONFIGURATOR_H
#define PKTANON__DEFAULTTRANSFORMATIONSCONFIGURATOR_H

#include <PktAnonConfig.h>

namespace pktanon
{

class DefaultTransformationsConfigurator
{
public:
  static void configure_ethernet_packet(const PktAnonConfig::PacketConfig& packet_config, const pktanon::PktAnonConfig& config);
  static void configure_vlan_tag(const PktAnonConfig::PacketConfig& packet_config, const pktanon::PktAnonConfig& config);
  static void configure_linux_sll_packet(const PktAnonConfig::PacketConfig& packet_config, const pktanon::PktAnonConfig& config);
  
  static void configure_arp_packet(const PktAnonConfig::PacketConfig& packet_config, const pktanon::PktAnonConfig& config);
  
  static void configure_ipv4_packet(const PktAnonConfig::PacketConfig& packet_config, const pktanon::PktAnonConfig& config);
  static void configure_ipv6_packet(const PktAnonConfig::PacketConfig& packet_config, const pktanon::PktAnonConfig& config);
  
  static void configure_icmp_packet(const PktAnonConfig::PacketConfig& packet_config, const pktanon::PktAnonConfig& config);
  static void configure_icmpv6_packet(const PktAnonConfig::PacketConfig& packet_config, const pktanon::PktAnonConfig& config);
  
  static void configure_tcp_packet(const PktAnonConfig::PacketConfig& packet_config, const pktanon::PktAnonConfig& config);
  static void configure_udp_packet(const PktAnonConfig::PacketConfig& packet_config, const pktanon::PktAnonConfig& config);
  
  static void configure_payload_packet(const PktAnonConfig::PacketConfig& packet_config, const pktanon::PktAnonConfig& config);
};

}

#endif // PKTANON_DEFAULTTRANSFORMATIONSCONFIGURATOR_H
