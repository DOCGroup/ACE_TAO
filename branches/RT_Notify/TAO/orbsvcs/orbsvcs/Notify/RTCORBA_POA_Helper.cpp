// $Id$

#include "RTCORBA_POA_Helper.h"
#include "tao/RTCORBA/RTCORBA.h"
#include "tao/debug.h"
#include "orbsvcs/NotifyExtC.h"
#include "RTCORBA_Properties.h"

#if ! defined (__ACE_INLINE__)
#include "RTCORBA_POA_Helper.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(RT_Notify, TAO_NS_RTCORBA_POA_Helper, "$Id$")

TAO_NS_RTCORBA_POA_Helper::~TAO_NS_RTCORBA_POA_Helper ()
{
}

void
TAO_NS_RTCORBA_POA_Helper::init (PortableServer::POA_ptr parent_poa, NotifyExt::ThreadPoolParams* tp_params ACE_ENV_ARG_DECL)
{
  char child_poa_name[32]; 
  ACE_OS_String::itoa (ACE_OS::rand (), child_poa_name, 10);
  
  this->init (parent_poa, child_poa_name, tp_params ACE_ENV_ARG_PARAMETER);
}

void
TAO_NS_RTCORBA_POA_Helper::init (PortableServer::POA_ptr parent_poa, const char* poa_name, NotifyExt::ThreadPoolParams* tp_params ACE_ENV_ARG_DECL)
{
  CORBA::PolicyList policy_list (4);

  this->set_policy (parent_poa, policy_list ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  
  RTCORBA::RTORB_var rt_orb = TAO_NS_RTCORBA_PROPERTIES::instance ()->rt_orb ();

  policy_list.length (3);
  policy_list[2] =
    rt_orb->create_priority_model_policy (RTCORBA::CLIENT_PROPAGATED,
					  0
					  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
   
  if (tp_params != 0)
    {  
      if (TAO_debug_level > 0)
	{
	  ACE_DEBUG ((LM_DEBUG, "Creating threadpool: static threads = %d, def. prio = %d\n", tp_params->static_threads, tp_params->default_priority));
	}

      // Create the thread-pool.
      RTCORBA::ThreadpoolId threadpool_id =
      rt_orb->create_threadpool (tp_params->stacksize,
				 tp_params->static_threads,
				 tp_params->dynamic_threads,
				 tp_params->default_priority,
				 tp_params->allow_request_buffering,
				 tp_params->max_buffered_requests,
				 tp_params->max_request_buffer_size
				 ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      policy_list.length (4);
      policy_list[3] =	
		rt_orb->create_threadpool_policy (threadpool_id
			                                  ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
  
  this->create_i (parent_poa, poa_name, policy_list ACE_ENV_ARG_PARAMETER);
}
