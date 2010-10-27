// $Id$

#include "tao/Wait_On_Read.h"
#include "tao/Transport.h"
#include "tao/Resume_Handle.h"
#include "tao/Synch_Reply_Dispatcher.h"
#include "tao/Client_Strategy_Factory.h"
#include "tao/ORB_Core.h"
#include "ace/Reactor.h"
#include "ace/Countdown_Time.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Constructor.
TAO_Wait_On_Read::TAO_Wait_On_Read (TAO_Transport *transport)
  : TAO_Wait_Strategy (transport)
{
}

// Destructor.
TAO_Wait_On_Read::~TAO_Wait_On_Read (void)
{
}

/*
 * Hook to specialize the wait strategy when the concrete strategy is
 * a simple "rw" strategy. Add all public/protected/private methods
 * within the *COPY* hooks.
 */

//@@ WAIT_STRATEGY_SPL_COPY_HOOK_START

// Wait on the read operation.
int
TAO_Wait_On_Read::wait (ACE_Time_Value * max_wait_time,
                        TAO_Synch_Reply_Dispatcher &rd)
{
  // Start the count down timer to account for the time spent in this
  // method.
  ACE_Countdown_Time countdown (max_wait_time);

  rd.state_changed (TAO_LF_Event::LFS_ACTIVE,
                    this->transport_->orb_core ()->leader_follower ());

  // Do the same sort of looping that is done in other wait
  // strategies.
  int retval = 0;
  TAO_Resume_Handle rh;
  while (1)
    {
      retval = this->transport_->handle_input (rh, max_wait_time);

      // If we got our reply, no need to run the loop any
      // further.
      if (!rd.keep_waiting ())
        break;

      // @@ We are not checking for timeouts here...

      // If we got an error just break
      if (retval == -1)
        break;
    }

  if (rd.error_detected () == -1 || retval == -1)
    {
      this->transport_->close_connection ();
    }

  if (rd.successful ())
     {
       TAO_ORB_Core * const oc =
         this->transport_->orb_core ();

       if (!oc->client_factory ()->use_cleanup_options ())
         return 0;

       if (TAO_debug_level > 0)
         ACE_DEBUG ((LM_DEBUG,
                     ACE_TEXT ("TAO (%P|%t) - TAO_Wait_On_Read[%d]::wait (), ")
                     ACE_TEXT ("registering handle for cleanup\n"),
                     this->transport_->id ()));

       ACE_Event_Handler * const eh =
         this->transport_->event_handler_i ();

       ACE_Reactor * const r =
         this->transport_->orb_core ()->reactor ();

       if (r->register_handler (eh,
                                ACE_Event_Handler::READ_MASK) == -1)
         {
           if (TAO_debug_level > 0)
             ACE_ERROR ((LM_ERROR,
                         ACE_TEXT ("TAO (%P|%t) - TAO_Wait_On_Read[%d]::wait (), ")
                        ACE_TEXT ("registration with reactor returned an error\n"),
                         this->transport_->id ()));
        }
       else
        {
          // Only set this flag when registration succeeds
          this->is_registered_ = true;
        }

       return 0;
     }

  if (rd.error_detected ())
    return -1;

  return 1;
}

// No-op.
int
TAO_Wait_On_Read::register_handler (void)
{
  return 0;
}

bool
TAO_Wait_On_Read::non_blocking (void) const
{
  return false;
}

bool
TAO_Wait_On_Read::can_process_upcalls (void) const
{
  return true;
}

//@@ WAIT_STRATEGY_SPL_COPY_HOOK_END
/*
 * End copy hook.
 */

TAO_END_VERSIONED_NAMESPACE_DECL
