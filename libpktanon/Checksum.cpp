/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#include "Checksum.h"

#include "debug.h"
#include <netinet/in.h>
#include <optmacros.h>

using namespace pktanon;

void Checksum::update(void* buffer, size_t length)
{
  uint16_t* buff = (uint16_t*) buffer;
  const unsigned buff_len = length / 2;

  for (size_t nWords = (buff_len/4); nWords > 0; nWords-- )  
  {
    sum += *buff++; sum += *buff++; sum += *buff++; sum += *buff++;
  }

  for (size_t nWords = (buff_len & 0x03) ; nWords > 0; nWords-- )
    sum += *buff++;

  if (unlikely(length & 1))
    sum += *buff & htons(0xFF00);

}

uint16_t Checksum::final()
{
  sum = (sum & 0xFFFF) + (sum >> 16);
  sum = (sum & 0xFFFF) + (sum >> 16);
  sum = ~sum;

  return sum;
}

// #pragma GCC pop_options


// checksum algorithm
// u16 tcp_sum_calc(u16 len_tcp, u16 src_addr[],u16 dest_addr[], BOOL padding, u16 buff[])
// {
//   u16 prot_tcp=6;
//   u16 padd=0;
//   u16 word16;
//   u32 sum;
//
//   // Find out if the length of data is even or odd number. If odd,
//   // add a padding byte = 0 at the end of packet
//   if (padding&1==1){
//     padd=1;
//     buff[len_tcp]=0;
//     }
//
//   //initialize sum to zero
//   sum=0;
//
//   // make 16 bit words out of every two adjacent 8 bit words and
//   // calculate the sum of all 16 vit words
//   for (i=0;i<len_tcp+padd;i=i+2){
//     word16 =((buff[i]<<8)&0xFF00)+(buff[i+1]&0xFF);
//     sum = sum + (unsigned long)word16;
//     }
//   // add the TCP pseudo header which contains:
//   // the IP source and destinationn addresses,
//   for (i=0;i<4;i=i+2){
//     word16 =((src_addr[i]<<8)&0xFF00)+(src_addr[i+1]&0xFF);
//     sum=sum+word16;
//     }
//   for (i=0;i<4;i=i+2){
//     word16 =((dest_addr[i]<<8)&0xFF00)+(dest_addr[i+1]&0xFF);
//     sum=sum+word16;
//     }
//   // the protocol number and the length of the TCP packet
//   sum = sum + prot_tcp + len_tcp;
//
//   // keep only the last 16 bits of the 32 bit calculated sum and add the carries
//   while (sum>>16)
//   sum = (sum & 0xFFFF)+(sum >> 16);
//
//   // Take the one's complement of sum
//   sum = ~sum;
//
//   return ((unsigned short) sum);
// }
