/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#include "DefaultTransformationsConfigurator.h"
//---------------------------------------------------------------------------------
#include "log.h"

#include "AnonFactory.h"
#include "anonprimitives/AnonPrimitive.h"
#include "anonprimitives/AnonIdentity.h"
#include "transformations/TransformationsConfigurator.h"
#include "Transformation.h"
#include "ProtocolUtils.h"

#include "EthernetPacketTransformation.h"
#include "VlanTagTransformation.h"
#include "LinuxCookedCaptureTransformation.h"

#include "ArpPacketTransformation.h"

#include "IPv4PacketTransformation.h"
#include "IPv6PacketTransformation.h"

#include "UdpPacketTransformation.h"
#include "TcpPacketTransformation.h"

#include "IcmpPacketTransformation.h"
#include "ICMPv6PacketTransformation.h"

#include "PayloadTransformation.h"

using namespace pktanon;

void DefaultTransformationsConfigurator::configure_ethernet_packet ( const PktAnonConfig::PacketConfig& packet_config, const PktAnonConfig& config )
{
  _plg_verbose ("\tconfiguring ethernet packet:");

  AnonPrimitive* anon_mac_source = TransformationsConfigurator::configure_packet_field ("mac-source", packet_config);
  AnonPrimitive* anon_mac_destination = TransformationsConfigurator::configure_packet_field ("mac-dest", packet_config);
  AnonPrimitive* anon_ethertype = TransformationsConfigurator::configure_packet_field ("ethertype",  packet_config);

  EthernetPacketTransformation::PAD pad_mode = EthernetPacketTransformation::PAD::KEEP_LEN;

  const auto& params = config.get_global_parameters();

  if (params.has_param ("pad-ethernet-packets"))
  {
    const string& pad_config = params.get_param ("pad-ethernet-packets");
    if (pad_config.compare ("always") == 0)
      pad_mode = EthernetPacketTransformation::PAD::ALWAYS;
    else if (pad_config.compare ("keep-length") == 0)
      pad_mode = EthernetPacketTransformation::PAD::KEEP_LEN;
    else if (pad_config.compare ("never") == 0)
      pad_mode = EthernetPacketTransformation::PAD::NEVER;
    else
      throw std::runtime_error ("wrong value for 'pad-ethernet-packets' parameter: use always/keep-length/never");

    _plg_verbose ("\t\tpad-ethernet-packets = " << pad_config);
  }

  EthernetPacketTransformation* ether_tr =
    new EthernetPacketTransformation (anon_mac_source,  anon_mac_destination, anon_ethertype, pad_mode);

  TransformationsConfigurator::instance().add_linktype (1, ether_tr);
}

void DefaultTransformationsConfigurator::configure_vlan_tag ( const PktAnonConfig::PacketConfig& packet_config, const PktAnonConfig& config )
{
  _plg_verbose ("\tconfiguring vlan tag:");

  AnonPrimitive* anon_priority = TransformationsConfigurator::configure_packet_field ("priority", packet_config);
  AnonPrimitive* anon_cfi = TransformationsConfigurator::configure_packet_field ("cfi", packet_config);
  AnonPrimitive* anon_vlan_id = TransformationsConfigurator::configure_packet_field ("id", packet_config);
  AnonPrimitive* anon_type = TransformationsConfigurator::configure_packet_field ("ethertype", packet_config);

  VlanTagTransformation* vlan_tr = new VlanTagTransformation (anon_priority, anon_cfi, anon_vlan_id, anon_type);

  TransformationsConfigurator::instance().add_ethertype (EtherTypes::ETHERTYPE_VLAN, vlan_tr);

}

void DefaultTransformationsConfigurator::configure_linux_sll_packet( const PktAnonConfig::PacketConfig& packet_config, const PktAnonConfig& config )
{
  _plg_verbose ("\tconfiguring linux cooked capture packet:");

  AnonPrimitive* anon_packet_type = TransformationsConfigurator::configure_packet_field ("packet-type", packet_config);
  AnonPrimitive* anon_hardware_type = TransformationsConfigurator::configure_packet_field ("ll-address-type", packet_config);
  AnonPrimitive* anon_link_layer_addr = TransformationsConfigurator::configure_packet_field ("ll-address",  packet_config);
  AnonPrimitive* anon_protocol_type = TransformationsConfigurator::configure_packet_field ("protocol",  packet_config);

  LinuxCookedCaptureTransformation* sll_tr =
    new LinuxCookedCaptureTransformation (anon_packet_type,  anon_hardware_type, anon_link_layer_addr, anon_protocol_type);

  TransformationsConfigurator::instance().add_linktype (113, sll_tr);
}



