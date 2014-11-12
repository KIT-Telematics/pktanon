/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#ifndef PKTANON_PKTANONEXEC_H
#define PKTANON_PKTANONEXEC_H

namespace pktanon
{

class PktAnonRuntime
{
public:
  static void initialize();
  static void run();
  static void cleanup();
  
  static void interrupt();
  
  static void print_statistics();
};
}

#endif // PKTANON_PKTANONEXEC_H
