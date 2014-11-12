/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#include <getopt.h>
#include <iostream>

// #include "PktAnon.h"
#include "PktAnonRuntime.h"
#include "log.h"
#include "console_utils.h" 

#include <signal.h>

// std::mutex dbg_mutex;

using namespace pktanon;

volatile sig_atomic_t termination_in_progress = 0;

void termination_signal_handler ( int signal )
{
  _plg_warn ( "interrupted: signal = " <<  signal );

  if ( termination_in_progress ) raise ( signal );
  termination_in_progress = 1;
  
  PktAnonRuntime::interrupt();
  PktAnonRuntime::print_statistics();
  PktAnonRuntime::cleanup();

  raise ( signal );
}

int main ( int argc, char* argv[] )
{
  // speed up std::cout
  std::ios_base::sync_with_stdio ( false );

  // parse options
  int res = get_console_opts ( argc, argv );
  if ( res < 0 )
  {
    return EXIT_FAILURE;
  }
  else if ( res == 0 )
  {
    return EXIT_SUCCESS;
  }

  // set signal handlers
  // TODO what to do with multithreaded version
  signal ( SIGTERM,  termination_signal_handler );
  signal ( SIGINT,   termination_signal_handler );
  signal ( SIGQUIT,  termination_signal_handler );

  // start pktanon
  
  _plg_info ( "-----------------------------------------------" );
  _plg_info ( "pktanon --- profile-based traffic anonymization" );
  _plg_info ( "-----------------------------------------------" );

  int exit_status = EXIT_SUCCESS;

  try
  {
    PktAnonRuntime::initialize ();
    _plg_info ( "initialized" );

    PktAnonRuntime::run();
    _plg_info ( "complete" );
    
    PktAnonRuntime::print_statistics();

  }
  catch ( std::exception& e )
  {
    _plg_error ( e.what() );
    exit_status = EXIT_FAILURE;
  }

  PktAnonRuntime::cleanup();

  return exit_status;
}