void DefaultTransformationsConfigurator::configure_arp_packet ( const PktAnonConfig::PacketConfig& packet_config, const PktAnonConfig& config )
{
  _plg_verbose ("\tconfiguring arp packet:");
  AnonPrimitive* anon_hardware_type = TransformationsConfigurator::configure_packet_field ("hardware-type", packet_config);
  AnonPrimitive* anon_protocol_type = TransformationsConfigurator::configure_packet_field ("protocol-type", packet_config);
  AnonPrimitive* anon_hardware_size = TransformationsConfigurator::configure_packet_field ("hardware-size", packet_config);
  AnonPrimitive* anon_protocol_size = TransformationsConfigurator::configure_packet_field ("protocol-size", packet_config);
  AnonPrimitive* anon_opcode 	    = TransformationsConfigurator::configure_packet_field ("opcode", packet_config);
  AnonPrimitive* anon_sender_mac    = TransformationsConfigurator::configure_packet_field ("sender-mac", packet_config);
  AnonPrimitive* anon_sender_ip     = TransformationsConfigurator::configure_packet_field ("sender-ip", packet_config);
  AnonPrimitive* anon_target_mac    = TransformationsConfigurator::configure_packet_field ("target-mac", packet_config);
  AnonPrimitive* anon_target_ip     = TransformationsConfigurator::configure_packet_field ("target-ip", packet_config);


  ArpPacketTransformation* arp_tr = new ArpPacketTransformation (
    anon_hardware_type,  anon_protocol_type,
    anon_hardware_size,  anon_protocol_size,
    anon_opcode,
    anon_sender_mac, anon_sender_ip,
    anon_target_mac, anon_target_ip
  );

  TransformationsConfigurator::instance().add_ethertype (EtherTypes::ETHERTYPE_ARP,  arp_tr);
}


void DefaultTransformationsConfigurator::configure_ipv4_packet ( const PktAnonConfig::PacketConfig& packet_config, const PktAnonConfig& config )
{
  _plg_verbose ("\tconfiguring ip(v4) packet:");
  AnonPrimitive* anon_tos = TransformationsConfigurator::configure_packet_field ("tos", packet_config);
  AnonPrimitive* anon_identification = TransformationsConfigurator::configure_packet_field ("identification", packet_config);
  AnonPrimitive* anon_flags = TransformationsConfigurator::configure_packet_field ("flags", packet_config);
  AnonPrimitive* anon_fragment_offset = TransformationsConfigurator::configure_packet_field ("fragment", packet_config);
  AnonPrimitive* anon_ttl = TransformationsConfigurator::configure_packet_field ("ttl", packet_config);
  AnonPrimitive* anon_protocol = TransformationsConfigurator::configure_packet_field ("protocol", packet_config);
  AnonPrimitive* anon_source_ip = TransformationsConfigurator::configure_packet_field ("src-ip", packet_config);
  AnonPrimitive* anon_destination_ip = TransformationsConfigurator::configure_packet_field ("dest-ip", packet_config);
  AnonPrimitive* anon_options = TransformationsConfigurator::configure_packet_field ("options", packet_config);

  bool recalculate_ipv4_header_checksum = true;
  bool recalculate_payload_checksums = true;

  const auto& params = config.get_global_parameters();

  if (params.has_param ("recalculate-ipv4-header-checksum"))
  {
    const string& pad_config = params.get_param ("recalculate-ipv4-header-checksum");
    if (pad_config.compare ("yes") == 0)
      recalculate_ipv4_header_checksum = true;
    else if (pad_config.compare ("no") == 0)
      recalculate_ipv4_header_checksum = false;
    else
      throw std::runtime_error ("wrong value for 'recalculate-ipv4-header-checksum' parameter: use yes/no");

    _plg_verbose ("\t\trecalculate-ipv4-header-checksum = " << recalculate_ipv4_header_checksum);
  }

  if (params.has_param ("recalculate-payload-checksums"))
  {
    const string& pad_config = params.get_param ("recalculate-payload-checksums");
    if (pad_config.compare ("yes") == 0)
      recalculate_payload_checksums = true;
    else if (pad_config.compare ("no") == 0)
      recalculate_payload_checksums = false;
    else
      throw std::runtime_error ("wrong value for 'recalculate-payload-checksums' parameter: use yes/no");

    _plg_verbose ("\t\trecalculate-payload-checksums = " << recalculate_payload_checksums);
  }


  IPv4PacketTransformation* ip4_tr = new IPv4PacketTransformation (
    anon_tos,  anon_identification,  anon_flags,  anon_fragment_offset,
    anon_ttl, anon_protocol, anon_source_ip,  anon_destination_ip,
    anon_options, recalculate_ipv4_header_checksum, recalculate_payload_checksums
  );

  TransformationsConfigurator::instance().add_ethertype (EtherTypes::ETHERTYPE_IP,  ip4_tr);
  TransformationsConfigurator::instance().add_protocol (IPPROTO_IPIP,  ip4_tr);
}

