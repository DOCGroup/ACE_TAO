//$Id$

#ifndef UTILS_C
#define UTILS_C

#include "Utils.h"
#include "ace/OS_NS_stdlib.h"

char *
CIAO::Config_Handler::Utils::parse_string (DOMNodeIterator * iter)
{
  DOMNode * node = iter->nextNode();
  DOMText* text = ACE_reinterpret_cast (DOMText*, node);
  return XMLString::transcode (text->getNodeValue ());
}

CORBA::ULong
CIAO::Config_Handler::Utils::parse_long (DOMNodeIterator * iter)
{
  char *temp (Config_Handler::Utils::parse_string (iter));
  CORBA::ULong ret_val = ACE_OS::strtoul (temp, 0, 10);
  XMLString::release (&temp);
  return ret_val;
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

#endif /* UTILS_C */
