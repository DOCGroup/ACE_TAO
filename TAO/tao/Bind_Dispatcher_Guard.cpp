// This may look like C, but it's really -*- C++ -*-
// $Id$


#include "tao/Bind_Dispatcher_Guard.h"

#if !defined (__ACE_INLINE__)
# include "tao/Bind_Dispatcher_Guard.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID (tao,
           Bind_Dispatcher_Guard,
           "$Id$")

TAO_Bind_Dispatcher_Guard::TAO_Bind_Dispatcher_Guard (
                                         CORBA::ULong request_id,
                                         TAO_Reply_Dispatcher* rd,
                                         TAO_Transport_Mux_Strategy* tms)
 : status_(TAO_Bind_Dispatcher_Guard::UNBIND),
   request_id_(request_id),
   rd_(rd),
   tms_(tms)
{
  int retval =
    this->tms_->bind_dispatcher (this->request_id_,
                                 this->rd_);

  if (retval == -1 )
    this->status_ =
      TAO_Bind_Dispatcher_Guard::NO_UNBIND;
}

TAO_Bind_Dispatcher_Guard::~TAO_Bind_Dispatcher_Guard (void)
{
  // We try unbinding. If it works it works, else cant do much about
  // it..
  if (this->status_ == TAO_Bind_Dispatcher_Guard::UNBIND)
    (void) this->tms_->unbind_dispatcher (this->request_id_);
}
