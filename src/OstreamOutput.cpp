/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#include "OstreamOutput.h"
#include "includes.h"
#include "PcapHeaders.h"

#include <fstream>

using namespace pktanon;

OstreamOutput::~OstreamOutput()
{
  output_stream->flush();
  if ( output_stream != &std::cout )
  {
    delete output_stream;
  }
}

OstreamOutput::OstreamOutput() :
  output_stream ( 0 ), packet_buffered ( false )
{
  const auto* cfg = runtime_config.getOutputConfig<OutputFileConfig>();
  const std::string& output_file = cfg->file_name;

  if ( output_file.compare ( "stdout" ) ==  0  || output_file.compare ( "-" ) ==  0 )
  {
    output_stream = &std::cout;
    _plg_info ( "ostream: writing to stdout" );
  }
  else
  {
    output_stream = new std::ofstream ( output_file, std::fstream::out | std::fstream::trunc );
    if ( !output_stream->good() )
    {
      throw std::runtime_error ( "failed to open output file: " + output_file );
    }

    _plg_info ( "ostream: opened output file " <<  output_file );
  }
}

void OstreamOutput::write_file_header ( PCAP_FILE_HEADER* file_header )
{
  output_stream->write ( ( char* ) file_header,  sizeof ( PCAP_FILE_HEADER ) );
  output_stream->flush();
  if ( !output_stream->good())
  {
    throw std::runtime_error ( "failed to write file header" );
  }

}

int OstreamOutput::write_packet ( PCAP_REC_HEADER* record_header, uint8_t* transformed_packet_buffer )
{
  output_stream->write ( ( char* ) record_header,  sizeof ( PCAP_REC_HEADER ) );
  output_stream->write ( ( char* ) transformed_packet_buffer,  record_header->incl_len );

  return output_stream->good() ? 1 : -1;
}


