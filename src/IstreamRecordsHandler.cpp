/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#include "IstreamRecordsHandler.h"
#include "RuntimeFactory.h"

#include <stdexcept>
#include <fstream>

#include <RuntimeConfig.h>
#include <PcapHeaders.h>
#include "PcapUtils.h"
#include "OutputSource.h"
#include "Utils.h"
#include <globals.h>
#include <log.h>
#include <PktAnon.h>

using namespace pktanon;

IstreamInput::~IstreamInput()
{
  delete output_source;
  if ( input_stream != &std::cin )
  {
    delete input_stream;
  }
}

IstreamInput::IstreamInput ( Stats& stats ) :
  RecordsHandler ( stats ),
  input_stream ( 0 ), output_source ( 0 )
{
  // open input file/stdin
  const auto* cfg = runtime_config.getInputConfig<InputFileConfig>();
  const std::string& input_file = cfg->file_name;

  if ( input_file.compare ( "stdin" ) ==  0  || input_file.compare ( "-" ) ==  0 )
  {
    if ( freopen ( nullptr, "rb", stdin ) ==  0 )
    {
      throw std::runtime_error ( "cannot reopen std::cin in binary mode" );
    }
    input_stream = &std::cin;
    _plg_info ( "istream: reading packets from stdin" );
  }
  else
  {
    input_stream = new std::ifstream ( input_file, std::fstream::in | std::fstream::binary );
    if ( !input_stream->good() )
      throw std::runtime_error ( "failed to open input file: " + input_file );

    _plg_info ( "istream: opened file " <<  input_file );
  }

  snaplen = cfg->snaplen;
  output_source = RuntimeFactory::createOutputSource();
}

void IstreamInput::process_file_header()
{
  PCAP_FILE_HEADER* file_header = new PCAP_FILE_HEADER();
  input_stream->read ( ( char* ) file_header,  sizeof ( PCAP_FILE_HEADER ) );

  if ( input_stream->gcount() != sizeof ( PCAP_FILE_HEADER ) )
  {
    throw std::runtime_error ( "istream: failed to read pcap file header, aborting" );
  }

  auto link_type = parse_pcap_file_header ( ( char* ) file_header );

  if ( !PktAnon::set_link_type ( link_type ) )
  {
    throw std::runtime_error ( "unsupported link type: " + std::to_string ( file_header->network ) );
  }

  output_source->write_file_header ( file_header );
}


void IstreamInput::read_packets()
{
  process_file_header();

  //-------------------------------------------------------------

  PCAP_REC_HEADER record_header;
  uint8_t* original_packet = ( uint8_t* ) alloca ( snaplen );
  uint8_t* transformed_packet = ( uint8_t* ) alloca ( snaplen );

  while ( 1 )
  {
    // read pcap_rec_header
    input_stream->read ( ( char* ) &record_header,  sizeof ( PCAP_REC_HEADER ) );

    if ( input_stream->gcount() != sizeof ( PCAP_REC_HEADER ) ) { break; }

//     TRACEV ( record_header.incl_len );

    // read packet length from header and read packet body (parts)
    const std::streamsize pkt_snaplen = std::min ( record_header.incl_len, this->snaplen );
    input_stream->read ( ( char* ) original_packet, pkt_snaplen );

    if ( input_stream->gcount() != pkt_snaplen )
    {
      _plg_error ( "istream*: reading pcap record body from file" );
      break;
    }

    // ignore rest of the packet
    if ( pkt_snaplen < record_header.incl_len )
    {
      input_stream->ignore ( record_header.incl_len - pkt_snaplen );
    }

    // transform packet
    transform_packet ( record_header, original_packet, transformed_packet, pkt_snaplen, stats );

    // write transformed packet
    if ( output_source->write_packet ( &record_header, transformed_packet ) < 0 ) { break; }
  }

  return;

}

