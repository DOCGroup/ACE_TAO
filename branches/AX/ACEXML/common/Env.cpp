// -*- C++ -*-  $Id$

#include "common/Env.h"

#if !defined (__ACEXML_INLINE__)
# include "common/Env.i"
#endif /* __ACEXML_INLINE__ */

ACEXML_Env::ACEXML_Env (void)
  : exception_ (0)
{
}

ACEXML_Env::ACEXML_Env (const ACEXML_Env &ev)
  : exception_ (ev.exception_->duplicate ())
{
}

ACEXML_Env::~ACEXML_Env (void)
{
  delete this->exception_;
}
