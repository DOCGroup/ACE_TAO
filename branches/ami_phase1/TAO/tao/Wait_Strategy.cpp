// $Id$

#include "tao/Wait_Strategy.h"
#include "tao/Pluggable.h"
#include "tao/ORB_Core.h"

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
  : TAO_Wait_Strategy (transport)
{
}

// Destructor.
TAO_Wait_On_Reactor::~TAO_Wait_On_Reactor (void)
{
}

int
TAO_Wait_On_Reactor::wait (void)
{
  int result = 0;

  while (result == 0)
    {
      // Do the event loop.
      result = this->transport_->orb_core ()->reactor ()->handle_events (/* timeout */);
    }

  if (result == -1)
    return -1;
  else
    return 0;
}

int
TAO_Wait_On_Reactor::handle_input (void)
{
  int result = 0;

  // Temporarily remove ourself from notification so that if
  // another sub event loop is in effect still waiting for its
  // response, it doesn't spin tightly gobbling up CPU.
  result = this->transport_->suspend_handler ();
  if (result != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "TAO:%N:%l:TAO_Wait_On_Reactor::handle_input:%p\n",
                       "suspend_handler failed"),
                      -1);

  // Ask the Transport object to read the input without blocking. 
  result = this->transport_->handle_client_input (0);
  if (result == -1)
    return -1;
      
  // Resume the handler.
  if (this->transport_->resume_handler () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "TAO:%N:%l:TAO_Wait_On_Reactor::handle_input:%p\n",
                       "resume_handler failed"),
                      -1);

  return result;
}  

// Register the handler with the Reactor.
int
TAO_Wait_On_Reactor::register_handler (TAO_IIOP_Handler_Base *handler)
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
    input_available_ (0)
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

  int result = 0;

#if 0
  ACE_Reactor *r = this->transport_->orb_core ()->reactor ();
  
  if (this->handler_reactor () != r)
    {
          ACE_Reactor_Mask mask =
            ACE_Event_Handler::ALL_EVENTS_MASK | ACE_Event_Handler::DONT_CALL;
          this->reactor ()->remove_handler (this, mask);

          r->register_handler (this,
                               ACE_Event_Handler::READ_MASK);
    }
#endif /* 0 */

  // Obtain the lock.
  if (this->transport_->orb_core ()->leader_follower_lock ().acquire() == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "TAO:%N:%l:(%P|%t):TAO_Wait_On_Leader_Follower::wait: "
                       "Failed to get the lock.\n"),
                      -1);

  // Set the state so that we know we're looking for a response. 
  this->expecting_response_ = 1;
  
  // Remember in which thread the client connection handler was running
  this->calling_thread_ = ACE_Thread::self ();

  // Check if there is a leader, but the leader is not us
  if (this->transport_->orb_core ()->leader_available () &&
      !this->transport_->orb_core ()->I_am_the_leader_thread ())
    {
      // = Wait as a follower.
      
      // wait as long as no input is available and/or
      // no leader is available
      while (!this->input_available_ &&
             this->transport_->orb_core ()->leader_available ())
        {
          ACE_SYNCH_CONDITION* cond =
            this->cond_response_available ();
          if (this->transport_->orb_core ()->add_follower (cond) == -1)
            ACE_ERROR ((LM_ERROR,
                        "TAO:%N:%l:(%P|%t):TAO_Wait_On_Leader_Follower::wait: "
                        "Failed to add a follower thread\n"));
          cond->wait ();
        }
      
      // Now somebody woke us up to become a leader or to handle
      // our input. We are already removed from the follower queue. 
      if (this->input_available_)
        {
          // There is input waiting for me.

          /// Release the lock.
          if (this->transport_->orb_core ()->leader_follower_lock ().release () == -1)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "TAO:%N:%l:(%P|%t): TAO_Wait_On_Leader_Follower::wait: "
                               "Failed to release the lock.\n"),
                              -1);
          
          // The following variables are safe, because we are not
          // registered with the reactor any more.
          this->input_available_ = 0;
          this->expecting_response_ = 0;
          this->calling_thread_ = ACE_OS::NULL_thread;

          // Ready to receive the input message. 
          // @@ Is it ok to read it blockingly. (Alex).
          result = this->transport_->handle_client_input (1);
          
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
    }

  // Become a leader, because there is no leader or we have to
  // update to a leader or we are doing nested upcalls in this
  // case we do increase the refcount on the leader in
  // TAO_ORB_Core.
  
  // This might increase the refcount of the leader. 
  this->transport_->orb_core ()->set_leader_thread ();
  
  // Release the lock.
  if (this->transport_->orb_core ()->leader_follower_lock ().release () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "TAO:%N:%l:(%P|%t): TAO_Wait_On_Leader_Follower::wait: "
                       "Failed to release the lock.\n"),
                      -1);

  // Become owner of the reactor.
  this->transport_->orb_core ()->reactor ()->owner (ACE_Thread::self ());
  
  // Run the reactor event loop.
  
  result = 0;

  while (result != -1 && !this->input_available_)
    result = this->transport_->orb_core ()->reactor ()->handle_events ();
  
  if (result == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "TAO:%N:%l:(%P|%t):TAO_Wait_On_Leader_Follower::wait: "
                       "handle_events failed.\n"),
                      -1);
  
  // Wake up the next leader, we cannot do that in handle_input,
  // because the woken up thread would try to get into
  // handle_events, which is at the time in handle_input still
  // occupied.

  if (this->transport_->orb_core ()->unset_leader_wake_up_follower () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "TAO:%N:%l:(%P|%t):TAO_Client_Connection_Handler::send_request: "
                       "Failed to unset the leader and wake up a new follower.\n"),
                      -1);
  
  // Make use reusable
  this->input_available_ = 0;
  this->expecting_response_ = 0;
  this->calling_thread_ = ACE_OS::NULL_thread;
  
  return 0;

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
}

