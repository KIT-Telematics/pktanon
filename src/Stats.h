/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#ifndef PKTANON_STATS_H
#define PKTANON_STATS_H

# include <chrono>
# include <cstdlib>

namespace pktanon
{

class Stats
{
public:

  void start_collecting()
  {
    processed_packets = 0;
    processed_bytes = 0;
    errors_in_packets = 0;
    start = std::chrono::system_clock::now();
  }

  void stop_collecting()
  {
    end = std::chrono::system_clock::now();
  }

  void add_processed_packet ( unsigned int _processed_bytes = 0 )
  {
    processed_packets++;
    processed_bytes+=_processed_bytes;
  }

  void add_error_in_packet()
  {
    errors_in_packets++;
  }

  void print_statistics();

  unsigned int getProcessedPackets()
  {
    return processed_packets;
  };
  unsigned int getErrorsInPackets()
  {
    return errors_in_packets;
  };

  int getElapsedTimeUS()
  {
    return std::chrono::duration_cast<std::chrono::microseconds> ( end - start ).count();
  };

  float getElapsedTimeS()
  {
    using FpSeconds = std::chrono::duration<float, std::chrono::seconds::period>;
    auto elapsed_seconds = FpSeconds ( end -start );
    return elapsed_seconds.count();
  };

  float getMPPS()
  {
    return ( float ) processed_packets / (float) getElapsedTimeUS();
  };

private:
  unsigned int processed_packets;
  unsigned int processed_bytes;
  unsigned int errors_in_packets;

  std::chrono::time_point<std::chrono::system_clock> start;
  std::chrono::time_point<std::chrono::system_clock> end;

};
}

#endif // PKTANON_MEMPOOL_IMPL_H
