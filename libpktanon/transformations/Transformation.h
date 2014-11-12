/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 *
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence.
 * See LICENSE file found in the top-level directory of this distribution.
 */

#ifndef PKTANON_TRANSFORMATION_H
#define PKTANON_TRANSFORMATION_H

#include <unordered_map>
#include <netinet/in.h>

#include "anonprimitives/AnonPrimitive.h"
#include "PHT.h"

namespace pktanon
{

class Transformation
{
public:
  virtual ~Transformation();

  static Transformation* getTransformationLinktype ( uint32_t linktype );

  static  const Transformation* const getTransformationEthertype ( uint16_t ethertype )
  {
    return ethetype_lookup_table.lookup ( ethertype, payloadTransformation );
  };

  static const Transformation* const getTransformationProtocol ( uint8_t protocol )
  {
    return protocol_lookup_table.lookup ( protocol, payloadTransformation );
  };

  static Transformation* getPayloadTransformation() {return payloadTransformation;};

  static bool hasPseudoHeader ( uint8_t protocol )
  {
    return checksums_lookup_table.contains ( protocol );
  };

private:
  friend class TransformationsConfigurator;

  static std::unordered_map<uint32_t,  Transformation*> linktype_lookup_table;
  static PHT<uint16_t,  Transformation*> ethetype_lookup_table;
  static PHT<uint8_t,  Transformation*> protocol_lookup_table;
  static PHT<unsigned,  bool> checksums_lookup_table;
  static Transformation* payloadTransformation;

public:

  virtual int transform ( const uint8_t* source_buffer,  uint8_t* destination_buffer, unsigned max_packet_length ) const noexcept = 0;

  static size_t transform_field ( AnonPrimitive const * anon, const void*  input_buffer, void* output_buffer,  size_t field_orig_length )
  {
    if ( anon == nullptr ) return 0;
    return anon->anonimyze ( input_buffer, output_buffer, field_orig_length );
  };

  static void ntoh16 ( uint16_t& val ) { val = ntohs ( val );}
  static void ntoh32 ( uint32_t& val ) { val = ntohl ( val );}
  static void hton16 ( uint16_t& val ) { val = htons ( val );}
  static void hton32 ( uint32_t& val ) { val = htonl ( val );}


};
}

#endif // PKTANON_TRANSFORMATION_H
