// $Id$

#include "test_i.h"

#include "tao/ORB_Core.h"
#include "tao/debug.h"
#include "tao/Transport_Cache_Manager.h"
#include "tao/Thread_Lane_Resources.h"

#if !defined(__ACE_INLINE__)
#include "test_i.inl"
#endif /* __ACE_INLINE__ */



void
Callback_i::shutdown (void)
{
  ACE_DEBUG ((LM_DEBUG, "Performing clean shutdown\n"));
  this->orb_->shutdown (0);
}

void
Callback_i::callback_method (void)
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "Callback method called\n"));
}


// ****************************************************************

CORBA::Long
Simple_Server_i::test_method (CORBA::Boolean do_callback
 )
{
  if (do_callback)
    {
      this->flag_ = 1;
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

int
Simple_Server_i::call_client (void)
{
  if (this->flag_)
    {
      ACE_DEBUG ((LM_DEBUG,"calling client\n"));

      for (int times = 0; times < this->no_iterations_; ++times)
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
      this->flag_ = 0;

      return 1;
    }

  return 0;
}


void
Simple_Server_i::shutdown (void)
{
  this->orb_->shutdown (0);
}
