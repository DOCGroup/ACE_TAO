// $Id$

#include "tao/Wait_Strategy.h"
#include "tao/Pluggable.h"
#include "tao/ORB_Core.h"
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
                                    int /* two_way */)
{
  return 0;
}

// *********************************************************************

// Constructor.
TAO_Wait_On_Reactor::TAO_Wait_On_Reactor (TAO_Transport *transport)
  : TAO_Wait_Strategy (transport),
    reply_received_ (0)
{
}

// Destructor.
TAO_Wait_On_Reactor::~TAO_Wait_On_Reactor (void)
{
}

int
TAO_Wait_On_Reactor::wait (void)
{
  // Reactor does not change inside the loop.
  ACE_Reactor* reactor =
    this->transport_->orb_core ()->reactor ();

  // @@ Carlos: Can we rely on <reply_received> flag in the AMI case?
  //    It depends on whether we are expecting replies or not, right?
  //    So, I think we can simply return from this loop, when some
  //    event occurs, and the invocation guy can call us again, if it
  //    wants to. (AMI will call, if it is expecting replies, SMI will
  //    call if the reply is not arrived) (Alex).
  // @@ Alex: I think you are right, let's fix it later....

  // Do the event loop, till we received the reply.

  int result = 0;
  this->reply_received_ = 0;
  while (this->reply_received_ == 0 && result >= 0)
    {
      result = reactor->handle_events (/* timeout */);
    }

  if (result == -1 || this->reply_received_ == -1)
    return -1;

  return 0;
}

int
TAO_Wait_On_Reactor::handle_input (void)
{
  int result = this->transport_->handle_client_input (0);

  if (result == 1)
    {
      this->reply_received_ = 1;
      result = 0;
    }

  if (result == -1)
    this->reply_received_ = -1;

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
  : TAO_Wait_Strategy (transport),
    calling_thread_ (ACE_OS::NULL_thread),
    cond_response_available_ (0),
    expecting_response_ (0),
    reply_received_ (0)
{
}

// Destructor.
TAO_Wait_On_Leader_Follower::~TAO_Wait_On_Leader_Follower (void)
{
  delete this->cond_response_available_;
  this->cond_response_available_ = 0;
}


// @@ Why do we need <orb_core> and the <two_way> flag? <orb_core> is
//    with the <Transport> object and <two_way> flag wont make sense
//    at this level since this is common for AMI also. (Alex).
int
TAO_Wait_On_Leader_Follower::sending_request (TAO_ORB_Core *orb_core,
                                              int two_way)
{
  {
    ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon,
                      orb_core->leader_follower_lock (), -1);

    // The last request may have left this unitialized
    this->reply_received_ = 0;

    // Set the state so that we know we're looking for a response.
    this->expecting_response_ = two_way;

    // remember in which thread the client connection handler was running
    this->calling_thread_ = ACE_Thread::self ();

    //if (TAO_debug_level > 0)
    //ACE_DEBUG ((LM_DEBUG, "TAO (%P|%t) - sending request for <%x>\n",
    //this->transport_));

  }

  // @@ Should we do here that checking for the difference in the
  //    Reactor used??? (Alex).

  // Register the handler.
  this->transport_->register_handler ();
  // @@ Carlos: We do this only if the reactor is different right?
  //    (Alex)
  // @@ Alex: that is taken care of in
  //    IIOP_Transport::register_handler, but maybe we shouldn't do
  //    this checking everytime, I recall that there was a problem
  //    (sometime ago) about using the wrong ORB core, but that may
  //    have been fixed...

  // Send the request
  int result =
    this->TAO_Wait_Strategy::sending_request (orb_core,
                                              two_way);

  if (result == -1)
    {
      ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon,
                        orb_core->leader_follower_lock (), -1);

      this->reply_received_ = 0;
      this->expecting_response_ = 0;
      this->calling_thread_ = ACE_OS::NULL_thread;

      //ACE_DEBUG ((LM_DEBUG, "TAO (%P|%t) - failed request for <%x>\n",
      //this->transport_));
    }
  return result;
}

