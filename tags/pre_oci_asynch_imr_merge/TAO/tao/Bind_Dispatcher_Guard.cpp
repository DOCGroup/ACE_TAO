// -*- C++ -*-
// $Id$

#include "tao/Bind_Dispatcher_Guard.h"

#if !defined (__ACE_INLINE__)
# include "tao/Bind_Dispatcher_Guard.inl"
#endif /* ! __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Bind_Dispatcher_Guard::TAO_Bind_Dispatcher_Guard (CORBA::ULong request_id,
                                                      TAO_Reply_Dispatcher *rd,
                                                      TAO_Transport_Mux_Strategy *tms)
  : status_(TAO_Bind_Dispatcher_Guard::UNBIND),
    request_id_(request_id),
    tms_(tms)
{
  int const retval =
    this->tms_->bind_dispatcher (this->request_id_, rd);

  if (retval == -1)
    this->status_ = TAO_Bind_Dispatcher_Guard::NO_UNBIND;
}

TAO_Bind_Dispatcher_Guard::~TAO_Bind_Dispatcher_Guard (void)
{
  // We try unbinding. If it works it works, else cant do much about
  // it..
  if (this->status_ == TAO_Bind_Dispatcher_Guard::UNBIND)
    (void)this->tms_->unbind_dispatcher (this->request_id_);
}

TAO_END_VERSIONED_NAMESPACE_DECL
