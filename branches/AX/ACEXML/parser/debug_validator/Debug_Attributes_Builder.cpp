// $Id$

#include "common/SAXExceptions.h"
#include "parser/debug_validator/Debug_Attributes_Builder.h"

ACEXML_Debug_Attribute_Builder::ACEXML_Debug_Attribute_Builder ()
  : type_ (ERROR_TYPE),
    default_decl_ (INVALID)
{
}

ACEXML_Debug_Attribute_Builder::~ACEXML_Debug_Attribute_Builder ()
{
}

int
ACEXML_Debug_Attribute_Builder::setAttType (const ATT_TYPE type,
                                            ACEXML_Env &xmlenv)
  // ACE_THORW_SPEC ((ACEXML_SAXException))
{
  if (this->type_ == ERROR_TYPE)
    {
      this->type_ = type;
      return 0;
    }
  xmlenv.exception (new ACEXML_SAXParseException ("Attribute type redefinition in Debug Validator"));
  return -1;
}

int
ACEXML_Debug_Attribute_Builder::insertList (const ACEXML_Char Name,
                                            ACEXML_Env &xmlenv)
  // ACE_THORW_SPEC ((ACEXML_SAXException))
{

}

int
ACEXML_Debug_Attribute_Builder::setDefault (const DEFAULT_DECL def,
                                            const ACEXML_Char *value,
                                            ACEXML_Env &xmlenv)
  // ACE_THORW_SPEC ((ACEXML_SAXException))
{

}

int
ACEXML_Debug_Attribute_Builder::validAttr (void)
{
}

// ========================================

ACEXML_Debug_Attributes_Builder::ACEXML_Debug_Attributes_Builder ()
{
}

ACEXML_Debug_Attributes_Builder::~ACEXML_Debug_Attributes_Builder ()
{
}

int
ACEXML_Debug_Attributes_Builder::setElement (const ACEXML_Char *namespaceURI,
                                             const ACEXML_Char *localName,
                                             const ACEXML_Char *qName,
                                             ACEXML_Env &xmlenv)
  //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
}

ACEXML_Attribute_Def_Builder *
ACEXML_Debug_Attributes_Builder::getAttribute_Def_Builder ()
{
  ACEXML_Attribute_Def_Builder *tmp;

  ACE_NEW_RETURN (tmp,
                  ACEXML_Debug_Attribute_Builder (),
                  0);
  return tmp;
}

int
ACEXML_Debug_Attributes_Builder::insertAttribute (ACEXML_Attribute_Def_Builder *def,
                                                  ACEXML_Env &)
{
  ACEXML_Attribute_Def_Builder::VAR ptr (def);

  return 0;
}
