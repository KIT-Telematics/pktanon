/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#ifndef PKTANON_PHF_H
#define PKTANON_PHF_H

# include <stdint.h>
#include <functional>
#include <vector>
#include <unordered_map>
#include <set>
#include <debug.h>

namespace pktanon
{

static const unsigned prime = 31;
/// family of functions h = ((a*key + b) mod p) mod size
/// see Ramakrishna, M. V. "A Simple Perfect Hashing Method for Static Sets." ICCI. 1992.

unsigned int phf_family(unsigned a,  unsigned b,  unsigned size, unsigned key)
{
  // size will be power of two
//   return ((a * key + b) % prime ) % size;
  return ((a * key + b) % prime ) & (size - 1);
}

std::size_t round_to_pow2(std::size_t size)
{
  // check if size is a power of two (http://graphics.stanford.edu/~seander/bithacks.html#ModulusDivisionEasy)
  if ((size & (size - 1)) == 0)
  {
    return size;
  }
  
  // round to the pow2
  size--;
  size |= size >> 1;
  size |= size >> 2;
  size |= size >> 4;
  size |= size >> 8;
  size |= size >> 16;
  size++;
  
  return size;
}

template <typename UINT>
std::function<unsigned(unsigned)> generate_phf(std::unordered_map<UINT, Transformation*>& keys_values,  std::size_t& table_size)
{
  if (keys_values.size() == 0)
  {
    table_size = 0;
    return [](unsigned key) {return 0;};
  }
  
  // round size to the next power of two 
  table_size = round_to_pow2(keys_values.size());
//   TRACEV(table_size);
  
  // calculate a and b

  std::set<unsigned> hashes;
  std::function<unsigned int(unsigned)> h;

  unsigned a_0 = 1;
  unsigned b_0 = 0;
  unsigned size = table_size;

  for (unsigned a = a_0; a <= prime; a++)
  {
    for (unsigned b = b_0; b <= prime; b++)
    {

      hashes.clear();
      bool collision = false;

      h = std::bind(phf_family, a, b,  size, std::placeholders::_1);

      for (auto& key_value_pair: keys_values)
      {
        auto hash = h(key_value_pair.first);
        if (hashes.find(hash) == hashes.end())
        {
          hashes.insert(hash);
        }
        else
        {
          collision = true;
          break;
        }
      }

      if (!collision)
      {
//         TRACEV(a);
//         TRACEV(b);
//         TRACEV(size);
        return h;
      }

    }
  }

  throw std::runtime_error("phf not found");
}
}

#endif // PKTANON_PHF_H
