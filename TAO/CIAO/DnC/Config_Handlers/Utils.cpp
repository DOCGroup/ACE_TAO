//$Id$

#include "Utils.h"
#include "ace/OS_NS_stdlib.h"
#include "ace/Log_Msg.h"
#include "ace/SString.h"
#include "ace/Auto_Ptr.h"

#include "XercesString.h"
#include <xercesc/util/XMLURL.hpp>
#include <xercesc/util/XMLUri.hpp>
#include <xercesc/util/XMLUniDefs.hpp>

using xercesc::XMLUri;
using xercesc::XMLURL;
using xercesc::DOMDocument;
using xercesc::DOMDocumentTraversal;
using xercesc::XMLUni;
using xercesc::DOMImplementation;
using xercesc::DOMImplementationRegistry;
using xercesc::DOMNodeFilter;
using xercesc::DOMBuilder;
using xercesc::DOMImplementationLS;

char *
CIAO::Config_Handler::Utils::parse_string (DOMNodeIterator * iter)
{
  DOMNode * node = iter->nextNode();
  DOMText* text = ACE_reinterpret_cast (DOMText*, node);
  return XMLString::transcode (text->getNodeValue ());
}

CORBA::Short
CIAO::Config_Handler::Utils::parse_short (DOMNodeIterator * iter)
{
  char *temp (Config_Handler::Utils::parse_string (iter));
  CORBA::Short ret_val = ACE_OS::atoi (temp);
  XMLString::release (&temp);
  return ret_val;
}

CORBA::ULong
CIAO::Config_Handler::Utils::parse_ulong (DOMNodeIterator * iter)
{
  char *temp (Config_Handler::Utils::parse_string (iter));
  CORBA::ULong ret_val = ACE_OS::strtoul (temp, 0, 10);
  XMLString::release (&temp);
  return ret_val;
}

CORBA::Long
CIAO::Config_Handler::Utils::parse_long (DOMNodeIterator * iter)
{
  char *temp (Config_Handler::Utils::parse_string (iter));
  CORBA::ULong ret_val = ACE_OS::strtol (temp, 0, 10);
  XMLString::release (&temp);
  return ret_val;
}

CORBA::Double
CIAO::Config_Handler::Utils::parse_double (DOMNodeIterator * iter)
{
  char *temp (Config_Handler::Utils::parse_string (iter));
  CORBA::Double ret_val = ACE_OS::strtod (temp, 0);
  XMLString::release (&temp);
  return ret_val;
}

CORBA::Float
CIAO::Config_Handler::Utils::parse_float (DOMNodeIterator * iter)
{
  return ACE_static_cast (CORBA::Float, Utils::parse_double (iter));
}

CORBA::Boolean
CIAO::Config_Handler::Utils::parse_bool (DOMNodeIterator * iter)
{
  XStr true_val ("true");
  XStr true_cap_val ("TRUE");

  DOMText * text = ACE_reinterpret_cast (DOMText *, iter->nextNode ());
  XStr value (text->getNodeValue ());

  if (value == true_val || value == true_cap_val)
    return 1;
  else
    return 0;
}

CORBA::Char
CIAO::Config_Handler::Utils::parse_char (DOMNodeIterator * iter)
{
  DOMText * text = ACE_reinterpret_cast (DOMText *, iter->nextNode ());
  char * temp_string = XMLString::transcode (text->getNodeValue ());

  // Should be non-null
  ACE_ASSERT (temp_string != 0);

  // Return the first character in the node as char value
  char ret_char = temp_string [0];
  XMLString::release (&temp_string);
  return ret_char;
}

CORBA::Octet
CIAO::Config_Handler::Utils::parse_octet (DOMNodeIterator * iter)
{
  return Utils::parse_char (iter);
}

DOMBuilder*
CIAO::Config_Handler::Utils::create_parser ()
{
  xercesc::XMLPlatformUtils::Initialize();
  static const XMLCh gLS[] = { xercesc::chLatin_L,
                               xercesc::chLatin_S,
                               xercesc::chNull };

  DOMImplementation* impl
    = DOMImplementationRegistry::getDOMImplementation(gLS);
  //auto_ptr<DOMImplementation> cleanup_impl (impl);

  DOMBuilder* parser =
    ((DOMImplementationLS*)impl)->
    createDOMBuilder(DOMImplementationLS::MODE_SYNCHRONOUS, 0);
  //auto_ptr<DOMBuilder> cleanup_parser (parser);

  // Discard comment nodes in the document
  parser->setFeature (XMLUni::fgDOMComments, false);

  // Disable datatype normalization. The XML 1.0 attribute value
  // normalization always occurs though.
  parser->setFeature (XMLUni::fgDOMDatatypeNormalization, true);

  // Do not create EntityReference nodes in the DOM tree. No
  // EntityReference nodes will be created, only the nodes
  // corresponding to their fully expanded sustitution text will be
  // created.
  parser->setFeature (XMLUni::fgDOMEntities, false);

  // Perform Namespace processing.
  parser->setFeature (XMLUni::fgDOMNamespaces, true);

  // Perform Validation
  parser->setFeature (XMLUni::fgDOMValidation, true);

  // Do not include ignorable whitespace in the DOM tree.
  parser->setFeature (XMLUni::fgDOMWhitespaceInElementContent, false);

  // Enable the parser's schema support.
  parser->setFeature (XMLUni::fgXercesSchema, true);

  // Enable full schema constraint checking, including checking which
  // may be time-consuming or memory intensive. Currently, particle
  // unique attribution constraint checking and particle derivation
  // restriction checking are controlled by this option.
  parser->setFeature (XMLUni::fgXercesSchemaFullChecking, true);

  // The parser will treat validation error as fatal and will exit.
  parser->setFeature (XMLUni::fgXercesValidationErrorAsFatal, true);

  return parser;
}
