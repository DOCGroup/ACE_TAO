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

ACE_RCSID(BiDirectional_NestedUpcall, test_i, "$Id$")

void
Callback_i::shutdown (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG, "Performing clean shutdown\n"));
  this->orb_->shutdown (0 ACE_ENV_ARG_PARAMETER);
}

void
Callback_i::callback_method (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "Callback method called \n"));
}


// ****************************************************************

int
Simple_Server_i::handle_timeout (const ACE_Time_Value &,
                                 const void *)
{
  if (TAO_debug_level > 0)
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("About to make a remote call in the Upcall \n")));

  for (int times = 0;
        times < this->no_iterations_;
        ++times)
    {
      this->callback_->callback_method (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      if (this->orb_->orb_core ()->lane_resources ().transport_cache ().current_size () > 1)
        {
          ACE_ERROR ((LM_ERROR,
                      "(%P|%t) The cache has grown, aborting ..\n"));

          ACE_OS::abort ();
        }
    }

  this->callback_->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return 0;
}

CORBA::Long
Simple_Server_i::test_method (CORBA::Boolean do_callback
                              ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
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
                                  ACE_ENV_ARG_DECL_NOT_USED )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Store the callback object
  this->callback_ = Callback::_duplicate (callback);
}


void
Simple_Server_i::shutdown (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (false ACE_ENV_ARG_PARAMETER);
}
