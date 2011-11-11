// $Id$

#include "test_i.h"
#include "tao/debug.h"
#include "tao/ORB_Core.h"
#include "tao/LF_Strategy.h"
#include "ace/Reactor.h"

#if !defined(__ACE_INLINE__)
#include "test_i.inl"
#endif /* __ACE_INLINE__ */

char *
Simple_Server_i::test_method (Simple_Server_ptr objref)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) test_method called\n"));
  if (CORBA::is_nil (objref))
    {
      ACE_ERROR ((LM_ERROR, "Nil object reference!\n"));
    }
  else
    {
      try
        {
          {
            ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->lock_, 0);
            if (!this->timer_registed_)
              {
                ACE_DEBUG ((LM_DEBUG, "(%P|%t) Scheduling timeout...\n"));
                ACE_Time_Value timeout (2, 0);
                ACE_Reactor * reactor = this->orb_->orb_core ()->reactor ();
                reactor->schedule_timer (this, (void*)0, timeout);
                this->timer_registed_ = true;
              }
          }
          ACE_DEBUG ((LM_DEBUG, "(%P|%t)  Calling test_method() on client, "
                      "which will sleep for 3 seconds\n"));
          CORBA::String_var str = objref->test_method (_this());
          ACE_DEBUG ((LM_DEBUG, "(%P|%t) Received \"%C\"\n", str.in ()));

        }
      catch (...)
        {
          ACE_ERROR ((LM_ERROR, "(%P|%t) Caught exception\n"));
        }
    }
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) leaving test_method\n"));
  return CORBA::string_dup("Hello world");
}

void
Simple_Server_i::client_done ()
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Simple_Server_i::client_done () called\n"));
  this->cond_.signal ();
}

int
Simple_Server_i::handle_timeout (const ACE_Time_Value &,
                                 const void *)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->lock_, 0);
  // We are the (client) leader.  Signal the leader-follower pattern to
  // elect a new leader
  TAO_ORB_Core *oc = orb_->orb_core();
  oc->lf_strategy ().set_upcall_thread (oc->leader_follower ());
  // Block until another thread is elected leader and handles a *new* event.
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) handle_timeout () called - waiting...\n"));
  this->cond_.wait();
  return 0;
}


void
Simple_Server_i::shutdown ()
{
  this->orb_->shutdown (0);
}
