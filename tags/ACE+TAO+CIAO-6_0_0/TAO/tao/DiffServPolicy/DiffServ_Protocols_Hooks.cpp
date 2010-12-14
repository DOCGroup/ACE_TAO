// -*- C++ -*-
// $Id$

#include "tao/DiffServPolicy/DiffServ_Protocols_Hooks.h"
#include "tao/Stub.h"
#include "tao/Profile.h"
#include "tao/ORB_Core.h"
#include "tao/Service_Context.h"
#include "tao/CDR.h"
#include "tao/SystemException.h"
#include "tao/DiffServPolicy/Client_Network_Priority_Policy.h"
#include "ace/OS_NS_string.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_DS_Network_Priority_Protocols_Hooks::
TAO_DS_Network_Priority_Protocols_Hooks (void)
  : orb_core_ (0)
{
}


TAO_DS_Network_Priority_Protocols_Hooks::
~TAO_DS_Network_Priority_Protocols_Hooks (void)
{
}

void
TAO_DS_Network_Priority_Protocols_Hooks::init_hooks (TAO_ORB_Core *orb_core)
{
  this->orb_core_ = orb_core;
}

CORBA::Long
TAO_DS_Network_Priority_Protocols_Hooks::get_dscp_codepoint (
  TAO_Service_Context &sc)
{
  CORBA::Long dscp_codepoint = 0;
  const IOP::ServiceContext *context = 0;

  if (sc.get_context (IOP::REP_NWPRIORITY, &context) == 1)
    {
      TAO_InputCDR cdr (reinterpret_cast
                        <const char*>
                        (context->context_data.get_buffer ()),
                        context->context_data.length ());

      CORBA::Boolean byte_order;
      if ((cdr >> ACE_InputCDR::to_boolean (byte_order)) == 0)
        {
          throw CORBA::MARSHAL ();
        }
      cdr.reset_byte_order (static_cast<int> (byte_order));

      if ((cdr >> dscp_codepoint) == 0)
        {
          throw CORBA::MARSHAL ();
        }
    }

  return dscp_codepoint;
}

CORBA::Long
TAO_DS_Network_Priority_Protocols_Hooks::get_dscp_codepoint (
  TAO_Stub *stub, CORBA::Object *object)
{
  CORBA::Long dscp = 0;
  TAO::DiffservCodepoint diffserv_codepoint;
  CORBA::Policy_var server_nw_priority_policy;

  CORBA::Policy_var client_nw_priority_policy =
    stub->get_cached_policy (
        TAO_CACHED_POLICY_CLIENT_NETWORK_PRIORITY);

  if (CORBA::is_nil (client_nw_priority_policy.in ()))
    {
      TAO_Stub *server_stub = object->_stubobj ();
      TAO_MProfile server_profiles = server_stub->base_profiles ();
      CORBA::PolicyList_var policy_list =
        server_profiles.policy_list ();

      CORBA::ULong length = policy_list->length ();

      for (CORBA::ULong i = 0; i < length; ++i)
        {
          if (policy_list[i]->policy_type () == TAO::NETWORK_PRIORITY_TYPE)
            {
              server_nw_priority_policy =
                CORBA::Policy::_duplicate (policy_list[i]);
            }
        }

      if (CORBA::is_nil (server_nw_priority_policy.in ()))
        {
          return dscp;
        }
      else
        {
          TAO::NetworkPriorityPolicy_var npp =
            TAO::NetworkPriorityPolicy::_narrow (
              server_nw_priority_policy.in ());

          if (!CORBA::is_nil (npp.in ()))
            {
              TAO::NetworkPriorityModel network_priority_model =
                npp->network_priority_model ();

              if (network_priority_model ==
                  TAO::SERVER_DECLARED_NETWORK_PRIORITY)
                {
                  diffserv_codepoint =
                    npp->request_diffserv_codepoint ();
                  dscp = diffserv_codepoint;
                  return dscp;
                }
            }
        }
    }
  else
    {
      TAO::NetworkPriorityPolicy_var client_nw_priority =
        TAO::NetworkPriorityPolicy::_narrow (client_nw_priority_policy.in ());

      if (!CORBA::is_nil (client_nw_priority.in ()))
        {
          diffserv_codepoint =
            client_nw_priority->request_diffserv_codepoint ();
          dscp = diffserv_codepoint;
          return dscp;
        }
    }

  return dscp;
}


ACE_STATIC_SVC_DEFINE (TAO_DS_Network_Priority_Protocols_Hooks,
                       ACE_TEXT ("DS_Network_Priority_Protocols_Hooks"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_DS_Network_Priority_Protocols_Hooks),
                       ACE_Service_Type::DELETE_THIS
                       | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (TAO_DiffServPolicy,
                    TAO_DS_Network_Priority_Protocols_Hooks)
TAO_END_VERSIONED_NAMESPACE_DECL
