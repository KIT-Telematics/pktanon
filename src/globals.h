/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

# ifndef PKTANON_GLOBALS_H
# define PKTANON_GLOBALS_H

# include <cstdint>
# include <cstdlib>
# include <atomic>

namespace pktanon {

// class PktAnonConfig;
// class ExecutionModel;
class RuntimeConfig;

// extern PktAnonConfig config;
// extern ExecutionModel* exec; 
extern const RuntimeConfig runtime_config;

// TODO 
// extern std::atomic<void*> io_metadata;// TODO delete io_metadata (after writing?)
// extern std::atomic<uint32_t> link_type;

// extern bool _plg_quiet;
// extern bool _plg_verbose;
 
using std::size_t;
using std::uint32_t;
using std::uint8_t;

}

#endif                                                      // PKTANON_GLOBALS_H
