/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 *
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence.
 * See LICENSE file found in the top-level directory of this distribution.
 */

#ifndef PKTANON__ANONCONSTOVERWRITERANGE_H
#define PKTANON__ANONCONSTOVERWRITERANGE_H

#include "AnonPrimitive.h"
#include <cstdint>

namespace pktanon
{

class AnonConstOverwriteRange : public AnonPrimitive
{
public:
  static AnonConstOverwriteRange* construct ( const pktanon::Params& params );

  AnonConstOverwriteRange ( int range_begin,  int range_length,  uint8_t value );
  virtual ~AnonConstOverwriteRange();

protected:
  virtual ANON_RESULT anonymize_chain ( void* buf, unsigned int len ) const;
  virtual ANON_RESULT anonymize_internal ( const void*const src_buff, void* dst_buff, unsigned int len ) const;

private:
  int range_begin;
  int range_length;
  uint8_t value;
};
}

#endif // PKTANON_ANONCONSTOVERWRITERANGE_H
