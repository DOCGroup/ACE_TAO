// @(#) $Id$
#include "tao/EndpointPolicy/EndpointPolicy_Factory.h"

ACE_RCSID (EndpointPolicy,
           EndpointPolicy_Factory,
           "$Id$")

#include "tao/EndpointPolicy/EndpointPolicy_i.h"
#include "tao/EndpointPolicy/EndpointPolicyA.h"
#include "tao/EndpointPolicy/Endpoint_Value_Impl.h"

#include "tao/PolicyC.h"
#include "tao/ORB_Constants.h"
#include "tao/ORB_Core.h"
#include "tao/Acceptor_Registry.h"
#include "tao/AnyTypeCode/TAOA.h"
#include "tao/AnyTypeCode/Any.h"
#include "tao/Thread_Lane_Resources.h"
#include "tao/Transport_Acceptor.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL


TAO_EndpointPolicy_Factory::TAO_EndpointPolicy_Factory (TAO_ORB_Core * orb_core)
: orb_core_ (orb_core)
{
}


CORBA::Policy_ptr
TAO_EndpointPolicy_Factory::create_policy (
    CORBA::PolicyType type,
    const CORBA::Any &value
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   CORBA::PolicyError))
{
  if (type == EndpointPolicy::ENDPOINT_POLICY_TYPE)
  {
    const EndpointPolicy::EndpointList* endpoint_list;
    if ((value >>= endpoint_list) == 0)
      ACE_THROW_RETURN (CORBA::PolicyError (CORBA::BAD_POLICY_VALUE),
      CORBA::Policy::_nil ());

    TAO_Acceptor_Registry & registry
      = this->orb_core_->lane_resources ().acceptor_registry ();

    TAO_Acceptor ** acceptors_begin = registry.begin ();
    TAO_Acceptor ** acceptors_end = registry.end ();
    CORBA::ULong num_eps = endpoint_list->length ();

    // The endpoint list in the value is validated to ensure that
    // at least one endpoint in the list matches an endpoint the
    // ORB is listening on.

    bool found_one = false;
    for (CORBA::ULong idx = 0; !found_one && idx < num_eps; ++idx)
    {
      CORBA::ULong prot_tag = (*endpoint_list)[idx]->protocol_tag();

      const TAO_Endpoint_Value_Impl *evi =
        dynamic_cast <const TAO_Endpoint_Value_Impl*> ((*endpoint_list)[idx]);

      for (TAO_Acceptor** acceptor = acceptors_begin;
           !found_one && acceptor != acceptors_end;
           ++acceptor)
        {
          if ((*acceptor)->tag() == prot_tag)
            found_one = evi->validate_acceptor(*acceptor);
        }
    }
    // There is no endpoint policy value matches an endpoint the ORB
    // is listening on. A CORBA::PolicyError exception with a
    // PolicyErrorCode of UNSUPPORTED_POLICY_VALUE is raised.
    if (!found_one)
      ACE_THROW_RETURN (CORBA::PolicyError (CORBA::UNSUPPORTED_POLICY_VALUE),
                        CORBA::Policy::_nil ());

    TAO_EndpointPolicy_i *tmp;
    ACE_NEW_THROW_EX (tmp,
      TAO_EndpointPolicy_i (*endpoint_list),
      CORBA::NO_MEMORY (TAO::VMCID,
      CORBA::COMPLETED_NO));
    ACE_CHECK_RETURN (CORBA::Policy::_nil ());

    return tmp;
  }
  else
    ACE_THROW_RETURN (CORBA::PolicyError (CORBA::BAD_POLICY_TYPE),
                      CORBA::Policy::_nil ());
}


TAO_END_VERSIONED_NAMESPACE_DECL
