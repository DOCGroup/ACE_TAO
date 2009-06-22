// -*- C++ -*-
//
// $Id$

// ===========================================================================
//
// = LIBRARY
//     TAO
//
// = AUTHOR
//     Carlos O'Ryan <coryan@uci.edu>,
//     Chad Elliott <elliott_c@ociweb.com>
//
// ===========================================================================

#include "tao/Reply_Dispatcher.h"
#include "tao/Transport_Mux_Strategy.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE void
TAO_Bind_Dispatcher_Guard::status (TAO_Bind_Dispatcher_Status s)
{
  this->status_ = s;
}

ACE_INLINE TAO_Bind_Dispatcher_Guard::TAO_Bind_Dispatcher_Status
TAO_Bind_Dispatcher_Guard::status (void) const
{
  return this->status_;
}

ACE_INLINE int
TAO_Bind_Dispatcher_Guard::unbind_dispatcher (void)
{
  int const retval =
    this->tms_->unbind_dispatcher (this->request_id_);

  // Already unbound and so do not try again during destruction.
  this->status_ =
    TAO_Bind_Dispatcher_Guard::NO_UNBIND;

  return retval;
}

TAO_END_VERSIONED_NAMESPACE_DECL
