/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#include "console_utils.h"
#include <getopt.h>
#include "log.h"
#include "debug.h"
#include "globals.h"
#include "RuntimeConfig.h"

namespace pktanon
{

static const char* default_config_file =  "profile.xml";

static struct ::option long_options[] =
{
// config
  {"config", required_argument, 0, 'c'},
// input
  {"snaplen", required_argument, 0, 's'},
  {"interface", required_argument, 0, 'i'},
  {"no-promiscuous-mode", no_argument, 0, 'p'},
  {"pcap-filter", required_argument, 0, 'e'},
  {"use-raw-sockets", no_argument, 0, 'r'},
// output
  {"packet-buffered", no_argument, 0, 'U'},
  {"socket", required_argument, 0, 'o'},
  {"use-ipv6", no_argument, 0, '6'},
  {"use-udp", no_argument, 0, 'u'},
  {"network", required_argument, 0, 'n'},
// threads
  {"threads", optional_argument, 0, 't'},
  {"separate-input", no_argument, 0, 'x'},
  {"separate-output", no_argument, 0, 'y'},

// miscellaneous
  {"use-libpcap", no_argument, 0, 'l'},
// help
  {"help", no_argument, 0, 'h'},
  {"usage", no_argument, 0, 'h'},
  {"quiet", no_argument, 0, 'q'},
  {"verbose", no_argument, 0, 'v'}
};

static const char* short_options = "c:s:i:pe:rUo:n:s:6ut::xylhqv";

int get_console_opts ( int argc, char* argv[] )
{
  // options values
  std::string config_file = default_config_file;
// input
  int snaplen = -1;
  std::string interface;
  bool promiscuous = true;
  std::string pcap_filter;
  bool use_raw_sockets = false;
// output
  bool packet_buffered = false;
  std::string sockaddr_port;
  bool use_ipv6 = false;
  bool use_udp = false;
  std::string output_interface;
// threads
  int threads = -1;
  bool separate_input = false;
  bool separate_output = false;
// misc
  bool use_libpcap_for_file_io = false;

  // getoptlong -------------------------------------------------------------------------------

  int opt;

  while ( 1 )
  {
    int option_index = 0;
    opt = ::getopt_long ( argc, argv, short_options, long_options, &option_index );

    if ( opt==-1 ) break;

    switch ( opt )
    {
      case 'c':
        config_file = optarg;
        break;
// input
      case 's':
        snaplen = std::atoi ( optarg );
        // if optarg is not a digit string or snaplen is negative
        if ( snaplen <= 0 )
        {
          print_error_message ( "invalid value for \"snaplen\", must be a positive number" );
          return -1;
        }
        break;
      case 'i':
        interface += optarg;
        break;
      case 'p':
        promiscuous = false;
        break;
      case 'e':
        pcap_filter += optarg;
        break;
      case 'r':
        use_raw_sockets = true;
        break;
// output
      case 'U':
        packet_buffered = true;
        break;
      case 'o':
        sockaddr_port += optarg;
        break;
      case '6':
        use_ipv6 = true;
        break;
      case 'u':
        use_udp = true;
        break;
      case 'n':
        output_interface += optarg;
// threads ------------------------------------------------
//       case 't':
//         if ( optarg == 0 )
//         {
//           threads = 0;
//         }
//         else
//         {
//           threads = std::atoi ( optarg );
//         }
//         break;
//       case 'x':
//         separate_input = true;
//         break;
//       case 'y':
//         separate_output = true;
//         break;
// misc ------------------------------------------------
      case 'l':
        use_libpcap_for_file_io = true;
        break;
// help ------------------------------------------------
      case 'h':
        print_usage();
        return 0;
        break;
      case 'q':
        _plg_quiet = true;
        break;
      case 'v':
        _plg_verbose = true;
        break;
      case '?':
//       TRACE("? is returned");
        return -1;
        break;
      default:
	std::cerr << "option " << opt << " is not yet implemented" << std::endl;
	return -1;
    }
  }

  // last two parameters -------------------------------------------------------------------------------

  std::string infile;
  std::string outfile;

//   TRACE ( optind );
//   TRACE ( argc );

//   for ( int j = optind; j < argc ; j++ )
//   {
//     TRACE ( argv[j] );
//   }

  if ( optind < argc )
  {
    infile += argv[optind++];

    if ( optind < argc )
    {
      outfile += argv[optind++];

      if ( optind < argc )
      {
        print_error_message ( "too many file names at the end" );
        return -1;
      }
    }
  }

  // create pktanon-runtime config  -------------------------------------------------------------------------------

  RuntimeConfig* cfg = const_cast<RuntimeConfig*> ( &runtime_config );

  cfg->setAnonProfile ( std::move ( config_file ) );

  // input:
  if ( interface.length() == 0 ) // input is files
  {
    if ( infile.length() == 0 )
    {
      print_error_message ( "no input file given" );
    }

    cfg->addInputFileConfig ( infile, use_libpcap_for_file_io );
  }
  else
  {
    cfg->addInputInterfaceConfig ( interface, promiscuous, !use_raw_sockets, pcap_filter );
    outfile = infile;
  }

  // TODO check that not all three variants are given

  // output:
  // check if socket is given:
  if ( sockaddr_port.length() != 0 )
  {
    // parse addr and port
    auto colon_pos = sockaddr_port.rfind ( ':' );
    if ( colon_pos == std::string::npos )
    {
      print_error_message ( "no addr/port given, please specify addr:port" );
      return -1;
    }
    std::string sockaddr = sockaddr_port.substr ( 0,colon_pos );
    std::string sockport_str = sockaddr_port.substr ( colon_pos+1 );
    uint16_t sockport = std::stoul ( sockport_str );
//     TRACEV ( sockaddr );
//     TRACEV ( sockport_str );
//     TRACEV ( sockport );
    cfg->addOutputSockConfig ( sockaddr, sockport, use_ipv6, use_udp );
  }
  else if ( output_interface.length() != 0 )
  {
    cfg->addOutputNetworConfig ( output_interface );
  }
  else if ( outfile.length() == 0 )
  {
    print_error_message ( "no output file given" );
    return -1;
  }
  else
  {
//     TRACEV ( use_libpcap_for_file_io );
    cfg->addOutputFileConfig ( outfile, packet_buffered, use_libpcap_for_file_io );
  }

  if ( threads >= 0 )
  {
    cfg->addThreadConfig ( threads, separate_input, separate_output );
  }

  return 1;
}

void print_error_message ( const char* error )
{
  std::cout << "Error: " << error << std::endl;
  std::cout << "Use pktanon -h for help" << std::endl;
}


void print_usage()
{
  std::cout << "pktanon -- profile-based traffic anonymizer" << std::endl;
  std::cout << "Usage: " << std::endl;
  std::cout << "\t pktanon [-c profile] input_file output_file" << std::endl;
  std::cout << "\t pktanon [options] [input file] [output file]" << std::endl;
  std::cout << "" << std::endl;
  std::cout << "Configuration:" <<std::endl;
  std::cout << "  -c, --config <config file>\t pktanon anonymization file [default: profile.xml]" <<std::endl;
  std::cout << "" << std::endl;
  std::cout << "Input Parameters:" <<std::endl;
  std::cout << "  -s, --snaplen <number>\tread first snaplen bytes from each packet" << std::endl;
  std::cout << "  \t\t\t\t(for both captures and files) [default:256]" << std::endl;
  std::cout << "  -i, --interface <interface>\tcapture pakcets from interface" << std::endl;
  std::cout << "" << std::endl;
  std::cout << "  if '--interface' option is set:" << std::endl;
  std::cout << "    -p, --no-promiscuous-mode\tdon't set interface in promiscuous mode" << std::endl;
  std::cout << "    -e, --pcap-filter <filter>\tfilter specification for libpcap" << std::endl;
  std::cout << "    -r, --use-raw-sockets\tuse linux raw sockets instead of libpcap" << std::endl;
  std::cout << "                         \t(-e option is not available)" << std::endl;
  std::cout << "" << std::endl;
  std::cout << "Output Parameters:" <<std::endl;
  std::cout << "  -U, --packet-buffered\t\toutput (to file) is flushed every N packets" << std::endl;
  std::cout << "  -n, --network <interface>\tsend packets to <interface> using libpcap-inject" << std::endl;
  std::cout << "  \t\t\t\t(EXPERIMENTAL)" << std::endl;
  std::cout << "  -o, --socket <Addr:Port>\tsend packets to Addr:Port using sockets" << std::endl;
  std::cout << "  " << std::endl;
  std::cout << "  if '--socket' option is set:" << std::endl;
  std::cout << "    -6, --use-ipv6 \t" << std::endl;
  std::cout << "    -u, --use-udp \t" << std::endl;
  std::cout << "  " << std::endl;
//   std::cout << "Multithreaded Version (EXPERIMENTAL):" << std::endl;
//   std::cout << "  *-t, --threads [<number>]\tstart multithreaded version with <number> threads." << std::endl;
//   std::cout << "  \t\t\t\tif no argument is given, hardware concurrency is used" << std::endl;
//   std::cout << "  *-x, --separate-input\t\tseparate input thread" << std::endl;
//   std::cout << "  *-y, --separate-output\tseparate output thread" << std::endl;
//   std::cout << "  " << std::endl;
  std::cout << "Miscellaneous:" << std::endl;
  std::cout << "  -l, --use-libpcap\t use libpcap for file i/o" << std::endl;
  std::cout << "  -h, --help, --usage\t print this message and exit" << std::endl;
  std::cout << "  -q, --quiet" << std::endl;
  std::cout << "  -v, --verbose" << std::endl;
  std::cout << "" << std::endl;
}


}


