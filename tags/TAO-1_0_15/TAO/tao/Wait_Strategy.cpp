// $Id$

#include "tao/Wait_Strategy.h"
#include "tao/Pluggable.h"
#include "tao/ORB_Core.h"
#include "tao/Leader_Follower.h"
#include "tao/debug.h"

ACE_RCSID(tao, Wait_Strategy, "$Id$")

// Constructor.
TAO_Wait_Strategy::TAO_Wait_Strategy (TAO_Transport *transport)
  : transport_ (transport)
{
}

// Destructor.
TAO_Wait_Strategy::~TAO_Wait_Strategy (void)
{
}

int
TAO_Wait_Strategy::sending_request (TAO_ORB_Core * /* orb_core */,
                                    int            /* two_way */)
{
  return 0;
}

ACE_SYNCH_CONDITION *
TAO_Wait_Strategy::leader_follower_condition_variable (void)
{
  return 0;
}

// *********************************************************************

// Constructor.
TAO_Wait_On_Reactor::TAO_Wait_On_Reactor (TAO_Transport *transport)
  : TAO_Wait_Strategy (transport)
    // reply_received_ (0)
{
}

// Destructor.
TAO_Wait_On_Reactor::~TAO_Wait_On_Reactor (void)
{
}

int
TAO_Wait_On_Reactor::wait (ACE_Time_Value *max_wait_time,
                           int &reply_received)
{
  // Reactor does not change inside the loop.
  ACE_Reactor* reactor =
    this->transport_->orb_core ()->reactor ();

  // Do the event loop, till we fully receive a reply.
  int result = 0;
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

  if (result == -1 || reply_received == -1)
    return -1;

  // Return an error if there was a problem receiving the reply.
  if (max_wait_time != 0)
    {
      if (reply_received != 1 &&
          *max_wait_time == ACE_Time_Value::zero)
        {
          result = -1;
          errno = ETIME;
        }
    }
  else
    {
      result = 0;
      if (reply_received == -1)
        result = -1;
    }

  return result;
}

int
TAO_Wait_On_Reactor::handle_input (void)
{
  int result = this->transport_->handle_client_input (0);

  if (result == 1)
    {
      // this->reply_received_ = 1;
      result = 0;
    }

  // if (result == -1)
  //  reply_received = -1;

 return result;
}

// Register the handler with the Reactor.
int
TAO_Wait_On_Reactor::register_handler (void)
{
  return this->transport_->register_handler ();
}

// *********************************************************************

// Constructor.
TAO_Wait_On_Leader_Follower::TAO_Wait_On_Leader_Follower (TAO_Transport *transport)
  : TAO_Wait_Strategy (transport)
{
}

// Destructor.
TAO_Wait_On_Leader_Follower::~TAO_Wait_On_Leader_Follower (void)
{
}

// Register the handler.
int
TAO_Wait_On_Leader_Follower::register_handler (void)
{
  return this->transport_->register_handler ();
}

// *********************************************************************

// Constructor.
TAO_Exclusive_Wait_On_Leader_Follower::TAO_Exclusive_Wait_On_Leader_Follower (TAO_Transport *transport)
  : TAO_Wait_On_Leader_Follower (transport),
    calling_thread_ (ACE_OS::NULL_thread),
    cond_response_available_ (0),
    expecting_response_ (0),
    reply_received_ (0)
{
}

// Destructor.
TAO_Exclusive_Wait_On_Leader_Follower::~TAO_Exclusive_Wait_On_Leader_Follower (void)
{
  delete this->cond_response_available_;
  this->cond_response_available_ = 0;
}


