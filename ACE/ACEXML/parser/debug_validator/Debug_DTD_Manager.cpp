// -*- C++ -*-

#include "ACEXML/common/SAXExceptions.h"
#include "ACEXML/parser/debug_validator/Debug_DTD_Manager.h"
#include "ACEXML/parser/debug_validator/Debug_Element_Builder.h"
#include "ACEXML/parser/debug_validator/Debug_Attributes_Builder.h"

ACEXML_Debug_DTD_Manager::~ACEXML_Debug_DTD_Manager () = default;

ACEXML_Element_Def_Builder *
ACEXML_Debug_DTD_Manager::getElement_Def_Builder ()
{
  ACEXML_Element_Def_Builder *builder {};
  ACE_NEW_RETURN (builder, ACEXML_Debug_Element_Builder (), nullptr);
  return builder;
}

int
ACEXML_Debug_DTD_Manager::insertElement_Definition (ACEXML_Element_Def_Builder *def)
{
  ACEXML_Element_Def_Builder::VAR ptr (def);

  if (def != nullptr)
    {
      ptr->dump ();
      return 0;
    }

  throw ACEXML_SAXParseException (ACE_TEXT("ACEXML_Debug_Attributes_Builder internal error"));
}

ACEXML_Attributes_Def_Builder *
ACEXML_Debug_DTD_Manager::getAttribute_Def_Builder ()
{
  ACEXML_Attributes_Def_Builder *builder {};
  ACE_NEW_RETURN (builder,
                  ACEXML_Debug_Attributes_Builder (),
                  nullptr);
  return builder;
}

int
ACEXML_Debug_DTD_Manager::insertAttributes_Definition (ACEXML_Attributes_Def_Builder *def)
{
  ACEXML_Attributes_Def_Builder::VAR ptr (def);
  if (def != nullptr)
    {
      ptr->dump ();
      return 0;
    }

  throw ACEXML_SAXParseException (ACE_TEXT("ACEXML_Debug_Attributes_Builder internal error"));
}

ACEXML_Validator *
ACEXML_Debug_DTD_Manager::getValidator (ACEXML_Char const *,
                                        ACEXML_Char const *,
                                        ACEXML_Char const *)
{
  throw ACEXML_SAXNotSupportedException (ACE_TEXT ("getValidator()"));
}
