/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#ifndef PKTANON__CONSOLE_UTILS_H
#define PKTANON__CONSOLE_UTILS_H
#include <string>

namespace pktanon {

int get_console_opts(int argc, char* argv[]);

void print_error_message(const char* error);
void print_usage();

}
#endif
