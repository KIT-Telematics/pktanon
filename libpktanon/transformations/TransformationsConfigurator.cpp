/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#include "TransformationsConfigurator.h"
//----------------------------------------------------------------------------------------------
#include "AnonFactory.h"
#include "anonprimitives/AnonPrimitive.h"
#include "transformations/PseudoheaderAwareTransformation.h"

#include <PHF.h>
#include <PHT.h>

#include "log.h"
#include "debug.h"

using namespace pktanon;

TransformationsConfigurator& TransformationsConfigurator::instance()
{
  static TransformationsConfigurator tc;
  return tc;
}

void TransformationsConfigurator::register_configurator(std::string config_name, TransformationsConfigurator::Configurator configurator)
{
  instance().supported_packets.emplace(config_name, configurator);
}

void TransformationsConfigurator::configure_packet_transformations( const PktAnonConfig& config )
{
//   PRINT("REGISTERED PACKETS[" <<  supported_packets.size() <<  "]");
//   for (auto& packet:supported_packets)
//     PRINT(packet.first);

  for (const auto& packet_config: config.get_packets())
  {
    auto it = supported_packets.find(packet_config.get_protocol());
    if (it != supported_packets.end())
    {
      (*it).second(packet_config, config);
    }
    else
    {
      _plg_error("unknown packet: " <<  packet_config.get_protocol());
    }
  }
}

void TransformationsConfigurator::configure_transformations_lookups()
{
  //TODO remove me!
  if (!payload_transformation)
  {
    throw std::runtime_error("payload packet must be configured");
  }
  
  // linktype lookup is done only once,  so just copy the data there
  Transformation::linktype_lookup_table = std::move(included_linktypes);

  // configure lookup by ethertype
  {
    
    size_t table_size;
    auto hash_fn = generate_phf(included_ethertypes,  table_size);

    PHT<uint16_t,  Transformation*> ethetype_lookup_table(hash_fn, table_size);
    
    for (auto included_ethertype :  included_ethertypes)
    {
      ethetype_lookup_table.insert(included_ethertype.first,  included_ethertype.second);
    }
    
    Transformation::ethetype_lookup_table =std::move(ethetype_lookup_table); //!MOVE!
  }
  
  //configure lookup by protocol
  {
    std::unordered_map<unsigned, Transformation*> checksum_protocols;
    checksum_protocols.reserve(included_protocols.size());

    for (auto included_protocol :  included_protocols)
    {
      if (dynamic_cast<PseudoheaderAwareTransformation*>(included_protocol.second) != nullptr)
      {
        checksum_protocols.emplace(included_protocol.first, included_protocol.second);
      }
    }
    
    // IPv6 Hop-by-Hop Option has proto value 0 and is not checksum aware
    // p is 31,  the value 31 * 16 is not valid proto value and will map to 0
    checksum_protocols.emplace(prime * 16,  nullptr);
    
    size_t table_size;
    size_t cp_table_size;
    auto hash_fn = generate_phf(included_protocols, table_size);
    auto cp_hash_fn = generate_phf(checksum_protocols, cp_table_size);

    PHT<uint8_t,  Transformation*> protocol_lookup_table(hash_fn, table_size);
    PHT<unsigned,  bool> checksums_lookup_table(cp_hash_fn, cp_table_size);
    
    for (auto included_protocol :  included_protocols)
    {
      protocol_lookup_table.insert(included_protocol.first,  included_protocol.second);
//       TRACEV((int)included_protocol.first);
//       if (dynamic_cast<PseudoheaderAwareTransformation*>(included_protocol.second) != nullptr)
//       {
//         checksums_lookup_table.insert(included_protocol.first,  true);
//       }
    }

    for (auto checksum_protocol :  checksum_protocols)
    {
      checksums_lookup_table.insert(checksum_protocol.first,  true);
//       TRACEV(checksum_protocol.first);
//       TRACEV(cp_hash_fn(checksum_protocol.first));
    }

//       TRACEV(cp_hash_fn(0));

    
    Transformation::protocol_lookup_table = std::move(protocol_lookup_table);
    Transformation::checksums_lookup_table = std::move(checksums_lookup_table);
  }
  
  // add payload packet
  Transformation::payloadTransformation = payload_transformation;
}

AnonPrimitive* TransformationsConfigurator::configure_packet_field(const string& field, const PktAnonConfig::PacketConfig& packet_config)
{
  if (!packet_config.has_field(field)) return nullptr;
 
  auto& anons_config = packet_config.get_anon_config(field);

  if (anons_config.size() == 0) throw std::runtime_error("empty configuration for field: " + field); //TODO exception or nullptr

  _plg_verbose ("\t\t" << field << "[0]: " << anons_config[0].get_anon_class());
  AnonPrimitive* first = AnonFactory::create_anon(anons_config[0].get_anon_class(),  anons_config[0].get_anon_params());
  AnonPrimitive* prev = first;

  for (int j = 1; j < anons_config.size() ; j++)
  {
    _plg_verbose("\t\t" << field << "[" << j <<"]: " << anons_config[j].get_anon_class());
    AnonPrimitive* next = AnonFactory::create_anon(anons_config[j].get_anon_class(),  anons_config[j].get_anon_params());
    prev->setNext(next);
    prev = next;
  }

  return first;
}