void DefaultTransformationsConfigurator::configure_ipv6_packet ( const PktAnonConfig::PacketConfig& packet_config, const PktAnonConfig& config )
{
  _plg_verbose ("\tconfiguring ipv6 packet:");
  AnonPrimitive* anon_traffic_class  = TransformationsConfigurator::configure_packet_field ("traffic-class", packet_config);;
  AnonPrimitive* anon_flow_label  = TransformationsConfigurator::configure_packet_field ("flow-label", packet_config);;
  AnonPrimitive* anon_next_header  = TransformationsConfigurator::configure_packet_field ("next-header", packet_config);;
  AnonPrimitive* anon_hop_limit  = TransformationsConfigurator::configure_packet_field ("hop-limit", packet_config);;
  AnonPrimitive* anon_source_addr  = TransformationsConfigurator::configure_packet_field ("src-ip", packet_config);;
  AnonPrimitive* anon_destination_addr  = TransformationsConfigurator::configure_packet_field ("dest-ip", packet_config);;

  if (anon_next_header ==  nullptr)
    _plg_warn ("next-header field will be absent in the trace");

  bool recalculate_payload_checksums = true;

  const auto& params = config.get_global_parameters();

  if (params.has_param ("recalculate-payload-checksums"))
  {
    const string& pad_config = params.get_param ("recalculate-payload-checksums");
    if (pad_config.compare ("yes") == 0)
      recalculate_payload_checksums = true;
    else if (pad_config.compare ("no") == 0)
      recalculate_payload_checksums = false;
    else
      throw std::runtime_error ("wrong value for 'recalculate-payload-checksums' parameter: use yes/no");

    _plg_verbose ("\t\trecalculate-payload-checksums = " << recalculate_payload_checksums);
  }

  IPv6PacketTransformation* ipv6_tr = new IPv6PacketTransformation (
    anon_traffic_class, anon_flow_label,
    anon_next_header, anon_hop_limit,
    anon_source_addr,  anon_destination_addr,
    recalculate_payload_checksums
  );

  TransformationsConfigurator::instance().add_ethertype (EtherTypes::ETHERTYPE_IPV6,  ipv6_tr);
  TransformationsConfigurator::instance().add_protocol (IPPROTO_IPV6,  ipv6_tr);
}

void DefaultTransformationsConfigurator::configure_icmp_packet ( const PktAnonConfig::PacketConfig& packet_config, const PktAnonConfig& config )
{
  _plg_verbose ("\tconfiguring icmp(v4) packet:");
  AnonPrimitive* anon_type = TransformationsConfigurator::configure_packet_field ("type", packet_config);
  AnonPrimitive* anon_code = TransformationsConfigurator::configure_packet_field ("code", packet_config);
  AnonPrimitive* anon_misc = TransformationsConfigurator::configure_packet_field ("rest", packet_config);

  IcmpPacketTransformation* icmp_tr = new IcmpPacketTransformation (
    anon_type,  anon_code,  anon_misc
  );

  TransformationsConfigurator::instance().add_protocol (IPPROTO_ICMP,  icmp_tr);
}

void DefaultTransformationsConfigurator::configure_icmpv6_packet ( const PktAnonConfig::PacketConfig& packet_config, const PktAnonConfig& config )
{
  _plg_verbose ("\tconfiguring icmp(v6) packet:");
  AnonPrimitive* anon_type = TransformationsConfigurator::configure_packet_field ("type", packet_config);
  AnonPrimitive* anon_code = TransformationsConfigurator::configure_packet_field ("code", packet_config);
  AnonPrimitive* anon_misc = TransformationsConfigurator::configure_packet_field ("rest", packet_config);
  AnonPrimitive* anon_target_address = TransformationsConfigurator::configure_packet_field ("target-address", packet_config);

  ICMPv6PacketTransformation* icmp_tr = new ICMPv6PacketTransformation (
    anon_type,  anon_code,  anon_misc,  anon_target_address
  );

  TransformationsConfigurator::instance().add_protocol (IPPROTO_ICMPV6,  icmp_tr);
}