// @@ Why do we need <orb_core> and the <two_way> flag? <orb_core> is
//    with the <Transport> object and <two_way> flag wont make sense
//    at this level since this is common for AMI also. (Alex).
int
TAO_Exclusive_Wait_On_Leader_Follower::sending_request (TAO_ORB_Core *orb_core,
                                                        int two_way)
{
  //
  // Note that in previous versions of this code, the assignment of
  // the following three variables was protected by the
  // leader/followers lock.  In the case of oneways, these fields are
  // not used, so this seems excessive. In the case of twoways, this
  // stuff is done before the request is actually sent, therefore,
  // there isn't any chance of the leader resetting these fields
  // simultaneously.  Therefore, the lock has been removed.
  //

  // The last request may have left this unitialized
  this->reply_received_ = 0;

  // Set the state so that we know we're looking for a response.
  this->expecting_response_ = two_way;

  // remember in which thread the client connection handler was running
  this->calling_thread_ = ACE_Thread::self ();

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("TAO (%P|%t) - sending request for <%x>\n"),
                this->transport_));

  // Register the handler.
  this->transport_->register_handler ();

  // Send the request.
  int result =
    this->TAO_Wait_Strategy::sending_request (orb_core,
                                              two_way);

  if (result == -1)
    {
      ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon,
                        orb_core->leader_follower ().lock (), -1);

      this->reply_received_ = 0;
      this->expecting_response_ = 0;
      this->calling_thread_ = ACE_OS::NULL_thread;

      //ACE_DEBUG ((LM_DEBUG, "TAO (%P|%t) - failed request for <%x>\n",
      //this->transport_));
    }
  return result;
}

int
TAO_Exclusive_Wait_On_Leader_Follower::wait (ACE_Time_Value *max_wait_time,
                                             int &)
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

  int result = 0;

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

        // wait until we have input available or there is no leader, in
        // which case we must become the leader anyway....
        // @@ Alex: I am uncertain about how many condition variables
        //    should we have, should there be one-per-thread (after all
        //    the thread blocks on the condition variable) or there
        //    should be one per-connection.  I think the first case is
        //    the "Right Thing"[tm]
        ACE_SYNCH_CONDITION* cond =
          this->cond_response_available ();

#if defined (TAO_DEBUG_LEADER_FOLLOWER)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t): TAO_Wait_On_LF::wait - "
                    "(follower) on <%x>\n",
                    cond));
#endif /* TAO_DEBUG_LEADER_FOLLOWER */

        while (!this->reply_received_
               && leader_follower.leader_available ())
          {
            // Add to the follower set, that operation will if the
            // condition variable returns due to an spurious wake up
            // (i.e. a wait interrupted by the OS) but otherwise we
            // risk dead-locks:
            //   Assume that we are the only follower, another thread
            //   is the leader and it completes its work, it sends us
            //   the signal and removes us from the set.  Before
            //   waking up another thread becomes the leader, when we
            //   do wake up we believe that it was a false return from
            //   the condition variable and go into the loop again.
            //   But now the follower set is empty and nobody is ever
            //   going to wake us up, dead-locking the application.

            if (leader_follower.add_follower (cond) == -1)
              {
                // -1 indicates a severe problem, like running out of
                // memory, the comment above does not apply in this
                // case.
                return -1;
              }

#if defined (TAO_DEBUG_LEADER_FOLLOWER)
            ACE_DEBUG ((LM_DEBUG,
                        "TAO (%P|%t): TAO_Wait_On_LF::wait - "
                        "waiting in follower <%x>\n",
                        cond));
#endif /* TAO_DEBUG_LEADER_FOLLOWER */

            if (max_wait_time == 0)
              {
                if (cond == 0 || cond->wait () == -1)
                  return -1;
              }
            else
              {
                countdown.update ();
                ACE_Time_Value tv = ACE_OS::gettimeofday ();
                tv += *max_wait_time;
                if (cond == 0 || cond->wait (&tv) == -1)
                  return -1;
              }
          }

        countdown.update ();

#if defined (TAO_DEBUG_LEADER_FOLLOWER)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t): TAO_Wait_On_LF::wait - "
                    "done (follower:%d) on <%x>\n",
                    this->reply_received_, cond));
#endif /* TAO_DEBUG_LEADER_FOLLOWER */

        // Now somebody woke us up to become a leader or to handle our
        // input. We are already removed from the follower queue.
        if (this->reply_received_ == 1)
          {
            // But first reset our state in case we are invoked
            // again...
            this->reply_received_ = 0;
            this->expecting_response_ = 0;
            this->calling_thread_ = ACE_OS::NULL_thread;

            return 0;
          }
        else if (this->reply_received_ == -1)
          {
            // But first reset our state in case we are invoked
            // again...
            this->reply_received_ = 0;
            this->expecting_response_ = 0;
            this->calling_thread_ = ACE_OS::NULL_thread;

            return -1;
          }
        // FALLTHROUGH
        // We only get here if we woke up but the reply is not
        // complete yet, time to assume the leader role....
        // i.e. ACE_ASSERT (this->reply_received_ == 0);
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

      // @@ Do we need to do this?
      // Become owner of the reactor.
      orb_core->reactor ()->owner (ACE_Thread::self ());

      // Run the reactor event loop.

