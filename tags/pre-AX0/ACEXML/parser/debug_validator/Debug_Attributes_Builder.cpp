// $Id$

#include "common/SAXExceptions.h"
#include "parser/debug_validator/Debug_Attributes_Builder.h"

ACEXML_Debug_Attribute_Builder::ACEXML_Debug_Attribute_Builder ()
  : type_ (ERROR_TYPE),
    default_decl_ (INVALID)
{
}

ACEXML_Debug_Attribute_Builder::ACEXML_Debug_Attribute_Builder (const ACEXML_Debug_Attribute_Builder &rhs)
  : name_ (rhs.name_),
    type_ (rhs.type_),
    default_decl_ (rhs.default_decl_),
    default_value_ (rhs.default_value_),
    att_value_queue_ (rhs.att_value_queue_)
{
}

ACEXML_Debug_Attribute_Builder::~ACEXML_Debug_Attribute_Builder ()
{
}

int
ACEXML_Debug_Attribute_Builder::setName (const ACEXML_Char *n)
{
  this->name_.set (n, 0);
  return 0;
}

const ACEXML_Char *
ACEXML_Debug_Attribute_Builder::getName (void)
{
  return this->name_.fast_rep ();
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
  xmlenv.exception (new ACEXML_SAXParseException (ACE_LIB_TEXT("Attribute type redefinition in Debug Validator")));
  return -1;
}

int
ACEXML_Debug_Attribute_Builder::insertList (const ACEXML_Char *n,
                                            ACEXML_Env &)
  // ACE_THORW_SPEC ((ACEXML_SAXException))
{
  ACEXML_String str (n, 0, 0);

  this->att_value_queue_.enqueue_tail (str);
  return 0;
}

int
ACEXML_Debug_Attribute_Builder::setDefault (const DEFAULT_DECL def,
                                            const ACEXML_Char *value,
                                            ACEXML_Env &)
  // ACE_THORW_SPEC ((ACEXML_SAXException))
{
  this->default_decl_ = def;
  this->default_value_.set (value, 0);
  return 0;
}

int
ACEXML_Debug_Attribute_Builder::validAttr (void)
{
  // @@ Not implemented.  Always return 1 (true) for now.
  return 1;
}

void
ACEXML_Debug_Attribute_Builder::dump (void)
{
  cout << this->name_ << " ";

  switch (this->type_)
    {
    case CDATA:
      cout << "CDATA ";
      break;
    case ID:
      cout << "ID ";
      break;
    case IDREF:
      cout << "IDREF ";
      break;
    case IDREFS:
      cout << "IDREFS ";
      break;
    case ENTITY:
      cout << "ENTITY ";
      break;
    case ENTITIES:
      cout << "ENTITIES ";
      break;
    case NMTOKEN:
      cout << "NMTOKEN ";
      break;
    case NMTOKENS:
      cout << "NMTOKENS ";
      break;
    case NOTATION:
      cout << "NOTATION ";
      // Fall thru
    case ENUMERATION:
      {
        cout << "(";
        ACEXML_STRING_QUEUE_ITERATOR iter (this->att_value_queue_);
        ACEXML_String *n = 0;

        while (iter.advance () != 0)
          {
            if (n == 0)
              cout << " | ";
            iter.next (n);
            cout << *n;
          }
        cout << ") ";
      }
      break;
    default:
      cout << "*** UNKNOW TYPE ***";
      break;
    }

  switch (this->default_decl_)
    {
    case REQUIRED:
      cout << "#REQUIRED";
      break;
    case IMPLIED:
      cout << "#IMPLIED";
      break;
    case FIXED:
      cout << "#FIXED " << this->default_value_;
      break;
    default:
      cout << "**** UNDEFINED DEFAULT DECL ****";
      break;
    }
}
// ========================================

ACEXML_Debug_Attributes_Builder::ACEXML_Debug_Attributes_Builder ()
{
}

ACEXML_Debug_Attributes_Builder::~ACEXML_Debug_Attributes_Builder ()
{
}

int
ACEXML_Debug_Attributes_Builder::setElement (const ACEXML_Char *,
                                             const ACEXML_Char *,
                                             const ACEXML_Char *qName,
                                             ACEXML_Env &)
  //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  this->element_name_.set (qName, 0);
  return 0;
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
                                                  ACEXML_Env &xmlenv)
{
  ACEXML_Attribute_Def_Builder::VAR ptr (def);

  if (def != 0)
    {
      ACEXML_String attname (def->getName (), 0, 0);
      ACEXML_Debug_Attribute_Builder *ptr =
        ACE_dynamic_cast (ACEXML_Debug_Attribute_Builder *, def);
      this->attributes_.bind (attname, *ptr);
      return 0;
    }
  xmlenv.exception (new ACEXML_SAXParseException (ACE_LIB_TEXT("ACEXML_Debug_Attributes_Builder internal error")));
  return -1;
}

void
ACEXML_Debug_Attributes_Builder::dump (void)
{
  // @@ Print print.
  cout << "<!ATTLIST " << this->element_name_ << endl;

  ACEXML_ATT_MAP_ITER iter (this->attributes_);
  ACEXML_ATT_MAP_ENTRY *item;

  while (iter.advance () != 0)
    {
      iter.next (item);
      cout << "\n\t";
      item->int_id_.dump ();
    }
  cout << ">" << endl;
}
