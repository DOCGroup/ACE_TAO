// $Id$

#include "test_i.h"

#include "tao/ORB_Core.h"
#include "tao/debug.h"
#include "tao/Transport_Cache_Manager.h"
#include "tao/Thread_Lane_Resources.h"

#if !defined(__ACE_INLINE__)
#include "test_i.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(BiDirectional, test_i, "$Id$")

void
Callback_i::shutdown (TAO_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG, "Performing clean shutdown\n"));
  this->orb_->shutdown (0 TAO_ENV_ARG_PARAMETER);
}

void
Callback_i::callback_method (TAO_ENV_SINGLE_ARG_DECL_NOT_USED /*TAO_ENV_SINGLE_ARG_PARAMETER*/)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "Callback method called \n"));
}


// ****************************************************************

CORBA::Long
Simple_Server_i::test_method (CORBA::Boolean do_callback
                              TAO_ENV_ARG_DECL_NOT_USED )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (do_callback)
    {
      this->flag_ = 1;
    }

  return 0;
}

void
Simple_Server_i::callback_object (Callback_ptr callback
                                  TAO_ENV_ARG_DECL_NOT_USED )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Store the callback object
  this->callback_ = Callback::_duplicate (callback);
}

int
Simple_Server_i::call_client (TAO_ENV_SINGLE_ARG_DECL)
{
  if (this->flag_)
    {
      for (int times = 0; times < this->no_iterations_; ++times)
        {
          this->callback_->callback_method (TAO_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK_RETURN (0);

          if (this->orb_->orb_core ()->lane_resources ().transport_cache ().current_size () > 1)
            {
              ACE_ERROR ((LM_ERROR,
                          "(%P|%t) The cache has grown, aborting ..\n"));

              ACE_OS::abort ();
            }
        }

      this->callback_->shutdown (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);
      this->flag_ = 0;

      return 1;
    }

  return 0;
}


void
Simple_Server_i::shutdown (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0 TAO_ENV_ARG_PARAMETER);
}
