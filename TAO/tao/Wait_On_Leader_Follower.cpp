// $Id$

#include "tao/Wait_On_Leader_Follower.h"
#include "tao/Pluggable.h"
#include "tao/ORB_Core.h"
#include "tao/Leader_Follower.h"
#include "tao/debug.h"

ACE_RCSID(tao, Wait_On_Leader_Follower, "$Id$")

TAO_Wait_On_Leader_Follower::TAO_Wait_On_Leader_Follower (TAO_Transport *transport)
  : TAO_Wait_Strategy (transport)
{
}

TAO_Wait_On_Leader_Follower::~TAO_Wait_On_Leader_Follower (void)
{
}

int
TAO_Wait_On_Leader_Follower::register_handler (void)
{
  return this->transport_->register_handler ();
}

int
TAO_Wait_On_Leader_Follower::non_blocking (void)
{
  return 1;
}

int
TAO_Wait_On_Leader_Follower::sending_request (TAO_ORB_Core *orb_core,
                                              int two_way)
{
  // Register the handler.
  // @@ We could probably move this somewhere else, and remove this
  //    function totally. (Alex).
  this->transport_->register_handler ();

  // Send the request.
  return this->TAO_Wait_Strategy::sending_request (orb_core,
                                                   two_way);
}

int
TAO_Wait_On_Leader_Follower::wait (ACE_Time_Value *max_wait_time,
                                   int &reply_received)
{
  // Cache the ORB core, it won't change and is used multiple times
  // below:
  TAO_ORB_Core* orb_core =
    this->transport_->orb_core ();

  TAO_Leader_Follower& leader_follower =
    orb_core->leader_follower ();

  // Obtain the lock.
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon,
                    leader_follower.lock (), -1);

  // Optmize the first iteration [no access to errno]
  int result = 1;

  //
  // Begin artificial scope for auto_ptr like helpers calling:
  // leader_follower.set_client_thread () and (maybe later on)
  // leader_follower.set_client_leader_thread ().
  //
  {
    // Calls leader_follower.set_client_thread () on construction and
    // leader_follower.reset_client_thread () on destruction.
    TAO_LF_Client_Thread_Helper client_thread_helper (leader_follower);
    ACE_UNUSED_ARG (client_thread_helper);

    ACE_Countdown_Time countdown (max_wait_time);

    // Check if there is a leader.  Note that it cannot be us since we
    // gave up our leadership when we became a client.
    if (leader_follower.leader_available ())
      {
        // = Wait as a follower.

        // Grab the condtion variable.
        ACE_SYNCH_CONDITION* cond =
          orb_core->leader_follower_condition_variable ();

        if (TAO_debug_level >= 5)
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) - wait (follower) on Transport <%x>, cond <%x>\n"),
                      this->transport_,
                      cond));

        while (!reply_received &&
               leader_follower.leader_available ())
          {
            // Add ourselves to the list, do it everytime we wake up
            // from the CV loop. Because:
            //
            // - The leader thread could have elected us as the new
            // leader.
            // - Before we can assume the role another thread becomes
            // the leader
            // - But our condition variable could have been removed
            // already, if we don't add it again we will never wake
            // up.
            //
            // Notice that we can have spurious wake ups, in that case
            // adding the leader results in an error, that must be
            // ignored.
            // You may be thinking of not removing the condition
            // variable in the code that sends the signal, but
            // removing it here, that does not work either, in that
            // case the condition variable may be used to:
            //  - Wake up because its reply arrived
            //  - Wake up because it must become the leader
            // but only the first one has any effect, so the leader is
            // lost.
            //

            (void) leader_follower.add_follower (cond);

            if (max_wait_time == 0)
              {
                if (cond == 0 || cond->wait () == -1)
                  {
                    if (TAO_debug_level >= 5)
                      ACE_DEBUG ((LM_DEBUG,
                                  ACE_TEXT ("TAO (%P|%t) - wait (follower) on <%x> ")
                                  ACE_TEXT ("cond == 0 || cond->wait () == -1 : cond = %d\n"),
                                  this->transport_, (cond == 0) ? 0 : cond));
                    return -1;
                  }
              }
            else
              {
                countdown.update ();
                ACE_Time_Value tv = ACE_OS::gettimeofday ();
                tv += *max_wait_time;
                if (cond == 0 || cond->wait (&tv) == -1)
                  {
                    if (TAO_debug_level >= 5)
                      ACE_DEBUG ((LM_DEBUG,
                                  ACE_TEXT ("TAO (%P|%t) - wait (follower) on <%x> ")
                                  ACE_TEXT ("cond == 0 || cond->wait (tv) == -1\n"),
                                  this->transport_));
                  return -1;
                  }
              }
          }

        countdown.update ();

#if 0
        // Cannot remove the follower here, we *must* remove it when
        // we signal it so the same condition is not signalled for
        // both wake up as a follower and as the next leader.
        if (leader_follower.remove_follower (cond) == -1)
          ACE_ERROR ((LM_ERROR,
                      "TAO (%P|%t) TAO_Wait_On_Leader_Follower::wait - "
                      "remove_follower failed for <%x>\n", cond));
