/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#include "RuntimeConfig.h"

using namespace pktanon;

InputFileConfig::InputFileConfig ( std::string file_name, int snaplen ) :
  file_name ( file_name ), snaplen ( snaplen ) {}

InputNetworkConfig::InputNetworkConfig ( std::string interface, bool promisc, std::string pcap_filter, int snaplen ) :
  interface ( interface ), promisc ( promisc ), pcap_filter ( pcap_filter ), snaplen ( snaplen ) { }

OutputFileConfig::OutputFileConfig ( std::string file_name, bool flush_regulary ) :
  file_name ( file_name ), packet_buffered ( flush_regulary ) { }


//--------------------------------------------------------------------------------------------------------------------------------

RuntimeConfig::RuntimeConfig() :
  input_type ( InputType::UNDEFINED ), input_config ( nullptr ),
  output_type ( OutputType::UNDEFINED ), output_config ( nullptr ),
  thread_config(nullptr)
{ }

void RuntimeConfig::addInputFileConfig ( std::string file_name, bool use_libpcap )
{
  input_type = use_libpcap ? InputType::LIBPCAP_DUMP : InputType::ISTREAM;
  input_config= new InputFileConfig ( file_name );
}

void RuntimeConfig::addInputInterfaceConfig ( std::string interface, bool promisc, bool use_libpcap, std::string pcap_filter )
{
  input_type = use_libpcap ? InputType::LIBPCAP_LIVE : InputType::RAW_SOCKETS;
  input_config = new InputNetworkConfig ( interface, promisc, pcap_filter );
}

void RuntimeConfig::addOutputFileConfig ( std::string file_name, bool packet_buffered, bool use_libpcap )
{
  output_type = use_libpcap ? OutputType::LIBPCAP_DUMP : OutputType::OSTREAM; // TODO do not ignore use_libpcap
  output_config = new OutputFileConfig ( file_name, packet_buffered );
}

void RuntimeConfig::addOutputSockConfig ( std::string sock_addr, uint16_t sock_port, bool use_ipv6, bool use_udp )
{
  output_type = OutputType::SOCKET;
  output_config = new OutputSocketConfig ( sock_addr, sock_port, use_ipv6, use_udp );
}

void RuntimeConfig::addOutputNetworConfig ( std::string interface )
{
  output_type = OutputType::LIBPCAP_INJECT;
  output_config = new OutputNetworkConfig ( interface );
}

void RuntimeConfig::addThreadConfig ( int threads_number, bool separate_input, bool separate_output )
{
  thread_config = new ThreadConfig( threads_number, separate_input, separate_output);
}

