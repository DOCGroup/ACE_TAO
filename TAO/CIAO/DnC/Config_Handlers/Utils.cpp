//$Id$

#include "Utils.h"
#include "ace/OS_NS_stdlib.h"
#include "ace/Log_Msg.h"

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
