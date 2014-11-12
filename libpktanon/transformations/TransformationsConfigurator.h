/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#ifndef PKTANON_TRANSFORMATIONCONFIGURATOR_H
#define PKTANON_TRANSFORMATIONCONFIGURATOR_H

# include <string>
# include <functional>
# include <unordered_map>

# include <PktAnonConfig.h>

namespace pktanon
{

#define REGISTER_PACKET(PACKET,  FUNC) \
  TransformationsConfigurator::regger PACKET##regger(#PACKET,  FUNC);

class AnonPrimitive;  
class Transformation;  
  
class TransformationsConfigurator
{

public:
  typedef std::function<void(const PktAnonConfig::PacketConfig&, const PktAnonConfig&)> Configurator;

  static TransformationsConfigurator& instance();
//   static void destroy();

  static void register_configurator(std::string config_name,  Configurator configurator);
  
  struct regger { regger(std::string config_name,  Configurator configurator) { register_configurator(config_name, configurator);}; };
  
  
private:
  TransformationsConfigurator() : payload_transformation(nullptr) {};
  //----------------------------------------

public:
  void configure_packet_transformations(const PktAnonConfig& config);
  void configure_transformations_lookups();
  
  void add_linktype(uint32_t linktype, Transformation* trans) {included_linktypes.emplace(linktype, trans);};
  void add_ethertype(uint16_t ethertype, Transformation* trans) {included_ethertypes.emplace(ethertype, trans);};
  void add_protocol(uint8_t protocol, Transformation* trans) {included_protocols.emplace(protocol, trans);};
  void add_payload_transformation(Transformation* payload_transformation) {this->payload_transformation = payload_transformation;};
  
public:
  
  static AnonPrimitive* configure_packet_field(const std::string& field,  const PktAnonConfig::PacketConfig& packet_config);
  
private:
  static TransformationsConfigurator* _instance;

  std::unordered_map<std::string,  Configurator> supported_packets;
  std::unordered_map<uint32_t, Transformation*> included_linktypes;
  std::unordered_map<uint16_t, Transformation*> included_ethertypes;
  std::unordered_map<uint8_t, Transformation*> included_protocols;
  Transformation* payload_transformation;
};

}

#endif // PKTANON_TRANSFORMATIONCONFIGURATOR_H

