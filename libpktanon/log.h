/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#ifndef PKTANON_LOG_H
#define PKTANON_LOG_H

# include <iostream>

namespace pktanon
{

extern bool _plg_quiet;
extern bool _plg_verbose;

# define LOG_STREAM std::cerr

# define _plg_error(val) \
  LOG_STREAM <<  "error: " <<  val << std::endl;

# define _plg_warn(val) \
  LOG_STREAM <<  "warning: " <<  val << std::endl;

# define _plg_info(val) \
  if (!_plg_quiet) {LOG_STREAM <<  val << std::endl;}

# define _plg_verbose(val) \
  if (_plg_verbose) {LOG_STREAM <<  val << std::endl;}

}

#endif // PKTANON_LOG_H