int
TAO_Wait_On_Leader_Follower::wait (void)
{
  // @@ Do we need this code (checking for the difference in the
  //    Reactor)? (Alex).
  // @@ Alex: yes, the same connection may be used in multiple
  //    threads, each with its own reactor.
  // @@ Carlos: But, where is that code now? I  cant see it here now?
  //    (Alex).

  // Cache the ORB core, it won't change and is used multiple times
  // below:
  TAO_ORB_Core* orb_core =
    this->transport_->orb_core ();

  // Obtain the lock.
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon,
                    orb_core->leader_follower_lock (), -1);

  // Check if there is a leader, but the leader is not us
  if (orb_core->leader_available () && !orb_core->I_am_the_leader_thread ())
    {
      // = Wait as a follower.

      //ACE_DEBUG ((LM_DEBUG, "TAO (%P|%t) - wait (follower) on <%x>\n",
      //this->transport_));

      // wait until we have input available or there is no leader, in
      // which case we must become the leader anyway....
      // @@ Alex: I am uncertain about how many condition variables
      //    should we have, should there be one-per-thread (after all
      //    the thread blocks on the condition variable) or there
      //    should be one per-connection.  I think the first case is
      //    the "Right Thing"[tm]
      ACE_SYNCH_CONDITION* cond =
        this->cond_response_available ();

      // Add ourselves to the list, do it only once because we can
      // wake up multiple times from the CV loop
      if (orb_core->add_follower (cond) == -1)
        ACE_ERROR ((LM_ERROR,
                    "TAO (%P|%t) TAO_Wait_On_Leader_Follower::wait - "
                    "add_follower failed for <%x>\n", cond));

      while (!this->reply_received_ && orb_core->leader_available ())
        {
          if (cond == 0 || cond->wait () == -1)
            return -1;
        }

      if (orb_core->remove_follower (cond) == -1)
        ACE_ERROR ((LM_ERROR,
                    "TAO (%P|%t) TAO_Wait_On_Leader_Follower::wait - "
                    "remove_follower failed for <%x>\n", cond));

      //ACE_DEBUG ((LM_DEBUG, "TAO (%P|%t) - done (follower:%d) on <%x>\n",
      //this->reply_received_, this->transport_));

      // Now somebody woke us up to become a leader or to handle
      // our input. We are already removed from the follower queue.
      if (this->reply_received_ == 1)
        {
          // But first reset our state in case we are invoked again...
          this->reply_received_ = 0;
          this->expecting_response_ = 0;
          this->calling_thread_ = ACE_OS::NULL_thread;

          return 0;
        }
      else if (this->reply_received_ == -1)
        {
          // But first reset our state in case we are invoked again...
          this->reply_received_ = 0;
          this->expecting_response_ = 0;
          this->calling_thread_ = ACE_OS::NULL_thread;

          return -1;
        }
      // FALLTHROUGH
      // We only get here if we woke up but the reply is not complete
      // yet, time to assume the leader role....

    }

  // = Leader Code.

  // The only way to reach this point is if we must become the leader,
  // because there is no leader or we have to update to a leader or we
  // are doing nested upcalls in this case we do increase the refcount
  // on the leader in TAO_ORB_Core.

  // This might increase the refcount of the leader.
  orb_core->set_leader_thread ();

  // Release the lock.
  if (ace_mon.release () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "TAO:%N:%l:(%P|%t): TAO_Wait_On_Leader_Follower::wait: "
                       "Failed to release the lock.\n"),
                      -1);

  // Become owner of the reactor.
  orb_core->reactor ()->owner (ACE_Thread::self ());

  // Run the reactor event loop.

  int result = 0;

  //ACE_DEBUG ((LM_DEBUG, "TAO (%P|%t) - wait (leader) on <%x>\n",
  //this->transport_));

  while (result >= 0 && this->reply_received_ == 0)
    result = orb_core->reactor ()->handle_events ();

  //ACE_DEBUG ((LM_DEBUG, "TAO (%P|%t) - done (leader) on <%x>\n",
  //this->transport_));

  // Re-acquire the lock.
  if (ace_mon.acquire () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "TAO:%N:%l:(%P|%t): TAO_Wait_On_Leader_Follower::wait: "
                       "Failed to acquire the lock.\n"),
                      -1);

  // Wake up the next leader, we cannot do that in handle_input,
  // because the woken up thread would try to get into
  // handle_events, which is at the time in handle_input still
  // occupied. But do it before checking the error in <result>, even
  // if there is an error in our input we should continue running the
  // loop in another thread.

  if (orb_core->unset_leader_wake_up_follower () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "TAO:%N:%l:(%P|%t):TAO_Wait_On_Leader_Follower::send_request: "
                       "Failed to unset the leader and wake up a new follower.\n"),
                      -1);
  if (result == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "TAO:%N:%l:(%P|%t):TAO_Wait_On_Leader_Follower::wait: "
                       "handle_events failed.\n"),
                      -1);

  // Return an error if there was a problem receiving the reply...
  result = 0;
  if (this->reply_received_ == -1)
    {
      result = -1;
    }

  // Make us reusable
  this->reply_received_ = 0;
  this->expecting_response_ = 0;
  this->calling_thread_ = ACE_OS::NULL_thread;

  return result;
}

// Handle the input. Return -1 on error, 0 on success.
int
TAO_Wait_On_Leader_Follower::handle_input (void)
{
  TAO_ORB_Core* orb_core =
    this->transport_->orb_core ();

  // Obtain the lock.
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon,
                    orb_core->leader_follower_lock (), -1);

  //  ACE_DEBUG ((LM_DEBUG, "TAO (%P|%t) - reading reply <%x>\n",
  //              this->transport_));

  // A message is received but not data was sent, flag this as an
  // error, but we should do more....
  // @@ Alex: this could be a CloseConnection message or something
  //    similar, has to be handled...
  if (!this->expecting_response_)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) - Wait_On_LF::handle_input, "
                    "unexpected on <%x>\n",
                    this->transport_));
      return -1;
    }

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
                    "TAO (%P|%t) - Wait_On_LF::handle_input, "
                    "handle_client_input == -1\n"));
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

// Register the handler.
int
TAO_Wait_On_Leader_Follower::register_handler (void)
{
  return this->transport_->register_handler ();
}

ACE_SYNCH_CONDITION *
TAO_Wait_On_Leader_Follower::cond_response_available (void)
{
  // @@ TODO This condition variable should per-ORB-per-thread, not
  // per-connection, it is a waste to have more than one of this in
  // the same thread.
  if (this->cond_response_available_ == 0)
    {
      ACE_NEW_RETURN (this->cond_response_available_,
                      ACE_SYNCH_CONDITION (this->transport_->orb_core ()->leader_follower_lock ()),
                      0);
    }
  return this->cond_response_available_;
}

void
TAO_Wait_On_Leader_Follower::wake_up (void)
{
  if (ACE_OS::thr_equal (this->calling_thread_, ACE_Thread::self ()))
    {
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

  if (cond != 0)
    (void) cond->signal ();
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
TAO_Wait_On_Read::wait (void)
{
  int received_reply = 0;
  while (received_reply == 0)
    {
      received_reply = this->transport_->handle_client_input (1);
      if (received_reply == -1)
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
