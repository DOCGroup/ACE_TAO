// This may look like C, but it's really -*- C++ -*-
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


ACE_INLINE void
TAO_Bind_Dispatcher_Guard::status (int s)
{
  this->status_ = s;
}

ACE_INLINE int
TAO_Bind_Dispatcher_Guard::status (void) const
{
  return this->status_;
}

ACE_INLINE int
TAO_Bind_Dispatcher_Guard::unbind_dispatcher (void)
{
  int retval =
    this->tms_->unbind_dispatcher (this->request_id_);

  // Already unbound and so do not try again during destruction.
  this->status_ =
    TAO_Bind_Dispatcher_Guard::NO_UNBIND;

  return retval;
}
