#include "ACEXML/common/SAXExceptions.h"
#include "ACEXML/parser/debug_validator/Debug_Attributes_Builder.h"

ACEXML_Debug_Attribute_Builder::ACEXML_Debug_Attribute_Builder ()
  : type_ (ERROR_TYPE),
    default_decl_ (INVALID)
{
}

ACEXML_Debug_Attribute_Builder::ACEXML_Debug_Attribute_Builder (
  ACEXML_Debug_Attribute_Builder const &rhs)
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
ACEXML_Debug_Attribute_Builder::setName (ACEXML_Char const *n)
{
  this->name_.set (n, false);
  return 0;
}

ACEXML_Char const *
ACEXML_Debug_Attribute_Builder::getName ()
{
  return this->name_.fast_rep ();
}

int
ACEXML_Debug_Attribute_Builder::setAttType (const ATT_TYPE type)
{
  if (this->type_ == ERROR_TYPE)
    {
      this->type_ = type;
      return 0;
    }
  ACEXML_THROW_RETURN (ACEXML_SAXParseException (ACE_TEXT("Attribute type redefinition in Debug Validator")), -1);
}

int
ACEXML_Debug_Attribute_Builder::insertList (ACEXML_Char const *n)
{
  ACEXML_String str (n, nullptr, false);

  this->att_value_queue_.enqueue_tail (str);
  return 0;
}

int
ACEXML_Debug_Attribute_Builder::setDefault (DEFAULT_DECL def,
                                            ACEXML_Char const *value)
{
  this->default_decl_ = def;
  this->default_value_.set (value, false);
  return 0;
}

int
ACEXML_Debug_Attribute_Builder::validAttr ()
{
  // @@ Not implemented.  Always return 1 (true) for now.
  return 1;
}

void
ACEXML_Debug_Attribute_Builder::dump ()
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
        ACEXML_String *n = nullptr;

        while (iter.advance () != 0)
          {
            if (n == nullptr)
              cout << " | ";
            iter.next (n);
            cout << *n;
          }
        cout << ") ";
      }
      break;
    default:
      cout << "*** UNKNOWN TYPE ***";
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
ACEXML_Debug_Attributes_Builder::setElement (ACEXML_Char const *,
                                             ACEXML_Char const *,
                                             ACEXML_Char const *qName)
{
  this->element_name_.set (qName, false);
  return 0;
}

ACEXML_Attribute_Def_Builder *
ACEXML_Debug_Attributes_Builder::getAttribute_Def_Builder ()
{
  ACEXML_Attribute_Def_Builder *tmp;

  ACE_NEW_RETURN (tmp,
                  ACEXML_Debug_Attribute_Builder (),
                  nullptr);
  return tmp;
}

int
ACEXML_Debug_Attributes_Builder::insertAttribute (
  ACEXML_Attribute_Def_Builder *def)
{
  ACEXML_Attribute_Def_Builder::VAR ptr (def);

  if (def != nullptr)
    {
      ACEXML_String attname (def->getName (), nullptr, false);
      ACEXML_Debug_Attribute_Builder *ptr =
        dynamic_cast<ACEXML_Debug_Attribute_Builder *> (def);
      this->attributes_.bind (attname, *ptr);
      return 0;
    }
  ACEXML_THROW_RETURN (ACEXML_SAXParseException (ACE_TEXT("ACEXML_Debug_Attributes_Builder internal error")), -1);
}

void
ACEXML_Debug_Attributes_Builder::dump ()
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
