// -*- C++ -*-   $Id$

#include "ACEXML/common/SAXExceptions.h"
#include "ACEXML/parser/debug_validator/Debug_DTD_Manager.h"
#include "ACEXML/parser/debug_validator/Debug_Element_Builder.h"
#include "ACEXML/parser/debug_validator/Debug_Attributes_Builder.h"

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
ACEXML_Debug_DTD_Manager::insertElement_Definition (ACEXML_Element_Def_Builder *def ACEXML_ENV_ARG_DECL)
{
  ACEXML_Element_Def_Builder::VAR ptr (def);

  if (def != 0)
    {
      ptr->dump ();
      return 0;
    }

  ACEXML_THROW_RETURN (ACEXML_SAXParseException (ACE_TEXT("ACEXML_Debug_Attributes_Builder internal error")), -1);
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
ACEXML_Debug_DTD_Manager::insertAttributes_Definition (ACEXML_Attributes_Def_Builder *def ACEXML_ENV_ARG_DECL)
{
  ACEXML_Attributes_Def_Builder::VAR ptr (def);
  if (def != 0)
    {
      ptr->dump ();
      return 0;
    }

  ACEXML_THROW_RETURN (ACEXML_SAXParseException (ACE_TEXT("ACEXML_Debug_Attributes_Builder internal error")), -1);
}

ACEXML_Validator *
ACEXML_Debug_DTD_Manager::getValidator (const ACEXML_Char *namespaceURI,
                                        const ACEXML_Char *localName,
                                        const ACEXML_Char *qName ACEXML_ENV_ARG_DECL)
{
  ACE_UNUSED_ARG (namespaceURI);
  ACE_UNUSED_ARG (localName);
  ACE_UNUSED_ARG (qName);

  ACEXML_THROW_RETURN (ACEXML_SAXNotSupportedException (ACE_TEXT ("getValidator()")), 0);
}
