/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */


#include "PktAnon.h"
#include <chrono>
#include <string>

#include "ConfigSAXParser.h"
#include "transformations/Transformation.h"
#include "transformations/TransformationsConfigurator.h"
#include "transformations/ErrorCodes.h"
#include "RandomNumberGenerator.h"

#include "log.h"
#include "debug.h"

#ifdef NOXMLCONFIG
 #include "HardCodedConfig.h"
#endif

namespace pktanon
{

// define global variables
bool log_initialization = false;
bool _plg_quiet = false;
bool _plg_verbose = false;
# ifdef TRACE_ENABLED
 std::mutex dbg_mutex;
# endif

Transformation* PktAnon::link_layer_transformation = nullptr;

void PktAnon::initialize ( std::string config_filename )
{
  _plg_info ( "initializing PktAnon,  configuration = " <<  config_filename );
  _plg_verbose ( "parsing configuration file..." );
  
  PktAnonConfig config;

# ifndef NOXMLCONFIG
  try
  {
    ConfigSAXParser::instance().parseConfigFromFile ( config_filename, config);
  }
  catch ( std::exception& e )
  {
    _plg_error ( "failed to parse configuration: " <<  e.what() );
    throw e;
  }
  ConfigSAXParser::destroy();

  _plg_verbose ( "parsed configuration file." );
  _plg_verbose ( "configuring transformations..." );
# else
  // TODO fix this
  /*config =*/ HardCodedConfig::fill_pktanon_config();
# endif

  auto& tc = TransformationsConfigurator::instance();
  tc.configure_packet_transformations(config);
  tc.configure_transformations_lookups();
//   TransformationsConfigurator::destroy();

  RandomNumberGenerator::init();
  _plg_verbose ( "configured" );
}

bool PktAnon::set_link_type ( uint32_t link_type )  noexcept
{
//   TRACEV(link_type);
  link_layer_transformation = Transformation::getTransformationLinktype ( link_type );

  return link_layer_transformation !=  nullptr;
}

int PktAnon::transform_packet ( const std::uint8_t* source_buffer, std::uint8_t* destination_buffer, unsigned int max_packet_length ) noexcept
{
  assert ( link_layer_transformation != nullptr );
  return link_layer_transformation->transform ( source_buffer, destination_buffer,  max_packet_length );
}

int PktAnon::get_erroneus_packet_length ( int packet_length )
{
  return GET_LENGTH_VALUE(packet_length);
}


const char* PktAnon::get_error_string ( const int packet_length )
{
  return GET_ERROR_STRING(packet_length);
}


}
