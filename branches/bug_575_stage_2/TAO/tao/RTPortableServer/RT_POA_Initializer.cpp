// $Id$

#include "RT_POA_Initializer.h"
#include "RT_Servant_Dispatcher.h"
#include "RT_Policy_Validator.h"

#include "tao/PortableServer/Object_Adapter.h"

#include "tao/ORB_Core.h"

ACE_RCSID(tao, RT_POA_Initializer, "$Id$")

TAO_RT_POA_Initializer::~TAO_RT_POA_Initializer (void)
{
}

void
TAO_RT_POA_Initializer::register_hooks_impl (TAO_Object_Adapter &object_adapter,
                                             CORBA::Environment &ACE_TRY_ENV)
{
  // Create and register the RT servant dispatcher.
  TAO_RT_Servant_Dispatcher *rt_servant_dispatcher;
  ACE_NEW (rt_servant_dispatcher,
           TAO_RT_Servant_Dispatcher);
  object_adapter.servant_dispatcher (rt_servant_dispatcher);

  // Create and add the RT policy validator.
  TAO_POA_RT_Policy_Validator *rt_validator;
  ACE_NEW (rt_validator,
           TAO_POA_RT_Policy_Validator (&object_adapter.orb_core ()));

  object_adapter.validator ().add_validator (rt_validator);

  // Add the default RT policies to the list of default POA policies.
  this->init_rt_default_policies (object_adapter.default_poa_policies (),
                                  object_adapter.orb_core (),
                                  ACE_TRY_ENV);

}

void
TAO_RT_POA_Initializer::init_rt_default_policies (TAO_POA_Policy_Set &policies,
                                                  TAO_ORB_Core &orb_core,
                                                  CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::Policy_var policy =
    orb_core.get_cached_policy (TAO_CACHED_POLICY_PRIORITY_MODEL);
  if (!CORBA::is_nil (policy.in ()))
    {
      policies.merge_policy (policy.in (), ACE_TRY_ENV);
      ACE_CHECK;
    }

  CORBA::Policy_var protocol =
    orb_core.get_cached_policy (TAO_CACHED_POLICY_RT_SERVER_PROTOCOL);
  if (!CORBA::is_nil (protocol.in ()))
    {
      policies.merge_policy (protocol.in (), ACE_TRY_ENV);
      ACE_CHECK;
    }
}

