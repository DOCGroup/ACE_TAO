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

  // Ask the Transport object to read the input without blocking. 
  result = this->transport_->handle_client_input (0);

  // If message is not read fully, resume the handler.
  if (result == 0)
    result = this->transport_->resume_handler ();
  
  if (result != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "TAO: %N:%l:TAO_Wait_On_Reactor::handle_input:%p\n",
                       "resume_handler failed"),
                      -1);

  return 0;
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
  : TAO_Wait_Strategy (transport)
{
}

// Destructor.
TAO_Wait_On_Leader_Follower::~TAO_Wait_On_Leader_Follower (void)
{
}

int
TAO_Wait_On_Leader_Follower::wait (void)
{
#if 0
  // Grab leader follower lock.
  ACE_MT (ACE_GUARD_RETURN (ACE_SYNCH_MUTEX,
                            ace_mon,
                            this->orb_core_->leader_follower_lock (),
                            -1));

  // Bind the reply handler.

  TAO_Leader_Follower_Reply_Handler rh (..);

  rrs->bind (this->request_id, &rh);

  // Check if we need to become the leader.
  if (!this->orb_core_->leader_available ())
    {
      // This might increase the refcount of the leader.
      this->orb_core_->set_leader_thread ();

      // Do the reactor event loop.
      this->orb_core_->reactor ()->owner (ACE_Thread::self ());

      int result = 0;
      while (result != -1)
        result = this->orb_core_->reactor ()->handle_events ();

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
        this->cond_response_available (orb_core);
      if (this->orb_core_->add_follower (cond) == -1)
        ACE_ERROR ((LM_ERROR,
                    "%N:%l:(%P|%t) TAO_Wait_On_Leader_Follower::wait: "
                    "Failed to add a follower thread\n"));
      cond->wait ();
    }
#endif /* 0 */
  return 0;
}

// Handle the input. Delegate this job to Transport object. Before
// that, suspend the handler in the Reactor.
int
TAO_Wait_On_Leader_Follower::handle_input (void)
{
  return -1;
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
  return -1;
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
// Resume the connection handler. No-op. Returns 0.
int
TAO_Wait_On_Read::handle_input (void)
{
  return -1;
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
