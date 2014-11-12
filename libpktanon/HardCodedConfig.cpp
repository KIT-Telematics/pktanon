/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#include "HardCodedConfig.h"
#include "PktAnonConfig.h"
// #include "globals.h"

using namespace pktanon;

static PktAnonConfig::AnonConfig create_anon_identity_config()
{
  PktAnonConfig::AnonConfig anon_config;
  PktAnonConfig::set_anon_class(anon_config,"AnonIdentity");
  return anon_config;
}

static PktAnonConfig::AnonConfig create_anon_shorten_config()
{
  PktAnonConfig::AnonConfig anon_config;
  PktAnonConfig::set_anon_class(anon_config,"AnonShorten");
  PktAnonConfig::add_anon_param(anon_config,"newlen",  "0");
  return anon_config;
}

static PktAnonConfig::AnonConfig create_anon_hash_sha256_config()
{
  PktAnonConfig::AnonConfig anon_config;
  PktAnonConfig::set_anon_class(anon_config,"AnonHashSha256");
//   PktAnonConfig::add_anon_param(anon_config,"newlen",  "0");
  return anon_config;  
}

static PktAnonConfig::AnonConfig create_anon_white_noise_config()
{
  PktAnonConfig::AnonConfig anon_config;
  PktAnonConfig::set_anon_class(anon_config,"AnonWhitenoise");
  PktAnonConfig::add_anon_param(anon_config,"strength",  "4");
  return anon_config;  
}

static void config_identity(PktAnonConfig* config);
static void config_high(PktAnonConfig* config);

PktAnonConfig* HardCodedConfig::fill_pktanon_config()
{
//   TODO
  PktAnonConfig* conf = nullptr; // const_cast<PktAnonConfig*>(&config);

  ////// input = file (filename)
//   conf->set_input_type("file");
//   conf->add_input_param("filename",  "bigFlows.pcap");
//   conf->add_input_param("filename",  "smallFlows.pcap");
//   conf->add_input_param("filename",  "traces/ipv6trace.pcap");
//   conf->add_input_param("filename",  "traces/file.pcap"); // linux-coocked-captures
//   conf->add_input_param("filename",  "traces/iptest.pcap"); // something small
//   conf->add_input_param("filename",  "traces/arp_big.pcap"); // something small
//   conf->add_input_param("filename", "-");

  ///// input = live libpcap capture
//   conf->set_input_type("libpcap-live");
//   conf->add_input_param("interface", "enp2s1");            // interface name/any 
//   conf->add_input_param("filter", "tcp");                 // filter (as in tcpdump or wireshark)
//   conf->add_input_param("promiscuous", "yes");            // requires specififed value for interface
//     conf->add_input_param("pcap-timeout",  1000);	          // timeout value for pcap_open_life

//   conf->set_input_type("socket-raw");
//   conf->add_input_param("interface", "enp2s1");             // name/any (no auto!)
//   conf->add_input_param("promiscuous", "yes");            // requires specififed value for interface

//   conf->set_output_type("file");
//   conf->add_output_param("filename",  "out.pcap");
//   conf->add_output_param("flush-after-burst",  "yes");
//   conf->add_output_param("filename",  "-");

//   conf->set_output_type("libpcap-file");
//   conf->add_output_param("filename",  "out.pcap");
//   conf->add_output_param("flush-after-burst",  "yes");

//   conf->set_output_type("libpcap-inject");
//   conf->add_output_param("interface", "enp2s1");          // filter (as in tcpdump or wireshark)

//   conf->set_output_type("socket");
//   conf->add_output_param("use-ipv6",  "yes");             // yes|no (default no)
//   conf->add_output_param("protocol",  "udp");             // yes|no (default no)
//   conf->add_output_param("dst-host",  "::1");
//   conf->add_output_param("dst-port",  "19009");


  // make sure that ethernet payload is at least 46 bytes
  conf->add_global_param("pad-ethernet-packets", "never"); // always,keep-length,never
  conf->add_global_param("recalculate-ipv4-header-checksum", "yes"); // yes/no
  // ip and ipv6 will call checksum methods on PseudoheaderAwareTransformations
  conf->add_global_param("recalculate-payload-checksums", "yes"); // yes/no
  // don't read more than @value@ bytes from input source for each packet
  conf->add_global_param("pkt-burst-size", "32"); // numerical values
  conf->add_global_param("pkt-burst-timeout", "1000"); // numerical values
  conf->add_global_param("trim-input-packets", "256"); // numerical values

  //----------------------------------------------------------------------------
  // execution model selection

//   conf->add_global_param("multiple-threads", "yes");     // disable concurrency
  conf->add_global_param("threads-number", "1");         // auto == std::thread::hardware_concurrency()
//   conf->add_global_param("separate-input-thread", "no");  // auto == separate thread for live sources
  conf->add_global_param("separate-output-thread", "no"); // auto == no

  //----------------------------------------------------------------------------
  // execution model configuration (env_config)
  
  //----------------------------------------------------------------------------

//   config_identity(conf);
  config_high(conf);

  return conf;
}

