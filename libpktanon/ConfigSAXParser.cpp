/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#include "ConfigSAXParser.h"
//----------------------------------------------------------------
#include <memory>
#include "debug.h"

#include <xercesc/sax2/Attributes.hpp>
#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <xercesc/sax2/DefaultHandler.hpp>

using namespace xercesc;

namespace pktanon
{

class ConfigParserHandler : public xercesc::DefaultHandler
{

public:
  virtual ~ConfigParserHandler();
  ConfigParserHandler(PktAnonConfig& config);

  void startElement (
    const   XMLCh* const    uri,
    const   XMLCh* const    localname,
    const   XMLCh* const    qname,
    const   xercesc::Attributes&     attrs
  );

  virtual void endElement ( const XMLCh* const uri, const XMLCh* const localname, const XMLCh* const qname );

  virtual void characters ( const XMLCh* const chars, const XMLSize_t length );

  void fatalError ( const xercesc::SAXParseException& );

private:
  PktAnonConfig& config;

  std::string last_field_name;
  size_t current_element;

  std::string param_name;
};


ConfigParserHandler::ConfigParserHandler( PktAnonConfig& config ) :
  config ( config ) 
{ }

ConfigParserHandler::~ConfigParserHandler() = default;


const size_t element_hash_base = 512;

const size_t e_PKTANON_config = 195;
const size_t e_parameters = 439;
const size_t e_param = 51;
const size_t e_anonymizations = 122;
const size_t e_packet = 36;
const size_t e_field = 152;
const size_t e_anon = 184;


void ConfigParserHandler::startElement ( const XMLCh* const uri, const XMLCh* const localname, const XMLCh* const qname, const xercesc_3_1::Attributes& attrs )
{
  XMLString::lowerCase ( const_cast<XMLCh*> ( localname ) );
  size_t element =  XMLString::hash ( localname,  element_hash_base );

  XMLCh* attr_NAME = XMLString::transcode ( "name" );
  XMLCh* attr_ANON = XMLString::transcode ( "anon" );
  XMLCh* attr_TYPE = XMLString::transcode ( "type" );

  current_element = element;
  switch ( current_element )
  {
    case e_PKTANON_config: break;

    case e_parameters: break;
    case e_param:
    {
      param_name = XMLString::transcode ( attrs.getValue ( attr_NAME ) );
      break;
    }

    case e_anonymizations: break;

    case e_packet:
    {
      std::string packet_name = XMLString::transcode ( attrs.getValue ( ( XMLSize_t ) 0 ) );
      config.add_packet ( std::move ( packet_name ) );
      break;
    }

    case e_field:
    {
      if ( attrs.getLength() == 1 )
      {
        std::string field_name = XMLString::transcode ( attrs.getValue ( attr_NAME ) );
        last_field_name = field_name;
        config.add_field_to_last_added_packet ( std::move ( field_name ) );
      }
      else
      {
        std::string field_name;
        PktAnonConfig::AnonConfig anon_config;

        for ( XMLSize_t j = 0; j < attrs.getLength(); j++ )
        {
          std::string att1 = XMLString::transcode ( attrs.getLocalName ( ( XMLSize_t ) j ) );
          std::string att2 = XMLString::transcode ( attrs.getValue ( ( XMLSize_t ) j ) );

          auto* attr_name = attrs.getLocalName ( j );
          auto* attr_value = attrs.getValue ( j );

          if ( XMLString::compareString ( attr_NAME, attr_name ) == 0 )
          {
            field_name = std::string ( XMLString::transcode ( attr_value ) );
          }
          else if ( XMLString::compareString ( attr_ANON, attr_name ) == 0 )
          {
            config.set_anon_class ( anon_config, std::string ( XMLString::transcode ( attr_value ) ) );
          }
          else
          {
            config.add_anon_param ( anon_config,  std::string ( XMLString::transcode ( attr_name ) ), std::string ( XMLString::transcode ( attr_value ) ) );
          }
        }

        config.add_field_to_last_added_packet ( std::move ( field_name ),  std::move ( anon_config ) );
      }
      break;

      case e_anon:
      {
        PktAnonConfig::AnonConfig anon_config;

        for ( XMLSize_t j = 0; j < attrs.getLength(); j++ )
        {
          std::string att1 = XMLString::transcode ( attrs.getLocalName ( ( XMLSize_t ) j ) );
          std::string att2 = XMLString::transcode ( attrs.getValue ( ( XMLSize_t ) j ) );

          auto* attr_name = attrs.getLocalName ( j );
          auto* attr_value = attrs.getValue ( j );

          if ( XMLString::compareString ( attr_NAME, attr_name ) == 0 )
          {
            config.set_anon_class ( anon_config, std::string ( XMLString::transcode ( attr_value ) ) );
          }
          else
          {
            config.add_anon_param ( anon_config,  std::string ( XMLString::transcode ( attr_name ) ), std::string ( XMLString::transcode ( attr_value ) ) );
          }
        }

        config.add_anon_to_field_to_last_added_packet ( last_field_name,  std::move ( anon_config ) );
      }
      break;

      default:
        char* message = XMLString::transcode ( localname );
        PRINT ( "unknown element: " <<  message <<  ": " <<  element );
        XMLString::release ( &message );
      }

  }
}

void ConfigParserHandler::endElement ( const XMLCh* const uri, const XMLCh* const localname, const XMLCh* const qname )
{
  current_element = 0;
}

void ConfigParserHandler::characters ( const XMLCh* const chars, const XMLSize_t length )
{
  switch ( current_element )
  {
    case e_param:
    {
      std::string param_value ( XMLString::transcode ( chars ) );
      config.add_global_param ( std::move ( param_name ),  std::move ( param_value ) );
    }
  }
  current_element = 0;
}

void ConfigParserHandler::fatalError ( const xercesc::SAXParseException& exception )
{
  throw exception;
//   char* message = XMLString::transcode(exception.getMessage());
//   throw std::runtime_error(std::string("Fatal Error: ") + message + " at line: " + std::to_string(exception.getLineNumber()));
}

//-------------------------------------------------------------------------------------------------------------
// ConfigParser
//-------------------------------------------------------------------------------------------------------------

ConfigSAXParser* ConfigSAXParser::pInstance = new ConfigSAXParser();

ConfigSAXParser::~ConfigSAXParser() = default;
ConfigSAXParser::ConfigSAXParser() {}

void ConfigSAXParser::parseConfigFromFile ( const std::string& filename, pktanon::PktAnonConfig& config )
{
  try
  {
    XMLPlatformUtils::Initialize();
  }
  catch ( const XMLException& e )
  {
    char* message = XMLString::transcode ( e.getMessage() );
    throw std::runtime_error ( message );
  }

  std::unique_ptr<SAX2XMLReader> parser = std::unique_ptr<SAX2XMLReader> ( XMLReaderFactory::createXMLReader() );
  parser->setFeature ( XMLUni::fgSAX2CoreValidation, true );
  parser->setFeature ( XMLUni::fgSAX2CoreNameSpaces, true ); // optional

  ConfigParserHandler* config_parser = new ConfigParserHandler(config);
  parser->setContentHandler ( config_parser );
  parser->setErrorHandler ( config_parser );

  try
  {
    parser->parse ( filename.c_str() );
  }
  catch ( const XMLException& e )
  {
    char* message = XMLString::transcode ( e.getMessage() );
    throw std::runtime_error ( message );
  }
  catch ( const SAXParseException& e )
  {
    char* message = XMLString::transcode ( e.getMessage() );
    throw std::runtime_error ( std::string ( "Fatal Error: " ) + message + " at line: " + std::to_string ( e.getLineNumber() ) );
  }
  catch ( const std::exception& e )
  {
    throw e;
  }
  catch ( ... )
  {
    throw std::runtime_error ( "unexpected exception at parseConfigFromFile" );
  }
}

}

