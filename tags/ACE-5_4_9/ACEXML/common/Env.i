// -*- C++ -*- $Id$

#include "ace/Log_Msg.h"      /* to get ACE_ASSERT */
#include "ACEXML/common/Exception.h"

ACEXML_INLINE ACEXML_Exception *
ACEXML_Env::exception (void) const
{
  return this->exception_;
}

ACEXML_INLINE void
ACEXML_Env::exception (ACEXML_Exception *ex)
{
  ACE_ASSERT (ex != this->exception_);
  this->clear();
  this->exception_ = ex;
}

ACEXML_INLINE void
ACEXML_Env::clear (void)
{
  delete this->exception_;
  this->exception_ = 0;
}
