// $Id$

#include "ORB_Core_TSS_Resources.h"

TAO_ORB_Core_TSS_Resources::TAO_ORB_Core_TSS_Resources (void)
  : event_loop_thread_ (0)
  , client_leader_thread_ (0)
  , lane_ (0)
  , ts_objects_ ()
  , upcalls_temporarily_suspended_on_this_thread_ (false)
  , orb_core_ (0)
#if TAO_HAS_INTERCEPTORS == 1
    , pi_current_ ()
    , client_request_info_ (0)
#endif  /* TAO_HAS_INTERCEPTORS == 1 */
{
#if TAO_HAS_INTERCEPTORS == 1
  ACE_NEW (this->client_request_info_,
           TAO_ClientRequestInfo);
#endif  /* TAO_HAS_INTERCEPTORS == 1 */
}

TAO_ORB_Core_TSS_Resources::~TAO_ORB_Core_TSS_Resources (void)
{

#if TAO_HAS_INTERCEPTORS == 1
  CORBA::release (this->client_request_info_);
#endif  /* TAO_HAS_INTERCEPTORS == 1 */

  //@@ This is broken on platforms that use TSS emulation since this
  //   destructor is invoked after the ORB.  Since we're under
  //   pressure to release a beta, we'll have to leak the TSS objects
  //   stores in the <ts_objects_> array.  However, the only service
  //   known to currently use this array is the SSLIOP pluggable
  //   protocol.  Fortunately, it registeres a null cleanup function
  //   so we're not leaking anything yet.  We *do* need to fix this
  //   before other services start to use this array.
  //      -Ossama
  // if (this->orb_core_ != 0)
  //   this->orb_core_->tss_cleanup_funcs ()->cleanup (this->ts_objects_);
}

