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

ACEXML_INLINE void
ACEXML_LocatorImpl::setPublicId (const ACEXML_Char *id)
{
  delete this->publicId_;
  this->publicId_ = ACE::strnew (id);
}

ACEXML_INLINE void
ACEXML_LocatorImpl::setSystemId (const ACEXML_Char *id)
{
  delete this->systemId_;
  this->systemId_ = ACE::strnew (id);
}
