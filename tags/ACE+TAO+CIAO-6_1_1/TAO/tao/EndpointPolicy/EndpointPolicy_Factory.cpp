// -*- C++ -*-
// $Id$

#include "tao/EndpointPolicy/EndpointPolicy_Factory.h"

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
    const CORBA::Any &value)
{
  if (type == EndpointPolicy::ENDPOINT_POLICY_TYPE)
    {
      const EndpointPolicy::EndpointList* endpoint_list = 0;
      if (!(value >>= endpoint_list))
        throw ::CORBA::PolicyError (CORBA::BAD_POLICY_VALUE);

      TAO_Acceptor_Registry & registry =
        this->orb_core_->lane_resources ().acceptor_registry ();

      TAO_Acceptor ** const acceptors_begin = registry.begin ();
      TAO_Acceptor ** const acceptors_end = registry.end ();
      CORBA::ULong const num_eps = endpoint_list->length ();

      // need to count the protocol types offered by the acceptors
      // partially defaulted endpoint values are only acceptable if
      // there are more than one protocol available to the ORB.

      CORBA::ULong last_known_prot = 0xFFFFFFFF; // tag = 0 is IIOP
      int prot_count = 0;
      for (TAO_Acceptor** acceptor = acceptors_begin;
           acceptor != acceptors_end;
           ++acceptor)
        {
          if ((*acceptor)->tag () != last_known_prot)
            {
              last_known_prot = (*acceptor)->tag ();
              ++prot_count;
            }
        }


      // The endpoint list in the value is validated to ensure that
      // at least one endpoint in the list matches an endpoint the
      // ORB is listening on.

      bool found_one = false;
      for (CORBA::ULong idx = 0; !found_one && idx < num_eps; ++idx)
        {
          CORBA::ULong prot_tag = (*endpoint_list)[idx]->protocol_tag();

          TAO_Endpoint_Value_Impl const * const evi =
            dynamic_cast <TAO_Endpoint_Value_Impl const *> (
              (*endpoint_list)[idx].in ());

          if (!evi)
            continue;

          for (TAO_Acceptor** acceptor = acceptors_begin;
               !found_one && acceptor != acceptors_end;
               ++acceptor)
            {
              if ((*acceptor)->tag () == prot_tag)
                found_one = evi->validate_acceptor (*acceptor, prot_count > 1);
            }
        }

      // There is no endpoint policy value matches an endpoint the ORB
      // is listening on. A CORBA::PolicyError exception with a
      // PolicyErrorCode of UNSUPPORTED_POLICY_VALUE is raised.
      if (!found_one)
        throw ::CORBA::PolicyError (CORBA::UNSUPPORTED_POLICY_VALUE);

      TAO_EndpointPolicy_i *tmp = 0;
      ACE_NEW_THROW_EX (tmp,
                        TAO_EndpointPolicy_i (*endpoint_list),
                        CORBA::NO_MEMORY (TAO::VMCID,
                                          CORBA::COMPLETED_NO));

      return tmp;
    }
  else
    throw ::CORBA::PolicyError (CORBA::BAD_POLICY_TYPE);
}


TAO_END_VERSIONED_NAMESPACE_DECL
