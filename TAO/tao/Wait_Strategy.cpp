// $Id$

#include "tao/Wait_Strategy.h"
#include "tao/Pluggable.h"
#include "tao/ORB_Core.h"

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

// Return value just like the return value of
// <Reactor::handle_events>.
int
TAO_Wait_On_Reactor::wait (void)
{
  // @@ Alex: I assume the reactor was not changing inside of the
  //    loop, so I took it out, the code is more readable that way
  //    too.
  ACE_Reactor* reactor =
    this->transport_->orb_core ()->reactor ();

  // Do the event loop, till there are no events and no errors.

  int result = 0;
  this->reply_received_ = 0;
  while (this->reply_received_ == 0 && result >= 0)
    {
      result = reactor->handle_events (/* timeout */);
    }

  if (result == -1)
    return -1;

  return 0;
}

int
TAO_Wait_On_Reactor::handle_input (void)
{
  int result = this->transport_->handle_client_input (0);

  if (result == -1)
    return -1;

  if (result == 1)
    this->reply_received_ = 1;

  return 0;
}

// Register the handler with the Reactor.
int
TAO_Wait_On_Reactor::register_handler (void)
{
  return this->transport_->register_handler ();
}

int
TAO_Wait_On_Reactor::resume_handler (ACE_Reactor *reactor)
{
  return this->transport_->resume_handler ();
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

int
TAO_Wait_On_Leader_Follower::wait (void)
{
  // @@ Do we need this code? (Alex).
  // @@ Alex: yes, the same connection may be used in multiple
  //    threads, each with its own reactor.

  // Cache the ORB core, it won't change and is used multiple times
  // below:

  TAO_ORB_Core* orb_core = 
    this->transport_->orb_core ();

  this->transport_->register_handler ();

  // Obtain the lock.
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon,
                    orb_core->leader_follower_lock (), -1);

  // Set the state so that we know we're looking for a response.
  // @@ Alex: maybe this should be managed by the Demux strategy?
  // @@ Alex: this should be set *before* we enter this function,
  //    actually before we *send* the request, otherwise we can run
  //    into the following race condition:
  //    + Thread A is running the event loop
  //    + Thread B sends the request.
  //    + Before B sets this flag thread A receives the response.
  // IMHO the send_request method in the transport should invoke a
  // method in the Wait_Strategy so we can set the flag on time (and
  // while holding the L-F lock).
  this->expecting_response_ = 1;

  // Remember in which thread the client connection handler was running
  this->calling_thread_ = ACE_Thread::self ();

  // Check if there is a leader, but the leader is not us
  if (orb_core->leader_available () && !orb_core->I_am_the_leader_thread ())
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

      // Add ourselves to the list, do it only once because we can
      // wake up multiple times from the CV loop
      if (orb_core->add_follower (cond) == -1)
        ACE_ERROR ((LM_ERROR,
                    "TAO:%N:%l:(%P|%t):TAO_Wait_On_Leader_Follower::wait: "
                    "Failed to add a follower thread\n"));

      while (!this->reply_received_ && orb_core->leader_available ())
        {
          if (cond == 0 || cond->wait () == -1)
            return -1;
        }

      // Now somebody woke us up to become a leader or to handle
      // our input. We are already removed from the follower queue.
      if (this->reply_received_)
        {
          // We have received our reply, it is time to return control
          // to the application....

          // But first reset our state in case we are invoked again...
          this->reply_received_ = 0;
          this->expecting_response_ = 0;
          this->calling_thread_ = ACE_OS::NULL_thread;

          return 0;
        }
    }

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

  while (result == 0 && !this->reply_received_)
    result = orb_core->reactor ()->handle_events ();

  // Wake up the next leader, we cannot do that in handle_input,
  // because the woken up thread would try to get into
  // handle_events, which is at the time in handle_input still
  // occupied. But do it before checking the error in <result>, even
  // if there is an error in our input we should continue running the
  // loop in anothe rthread.

  if (orb_core->unset_leader_wake_up_follower () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "TAO:%N:%l:(%P|%t):TAO_Client_Connection_Handler::send_request: "
                       "Failed to unset the leader and wake up a new follower.\n"),
                      -1);
  if (result == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "TAO:%N:%l:(%P|%t):TAO_Wait_On_Leader_Follower::wait: "
                       "handle_events failed.\n"),
                      -1);

  // Make us reusable
  this->reply_received_ = 0;
  this->expecting_response_ = 0;
  this->calling_thread_ = ACE_OS::NULL_thread;

  return 0;

}

