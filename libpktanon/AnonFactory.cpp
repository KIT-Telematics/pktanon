/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#include "AnonFactory.h"

#include <iostream>
#include <string>

#include "debug.h"

namespace pktanon
{

AnonFactory::~AnonFactory() { }
AnonFactory::AnonFactory() { }

void AnonFactory::register_anon(string anon_name, pktanon::AnonFactory::AnonConstructor constructor)
{
  get_mappings().emplace(anon_name,  constructor);
}

AnonPrimitive* AnonFactory::create_anon(const string& anon_name, const pktanon::Params& params)
{
  auto constructor = get_mappings().find(anon_name);
  if (constructor == get_mappings().end())
    throw std::runtime_error("anon primitive " + anon_name + " not found");

  return (*constructor).second(params);
}

}

