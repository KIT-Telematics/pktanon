/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 *
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence.
 * See LICENSE file found in the top-level directory of this distribution.
 */

#ifndef PKTANON__PKTANONC_H
#define PKTANON__PKTANONC_H

#include <stdint.h>
#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

void PKTANON__initialize ( char* config_file, int verbose );
int PKTANON__set_link_type ( uint32_t linktype );
int PKTANON__transform_packet ( uint8_t* source_buffer,  uint8_t* destination_buffer, unsigned max_packet_length );

int PKTANON__get_erroneus_packet_length ( const int packet_length );
const char* PKTANON__get_error_string ( const int packet_length );

#ifdef __cplusplus
}
#endif

#endif // PKTANON__PKTANONC_H
