/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#ifndef __RANDOM_NUMBER_GENERATOR_H
#define __RANDOM_NUMBER_GENERATOR_H

#include <stdint.h>
#include <random>

namespace pktanon
{
class RandomNumberGenerator
{
public:
  static void init();

  static unsigned int generate (unsigned int rangemin, unsigned int rangemax);
  static void generate_random_bytes (uint8_t* buffer, size_t length);
private:
  static std::random_device engine;
};

}

#endif // __RANDOM_NUMBER_GENERATOR_H
