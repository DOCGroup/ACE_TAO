#include "tao/DiffServPolicy/DiffServ_Protocols_Hooks.h"
#include "tao/Stub.h"
#include "tao/ORB_Core.h"
#include "tao/Service_Context.h"
#include "tao/CDR.h"
#include "tao/DiffServPolicy/Client_Network_Priority_Policy.h"
#include "ace/OS_NS_string.h"

ACE_RCSID (DiffServPolicy,
           DiffServ_Protocols_Hooks,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_DS_Network_Priority_Protocols_Hooks::
TAO_DS_Network_Priority_Protocols_Hooks (void)
  : orb_core_ (0)
  , dscp_codepoint_ ()
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

void
TAO_DS_Network_Priority_Protocols_Hooks::np_service_context (
    TAO_Stub *stub,
    TAO_Service_Context &service_context,
    CORBA::Boolean restart)
{
  // If the restart flag is true, then this call for a
  // reinvocation. We need not prepare the Service Context List once
  // again. We can use the already existing one.
  if (!restart)
    {
      CORBA::Policy_var cnpp =
        stub->get_cached_policy (TAO_CACHED_POLICY_CLIENT_NETWORK_PRIORITY);

      if (!CORBA::is_nil (cnpp.in ()))
        {
          TAO::NetworkPriorityPolicy_var cnp =
            TAO::NetworkPriorityPolicy::_narrow (cnpp.in ());

          TAO::DiffservCodepoint reply_diffserv_codepoint;
          reply_diffserv_codepoint = cnp->reply_diffserv_codepoint ();

          CORBA::Long rep_dscp_codepoint = reply_diffserv_codepoint;

          this->add_rep_np_service_context_hook (service_context,
                                                 rep_dscp_codepoint);
        }
    }
}

void
TAO_DS_Network_Priority_Protocols_Hooks::add_rep_np_service_context_hook (
   TAO_Service_Context &service_context,
   CORBA::Long &dscp_codepoint)
{
  TAO_OutputCDR cdr;
  if ((cdr << ACE_OutputCDR::from_boolean (TAO_ENCAP_BYTE_ORDER) == 0)
      || (cdr << dscp_codepoint) == 0)
    {
      throw CORBA::MARSHAL ();
    }

  service_context.set_context (IOP::REP_NWPRIORITY, cdr);
}

void
TAO_DS_Network_Priority_Protocols_Hooks::set_dscp_codepoint (
  CORBA::Long dscp_codepoint)
{
  this->dscp_codepoint_ = dscp_codepoint;
}

const CORBA::Long
TAO_DS_Network_Priority_Protocols_Hooks::get_dscp_codepoint (
  TAO_Stub *stub, CORBA::Object *object)
{
  CORBA::Policy_var client_nw_priority_policy =
    stub->get_cached_policy (
        TAO_CACHED_POLICY_CLIENT_NETWORK_PRIORITY
        ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

  CORBA::Policy_var server_nw_priority_policy =
      object->_get_cached_policy (
        TAO_CACHED_POLICY_NETWORK_PRIORITY
        ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

  CORBA::Long dscp = 0;
  TAO::DiffservCodepoint diffserv_codepoint;

  if (CORBA::is_nil (client_nw_priority_policy.in ()))
    {
      if (CORBA::is_nil (server_nw_priority_policy.in ()))
        {
          // cannot happen
        }
      else
        {
          TAO::NetworkPriorityPolicy_var npp =
            TAO::NetworkPriorityPolicy::_narrow (
              server_nw_priority_policy.in ()
              ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          if (!CORBA::is_nil (npp.in ()))
            {
              TAO::NetworkPriorityModel network_priority_model =
                npp->network_priority_model (ACE_ENV_SINGLE_ARG_PARAMETER);
              ACE_CHECK;

              if (network_priority_model ==
                  TAO::SERVER_DECLARED_NETWORK_PRIORITY)
                {
                  diffserv_codepoint =
                    npp->request_diffserv_codepoint (
                       ACE_ENV_SINGLE_ARG_PARAMETER);
                  ACE_CHECK;
                  dscp = diffserv_codepoint;
                }
            }
        }
    }
  else
    {
      TAO::NetworkPriorityPolicy_var client_nw_priority =
        TAO::NetworkPriorityPolicy::_narrow (client_nw_priority_policy.in ()
                                             ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
       
      if (!CORBA::is_nil (client_nw_priority.in ()))
        {
          diffserv_codepoint =
            client_nw_priority->request_diffserv_codepoint ();
          dscp = diffserv_codepoint;
        }
    }
  return dscp;
}

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_STATIC_SVC_DEFINE (TAO_DS_Network_Priority_Protocols_Hooks,
                       ACE_TEXT ("DS_Network_Priority_Protocols_Hooks"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_DS_Network_Priority_Protocols_Hooks),
                       ACE_Service_Type::DELETE_THIS
                       | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (TAO_DiffServPolicy,
                    TAO_DS_Network_Priority_Protocols_Hooks)
