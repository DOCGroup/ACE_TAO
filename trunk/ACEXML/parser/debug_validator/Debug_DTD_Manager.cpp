// -*- C++ -*-   $Id$

#include "common/SAXExceptions.h"
#include "parser/debug_validator/Debug_DTD_Manager.h"
#include "parser/debug_validator/Debug_Element_Builder.h"
#include "parser/debug_validator/Debug_Attributes_Builder.h"

ACEXML_Debug_DTD_Manager::ACEXML_Debug_DTD_Manager ()
{

}

ACEXML_Debug_DTD_Manager::~ACEXML_Debug_DTD_Manager ()
{

}

ACEXML_Element_Def_Builder *
ACEXML_Debug_DTD_Manager::getElement_Def_Builder ()
{
  return new ACEXML_Debug_Element_Builder ();
}

int
ACEXML_Debug_DTD_Manager::insertElement_Definition (ACEXML_Element_Def_Builder *def,
                                                    ACEXML_Env &xmlenv)
{
  ACEXML_Element_Def_Builder::VAR ptr (def);

  if (def != 0)
    {
      ptr->dump ();
      return 0;
    }

  xmlenv.exception (new ACEXML_SAXParseException ("ACEXML_Debug_Attributes_Builder internal error"));
  return -1;
}

ACEXML_Attributes_Def_Builder *
ACEXML_Debug_DTD_Manager::getAttribute_Def_Builder ()
{
  ACEXML_Attributes_Def_Builder *tmp;
  ACE_NEW_RETURN (tmp,
                  ACEXML_Debug_Attributes_Builder (),
                  0);
  return tmp;
}

int
ACEXML_Debug_DTD_Manager::insertAttributes_Definition (ACEXML_Attributes_Def_Builder *def,
                                                       ACEXML_Env &xmlenv)
{
  ACEXML_Attributes_Def_Builder::VAR ptr (def);
  if (def != 0)
    {
      ptr->dump ();
      return 0;
    }

  xmlenv.exception (new ACEXML_SAXParseException ("ACEXML_Debug_Attributes_Builder internal error"));
  return -1;
}

ACEXML_Validator *
ACEXML_Debug_DTD_Manager::getValidator (const ACEXML_Char *namespaceURI,
                                        const ACEXML_Char *localName,
                                        const ACEXML_Char *qName,
                                        ACEXML_Env &xmlenv)
{
  ACE_UNUSED_ARG (namespaceURI);
  ACE_UNUSED_ARG (localName);
  ACE_UNUSED_ARG (qName);

  xmlenv.exception (new ACEXML_SAXNotSupportedException ());
  return 0;
}
