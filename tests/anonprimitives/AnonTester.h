/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#ifndef ANONTESTER_H
#define ANONTESTER_H 
// #include <AnonFactory.h>
#include <anonprimitives/AnonPrimitive.h>
#include <Params.h>
 
using namespace pktanon;

class AnonTester 
{
public:
  AnonTester();
  ~AnonTester();

  void addParam(std::string name, std::string value);
  
  void addAnon ( std::string anon_class );

  void eval(std::string val);

private:
  Params params;
  AnonPrimitive* anon;
  AnonPrimitive* last_anon;
  bool hex_output;
};

#endif // ANONTESTER_H
