// -*- C++ -*-

#include "ACEXML/common/LocatorImpl.h"

#if !defined (__ACEXML_INLINE__)
# include "ACEXML/common/LocatorImpl.inl"
#endif /* __ACEXML_INLINE__ */

ACEXML_LocatorImpl::ACEXML_LocatorImpl ()
{
}

ACEXML_LocatorImpl::ACEXML_LocatorImpl (ACEXML_Char const *systemId,
                                        ACEXML_Char const *publicId)
  : publicId_ (publicId ? ACE::strnew (publicId) : nullptr),
    systemId_ (systemId ? ACE::strnew (systemId) : nullptr)
{
}


ACEXML_LocatorImpl::ACEXML_LocatorImpl (ACEXML_Locator const& locator)
  : publicId_ (locator.getPublicId () ? ACE::strnew (locator.getPublicId ()) : nullptr),
    systemId_ (locator.getSystemId () ? ACE::strnew (locator.getSystemId ()) : nullptr),
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
  this->publicId_.reset ();
  this->systemId_.reset ();
  this->columnNumber_ = 0;
  this->lineNumber_ = 1;
}
