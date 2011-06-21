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
Callback_i::callback_method ()
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
  size_t pre_call_connections =
    this->orb_->orb_core ()->lane_resources ().transport_cache ().current_size ();

  if (this->flag_)
    {
      for (int times = 0; times < this->no_iterations_; ++times)
        {
          this->callback_->callback_method ();

          size_t cur_connections =
            this->orb_->orb_core()->lane_resources().transport_cache().current_size ();

          if (cur_connections > pre_call_connections)
            {
              ACE_ERROR ((LM_ERROR,
                          "(%P|%t) TEST OUTPUT: expected to find %d "
                          "connections in the transport cache, but found "
                          "%d instead.  Aborting.\n",
                          pre_call_connections,
                          cur_connections));

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
