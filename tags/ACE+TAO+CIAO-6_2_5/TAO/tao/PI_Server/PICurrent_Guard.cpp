// -*- C++ -*-
// $Id$

#include "tao/PI_Server/PICurrent_Guard.h"

#if TAO_HAS_INTERCEPTORS == 1

#include "tao/ORB_Core.h"
#include "tao/TAO_Server_Request.h"
#include "tao/PI/PICurrent.h"
#include "tao/PI/PICurrent_Impl.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO::PICurrent_Guard::PICurrent_Guard (TAO_ServerRequest &server_request,
                                       bool tsc_to_rsc)
  : src_ (0),
    dest_ (0)
{
  // This constructor is used on the server side.

  // Retrieve the thread scope current (no TSS access incurred yet).
  CORBA::Object_ptr pi_current_obj =
    server_request.orb_core ()->pi_current ();

  TAO::PICurrent *pi_current =
    dynamic_cast <TAO::PICurrent*> (pi_current_obj);

  // If the slot count is zero, there is nothing to copy.  Prevent any
  // copying (and hence TSS accesses) from occurring.
  if (pi_current != 0 && pi_current->slot_count () != 0)
    {
      // Retrieve the request scope current.
      PICurrent_Impl * rsc = server_request.rs_pi_current ();

      // Retrieve the thread scope current.
      PICurrent_Impl * tsc = pi_current->tsc ();

      if (tsc_to_rsc)
        {
          // TSC to RSC copy.
          // Occurs after receive_request() interception point and
          // upcall.
          this->src_  = tsc;
          this->dest_ = rsc;
        }
      else
        {
          // RSC to TSC copy.
          // Occurs after receive_request_service_contexts()
          // interception point.
          this->src_  = rsc;
          this->dest_ = tsc;
        }
    }
}

TAO::PICurrent_Guard::~PICurrent_Guard (void)
{
  if (this->src_ != 0 && this->dest_ != 0
      && this->src_ != this->dest_)
    {
      this->dest_->take_lazy_copy (this->src_);
    }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif  /* TAO_HAS_INTERCEPTORS == 1 */
