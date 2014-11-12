/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#ifndef PKTANON_RUNTIMECONFIG_H
#define PKTANON_RUNTIMECONFIG_H

#include <string>

namespace pktanon
{

enum class InputType
{
  UNDEFINED, ISTREAM, LIBPCAP_DUMP, LIBPCAP_LIVE, RAW_SOCKETS
};
enum class OutputType
{
  UNDEFINED, OSTREAM, LIBPCAP_DUMP, SOCKET, LIBPCAP_INJECT //, RAW_SOCKETS
};

struct default_values
{
  static const int default_pkt_snaplen = 256;
};

struct InputFileConfig
{
  InputFileConfig ( std::string file_name, int snaplen = default_values::default_pkt_snaplen );

  const std::string file_name;
  int snaplen;
};

struct InputNetworkConfig
{
  InputNetworkConfig ( std::string interface, bool promisc, std::string pcap_filter, int snaplen = default_values::default_pkt_snaplen );

  const std::string interface;
  const bool promisc;
  const std::string pcap_filter;
  int snaplen;
};

struct OutputFileConfig
{
  OutputFileConfig ( std::string file_name, bool flush_regulary );

  const std::string file_name;
  bool packet_buffered;
};

struct OutputSocketConfig
{
  OutputSocketConfig ( const std::string address, const uint16_t port, bool use_ipv6, bool use_udp ) :
    address ( address ), port ( port ), use_ipv6 ( use_ipv6 ), use_udp ( use_udp ) {};

  const std::string address;
  const uint16_t port;
  const bool use_ipv6;
  const bool use_udp;
};

struct OutputNetworkConfig
{
  OutputNetworkConfig ( std::string interface ) : interface ( interface ) {};

  const std::string interface;
};

struct ThreadConfig
{
  ThreadConfig ( int threads_number, bool separate_input, bool separate_output ) :
    threads_number ( threads_number ), separate_input ( separate_input ), separate_output ( separate_output )
  {}

  int threads_number;
  bool separate_input;
  bool separate_output;
};

class RuntimeConfig
{
public:
  RuntimeConfig();

  const std::string& getAnonProfile() const
  {
    return anon_profile;
  };

  InputType getInputType() const
  {
    return input_type;
  }
  OutputType getOutputType() const
  {
    return output_type;
  }

  bool is_multithreaded() const
  {
    return thread_config != nullptr;
  }


  template<typename InputConfig>
  const InputConfig* const getInputConfig() const
  {
    return static_cast<InputConfig*> ( input_config );
  };
  template<typename OutputConfig>
  const OutputConfig* const getOutputConfig() const
  {
    return static_cast<OutputConfig*> ( output_config );
  };

  const ThreadConfig* const getThreadConfig() const
  {
    return thread_config;
  };

  void setAnonProfile ( std::string&& _anon_profile )
  {
    anon_profile = _anon_profile;
  };

  void addInputFileConfig ( std::string file_name, bool use_libpcap );
  void addInputInterfaceConfig ( std::string interface, bool promisc, bool use_libpcap, std::string pcap_filter = "" );

  void addOutputFileConfig ( std::string file_name, bool packet_buffered, bool use_libpcap );
  void addOutputSockConfig ( std::string sock_addr, uint16_t sock_port,bool use_ipv6,  bool use_udp );
  void addOutputNetworConfig ( std::string interface );

  void addThreadConfig ( int threads_number, bool separate_input, bool separate_output );

private:
  std::string anon_profile;

  InputType input_type;
  void* input_config;
  OutputType output_type;
  void* output_config;
  ThreadConfig* thread_config;


};

}

#endif // PKTANON_RUNTIMECONFIG_H
