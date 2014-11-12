/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 *
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence.
 * See LICENSE file found in the top-level directory of this distribution.
 */
#ifndef PKTANON_ANONKNOWNETHERTYPEHANDLER_H
#define PKTANON_ANONKNOWNETHERTYPEHANDLER_H
#include "AnonPrimitive.h"

namespace pktanon
{

class AnonKnownEthertypeHandler : public AnonPrimitive
{
public:
  AnonKnownEthertypeHandler();
  virtual ~AnonKnownEthertypeHandler();
protected:
  virtual ANON_RESULT anonymize_internal ( const void*const src_buff, void* dst_buff, unsigned int len ) const;
  virtual ANON_RESULT anonymize_chain ( void* buf, unsigned int len ) const;
};

}

#endif // PKTANON_ANONKNOWNETHERTYPEHANDLER_H
