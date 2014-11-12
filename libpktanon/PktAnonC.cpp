/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */
 
#include "PktAnonC.h"
#include <PktAnon.h>
#include <transformations/Transformation.h>
#include <transformations/ErrorCodes.h>

static pktanon::Transformation* link_layer_trans;

void PKTANON__initialize ( char* config_file,  int verbose)
{
//   if (verbose)
//     pktanon::log_initialization = true;
  
  pktanon::PktAnon::initialize(config_file);
}

int PKTANON__set_link_type ( uint32_t linktype )
{
  return pktanon::PktAnon::set_link_type(linktype)  ?  1 : 0;
}

int PKTANON__transform_packet ( uint8_t* source_buffer, uint8_t* destination_buffer, unsigned int max_packet_length )
{
  return pktanon::PktAnon::transform_packet(source_buffer,  destination_buffer,  max_packet_length);
}

int PKTANON__get_erroneus_packet_length ( const int packet_length )
{
  return pktanon::GET_LENGTH_VALUE(packet_length);
}

const char* PKTANON__get_error_string ( const int packet_length )
{
  return pktanon::GET_ERROR_STRING(packet_length);
}
