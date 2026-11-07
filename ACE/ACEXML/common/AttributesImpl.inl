// -*- C++ -*-
//
#include "ace/ACE.h"
#include "ace/OS_NS_string.h"

ACEXML_INLINE
ACEXML_Attribute::ACEXML_Attribute (ACEXML_Attribute const &attr)
  : uri_ (ACE::strnew (attr.uri_.get ())),
    localName_ (ACE::strnew (attr.localName_.get ())),
    qName_ (ACE::strnew (attr.qName_.get ())),
    type_ (ACE::strnew (attr.type_.get ())),
    value_ (ACE::strnew (attr.value_.get ()))
{
}

ACEXML_INLINE
ACEXML_Attribute::ACEXML_Attribute (ACEXML_Char const *uri,
                                    ACEXML_Char const *localName,
                                    ACEXML_Char const *qName,
                                    ACEXML_Char const *type,
                                    ACEXML_Char const *value)
  : uri_ (ACE::strnew (uri)),
    localName_ (ACE::strnew (localName)),
    qName_ (ACE::strnew (qName)),
    type_ (ACE::strnew (type)),
    value_ (ACE::strnew (value))
{
}

ACEXML_INLINE ACEXML_Char const *
ACEXML_Attribute::uri () const
{
  return this->uri_.get ();
}

ACEXML_INLINE void
ACEXML_Attribute::uri (ACEXML_Char const *uri)
{
  this->uri_.reset (ACE::strnew (uri));
}

ACEXML_INLINE ACEXML_Char const *
ACEXML_Attribute::localName () const
{
  return this->localName_.get ();
}

ACEXML_INLINE void
ACEXML_Attribute::localName (ACEXML_Char const *localName)
{
  this->localName_.reset (ACE::strnew (localName));
}

ACEXML_INLINE ACEXML_Char const *
ACEXML_Attribute::qName () const
{
  return this->qName_.get ();
}

ACEXML_INLINE void
ACEXML_Attribute::qName (ACEXML_Char const *qName)
{
  this->qName_.reset (ACE::strnew (qName));
}

ACEXML_INLINE ACEXML_Char const *
ACEXML_Attribute::type () const
{
  return this->type_.get ();
}

ACEXML_INLINE void
ACEXML_Attribute::type (ACEXML_Char const *type)
{
  this->type_.reset (ACE::strnew (type));
}

ACEXML_INLINE ACEXML_Char const *
ACEXML_Attribute::value () const
{
  return this->value_.get ();
}

ACEXML_INLINE void
ACEXML_Attribute::value (ACEXML_Char const *value)
{
  this->value_.reset (ACE::strnew (value));
}

ACEXML_INLINE void
ACEXML_Attribute::setAttribute (ACEXML_Char const *uri,
                                ACEXML_Char const *localName,
                                ACEXML_Char const *qName,
                                ACEXML_Char const *type,
                                ACEXML_Char const *value)
{
  this->uri (uri);
  this->qName (qName);
  this->localName (localName);
  this->type (type);
  this->value (value);
}

ACEXML_INLINE ACEXML_Attribute &
ACEXML_Attribute::operator= (ACEXML_Attribute const &rhs)
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

ACEXML_INLINE bool
ACEXML_Attribute::operator!= (ACEXML_Attribute const &rhs) const
{
  return (ACE_OS::strcmp (this->uri_.get (), rhs.uri ()) == 0 &&
          ACE_OS::strcmp (this->localName_.get (), rhs.localName ()) == 0 &&
          ACE_OS::strcmp (this->qName_.get (), rhs.qName ()) == 0 &&
          ACE_OS::strcmp (this->type_.get (), rhs.type ()) == 0 &&
          ACE_OS::strcmp (this->value_.get (), rhs.value ()) == 0 ? false : true);
}
