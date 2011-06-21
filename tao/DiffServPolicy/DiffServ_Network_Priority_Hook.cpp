// -*- C++ -*-
// $Id$

#include "tao/DiffServPolicy/DiffServ_Network_Priority_Hook.h"
#include "tao/PortableServer/Root_POA.h"
#include "tao/PortableServer/POA_Policy_Set.h"
#include "tao/DiffServPolicy/Server_Network_Priority_Policy.h"
#include "tao/DiffServPolicy/DiffServPolicy.h"
#include "tao/Service_Context.h"
#include "tao/TAO_Server_Request.h"
#include "tao/Network_Priority_Protocols_Hooks.h"
#include "tao/ORB_Core.h"
#include "tao/Connection_Handler.h"
#include "tao/Transport.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL


TAO_DiffServ_Network_Priority_Hook::~TAO_DiffServ_Network_Priority_Hook(void)
{
}

void
TAO_DiffServ_Network_Priority_Hook::update_network_priority (
  TAO_Root_POA &poa, TAO_POA_Policy_Set &policy_set)
{
  for (CORBA::ULong i = 0; i < policy_set.num_policies (); i++)
    {
      CORBA::Policy_var policy = policy_set.get_policy_by_index (i);

      if (policy->policy_type () == TAO::NETWORK_PRIORITY_TYPE)
        {
          ::TAO::NetworkPriorityPolicy_var npp =
            ::TAO::NetworkPriorityPolicy::_narrow (policy.in ());

          if (!CORBA::is_nil (npp.in ()))
            {
              TAO::NetworkPriorityModel network_priority_model =
                npp->network_priority_model ();

              poa.cached_policies ().network_priority_model (
                TAO::Portable_Server::Cached_Policies::NetworkPriorityModel (
                  network_priority_model));

              TAO::DiffservCodepoint request_diffserv_codepoint  =
                npp->request_diffserv_codepoint ();

              TAO::DiffservCodepoint reply_diffserv_codepoint  =
                npp->reply_diffserv_codepoint ();

              poa.cached_policies ().request_diffserv_codepoint (
                request_diffserv_codepoint);
              poa.cached_policies ().reply_diffserv_codepoint (
                reply_diffserv_codepoint);
            }
        }
    }
}

void
TAO_DiffServ_Network_Priority_Hook::set_dscp_codepoint (
  TAO_ServerRequest &req, TAO_Root_POA &poa)
{
  CORBA::Long dscp_codepoint = 0;
  TAO_Service_Context &request_service_context =
    req.request_service_context ();

  TAO_Network_Priority_Protocols_Hooks *nph =
   poa.orb_core ().get_network_priority_protocols_hooks ();

  TAO::Portable_Server::Cached_Policies::NetworkPriorityModel npm =
    poa.cached_policies ().network_priority_model ();

  if (npm == TAO::Portable_Server::Cached_Policies::
             CLIENT_PROPAGATED_NETWORK_PRIORITY)
    {
      if (nph != 0)
        {
          dscp_codepoint = nph->get_dscp_codepoint (request_service_context);
        }
    }
  else if (npm == TAO::Portable_Server::Cached_Policies::
                  SERVER_DECLARED_NETWORK_PRIORITY)
    {
      dscp_codepoint = poa.cached_policies ().reply_diffserv_codepoint ();
    }

  TAO_Connection_Handler *connection_handler =
    req.transport ()->connection_handler ();
  connection_handler->set_dscp_codepoint (dscp_codepoint);
}


ACE_FACTORY_DEFINE (TAO_DiffServPolicy, TAO_DiffServ_Network_Priority_Hook)
ACE_STATIC_SVC_DEFINE (TAO_DiffServ_Network_Priority_Hook,
                       ACE_TEXT ("TAO_Network_Priority_Hook"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_DiffServ_Network_Priority_Hook),
                       ACE_Service_Type::DELETE_THIS
                       | ACE_Service_Type::DELETE_OBJ,
                       0)
TAO_END_VERSIONED_NAMESPACE_DECL
