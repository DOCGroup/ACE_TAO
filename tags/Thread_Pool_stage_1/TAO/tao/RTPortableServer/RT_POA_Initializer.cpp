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
           TAO_POA_RT_Policy_Validator (object_adapter.orb_core ()));

  object_adapter.validator ().add_validator (rt_validator);
}
