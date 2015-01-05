// -*- C++ -*-  $Id$

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
ACEXML_LocatorImpl::getColumnNumber (void) const
{
  return this->columnNumber_;
}

ACEXML_INLINE int
ACEXML_LocatorImpl::getLineNumber (void) const
{
  return this->lineNumber_;
}

ACEXML_INLINE void
ACEXML_LocatorImpl::setPublicId (const ACEXML_Char *id)
{
  delete[] this->publicId_;
  this->publicId_ = ACE::strnew (id);
}

ACEXML_INLINE void
ACEXML_LocatorImpl::setSystemId (const ACEXML_Char *id)
{
  delete[] this->systemId_;
  this->systemId_ = ACE::strnew (id);
}

ACEXML_INLINE const ACEXML_Char *
ACEXML_LocatorImpl::getPublicId (void) const
{
  return this->publicId_;
}

ACEXML_INLINE const ACEXML_Char *
ACEXML_LocatorImpl::getSystemId (void) const
{
  return this->systemId_;
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
