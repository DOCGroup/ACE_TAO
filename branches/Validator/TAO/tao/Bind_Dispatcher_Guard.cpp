// This may look like C, but it's really -*- C++ -*-
// $Id$

// ===========================================================================
//
// = LIBRARY
//     TAO
//
// = AUTHOR
//     Chad Elliott <elliott_c@ociweb.com>
//
// ===========================================================================

#include "tao/Bind_Dispatcher_Guard.h"

#if !defined (__ACE_INLINE__)
# include "tao/Bind_Dispatcher_Guard.i"
#endif /* ! __ACE_INLINE__ */

TAO_Bind_Dispatcher_Guard::~TAO_Bind_Dispatcher_Guard (void)
{
  if (this->status_ == 0) {
    this->tms_->unbind_dispatcher (this->request_id_);
  }
}