void config_identity(PktAnonConfig* config)
{
  config->add_packet("ethernet");
  config->add_field_to_last_added_packet("mac-source",  create_anon_identity_config());
  config->add_field_to_last_added_packet("mac-dest",  create_anon_identity_config());
  config->add_field_to_last_added_packet("ethertype",  create_anon_identity_config());

  config->add_packet("linux_sll");
  config->add_field_to_last_added_packet("packet-type",  create_anon_identity_config());
  config->add_field_to_last_added_packet("ll-address-type",  create_anon_identity_config());
  config->add_field_to_last_added_packet("ll-address",  create_anon_identity_config());
  config->add_field_to_last_added_packet("protocol",  create_anon_identity_config());

  config->add_packet("ipv4");
  config->add_field_to_last_added_packet("tos",  create_anon_identity_config());
  config->add_field_to_last_added_packet("identification",  create_anon_identity_config());
  config->add_field_to_last_added_packet("flags",  create_anon_identity_config());
  config->add_field_to_last_added_packet("fragment",  create_anon_identity_config());
  config->add_field_to_last_added_packet("ttl",  create_anon_identity_config());
  config->add_field_to_last_added_packet("protocol",  create_anon_identity_config());
  config->add_field_to_last_added_packet("src-ip",  create_anon_identity_config());
  config->add_field_to_last_added_packet("dest-ip",  create_anon_identity_config());
  config->add_field_to_last_added_packet("options",  create_anon_shorten_config());

/*
  <field name="tos" 	anon="AnonIdentity"/>
  <field name="identification" 	anon="AnonIdentity"/>
  <field name="flags" 	anon="AnonIdentity"/>
  <field name="fragment" 	anon="AnonIdentity"/>
  <field name="ttl" 	anon="AnonIdentity"/>
  <field name="protocol" 	anon="AnonIdentity"/>
  <field name="src-ip" 	anon="AnonIdentity"/>
  <field name="dest-ip" 	anon="AnonIdentity"/>
  <field name="options" 	anon="AnonIdentity"/>*/

  config->add_packet("ipv6");
  config->add_field_to_last_added_packet("traffic-class",  create_anon_identity_config());
  config->add_field_to_last_added_packet("flow-label",  create_anon_identity_config());
  config->add_field_to_last_added_packet("hop-limit",  create_anon_identity_config());
  config->add_field_to_last_added_packet("src-ip",  create_anon_identity_config());
  config->add_field_to_last_added_packet("dest-ip",  create_anon_identity_config());
  config->add_field_to_last_added_packet("next-header",  create_anon_identity_config());

  //   <packet protocol="udp">
  //     <field name="source-port" anon="AnonIdentity"/>
  //     <field name="dest-port" anon="AnonIdentity"/>
  //   </packet>

  config->add_packet("udp");
  config->add_field_to_last_added_packet("source-port",  create_anon_identity_config());
  config->add_field_to_last_added_packet("dest-port",  create_anon_identity_config());

//     <packet protocol="tcp">
//       <field name="source-port" anon="AnonIdentity"/>
//       <field name="dest-port"	anon="AnonIdentity"/>
//       <field name="seq" 	anon="AnonIdentity"/>
//       <field name="ack" 	anon="AnonIdentity"/>
//       <field name="flags" 	anon="AnonIdentity"/>
//       <field name="window-size"    anon="AnonIdentity"/>
//       <field name="urgent-pointer" anon="AnonIdentity"/>
//       <field name="options"	   anon="AnonIdentity"/>
//     </packet>

  config->add_packet("tcp");
  config->add_field_to_last_added_packet("source-port",  create_anon_identity_config());
  config->add_field_to_last_added_packet("dest-port",  create_anon_identity_config());  
  config->add_field_to_last_added_packet("seq",  create_anon_identity_config());  
  config->add_field_to_last_added_packet("ack",  create_anon_identity_config());  
  config->add_field_to_last_added_packet("flags",  create_anon_identity_config());  
  config->add_field_to_last_added_packet("window-size",  create_anon_identity_config());  
  config->add_field_to_last_added_packet("urgent-pointer",  create_anon_identity_config());  
  config->add_field_to_last_added_packet("options",  create_anon_shorten_config());  

  // payload must be present
  config->add_packet("payload");
  config->add_field_to_last_added_packet("payload",  create_anon_shorten_config());
}

