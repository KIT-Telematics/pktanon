/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */
 
#ifndef PKTANON__ANON_FACTORY_H
#define PKTANON__ANON_FACTORY_H

#include <unordered_map>
#include <string>
#include <functional>
#include <stdexcept>

# include "anonprimitives/AnonPrimitive.h"
#include "Params.h"

namespace pktanon
{
  
#define REGISTER_ANON(TYPE) \
  AnonFactory::creator TYPE##creator(#TYPE,  [](const Params& params) {return new TYPE();});

#define REGISTER_ANON_PARAM(TYPE,  FUNC) \
  AnonFactory::creator TYPE##creator(#TYPE,  FUNC);

class AnonFactory
{
public:
  AnonFactory();
  ~AnonFactory();

  typedef std::function<AnonPrimitive* (const Params& params)> AnonConstructor;

  static void register_anon(string anon_name, AnonConstructor constructor);

//   static AnonPrimitive* create_anon(std::string&& anon_name, std::unordered_map<string, string> attrmap);
  static AnonPrimitive* create_anon(const std::string& anon_name, const Params& params);

  struct creator
  {
    creator(std::string anon_name,  AnonConstructor constructor)
    {
      register_anon(anon_name, constructor);
    }
  };

  static std::unordered_map<std::string,  AnonConstructor>& get_mappings()
  {
    static std::unordered_map<std::string,  AnonConstructor> mappings;

    return mappings;
  }

private:
};

}
#endif // PKTANON__ANON_FACTORY_H
