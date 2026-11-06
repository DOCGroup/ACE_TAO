// -*- C++ -*-

#include "ace/ACE.h"

ACEXML_INLINE void
ACEXML_LocatorImpl::setColumnNumber (int cn)
{
  this->columnNumber_ = cn;
}

ACEXML_INLINE void
ACEXML_LocatorImpl::setLineNumber (int ln)
{
  this->lineNumber_ = ln;
}

ACEXML_INLINE int
ACEXML_LocatorImpl::getColumnNumber () const
{
  return this->columnNumber_;
}

ACEXML_INLINE int
ACEXML_LocatorImpl::getLineNumber () const
{
  return this->lineNumber_;
}

ACEXML_INLINE void
ACEXML_LocatorImpl::setPublicId (ACEXML_Char const *id)
{
  this->publicId_.reset (ACE::strnew (id));
}

ACEXML_INLINE void
ACEXML_LocatorImpl::setSystemId (ACEXML_Char const *id)
{
  this->systemId_.reset (ACE::strnew (id));
}

ACEXML_INLINE ACEXML_Char const *
ACEXML_LocatorImpl::getPublicId () const
{
  return this->publicId_.get ();
}

ACEXML_INLINE ACEXML_Char const *
ACEXML_LocatorImpl::getSystemId () const
{
  return this->systemId_.get ();
}

ACEXML_INLINE void
ACEXML_LocatorImpl::incrLineNumber ()
{
  ++this->lineNumber_;
}

ACEXML_INLINE void
ACEXML_LocatorImpl::incrColumnNumber ()
{
  ++this->columnNumber_;
}