void config_high(PktAnonConfig* config)
{
  config->add_packet("ethernet");
  config->add_field_to_last_added_packet("mac-source",  create_anon_hash_sha256_config());
  config->add_field_to_last_added_packet("mac-dest",  create_anon_hash_sha256_config());
  config->add_field_to_last_added_packet("ethertype",  create_anon_identity_config());

  config->add_packet("linux_sll");
  config->add_field_to_last_added_packet("packet-type",  create_anon_identity_config());
  config->add_field_to_last_added_packet("ll-address-type",  create_anon_identity_config());
  config->add_field_to_last_added_packet("ll-address",  create_anon_identity_config());
  config->add_field_to_last_added_packet("protocol",  create_anon_identity_config());

  config->add_packet("ipv4");
  config->add_field_to_last_added_packet("tos",  create_anon_identity_config());
  config->add_field_to_last_added_packet("identification",  create_anon_identity_config());
  config->add_field_to_last_added_packet("flags",  create_anon_identity_config());
  config->add_field_to_last_added_packet("fragment",  create_anon_identity_config());
  config->add_field_to_last_added_packet("ttl",  create_anon_identity_config());
  config->add_field_to_last_added_packet("protocol",  create_anon_identity_config());
  config->add_field_to_last_added_packet("src-ip",  create_anon_identity_config());
  config->add_field_to_last_added_packet("dest-ip",  create_anon_identity_config());
  config->add_field_to_last_added_packet("options",  create_anon_shorten_config());

/*
  <field name="tos" 	anon="AnonIdentity"/>
  <field name="identification" 	anon="AnonIdentity"/>
  <field name="flags" 	anon="AnonIdentity"/>
  <field name="fragment" 	anon="AnonIdentity"/>
  <field name="ttl" 	anon="AnonIdentity"/>
  <field name="protocol" 	anon="AnonIdentity"/>
  <field name="src-ip" 	anon="AnonIdentity"/>
  <field name="dest-ip" 	anon="AnonIdentity"/>
  <field name="options" 	anon="AnonIdentity"/>*/

  config->add_packet("ipv6");
  config->add_field_to_last_added_packet("traffic-class",  create_anon_identity_config());
  config->add_field_to_last_added_packet("flow-label",  create_anon_identity_config());
  config->add_field_to_last_added_packet("hop-limit",  create_anon_identity_config());
  config->add_field_to_last_added_packet("src-ip",  create_anon_hash_sha256_config());
  config->add_field_to_last_added_packet("dest-ip",  create_anon_hash_sha256_config());
  config->add_field_to_last_added_packet("next-header",  create_anon_identity_config());

  //   <packet protocol="udp">
  //     <field name="source-port" anon="AnonIdentity"/>
  //     <field name="dest-port" anon="AnonIdentity"/>
  //   </packet>

  config->add_packet("udp");
  config->add_field_to_last_added_packet("source-port",  create_anon_hash_sha256_config());
  config->add_field_to_last_added_packet("dest-port",  create_anon_hash_sha256_config());

//     <packet protocol="tcp">
//       <field name="source-port" anon="AnonIdentity"/>
//       <field name="dest-port"	anon="AnonIdentity"/>
//       <field name="seq" 	anon="AnonIdentity"/>
//       <field name="ack" 	anon="AnonIdentity"/>
//       <field name="flags" 	anon="AnonIdentity"/>
//       <field name="window-size"    anon="AnonIdentity"/>
//       <field name="urgent-pointer" anon="AnonIdentity"/>
//       <field name="options"	   anon="AnonIdentity"/>
//     </packet>  
  
  config->add_packet("tcp");
  config->add_field_to_last_added_packet("source-port",  create_anon_hash_sha256_config());
  config->add_field_to_last_added_packet("dest-port",  create_anon_hash_sha256_config());  
  config->add_field_to_last_added_packet("seq",  create_anon_hash_sha256_config());  
  config->add_field_to_last_added_packet("ack",  create_anon_hash_sha256_config());  
  config->add_field_to_last_added_packet("flags",  create_anon_identity_config());  
  config->add_field_to_last_added_packet("window-size",  create_anon_identity_config());  
  config->add_field_to_last_added_packet("urgent-pointer",  create_anon_hash_sha256_config());  
  config->add_field_to_last_added_packet("options",  create_anon_shorten_config());  
  
  // payload must be present
  config->add_packet("payload");
  config->add_field_to_last_added_packet("payload",  create_anon_shorten_config());
}


