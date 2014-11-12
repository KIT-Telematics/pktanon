/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#ifndef PKTANON_PKTANONCONFIG_H
#define PKTANON_PKTANONCONFIG_H

# include <string>
# include <unordered_map>
# include <vector>
# include <ostream>
# include <assert.h>

# include "Params.h"

namespace pktanon
{

struct io_config;

/**
 * PktAnon Configuration
 */
class PktAnonConfig
{
public:
  ~PktAnonConfig();
  PktAnonConfig();

  // structures
  
  class AnonConfig 
  {
  public:
    AnonConfig() {};
    AnonConfig(std::string&& anon_class,  Params&& attributes) :
    anon_class(std::move(anon_class)),  anon_params(std::move(attributes))
    {};
    
    const std::string& get_anon_class() const { return anon_class;};
    const Params& get_anon_params() const { return anon_params;};

  private:
    friend class PktAnonConfig;
    std::string anon_class;
    Params anon_params;
  };
  
  class PacketConfig
  {
  public:
    PacketConfig(std::string&& protocol) : protocol(std::move(protocol)) {}
    
    const std::string& get_protocol() const {return protocol;}
    
    bool has_field(const std::string& field_name)  const { return fields.find(field_name) !=  fields.end();}
    const std::vector<AnonConfig>& get_anon_config(const std::string& field_name) const {
      assert(has_field(field_name));
      return (*fields.find(field_name)).second;
    }

  private:
    friend class PktAnonConfig;
    
    const std::string protocol;
    std::unordered_map<std::string, std::vector<AnonConfig>> fields;
  };


  // reading
  const Params& get_global_parameters() const {return global_parameters;};

//   const std::string& get_input_type() const;
//   const Params& get_input_params() const;

//   const std::string& get_output_type() const;
//   const Params& get_output_params() const;

  const std::vector<PacketConfig>& get_packets() const {return packets;};

  static bool has_value(const std::string& param_value, const std::string&& expected_value) { return param_value.compare(expected_value) ==  0;}

  // modification
  void add_global_param(std::string&& key, std::string&& value);

//   void set_input_type(std::string&& type);
//   void add_input_param(std::string&& key, std::string&& value);

//   void set_output_type(std::string&& type);
//   void add_output_param(std::string&& key, std::string&& value);

  void add_packet(std::string&& protocol);
  void add_field_to_last_added_packet(std::string&& field_name);
  void add_field_to_last_added_packet(std::string&& field_name,  AnonConfig&& anon_config);
  void add_anon_to_field_to_last_added_packet(const std::string& field_name,  AnonConfig&& anon_config);
  static void set_anon_class(AnonConfig& anon_config,  std::string&& class_name);
  static void add_anon_param(AnonConfig& anon_config,  std::string&& param_name,  std::string&& param_value);
  
private:

//   io_config* input_config;
//   io_config* output_config;

  Params global_parameters;

  std::vector<PacketConfig> packets;

};

// inline std::ostream& operator<<(std::ostream& os,  PktAnonConfig& cfg)
// {
//   os <<  "PacketAnon Configuration: " <<  std::endl;
//   os <<  "\tinput file name: " << cfg.input_file <<   std::endl;
//   os <<  "\toutput file name: " << cfg.output_file <<   std::endl;
//   os <<  "\tparameters(size = " << cfg.parameters.size() << "): "  <<  std::endl;
//   for (auto& param :  cfg.parameters)
//   {
//     os <<  "\t\t" <<  param.first <<  ": " <<  param.second <<  std::endl;
//   }
//   os <<  "\tpackets configuration(size = " << cfg.packets.size() << "): "  <<  std::endl;
//   for (auto& pkt :  cfg.packets)
//   {
//     os <<  "\t\t" <<  pkt.pkt_name << " (fieldsnum= " <<  pkt.fields.size() << "):"<<  std::endl;
//     for (auto& field: pkt.fields)
//     {
//       os << "\t\t\t" <<  field.first <<  std::endl;
//       for (auto& anon: field.second)
//       {
//         os << "\t\t\t\t" <<  anon.anon_class <<  std::endl;
//         for (auto& attr: anon.attributes)
//         {
//           os << "\t\t\t\t\t" <<  attr.first << ": " <<  attr.second <<  std::endl;
//         }
//       }
//     }
//   }
// 
//   return os;
// }

}

#endif // PKTANON_PKTANONCONFIG_H