#if defined (TAO_DEBUG_LEADER_FOLLOWER)
      ACE_DEBUG ((LM_DEBUG,
                  "TAO (%P|%t): TAO_Wait_On_LF::wait - "
                  "wait (leader) on <%x>\n",
                  this->transport_));
#endif /* TAO_DEBUG_LEADER_FOLLOWER */

      while (1)
        {
          // Run the event loop.
          result = orb_core->reactor ()->handle_events (max_wait_time);

          // If we got our reply, no need to run the event loop any
          // further.
          if (this->reply_received_)
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

#if defined (TAO_DEBUG_LEADER_FOLLOWER)
      ACE_DEBUG ((LM_DEBUG,
                  "TAO (%P|%t): TAO_Wait_On_LF::wait - "
                  "done (leader) on <%x>\n",
                  this->transport_));
#endif /* TAO_DEBUG_LEADER_FOLLOWER */
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
                       ASYS_TEXT ("TAO (%P|%t): TAO_Wait_On_LF::wait - ")
                       ASYS_TEXT ("Failed to unset the leader and wake up a ")
                       ASYS_TEXT ("new follower.\n")),
                      -1);

  if (result == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ASYS_TEXT ("TAO (%P|%t): TAO_Wait_On_LF::wait: %p.\n"),
                       ASYS_TEXT ("handle_events failed")),
                      -1);

  // Return an error if there was a problem receiving the reply...
  if (max_wait_time != 0)
    {
      if (this->reply_received_ != 1
          && *max_wait_time == ACE_Time_Value::zero)
        {
          result = -1;
          errno = ETIME;
        }
    }
  else
    {
      result = 0;
      if (this->reply_received_ == -1)
        {
          result = -1;
        }
    }

  // Make us reusable
  this->reply_received_ = 0;
  this->expecting_response_ = 0;
  this->calling_thread_ = ACE_OS::NULL_thread;

  return result;
}

// Handle the input. Return -1 on error, 0 on success.
int
TAO_Exclusive_Wait_On_Leader_Follower::handle_input (void)
{
  TAO_ORB_Core* orb_core =
    this->transport_->orb_core ();

  // Obtain the lock.
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon,
                    orb_core->leader_follower ().lock (),
                    -1);

  //  ACE_DEBUG ((LM_DEBUG, "TAO (%P|%t) Wait_On_LF::handle_input - "
  //              "reading reply <%x>\n",
  //              this->transport_));

  // A message is received but not data was sent, flag this as an
  // error, but we should do more....
  // @@ Alex: this could be a CloseConnection message or something
  //    similar, has to be handled...

  //
  // The following check is conflicting with the ability to buffer
  // asynchronous calls.  If we mark the asynchronous call as a twoway
  // call, then buffering cannot take place.  If we mark it as a
  // oneway call, then the following check fails.  For now I have
  // selected to disable the check.  The long term fix is to separate
  // out the two concerns (a) can the call be buffered and (b) are we
  // expecting a response.
  //

  /*
  if (!this->expecting_response_)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ASYS_TEXT ("TAO (%P|%t) - Wait_On_LF::handle_input, ")
                    ASYS_TEXT ("unexpected on <%x>\n"),
                    this->transport_));
      return -1;
    }
  */

  // Receive any data that is available, without blocking...
  int result = this->transport_->handle_client_input (0);

  // Data was read, but there the reply has not been completely
  // received...
  if (result == 0)
    return 0;

  if (result == -1)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ASYS_TEXT ("TAO (%P|%t) - Wait_On_LF::handle_input, ")
                    ASYS_TEXT ("handle_client_input == -1\n")));
      this->reply_received_ = -1;
    }

  if (result == 1)
    {
      // Change the result value to something that the Reactor can
      // understand
      result = 0;
      this->reply_received_ = 1;
    }

  // Wake up any threads waiting for this message, either because the
  // message failed or because we really received it.
  this->wake_up ();

  return result;
}

