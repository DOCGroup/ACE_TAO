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


ACE_INLINE
TAO_Bind_Dispatcher_Guard::TAO_Bind_Dispatcher_Guard (
                                         CORBA::ULong request_id,
                                         TAO_Reply_Dispatcher* rd,
                                         TAO_Transport_Mux_Strategy* tms)
 : status_(0),
   request_id_(request_id),
   rd_(rd),
   tms_(tms)
{
  this->status_ = this->tms_->bind_dispatcher (this->request_id_,
                                               this->rd_);
}


ACE_INLINE
TAO_Bind_Dispatcher_Guard::~TAO_Bind_Dispatcher_Guard (void)
{
  if (this->status_ == 0) {
    this->tms_->unbind_dispatcher (this->request_id_);
  }
}


ACE_INLINE int &
TAO_Bind_Dispatcher_Guard::status (void)
{
  return status_;
}
