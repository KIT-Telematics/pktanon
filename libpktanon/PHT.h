/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#ifndef PKTANON_PHT_H
#define PKTANON_PHT_H

// # include <utility>
# include <functional>
#include <string.h>
# include <debug.h>

namespace pktanon
{

template <typename Key,  typename Value>
class PHT
{
public:

  typedef std::pair<Key, Value> pht_entry;
  typedef std::function<std::size_t(Key)> phf;

  ~PHT();
  PHT();
  PHT(phf h, std::size_t num_entries);
  PHT(PHT&& other);
  PHT(const PHT& other) = delete;

  PHT& operator=(PHT& other) = delete;
  PHT& operator=(PHT&& other);

  void insert(Key key,  Value value);
  Value lookup(Key key, Value default_value) const;
  bool contains(Key key) const;

private:

  pht_entry* entries;
  phf h;

};

template <typename Key,  typename Value>
PHT<Key, Value>::~PHT()
{
  delete[] entries;
}

template <typename Key,  typename Value>
PHT<Key, Value>::PHT(phf h, std::size_t num_entries):
  h(h), entries(new pht_entry[num_entries])
{
  memset(entries, 0xff, num_entries * sizeof(pht_entry));
}

template <typename Key,  typename Value>
PHT<Key, Value>::PHT(): h(), entries(nullptr)
{ }

template <typename Key,  typename Value>
PHT<Key, Value>::PHT(PHT&& other):
  h(other.h),  entries(other.entries)
{
  other.entries = nullptr;
}

template <typename Key,  typename Value>
PHT<Key, Value>& PHT<Key, Value>::operator=(PHT&& other)
{
  h = other.h;

  delete[] entries;
  entries = other.entries;
  other.entries = nullptr;

  return *this;
}

//----------------------------------------------------------------------------------------------------------------------

template <typename Key,  typename Value>
void PHT<Key, Value>::insert(Key key, Value value)
{
//   TRACEV((int)key <<  " " <<  value);
  auto index = h(key);
  entries[index] = std::make_pair(key, value);
}

// template <typename Key,  typename Value>
// void PHT<Key, Value>::insert_raw(size_t index, Key key, Value value)
// {
// //   TRACE(index << " " <<  (int)key <<  " " <<  value);
//   entries[index] = std::make_pair(key, value);
// }

template <typename Key,  typename Value>
Value PHT<Key, Value>::lookup(Key key, Value default_value) const
{
  auto index = h(key);

  if (entries[index].first == key)
  {
    return entries[index].second;
  }
  else
  {
    return default_value;
  }
}

template <typename Key,  typename Value>
bool PHT<Key, Value>::contains(Key key) const
{
  auto index = h(key);
//   TRACEV((int)key);
//   TRACEV((int)entries[index].first);
  return (entries[index].first == key);
}

}

#endif // PKTANON_PHF_H
