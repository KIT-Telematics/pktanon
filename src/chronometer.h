/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

# ifndef PKTANON_CHRONOMETER
# define PKTANON_CHRONOMETER

# include <chrono>

namespace pktanon
{
class chronometer
{
public:
  chronometer() :
    start (std::chrono::system_clock::now())
  { }

  void reset ()
  {
    start = std::chrono::system_clock::now();
  }

  void lap_ns () const
  {
    std::chrono::duration<double> elapsed_time = std::chrono::system_clock::now() - start;
    _plg_info("elapsed time(ns): " <<  std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed_time).count() );
  }

  void lap_ms () const
  {
    std::chrono::duration<double> elapsed_time = std::chrono::system_clock::now() - start;
    _plg_info("elapsed time(mus): " <<  std::chrono::duration_cast<std::chrono::microseconds>(elapsed_time).count() );
  }
  
  size_t get_us() const
  {
    std::chrono::duration<double> elapsed_time = std::chrono::system_clock::now() - start;
    return std::chrono::duration_cast<std::chrono::microseconds>(elapsed_time).count();
  }


private:
  std::chrono::time_point<std::chrono::system_clock> start;
};
}

# endif
