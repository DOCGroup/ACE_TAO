// -*- C++ -*-  $Id$

#include "ACEXML/common/Env.h"

#if !defined (__ACEXML_INLINE__)
# include "ACEXML/common/Env.i"
#endif /* __ACEXML_INLINE__ */

ACEXML_Env::ACEXML_Env (void)
  : exception_ (0)
{
}

ACEXML_Env::ACEXML_Env (const ACEXML_Env &rhs)
  : exception_ (0)
{
  if (rhs.exception())
    this->exception_ = rhs.exception_->duplicate();
}

ACEXML_Env&
ACEXML_Env::operator= (const ACEXML_Env& rhs)
{
  ACEXML_Env tmp (rhs);
  ACEXML_Exception* tmp_ex = this->exception_;
  this->exception_ = tmp.exception_;
  tmp.exception_ = tmp_ex;
  return *this;
}

ACEXML_Env::~ACEXML_Env (void)
{
  this->clear();
}
