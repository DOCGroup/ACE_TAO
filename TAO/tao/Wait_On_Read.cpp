#include "tao/Wait_On_Read.h"
#include "tao/Transport.h"
#include "tao/Resume_Handle.h"
#include "tao/Synch_Reply_Dispatcher.h"
#include "tao/Client_Strategy_Factory.h"
#include "tao/ORB_Core.h"
#include "tao/ORB_Time_Policy.h"
#include "ace/Reactor.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Constructor.
TAO_Wait_On_Read::TAO_Wait_On_Read (TAO_Transport *transport)
  : TAO_Wait_Strategy (transport)
{
}

int
TAO_Wait_On_Read::sending_request (TAO_ORB_Core *orb_core,
                                   TAO_Message_Semantics msg_semantics)
{
  if ((this->transport_->opened_as () == TAO::TAO_CLIENT_ROLE) &&
      (this->transport_->bidirectional_flag () == -1))
    {
      // register the transport for event handling in case of AMI requests because
      // for these requests explicitly require event handling.
      if (msg_semantics.type_ == TAO_Message_Semantics::TAO_ONEWAY_REQUEST &&
          msg_semantics.mode_ == TAO_Message_Semantics::TAO_ASYNCH_CALLBACK)
        {
          // Register the handler unless already registered (should never happen).
          if (!this->is_registered_)
            {
              this->transport_->register_handler ();
            }
        }
    }

  // Send the request.
  return this->TAO_Wait_Strategy::sending_request (orb_core, msg_semantics);
}

void
TAO_Wait_On_Read::finished_request ()
{
  if ((this->transport_->opened_as () == TAO::TAO_CLIENT_ROLE) &&
      (this->transport_->bidirectional_flag () == -1))
    {
      // ORB finished request handling so if the transport was registered for
      // an AMI call, deregister it now
      if (!this->transport_->orb_core ()->client_factory ()->use_cleanup_options () &&
          this->is_registered_)
      {
        this->transport_->remove_handler ();
      }
    }

    this->TAO_Wait_Strategy::finished_request ();
}

// Wait on the read operation.
int
TAO_Wait_On_Read::wait (ACE_Time_Value * max_wait_time,
                        TAO_Synch_Reply_Dispatcher &rd)
{
  // Start the count down timer to account for the time spent in this
  // method.
  TAO::ORB_Countdown_Time countdown (max_wait_time);

  TAO_Leader_Follower &leader_follower =
    this->transport_->orb_core ()->leader_follower ();

  rd.state_changed (TAO_LF_Event::LFS_ACTIVE, leader_follower);

  // Do the same sort of looping that is done in other wait
  // strategies.
  int retval = 0;
  TAO_Resume_Handle rh;
  while (1)
    {
      retval = this->transport_->handle_input (rh, max_wait_time);

      // If we got our reply, no need to run the loop any
      // further.
      if (!rd.keep_waiting (leader_follower))
        break;

      // @@ We are not checking for timeouts here...

      // If we got an error just break
      if (retval == -1)
        break;
    }

  if (rd.error_detected (leader_follower) || retval == -1)
    {
      this->transport_->close_connection ();
    }

  if (rd.successful (leader_follower))
     {
       TAO_ORB_Core * const oc =
         this->transport_->orb_core ();

       if (!oc->client_factory ()->use_cleanup_options ())
         return 0;

       if (TAO_debug_level > 0)
         TAOLIB_DEBUG ((LM_DEBUG,
                     ACE_TEXT ("TAO (%P|%t) - Wait_On_Read[%d]::wait (), ")
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
             TAOLIB_ERROR ((LM_ERROR,
                         ACE_TEXT ("TAO (%P|%t) - Wait_On_Read[%d]::wait (), ")
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

  if (rd.error_detected (leader_follower))
    return -1;

  return 1;
}

// No-op.
int
TAO_Wait_On_Read::register_handler ()
{
  return 0;
}

bool
TAO_Wait_On_Read::non_blocking () const
{
  return false;
}

bool
TAO_Wait_On_Read::can_process_upcalls () const
{
  return true;
}

TAO_END_VERSIONED_NAMESPACE_DECL
