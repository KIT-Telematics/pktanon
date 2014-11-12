/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#include "Transformation.h"
#include <debug.h>

namespace pktanon
{
Transformation::~Transformation() = default;

std::unordered_map<uint32_t,  Transformation*> Transformation::linktype_lookup_table;
PHT<uint16_t,Transformation*> Transformation::ethetype_lookup_table;
PHT<uint8_t,Transformation*> Transformation::protocol_lookup_table;
PHT<unsigned,bool> Transformation::checksums_lookup_table;
Transformation*	Transformation::payloadTransformation = nullptr;

Transformation* Transformation::getTransformationLinktype(uint32_t linktype)
{
  auto iter = linktype_lookup_table.find(linktype);
  if (iter != linktype_lookup_table.end()) return (*iter).second;

  return nullptr;
}

}