#endif /* 0 */

        if (TAO_debug_level >= 5)
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) - done (follower) "
                                "on <%x>, reply_received %d\n"),
                      this->transport_, reply_received));

        // Now somebody woke us up to become a leader or to handle our
        // input. We are already removed from the follower queue.

        if (reply_received == 1)
          return 0;

        // FALLTHROUGH
        // We only get here if we woke up but the reply is not
        // complete yet, time to assume the leader role....
        // i.e. ACE_ASSERT (reply_received == 0);
      }

    // = Leader Code.

    // The only way to reach this point is if we must become the
    // leader, because there is no leader or we have to update to a
    // leader or we are doing nested upcalls in this case we do
    // increase the refcount on the leader in TAO_ORB_Core.

    // Calls leader_follower.set_client_leader_thread () on
    // construction and leader_follower.reset_client_leader_thread ()
    // on destruction.  Note that this may increase the refcount of
    // the leader.
    TAO_LF_Client_Leader_Thread_Helper client_leader_thread_helper (leader_follower);
    ACE_UNUSED_ARG (client_leader_thread_helper);

    {
      ACE_GUARD_RETURN (ACE_Reverse_Lock<ACE_SYNCH_MUTEX>, rev_mon,
                        leader_follower.reverse_lock (), -1);

      // Become owner of the reactor.
      ACE_Reactor *reactor = orb_core->reactor ();
      reactor->owner (ACE_Thread::self ());

      // Run the reactor event loop.

      if (TAO_debug_level >= 5)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO (%P|%t) - wait (leader):to enter reactor event loop on <%x>\n"),
                    this->transport_));

      while (1)
        {
          // Run the event loop.
          result = reactor->handle_events (max_wait_time);

          // If we got our reply, no need to run the event loop any
          // further.
          if (reply_received)
            break;

          // Did we timeout? If so, stop running the loop.
          if (result == 0 &&
              max_wait_time != 0 &&
              *max_wait_time == ACE_Time_Value::zero)
            break;

          // Other errors? If so, stop running the loop.
          if (result == -1)
            break;

          // Otherwise, keep going...
        }

      if (TAO_debug_level >= 5)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO (%P|%t) - wait : (leader) : done with reactor event loop on <%x>\n"),
                    this->transport_));
    }
  }
  //
  // End artificial scope for auto_ptr like helpers calling:
  // leader_follower.reset_client_thread () and (maybe)
  // leader_follower.reset_client_leader_thread ().
  //

  // Wake up the next leader, we cannot do that in handle_input,
  // because the woken up thread would try to get into handle_events,
  // which is at the time in handle_input still occupied. But do it
  // before checking the error in <result>, even if there is an error
  // in our input we should continue running the loop in another
  // thread.

  if (leader_follower.elect_new_leader () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("TAO (%P|%t):TAO_Wait_On_Leader_Follower::send_request: ")
                       ACE_TEXT ("Failed to unset the leader and wake up a new follower.\n")),
                      -1);

  if (result == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("TAO (%P|%t):TAO_Wait_On_Leader_Follower::wait: ")
                       ACE_TEXT ("handle_events failed.\n")),
                      -1);

  // Return an error if there was a problem receiving the reply...
  if (max_wait_time != 0)
    {
      if (reply_received != 1
          && *max_wait_time == ACE_Time_Value::zero)
        {
          result = -1;
          errno = ETIME;
        }
    }
  else
    {
      result = 0;
      if (reply_received == -1)
        {
          result = -1;
        }
    }

  return result;
}

ACE_SYNCH_CONDITION *
TAO_Wait_On_Leader_Follower::leader_follower_condition_variable (void)
{
  return this->transport_->orb_core ()->leader_follower_condition_variable ();
}

int
TAO_Wait_On_Leader_Follower::reply_dispatched (int &reply_received_flag,
                                               ACE_SYNCH_CONDITION *condition)
{
  if (condition == 0)
    return 0;

  TAO_Leader_Follower& leader_follower =
    this->transport_->orb_core ()->leader_follower ();

  // Obtain the lock.
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon,
                    leader_follower.lock (),
                    -1);

  reply_received_flag = 1;

  // We *must* remove it when we signal it so the same condition
  // is not signalled for both wake up as a follower and as the
  // next leader.
  // The follower may not be there if the reply is received while
  // the consumer is not yet waiting for it (i.e. it send the
  // request but has not blocked to receive the reply yet).
  // Ignore errors.
  (void) leader_follower.remove_follower (condition);

  if (condition->signal () == -1)
    return -1;

  return 0;
}

void
TAO_Wait_On_Leader_Follower::connection_closed (int &reply_received_flag,
                                                ACE_SYNCH_CONDITION *condition)
{
  if (condition == 0)
    return;

  TAO_Leader_Follower& leader_follower =
    this->transport_->orb_core ()->leader_follower ();

  // Obtain the lock.
  ACE_GUARD (ACE_SYNCH_MUTEX, ace_mon, leader_follower.lock ());

  reply_received_flag = -1;

  (void) leader_follower.remove_follower (condition);

  (void) condition->signal ();
}