// Handle the input.
// I am returning -1 on error, 1 on fully read message, 0 on
// paritially complete read.
int
TAO_Wait_On_Leader_Follower::handle_input (void)
{
  TAO_ORB_Core* orb_core =
    this->transport_->orb_core ();

  // Obtain the lock.
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon,
                    orb_core->leader_follower_lock (), -1);

  // A message is received but not data was sent, flag this as an
  // error, but we should do more....
  // @@ Alex: this could be a CloseConnection message or something
  //    similar, has to be handled...
  if (!this->expecting_response_)
    return -1;

  // Receive any data that is available, without blocking...
  int result = this->transport_->handle_client_input (0);

  // Severe error, abort....
  if (result == -1)
    return -1;

  // Bata was read, but there the reply has not been completely
  // received...
  if (result == 0)
    return 0;

  // All the data is here!
  this->reply_received_ = 1;

  if (ACE_OS::thr_equal (this->calling_thread_, ACE_Thread::self ()))
    {
      // We are the leader thread, simply return 1 to terminate the
      // event loop....
      return 1;
    }

  // We are not the leader thread, but we have our data, wake up
  // ourselves and then return 0 so the leader thread can continue
  // doing its job....

  // At this point we might fail to remove the follower, because
  // it has been already chosen to become the leader, so it is
  // awake and will get this too.
  ACE_SYNCH_CONDITION* cond =
    this->cond_response_available ();

  // Ignore any errors, may have been removed by another thread...
  (void) orb_core->remove_follower (cond);

  if (cond == 0 || cond->signal () == -1)
    {
      // Yikes, what do we do here????
      return 0;
    }

  return 0;
}

// ****************************************************************
// remove this code ASAP, but not before that ;-)
#if 0
{
  /// Release the lock.
  if (orb_core->leader_follower_lock ().release () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "TAO:%N:%l:(%P|%t): TAO_Wait_On_Leader_Follower::wait: "
                       "Failed to release the lock.\n"),
                      -1);
  
  // The following variables are safe, because we are not
  // registered with the reactor any more.
  
  // Ready to receive the input message.
  // @@ Is it ok to read it blockingly. (Alex).
  int result = this->transport_->handle_client_input (1);
        
  // Resume the handler.
  if (this->transport_->resume_handler () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "TAO:%N:%l:(%P|%t): TAO_Wait_On_Leader_Follower::wait: "
                       "<resume_handler> failed.\n"),
                      -1);
  
  // We should have read the whole message.
  if (result != 1)
    ACE_ERROR_RETURN ((LM_WARNING,
                       "TAO:%N:%l:(%P|%t):TAO_Wait_On_Leader_Follower::wait: "
                       "Input message was not read fully.\n"),
                      -1);
  return 0;
}
#endif
#if 0
// Grab leader follower lock.
ACE_MT (ACE_GUARD_RETURN (ACE_SYNCH_MUTEX,
                          ace_mon,
                          this->transport_->orb_core ()->leader_follower_lock (),
                          -1));

// Bind the reply handler.

TAO_Leader_Follower_Reply_Handler rh (..);

rrs->bind (this->request_id, &rh);

// Check if we need to become the leader.
if (!this->transport_->orb_core ()->leader_available ())
{
  // This might increase the refcount of the leader.
  this->transport_->orb_core ()->set_leader_thread ();
  
  // Do the reactor event loop.
  this->transport_->orb_core ()->reactor ()->owner (ACE_Thread::self ());
      
  result = 0;
  
  while (result != -1)
    result = this->transport_->orb_core ()->reactor ()->handle_events ();
  
  if (result == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%N:%l:(%P | %t):TAO_Wait_On_Leader_Follower::wait: "
                       "Reactor::handle_events failed.\n"),
                      -1);
}
else
{
  // Block on condition variable.
  ACE_SYNCH_CONDITION* cond =
    this->cond_response_available ();
  if (this->transport_->orb_core ()->add_follower (cond) == -1)
    ACE_ERROR ((LM_ERROR,
                "%N:%l:(%P|%t) TAO_Wait_On_Leader_Follower::wait: "
                "Failed to add a follower thread\n"));
  cond->wait ();
}
#endif /* 0 */

// ****************************************************************

// Register the handler.
int
TAO_Wait_On_Leader_Follower::register_handler (void)
{
  return 0;
}

// Resume the connection handler.
int
TAO_Wait_On_Leader_Follower::resume_handler (ACE_Reactor *reactor)
{
  return this->transport_->resume_handler ();
}

// Send the request in <stream>.
// @@ Why do I need <orb_core> here, when I have <transport>.
int
TAO_Wait_On_Leader_Follower::send_request (TAO_ORB_Core* /* orb_core */,
                                           TAO_OutputCDR &stream)
{
  // Send the request.
  int success  = (int) TAO_GIOP::send_request (this->transport_,
                                               stream,
                                               this->transport_->orb_core ());
  if (!success)
    return -1;
  else
    return 0;
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
      // @@ In this case sockets *must* be blocking.
      //    We need to control how they are set!
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

int
TAO_Wait_On_Read::resume_handler (ACE_Reactor *reactor)
{
  return -1;
}
