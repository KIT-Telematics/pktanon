/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#include "PktAnonConfig.h"
#include "debug.h"

namespace pktanon
{

struct io_config
{
  std::string type;
  Params params;
};

PktAnonConfig::~PktAnonConfig()
{
//   delete input_config;
//   delete output_config;
}


PktAnonConfig::PktAnonConfig() /*:*/
//   input_config(new io_config()),  output_config(new io_config())
{ }

//getters ------------------------------------------------------------

// const std::string& PktAnonConfig::get_input_type() const { return input_config->type; }
// const Params& PktAnonConfig::get_input_params() const { return input_config->params; }

// const std::string& PktAnonConfig::get_output_type() const { return output_config->type; }
// const Params& PktAnonConfig::get_output_params() const { return output_config->params; }


//setters ------------------------------------------------------------

# define ADD_PARAM(KEY, VALUE) add_param(std::move(KEY),  std::move(VALUE))

void PktAnonConfig::add_global_param ( std::string&& key, std::string&& value )
{
  global_parameters.ADD_PARAM ( key, value );
}

// void PktAnonConfig::set_input_type(std::string&& type)
// {
//   input_config->type = type.data();
// }
//
// void PktAnonConfig::add_input_param(std::string&& key, std::string&& value)
// {
//   input_config->params.ADD_PARAM(key, value);
// }
//
//
// void PktAnonConfig::set_output_type(std::string&& type)
// {
//   output_config->type = type;
// }
//
// void PktAnonConfig::add_output_param(std::string&& key, std::string&& value)
// {
//   output_config->params.ADD_PARAM(key, value);
// }

void PktAnonConfig::add_packet ( std::string&& protocol )
{
  packets.emplace_back ( std::move ( protocol ) );
}

void PktAnonConfig::add_field_to_last_added_packet ( std::string&& field_name )
{
  assert ( packets.size() > 0 );

  packets[packets.size() - 1].fields[field_name] = std::vector<AnonConfig>();
}

void PktAnonConfig::add_field_to_last_added_packet ( std::string&& field_name, PktAnonConfig::AnonConfig&& anon_config )
{
  packets[packets.size() - 1].fields[field_name] = std::vector<AnonConfig>();
  packets[packets.size() - 1].fields[field_name].emplace_back ( anon_config );
}

void PktAnonConfig::set_anon_class ( AnonConfig& anon_config, std::string&& class_name )
{
  anon_config.anon_class = std::move ( class_name );
}

void PktAnonConfig::add_anon_param ( PktAnonConfig::AnonConfig& anon_config, std::string&& param_name, std::string&& param_value )
{
  anon_config.anon_params.add_param ( std::move ( param_name ),  std::move ( param_value ) );
}

void PktAnonConfig::add_anon_to_field_to_last_added_packet ( const std::string& field_name, pktanon::PktAnonConfig::AnonConfig&& anon_config )
{
  packets[packets.size() - 1].fields[field_name].emplace_back ( anon_config );
}
}

