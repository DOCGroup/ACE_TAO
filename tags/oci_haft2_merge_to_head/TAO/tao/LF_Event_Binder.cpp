// -*- C++ -*-
// $Id$

#include "tao/LF_Event_Binder.h"

#if !defined (__ACE_INLINE__)
# include "tao/LF_Event_Binder.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(tao, LF_Event_Binder, "$Id$")

TAO_LF_Event_Binder::~TAO_LF_Event_Binder (void)
{
  this->event_->unbind ();
}