// Handle the input.
// I am returning -1 on error, 1 on fully read message, 0 on
// paritially complete read.
int
TAO_Wait_On_Leader_Follower::handle_input (void)
{
  int result = 0;

  // Obtain the lock.
  if (this->transport_->orb_core ()->leader_follower_lock ().acquire () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "TAO:%N:%l:(%P|%t):TAO_Wait_On_Leader_Follower::handle_input: "
                       "Failed to get the lock.\n"),
                      -1);

#if 0 
  // @@ Later (Alex).

  if (!this->expecting_response_)
    {
      // we got something, but did not want.
      // @@ wake up an other thread, we are lost

      if (this->orb_core_->leader_follower_lock ().release () == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) TAO_Client_Connection_Handler::handle_input: "
                           "Failed to release the lock.\n"),
                          -1);
      return this->check_unexpected_data ();
    }
#endif /* 0 */

  if (ACE_OS::thr_equal (this->calling_thread_,
                         ACE_Thread::self ()))
    {
      // We are now a leader getting its response.
      
      // Set the flag on.
      this->input_available_ = 1;

      // Release the lock.
      if (this->transport_->orb_core ()->leader_follower_lock ().release () == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) TAO_Client_Connection_Handler::handle_input: "
                           "Failed to release the lock.\n"),
                          -1);
      
      // Suspend the handler. <resume_handler> is called in
      // TAO_GIOP_Invocation::invoke 
      if (this->transport_->suspend_handler () == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "TAO:%N:%l:(%P|%t): TAO_Wait_On_Leader_Follower::handle_input: "
                           "<suspend_handler> failed\n"),
                          -1);

      // Ask the transport object to read the message. But we should
      // not block on receiving the whole reply.
      
      // @@ Are the states such as message_size and message_offset
      //    thread safe, in the Transport object? (Alex).
      result = this->transport_->handle_client_input (0);
      
      // Resume the handler.
      if (this->transport_->resume_handler () == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "TAO:%N:%l:(%P|%t): TAO_Wait_On_Leader_Follower::handle_input: "
                           "<resume_handler> failed\n"),
                          -1);

      // Return the result.
      return result;
    }
  else
    {
      // We are a leader, which got a response for one of the
      // followers, which means we are now a thread running the wrong
      // Client_Connection_Handler

      // At this point we might fail to remove the follower, because
      // it has been already chosen to become the leader, so it is
      // awake and will get this too.
      ACE_SYNCH_CONDITION* cond =
        this->cond_response_available ();

      this->transport_->orb_core ()->remove_follower (cond);

      // Release the lock.
      if (this->transport_->orb_core ()->leader_follower_lock ().release () == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "TAO:%N:%l:(%P|%t): TAO_Wait_On_Leader_Follower::handle_input: "
                           "Failed to release the lock.\n"),
                          -1);
      
      // We should wake suspend the thread before we wake him up.
      // resume_handler is called in TAO_GIOP_Invocation::invoke.
      if (this->transport_->suspend_handler () == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "TAO:%N:%l:(%P|%t): TAO_Wait_On_Leader_Follower::handle_input: "
                           "<suspend_handler> failed\n"),
                          -1);

      // @@ TODO (Michael): We might be able to optimize this in
      // doing the suspend_handler as last thing, but I am not sure
      // if a race condition would occur.

      // Obtain the lock.
      if (this->transport_->orb_core ()->leader_follower_lock ().acquire () == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "TAO:%N:%l:(%P|%t): TAO_Wait_On_Leader_Follower::handle_input: "
                           "Failed to acquire the lock.\n"),
                          -1);
      
      // The thread was already selected to become a leader, so we
      // will be called again.
      this->input_available_ = 1;
      cond->signal ();

      // Release the lock.
      if (this->transport_->orb_core ()->leader_follower_lock ().release () == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "TAO:%N:%l:(%P|%t): TAO_Wait_On_Leader_Follower::handle_input: "
                           "Failed to release the lock.\n"),
                          -1);
      return 0;
    }
}

// Register the handler.
int
TAO_Wait_On_Leader_Follower::register_handler (TAO_IIOP_Handler_Base *)
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
TAO_Wait_On_Read::register_handler (TAO_IIOP_Handler_Base *)
{
  return 0;
} 

int
TAO_Wait_On_Read::resume_handler (ACE_Reactor *reactor)
{
  return -1;
}