void DefaultTransformationsConfigurator::configure_tcp_packet ( const PktAnonConfig::PacketConfig& packet_config, const PktAnonConfig& config )
{
  _plg_verbose ("\tconfiguring tcp packet:");
  AnonPrimitive* anon_source_port =	TransformationsConfigurator::configure_packet_field ("source-port", packet_config);;
  AnonPrimitive* anon_dest_port =	TransformationsConfigurator::configure_packet_field ("dest-port", packet_config);;
  AnonPrimitive* anon_seq_num =		TransformationsConfigurator::configure_packet_field ("seq", packet_config);;
  AnonPrimitive* anon_ack_num = 	TransformationsConfigurator::configure_packet_field ("ack", packet_config);;
  AnonPrimitive* anon_flags = 		TransformationsConfigurator::configure_packet_field ("flags", packet_config);;
  AnonPrimitive* anon_window_size = 	TransformationsConfigurator::configure_packet_field ("window-size", packet_config);;
  AnonPrimitive* anon_urgent_pointer = 	TransformationsConfigurator::configure_packet_field ("urgent-pointer", packet_config);;
  AnonPrimitive* anon_options = 	TransformationsConfigurator::configure_packet_field ("options", packet_config);;

  TcpPacketTransformation* tcp_tr = new TcpPacketTransformation (
    anon_source_port,  anon_dest_port,
    anon_seq_num,  anon_ack_num,
    anon_flags,  anon_window_size,
    anon_urgent_pointer,  anon_options
  );

  TransformationsConfigurator::instance().add_protocol (IPPROTO_TCP,  tcp_tr);
}

void DefaultTransformationsConfigurator::configure_udp_packet ( const PktAnonConfig::PacketConfig& packet_config, const PktAnonConfig& config )
{
  _plg_verbose ("\tconfiguring udp packet:");
  AnonPrimitive* anon_source_port = TransformationsConfigurator::configure_packet_field ("source-port", packet_config);
  AnonPrimitive* anon_destination_port = TransformationsConfigurator::configure_packet_field ("dest-port", packet_config);

  UdpPacketTransformation* udp_tr = new UdpPacketTransformation (anon_source_port, anon_destination_port);

  TransformationsConfigurator::instance().add_protocol (IPPROTO_UDP,  udp_tr);
}

void DefaultTransformationsConfigurator::configure_payload_packet ( const PktAnonConfig::PacketConfig& packet_config, const PktAnonConfig& config )
{
  _plg_verbose ("\tconfiguring payload packet:");
  AnonPrimitive* anon_payload = TransformationsConfigurator::configure_packet_field ("payload", packet_config);
  PayloadTransformation* payload_tr = new PayloadTransformation (anon_payload);

  TransformationsConfigurator::instance().add_payload_transformation (payload_tr);
}


REGISTER_PACKET (ethernet,  DefaultTransformationsConfigurator::configure_ethernet_packet);
REGISTER_PACKET (vlan,  DefaultTransformationsConfigurator::configure_vlan_tag);
REGISTER_PACKET (linux_sll,  DefaultTransformationsConfigurator::configure_linux_sll_packet);

REGISTER_PACKET (arp,  DefaultTransformationsConfigurator::configure_arp_packet);

REGISTER_PACKET (ip,  DefaultTransformationsConfigurator::configure_ipv4_packet);
REGISTER_PACKET (ipv4,  DefaultTransformationsConfigurator::configure_ipv4_packet);
REGISTER_PACKET (ipv6,  DefaultTransformationsConfigurator::configure_ipv6_packet);

REGISTER_PACKET (icmp,  DefaultTransformationsConfigurator::configure_icmp_packet);
REGISTER_PACKET (icmpv6,  DefaultTransformationsConfigurator::configure_icmpv6_packet);

REGISTER_PACKET (tcp,  DefaultTransformationsConfigurator::configure_tcp_packet);
REGISTER_PACKET (udp,  DefaultTransformationsConfigurator::configure_udp_packet);

REGISTER_PACKET (payload,  DefaultTransformationsConfigurator::configure_payload_packet);
