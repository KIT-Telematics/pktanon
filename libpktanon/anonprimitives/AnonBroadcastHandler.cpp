/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#include "AnonBroadcastHandler.h"
#include "AnonFactory.h"
#include <algorithm>
#include <string.h>
#include <cstdint>

namespace pktanon
{

AnonBroadcastHandler::AnonBroadcastHandler() {}
AnonBroadcastHandler::~AnonBroadcastHandler() = default;

AnonPrimitive::ANON_RESULT AnonBroadcastHandler::anonymize_chain (void* buf, unsigned int len) const
{
    //TODO try with std::find_if_not ...
    unsigned char* buf1 = reinterpret_cast<unsigned char*> (buf);
    bool continue_anon = std::any_of (buf1, buf1 + len, [](uint8_t j) {
        return j != 0xFF;
    });

    return ANON_RESULT(len, continue_anon);
}

AnonPrimitive::ANON_RESULT AnonBroadcastHandler::anonymize_internal ( const void* const src_buff, void* dst_buff, unsigned int len ) const
{
    const unsigned char* buf1 = reinterpret_cast<const unsigned char*> (src_buff);
    bool continue_anon = std::any_of (buf1, buf1 + len, [](uint8_t j) {
        return j != 0xFF;
    });

    memcpy(dst_buff, src_buff, len);

    return ANON_RESULT(len, continue_anon);
}


REGISTER_ANON (AnonBroadcastHandler);
}