ACE_SYNCH_CONDITION *
TAO_Exclusive_Wait_On_Leader_Follower::cond_response_available (void)
{
  // @@ TODO This condition variable should per-ORB-per-thread, not
  // per-connection, it is a waste to have more than one of this in
  // the same thread.
  if (this->cond_response_available_ == 0)
    {
      ACE_SYNCH_MUTEX &lock =
        this->transport_->orb_core ()->leader_follower().lock ();
      ACE_NEW_RETURN (this->cond_response_available_,
                      ACE_SYNCH_CONDITION (lock),
                      0);
    }
  return this->cond_response_available_;
}

void
TAO_Exclusive_Wait_On_Leader_Follower::wake_up (void)
{
  if (ACE_OS::thr_equal (this->calling_thread_, ACE_Thread::self ()))
    {
#if defined (TAO_DEBUG_LEADER_FOLLOWER)
      ACE_DEBUG ((LM_DEBUG,
                  "TAO (%P|%t) Wait_On_LF::wake_up - "
                  "same thread\n"));
#endif /* TAO_DEBUG_LEADER_FOLLOWER */
      // We are the leader thread, simply return 0, handle_events()
      // will return because there was at least one event (this one!)
      return;
    }

  // We are not the leader thread, but we have our data, wake up
  // ourselves and then return 0 so the leader thread can continue
  // doing its job....

  // At this point we might fail to remove the follower, because
  // it has been already chosen to become the leader, so it is
  // awake and will get this too.
  ACE_SYNCH_CONDITION* cond =
    this->cond_response_available ();

#if defined (TAO_DEBUG_LEADER_FOLLOWER)
  ACE_DEBUG ((LM_DEBUG,
              "TAO (%P|%t) Wait_On_LF::wake_up - "
              "waking up <%x>\n",
              cond));
#endif /* TAO_DEBUG_LEADER_FOLLOWER */

  TAO_Leader_Follower& leader_follower =
    this->transport_->orb_core ()->leader_follower ();

  // We *must* remove it when we signal it so the same condition is
  // not signalled for both wake up as a follower and as the next
  // leader.
  // The follower may not be there if the reply is received while the
  // consumer is not yet waiting for it (i.e. it send the request but
  // has not blocked to receive the reply yet)
  (void) leader_follower.remove_follower (cond); // Ignore errors

  if (cond != 0)
    (void) cond->signal ();
}

// *********************************************************************

// Constructor.
TAO_Muxed_Wait_On_Leader_Follower::TAO_Muxed_Wait_On_Leader_Follower (TAO_Transport *transport)
  : TAO_Wait_On_Leader_Follower (transport)
{
}

// Destructor.
TAO_Muxed_Wait_On_Leader_Follower::~TAO_Muxed_Wait_On_Leader_Follower (void)
{
}

