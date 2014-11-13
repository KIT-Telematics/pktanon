/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#include "AnonTester.h"
#include <log.h>
#include <debug.h>
#include <AnonFactory.h>
#include <string>
#include <iomanip>
#include <string.h>
#include <boost/concept_check.hpp>

using namespace pktanon;

void hex_dump ( char *buff, int len )
{
  for ( unsigned j = 0; j < len; j++ ) 
  {
    printf ( "%02hhx", buff[j] );
//       std::cout  << std::hex << std::setw(2) << dst_buff[j];
  }
}

int ch2dec ( char c )
{
  switch ( c )
  {
    case 'a' :
      return 10;
    case 'b' :
      return 11;
    case 'c' :
      return 12;
    case 'd' :
      return 13;
    case 'e' :
      return 14;
    case 'f' :
      return 15;
    default:
      return atoi ( &c );
  }
}


AnonTester::~AnonTester() { }
AnonTester::AnonTester() : anon ( nullptr ), params(), hex_output ( 0 ) { }

void AnonTester::addParam ( string name, string value )
{
  if ( name.compare ( "hex_output" ) == 0 )
  {
    hex_output = ( value.at ( 0 ) == '1' );
  }
  else
  {
    params.add_param ( std::move ( name ), std::move ( value ) );
  }
}


void AnonTester::addAnon ( std::string anon_class )
{
  AnonPrimitive* prim = AnonFactory::create_anon ( anon_class, params );
  if ( anon == nullptr )
  {
//     TRACEV(anon_class);
    anon = prim;
    last_anon = prim;
  }
  else
  { 
//     TRACEV(anon_class);
    last_anon->setNext ( prim );
    last_anon = prim;
  }
}

void AnonTester::eval ( string val )
{
  auto src_buff = const_cast<char*> ( val.data() );
  auto src_len = val.size();

  if ( val.at ( 0 ) == '#' ) // the rest of the string should be interpreted as hex value
  {
    src_len = ( val.size()-1 ) /2;
    src_buff = new char[src_len];

    for ( int j = 1; j < val.size(); j+=2 )
    {
      int index = ( j-1 ) /2;
      int ch_val = ch2dec ( val.at ( j ) ) *16 + ch2dec ( val.at ( j+1 ) );
      src_buff[index] = ( char ) ch_val;
    }

//     hex_dump ( src_buff, src_len );
  }

  char dst_buff[src_len +1];
  memset ( &dst_buff, '\0', src_len +1 );

  auto result_len = anon->anonimyze ( src_buff, dst_buff, src_len );
//   TRACE(result_len);
  
  if ( hex_output )
  {
    std::cout << "result = {";
    hex_dump ( dst_buff, result_len );
    std::cout << std::dec << "}" << std::endl;
  }
  else
  {
    auto dst_str = std::string(dst_buff).substr(0,result_len);
    _plg_info ( "result = {" << std::hex << dst_str << "}" );
  }

}


int main ( int argc, char* argv[] )
{
  AnonTester tester;

//   _plg_info ( "=== AnonTester ===" );
//   _plg_info ( "a <AnonClass>; p <param> <value>; v <value> ;q" );


  std::string command;
  std::string anon_class;
  std::string param_name;
  std::string param_value;
  std::string buffer;

  while ( 1 )
  {
    _plg_info ( "enter command:" );
    std::cin >> command;
//     TRACEV ( command );

    switch ( command.at ( 0 ) )
    {
      case 'a':
        std::cin >> anon_class;
//         TRACEV ( anon_class );
        tester.addAnon ( anon_class );
        break;
      case 'p':
        std::cin >> param_name;
        std::cin >> param_value;
//  TRACEV(param_name);
//  TRACEV(param_value);
        tester.addParam ( param_name, param_value );
        break;
      case 'v':
        std::cin >> buffer;
//         TRACEV ( buffer );
        tester.eval ( buffer );
        break;
      case 'q':
        return 0;
      default:
        _plg_error ( "unnown command, use a, p, v, or q" );
    }
  }


//   std::string anon;
//   std::string buffer;
//
//   _plg_info ( "Enter Anon Class:" );
//   std::cin >> anon;
//
//
//   _plg_info ( "Enter Anon Value:" );
//   std::cin >> buffer;
//
//   TRACEV ( anon )
//   TRACEV ( buffer )

}
