// $Id$

#include "tao/IIOP_Wait_Strategy.h"
#include "tao/Pluggable.h"

// Constructor.
TAO_IIOP_Wait_Strategy::TAO_IIOP_Wait_Strategy (TAO_Transport *transport)
  : transport_ (transport)
{
}

// Destructor.  
TAO_IIOP_Wait_Strategy::~TAO_IIOP_Wait_Strategy (void)
{
}

// *********************************************************************

// Constructor.
TAO_Wait_On_Reactor::TAO_Wait_On_Reactor (TAO_Transport *transport)
  : TAO_IIOP_Wait_Strategy (transport)
{
}

// Destructor.
TAO_Wait_On_Reactor::~TAO_Wait_On_Reactor (void)
{
}

int
TAO_Wait_On_Reactor::wait (void)
{
  int end_loop_flag = 0;
  int result = 0;
  
  while (result != -1 && end_loop_flag == 0)
    result = this->transport_->orb_core_->reactor ()->handle_events (/* timeout */);  

  return 0;
}

// *********************************************************************

// Constructor.
TAO_Wait_On_Leader_Follower::TAO_Wait_On_Leader_Follower (TAO_Transport *transport)
  : TAO_IIOP_Wait_Strategy (transport)
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

// *********************************************************************

// Constructor.
TAO_Wait_On_Read::TAO_Wait_On_Read (TAO_Transport *transport)
  : TAO_IIOP_Wait_Strategy (transport)
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
  while (!received_reply and received_reply != -1)
    {
      // @@ In this case sockets *must* be blocking.
      //    We need to control how they are set!
      received_reply = this->transport_->handle_client_input (1);
    }
  
  return 0;
}
