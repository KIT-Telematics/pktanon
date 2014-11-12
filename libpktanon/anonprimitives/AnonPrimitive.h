/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#ifndef PKTANON__ANON_PRIMITIVE_H
#define PKTANON__ANON_PRIMITIVE_H

#include <string>

using std::string;
using std::min;

namespace pktanon
{

class Params;

class AnonPrimitive
{
public:
  AnonPrimitive();
  virtual ~AnonPrimitive();

  AnonPrimitive(const AnonPrimitive& other) = delete;
  AnonPrimitive& operator= (const AnonPrimitive& other) = delete;

  unsigned int anonimyze(const void* const src_buff, void* dst_buff, unsigned int len) const;

  AnonPrimitive* getNext() const;
  void setNext(AnonPrimitive* nextprim);

protected:

  struct ANON_RESULT
  {
    unsigned int newlength;
    bool call_next_anon;

    ANON_RESULT() : newlength(0),  call_next_anon(false) { }
    
    ANON_RESULT(unsigned int newlength, bool call_next_anon = true) : newlength(newlength),  call_next_anon(call_next_anon) {}
    
    ANON_RESULT& operator&=(const ANON_RESULT& rhs)
    {
      newlength = std::min(newlength,  rhs.newlength);
      call_next_anon &=  rhs.call_next_anon;

      return *this;
    }
  };

  virtual ANON_RESULT anonymize_internal(const void* const src_buff, void* dst_buff, unsigned int len) const;
  virtual ANON_RESULT anonymize_chain(void* buf, unsigned int len) const = 0;

private:
  AnonPrimitive* next;
};

}

#endif // __ANON_PRIMITIVE_H
