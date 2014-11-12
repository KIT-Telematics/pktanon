/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#include "PayloadTransformation.h"

using namespace pktanon;

PayloadTransformation::~PayloadTransformation()
{
    delete payload_anon;
}

PayloadTransformation::PayloadTransformation ( AnonPrimitive* payload_anon ) :
    payload_anon ( payload_anon )
{}

namespace pktanon
{
  
int PayloadTransformation::transform ( const uint8_t* source_buffer, uint8_t* destination_buffer, unsigned max_packet_length ) const noexcept
{
//   size_t new_length = transform_field (payload_anon, source_buffer, destination_buffer, max_packet_length);
    size_t new_length = payload_anon->anonimyze ( source_buffer, destination_buffer, max_packet_length );
    return new_length;
}

}

