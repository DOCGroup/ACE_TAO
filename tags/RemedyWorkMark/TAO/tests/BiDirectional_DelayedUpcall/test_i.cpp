// $Id$

#include "test_i.h"
#include "tao/ORB_Core.h"
#include "tao/debug.h"
#include "tao/Transport_Cache_Manager.h"
#include "tao/Thread_Lane_Resources.h"
#include "ace/Reactor.h"

#if !defined(__ACE_INLINE__)
#include "test_i.inl"
#endif /* __ACE_INLINE__ */

void
Callback_i::shutdown (void)
{
  ACE_DEBUG ((LM_DEBUG, "Performing clean shutdown\n"));
  // Shutdown the server before we shutdown our own ORB
  this->server_->shutdown ();
  // now we shut down
  this->orb_->shutdown (0);
}

void
Callback_i::callback_method (void)
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "Callback method called\n"));
}


// ****************************************************************

int
Simple_Server_i::handle_timeout (const ACE_Time_Value &,
                                 const void *)
{
  if (TAO_debug_level > 0)
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("About to make a remote call in the Upcall\n")));

  for (int times = 0;
        times < this->no_iterations_;
        ++times)
    {
      this->callback_->callback_method ();

      if (this->orb_->orb_core ()->lane_resources ().transport_cache ().current_size () > 1)
        {
          ACE_ERROR ((LM_ERROR,
                      "(%P|%t) The cache has grown, aborting ..\n"));

          ACE_OS::abort ();
        }
    }

  this->callback_->shutdown ();

  return 0;
}

CORBA::Long
Simple_Server_i::test_method (CORBA::Boolean do_callback)
{
  if (do_callback)
    {
      // Scheduling ourselves for doing the callbacks
      if (TAO_debug_level > 0)
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Scheduling for callback\n")));

      if (this->orb_->orb_core ()->reactor ()->schedule_timer (this,
                                                               0,
                                                               ACE_Time_Value (1)) == -1)
        {
          ACE_ERROR ((LM_ERROR,
                      "(%P|%t) Can't schedule timers, aborting ..\n"));

          ACE_OS::abort ();
        }
    }

  return 0;
}

void
Simple_Server_i::callback_object (Callback_ptr callback
 )
{
  // Store the callback object
  this->callback_ = Callback::_duplicate (callback);
}


void
Simple_Server_i::shutdown (void)
{
  this->orb_->shutdown (false);
}
