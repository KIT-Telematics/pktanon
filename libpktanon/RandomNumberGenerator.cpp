/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#include "RandomNumberGenerator.h"
#include <random>
#include <algorithm>


using namespace pktanon;

std::random_device RandomNumberGenerator::engine;

void RandomNumberGenerator::init()
{
  // DO NOTHING
}

unsigned int RandomNumberGenerator::generate (unsigned int min, unsigned int max)
{
  std::uniform_int_distribution<unsigned int> dist (min, max);
  return dist (engine);
}

void RandomNumberGenerator::generate_random_bytes (uint8_t* buffer, size_t length)
{
  std::uniform_int_distribution<uint8_t> distribution (0, 0xff);
  for(int j = 0; j < length ; j++)
  {
    buffer[j] = distribution(engine); 
  }
  
//   std::generate (buffer, buffer + length, [distribution,engine]() {return distribution (engine);});
}