int
TAO_Muxed_Wait_On_Leader_Follower::sending_request (TAO_ORB_Core *orb_core,
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
TAO_Muxed_Wait_On_Leader_Follower::wait (ACE_Time_Value *max_wait_time,
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
                      ASYS_TEXT ("TAO (%P|%t) - wait (follower) on Transport <%x>, cond <%x>\n"),
                      this->transport_,
                      cond));

        // Add ourselves to the list, do it only once because we can
        // wake up multiple times from the CV loop. And only do it if
        // the reply has not been received (it could have arrived
        // while we were preparing to receive it).

        if (!reply_received
            && leader_follower.leader_available ())
          {
            if (leader_follower.add_follower (cond) == -1)
              ACE_ERROR ((LM_ERROR,
                          ASYS_TEXT ("TAO (%P|%t) TAO_Muxed_Wait_On_Leader_Follower::wait - ")
                          ASYS_TEXT ("add_follower failed for <%x>\n"),
                          cond));
          }


        while (!reply_received &&
               leader_follower.leader_available ())
          {
            if (max_wait_time == 0)
              {
                if (cond == 0 || cond->wait () == -1)
                  {
                    if (TAO_debug_level >= 5)
                      ACE_DEBUG ((LM_DEBUG,
                                  ASYS_TEXT ("TAO (%P|%t) - wait (follower) on <%x> ")
                                  ASYS_TEXT ("cond == 0 || cond->wait () == -1 : cond = %d\n"),
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
                                  ASYS_TEXT ("TAO (%P|%t) - wait (follower) on <%x> ")
                                  ASYS_TEXT ("cond == 0 || cond->wait (tv) == -1\n"),
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
                      "TAO (%P|%t) TAO_Muxed_Wait_On_Leader_Follower::wait - "
                      "remove_follower failed for <%x>\n", cond));
#endif /* 0 */

        if (TAO_debug_level >= 5)
          ACE_DEBUG ((LM_DEBUG,
                      ASYS_TEXT ("TAO (%P|%t) - done (follower) on <%x>, reply_received %d\n"),
                      this->transport_, reply_received));

        // Now somebody woke us up to become a leader or to handle our
        // input. We are already removed from the follower queue.

        if (reply_received == 1)
          return 0;

        // FALLTHROUGH
        // We only get here if we woke up but the reply is not
        // complete yet, time to assume the leader role....
        // i.e. ACE_ASSERT (this->reply_received_ == 0);
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

      // @@ Do we need to do this?
      // Become owner of the reactor.
      orb_core->reactor ()->owner (ACE_Thread::self ());

      // Run the reactor event loop.

      if (TAO_debug_level >= 5)
        ACE_DEBUG ((LM_DEBUG,
                    ASYS_TEXT ("TAO (%P|%t) - wait (leader):to enter reactor event loop on <%x>\n"),
                    this->transport_));

      while (1)
        {
          // Run the event loop.
          result = orb_core->reactor ()->handle_events (max_wait_time);

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
                    ASYS_TEXT ("TAO (%P|%t) - wait : (leader) : done with reactor event loop on <%x>\n"),
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
                       ASYS_TEXT ("TAO:%N:%l:(%P|%t):TAO_Muxed_Wait_On_Leader_Follower::send_request: ")
                       ASYS_TEXT ("Failed to unset the leader and wake up a new follower.\n")),
                      -1);

  if (result == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ASYS_TEXT ("TAO:%N:%l:(%P|%t):TAO_Muxed_Wait_On_Leader_Follower::wait: ")
                       ASYS_TEXT ("handle_events failed.\n")),
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

// Handle the input. Return -1 on error, 0 on success.
int
TAO_Muxed_Wait_On_Leader_Follower::handle_input (void)
{
  // Cache the ORB core, it won't change and is used multiple times
  // below:
  TAO_ORB_Core* orb_core =
    this->transport_->orb_core ();

  // Obtain the lock.
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon,
                    orb_core->leader_follower ().lock (),
                    -1);

  if (TAO_debug_level >= 5)
    ACE_DEBUG ((LM_DEBUG,
                ASYS_TEXT ("TAO (%P|%t) - reading reply on <%x>\n"),
                this->transport_));

  // Receive any data that is available, without blocking...
  int result = this->transport_->handle_client_input (0);

  // Data was read, but there the reply has not been completely
  // received...
  if (result == 0)
    return 0;

  if (result == -1)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ASYS_TEXT ("TAO (%P|%t) - Wait_On_LF::handle_input, ")
                    ASYS_TEXT ("handle_client_input == -1\n")));
      // this->reply_received_ = -1;
    }

  if (result == 1)
    {
      // Change the result value to something that the Reactor can
      // understand
      result = 0;

      // reply_received_ = 1;
      // This would have been done by the dispatch already.
    }

  // Wake up any threads waiting for this message, either because the
  // message failed or because we really received it.
  // this->wake_up ();
  // <wake_up> will be done in the <dispatch_reply>

  return result;
}

ACE_SYNCH_CONDITION *
TAO_Muxed_Wait_On_Leader_Follower::leader_follower_condition_variable (void)
{
  return this->transport_->orb_core ()->leader_follower_condition_variable ();
}

// *********************************************************************

// Constructor.
TAO_Wait_On_Read::TAO_Wait_On_Read (TAO_Transport *transport)
  : TAO_Wait_Strategy (transport)
{
}

// Destructor.
TAO_Wait_On_Read::~TAO_Wait_On_Read (void)
{
}

// Wait on the read operation.
int
TAO_Wait_On_Read::wait (ACE_Time_Value * max_wait_time,
                        int &)
{
  int reply_complete = 0;
  while (reply_complete != 1)
    {
      reply_complete =
        this->transport_->handle_client_input (1, max_wait_time);
      if (reply_complete == -1)
        return -1;
    }

  return 0;
}

// Handle the input. Delegate this job to Transport object.
int
TAO_Wait_On_Read::handle_input (void)
{
  // Block to get the whole message.
  return this->transport_->handle_client_input (1);
}

// No-op.
int
TAO_Wait_On_Read::register_handler (void)
{
  return 0;
}
