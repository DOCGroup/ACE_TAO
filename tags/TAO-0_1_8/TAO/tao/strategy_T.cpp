// ============================================================================
//
// = FILENAME
//     strategy_T.cpp
//
// = AUTHOR
//     Chris Cleeland
//
// = VERSION
//     $Id$
// ============================================================================

#include "tao/strategy_T.h"

#if !defined (__ACE_INLINE__)
#include "tao/strategy_T.i"
#endif /* __ACE_INLINE__ */

template <class SH> int
TAO_Reactive_Strategy<SH>::activate_svc_handler (SH *svc_handler, void *arg)
{
  ACE_TRACE ("ACE_Reactive_Strategy<SVC_HANDLER>::activate_svc_handler");

  ACE_Reactor *r = TAO_ORB_Core_instance ()->reactor ();

  int result = 0;

  if (r == 0)
    result = -1;

  // Register with the Reactor with the appropriate <mask>.
  else if (r->register_handler (svc_handler, this->mask_) == -1)
    result = -1;

  // If the implementation of the reactor uses event associations
  else if (r->uses_event_associations ())
    {
      // If we don't have non-block on, it won't work with
      // WFMO_Reactor
      // This maybe too harsh
      // if (!ACE_BIT_ENABLED (this->flags_, ACE_NONBLOCK))
      // goto failure;
      if (svc_handler->open ((void *) this) != -1)
        return 0;
      else
        result = -1;
    }
  else
    // Call up to our parent to do the SVC_HANDLER initialization.
    return this->inherited::activate_svc_handler (svc_handler, arg);

  if (result == -1)
    svc_handler->close (0);

  return result;
}
