// -*- C++ -*-

#include "ACEXML/common/LocatorImpl.h"

#if !defined (__ACEXML_INLINE__)
# include "ACEXML/common/LocatorImpl.inl"
#endif /* __ACEXML_INLINE__ */

ACEXML_LocatorImpl::ACEXML_LocatorImpl ()
  : publicId_ (nullptr),
    systemId_ (nullptr),
    lineNumber_ (1),
    columnNumber_ (0)
{
}

ACEXML_LocatorImpl::ACEXML_LocatorImpl (const ACEXML_Char* systemId,
                                        const ACEXML_Char* publicId)
  : publicId_ (publicId ? ACE::strnew (publicId) : nullptr),
    systemId_ (systemId ? ACE::strnew (systemId) : nullptr),
    lineNumber_ (1),
    columnNumber_ (0)
{
}


ACEXML_LocatorImpl::ACEXML_LocatorImpl (const ACEXML_Locator& locator)
  : publicId_ (locator.getPublicId() ? ACE::strnew(locator.getPublicId ()): nullptr),
    systemId_ (locator.getSystemId() ? ACE::strnew(locator.getSystemId ()): nullptr),
    lineNumber_ (locator.getLineNumber ()),
    columnNumber_ (locator.getColumnNumber ())
{
}

ACEXML_LocatorImpl::~ACEXML_LocatorImpl ()
{
  this->reset();
}

void
ACEXML_LocatorImpl::reset ()
{
  delete[] this->publicId_;
  this->publicId_ = nullptr;
  delete[] this->systemId_;
  this->systemId_ = nullptr;
  this->columnNumber_ = 0;
  this->lineNumber_ = 1;
}
