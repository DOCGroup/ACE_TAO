// -*- C++ -*-  $Id$

#include "ACEXML/common/LocatorImpl.h"

#if !defined (__ACEXML_INLINE__)
# include "ACEXML/common/LocatorImpl.i"
#endif /* __ACEXML_INLINE__ */

ACEXML_LocatorImpl::ACEXML_LocatorImpl (void)
  : publicId_ (0),
    systemId_ (0),
    lineNumber_ (1),
    columnNumber_ (0)
{
}

ACEXML_LocatorImpl::ACEXML_LocatorImpl (const ACEXML_Locator& locator)
  : publicId_ (ACE::strnew (locator.getPublicId ())),
    systemId_ (ACE::strnew (locator.getSystemId ())),
    lineNumber_ (locator.getLineNumber ()),
    columnNumber_ (locator.getColumnNumber ())
{
}

ACEXML_LocatorImpl::~ACEXML_LocatorImpl (void)
{

}

void
ACEXML_LocatorImpl::reset (void)
{
  delete[] this->publicId_;
  this->publicId_ = 0;
  delete[] this->systemId_;
  this->systemId_ = 0;
  this->columnNumber_ = 0;
  this->lineNumber_ = 1;
}
