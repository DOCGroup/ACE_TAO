// -*- C++ -*-  $Id$

#include "ace/ACE.h"

ACEXML_INLINE
ACEXML_Attribute::ACEXML_Attribute (void)
  : uri_ (0),
    localName_ (0),
    qName_ (0),
    type_ (0),
    value_ (0)
{
}

ACEXML_INLINE
ACEXML_Attribute::ACEXML_Attribute (const ACEXML_Attribute &attr)
  : uri_ (ACE::strnew (attr.uri_)),
    localName_ (ACE::strnew (attr.localName_)),
    qName_ (ACE::strnew (attr.qName_)),
    type_ (ACE::strnew (attr.type_)),
    value_ (ACE::strnew (attr.value_))
{
}

ACEXML_INLINE
ACEXML_Attribute::ACEXML_Attribute (const ACEXML_Char *uri,
                                    const ACEXML_Char *localName,
                                    const ACEXML_Char *qName,
                                    const ACEXML_Char *type,
                                    const ACEXML_Char *value)
  : uri_ (ACE::strnew (uri)),
    localName_ (ACE::strnew (localName)),
    qName_ (ACE::strnew (qName)),
    type_ (ACE::strnew (type)),
    value_ (ACE::strnew (value))
{
}

ACEXML_INLINE
ACEXML_Attribute::~ACEXML_Attribute (void)
{
  delete[] this->uri_;
  delete[] this->localName_;
  delete[] this->qName_;
  delete[] this->type_;
  delete[] this->value_;
}

ACEXML_INLINE const ACEXML_Char *
ACEXML_Attribute::uri (void) const
{
  return this->uri_;
}

ACEXML_INLINE void
ACEXML_Attribute::uri (const ACEXML_Char *uri)
{
  delete[] this->uri_;
  this->uri_ = ACE::strnew (uri);
}

ACEXML_INLINE const ACEXML_Char *
ACEXML_Attribute::localName (void) const
{
  return this->localName_;
}

ACEXML_INLINE void
ACEXML_Attribute::localName (const ACEXML_Char *localName)
{
  delete[] this->localName_;
  this->localName_ = ACE::strnew (localName);
}

ACEXML_INLINE const ACEXML_Char *
ACEXML_Attribute::qName (void) const
{
  return this->qName_;
}

ACEXML_INLINE void
ACEXML_Attribute::qName (const ACEXML_Char *qName)
{
  delete[] this->qName_;
  this->qName_ = ACE::strnew (qName);
}

ACEXML_INLINE const ACEXML_Char *
ACEXML_Attribute::type (void) const
{
  return this->type_;
}

ACEXML_INLINE void
ACEXML_Attribute::type (const ACEXML_Char *type)
{
  delete[] this->type_;
  this->type_ = ACE::strnew (type);
}

ACEXML_INLINE const ACEXML_Char *
ACEXML_Attribute::value (void) const
{
  return this->value_;
}

ACEXML_INLINE void
ACEXML_Attribute::value (const ACEXML_Char *value)
{
  delete[] this->value_;
  this->value_ = ACE::strnew (value);
}

ACEXML_INLINE void
ACEXML_Attribute::setAttribute (const ACEXML_Char *uri,
                                const ACEXML_Char *localName,
                                const ACEXML_Char *qName,
                                const ACEXML_Char *type,
                                const ACEXML_Char *value)
{
  this->uri (uri);
  this->qName (qName);
  this->localName (localName);
  this->type (type);
  this->value (value);
}

ACEXML_INLINE ACEXML_Attribute &
ACEXML_Attribute::operator= (const ACEXML_Attribute &rhs)
{
  if (this != &rhs)             // Check for self assignment
    {
      this->uri (rhs.uri ());
      this->qName (rhs.qName ());
      this->localName (rhs.localName ());
      this->type (rhs.type ());
      this->value (rhs.value ());
    }
  return *this;
}

ACEXML_INLINE int
ACEXML_Attribute::operator!= (const ACEXML_Attribute &rhs) const
{
  return (ACE_OS::strcmp (this->uri_, rhs.uri ()) == 0 &&
          ACE_OS::strcmp (this->localName_, rhs.localName ()) == 0 &&
          ACE_OS::strcmp (this->qName_, rhs .qName ()) == 0 &&
          ACE_OS::strcmp (this->type_, rhs.type ()) == 0 &&
          ACE_OS::strcmp (this->value_, rhs.value ()) == 0 ? 0 : 1);

}
