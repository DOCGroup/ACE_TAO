// -*- C++ -*-  $Id$

#include "ACEXML/common/LocatorImpl.h"

#if !defined (__ACEXML_INLINE__)
# include "ACEXML/common/LocatorImpl.i"
#endif /* __ACEXML_INLINE__ */

ACEXML_LocatorImpl::ACEXML_LocatorImpl (void)
  : publicId_ (0),
    systemId_ (0),
    lineNumber_ (0),
    columnNumber_ (0)
{
}

ACEXML_LocatorImpl::ACEXML_LocatorImpl (const ACEXML_Locator *locator)
  : publicId_ (ACE::strnew (locator->getPublicId ())),
    systemId_ (ACE::strnew (locator->getSystemId ())),
    lineNumber_ (locator->getLineNumber ()),
    columnNumber_ (locator->getColumnNumber ())
{
}

ACEXML_LocatorImpl::~ACEXML_LocatorImpl (void)
{
  delete this->publicId_;
  delete this->systemId_;
}

int
ACEXML_LocatorImpl::getColumnNumber (void) const
{
  return this->columnNumber_;
}

int
ACEXML_LocatorImpl::getLineNumber (void) const
{
  return this->lineNumber_;
}

const ACEXML_Char *
ACEXML_LocatorImpl::getPublicId (void) const
{
  return this->publicId_;
}

const ACEXML_Char *
ACEXML_LocatorImpl::getSystemId (void) const
{
  return this->systemId_;
}
