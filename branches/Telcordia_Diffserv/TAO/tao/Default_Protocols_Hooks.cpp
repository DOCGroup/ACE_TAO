// $Id$

#include "tao/Default_Protocols_Hooks.h"
#include "ace/Log_Msg.h"

#include "tao/Stub.h"
#include "tao/ORB_Core.h"
#include "tao/MProfile.h"
#include "tao/Service_Context.h"
#include "tao/Acceptor_Registry.h"
#include "tao/Thread_Lane_Resources.h"
#include "tao/Protocol_Factory.h"
#include "tao/Transport_Acceptor.h"
#include "tao/Transport_Connector.h"
#include "tao/Policy_Set.h"
#include "tao/debug.h"
#include "tao/CDR.h"
#include "ace/Dynamic_Service.h"
#include "ace/OS_NS_string.h"
#include "tao/Client_Network_Priority_Policy.h"

ACE_RCSID (tao,
           Default_Protocols_Hooks,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Default_Protocols_Hooks::~TAO_Default_Protocols_Hooks (void)
{
}

void
TAO_Default_Protocols_Hooks::init_hooks (TAO_ORB_Core *
                                         ACE_ENV_ARG_DECL_NOT_USED)
{
}

void
TAO_Default_Protocols_Hooks::add_rep_np_service_context_hook (
   TAO_Service_Context &service_context,
   CORBA::Long &dscp_codepoint
   ACE_ENV_ARG_DECL_NOT_USED)
{
  TAO_OutputCDR cdr;
  if ((cdr << ACE_OutputCDR::from_boolean (TAO_ENCAP_BYTE_ORDER) == 0)
      || (cdr << dscp_codepoint) == 0)
    {
      ACE_THROW (CORBA::MARSHAL ());
    }

  service_context.set_context (IOP::REP_NWPRIORITY, cdr);
}

void
TAO_Default_Protocols_Hooks::add_rt_service_context_hook (TAO_Service_Context &,
                                                          CORBA::Policy *,
                                                          CORBA::Short &
                                                          ACE_ENV_ARG_DECL_NOT_USED)
{
}

void
TAO_Default_Protocols_Hooks::get_selector_hook (CORBA::Policy *,
                                                CORBA::Boolean &,
                                                CORBA::Short &)
{
}

void
TAO_Default_Protocols_Hooks::get_selector_bands_policy_hook (CORBA::Policy *,
                                                             CORBA::Short,
                                                             CORBA::Short &,
                                                             CORBA::Short &,
                                                             int &)
{
}

int
TAO_Default_Protocols_Hooks::get_thread_CORBA_priority (CORBA::Short &
                                                        ACE_ENV_ARG_DECL_NOT_USED)
{
  return 0;
}

int
TAO_Default_Protocols_Hooks::get_thread_native_priority (CORBA::Short &
                                                         ACE_ENV_ARG_DECL_NOT_USED)
{
  return 0;
}

int
TAO_Default_Protocols_Hooks::get_thread_CORBA_and_native_priority (CORBA::Short &,
                                                                   CORBA::Short &
                                                                   ACE_ENV_ARG_DECL_NOT_USED)
{
  return 0;
}

int
TAO_Default_Protocols_Hooks::set_thread_CORBA_priority (CORBA::Short
                                                        ACE_ENV_ARG_DECL_NOT_USED)
{
  return 0;
}

int
TAO_Default_Protocols_Hooks::set_thread_native_priority (CORBA::Short
                                                         ACE_ENV_ARG_DECL_NOT_USED)
{
  return 0;
}

void
TAO_Default_Protocols_Hooks::np_service_context (
    TAO_Stub *stub,
    TAO_Service_Context &service_context,
    CORBA::Boolean restart
    ACE_ENV_ARG_DECL_NOT_USED)
{
  // If the restart flag is true, then this call for a
  // reinvocation. We need not prepare the Service Context List once
  // again. We can use the already existing one.
  if (!restart)
    {
      CORBA::Policy_var cnpp =
        stub->get_cached_policy (TAO_CACHED_POLICY_CLIENT_NETWORK_PRIORITY
                        ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (!CORBA::is_nil (cnpp.in ()))
        {
          TAO::NetworkPriorityPolicy_var cnp =
            TAO::NetworkPriorityPolicy::_narrow (cnpp.in ()
                            ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          TAO::DiffservCodepoint reply_diffserv_codepoint;
          reply_diffserv_codepoint = cnp->get_reply_diffserv_codepoint ();

          CORBA::Long rep_dscp_codepoint = reply_diffserv_codepoint;

          this->add_rep_np_service_context_hook (service_context,
                                             rep_dscp_codepoint
                                             ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;
        }
    }
}

void
TAO_Default_Protocols_Hooks::rt_service_context (TAO_Stub *,
                                                 TAO_Service_Context &,
                                                 CORBA::Boolean
                                                 ACE_ENV_ARG_DECL_NOT_USED)
{
}

CORBA::Boolean
TAO_Default_Protocols_Hooks::set_client_network_priority (IOP::ProfileId,
                                                          TAO_Stub *
                                                          ACE_ENV_ARG_DECL_NOT_USED)
{
  return true;
}

CORBA::Boolean
TAO_Default_Protocols_Hooks::set_server_network_priority (IOP::ProfileId,
                                                          CORBA::Policy *
                                                          ACE_ENV_ARG_DECL_NOT_USED)
{
  return true;
}

void
TAO_Default_Protocols_Hooks::server_protocol_properties_at_orb_level (TAO_IIOP_Protocol_Properties &
                                                                      ACE_ENV_ARG_DECL_NOT_USED)
{
}

void
TAO_Default_Protocols_Hooks::client_protocol_properties_at_orb_level (TAO_IIOP_Protocol_Properties &
                                                                      ACE_ENV_ARG_DECL_NOT_USED)
{
}

void
TAO_Default_Protocols_Hooks::server_protocol_properties_at_orb_level (TAO_UIOP_Protocol_Properties &
                                                                      ACE_ENV_ARG_DECL_NOT_USED)
{
}

void
TAO_Default_Protocols_Hooks::client_protocol_properties_at_orb_level (TAO_UIOP_Protocol_Properties &
                                                                      ACE_ENV_ARG_DECL_NOT_USED)
{
}

void
TAO_Default_Protocols_Hooks::server_protocol_properties_at_orb_level (TAO_SHMIOP_Protocol_Properties &
                                                                      ACE_ENV_ARG_DECL_NOT_USED)
{
}

void
TAO_Default_Protocols_Hooks::client_protocol_properties_at_orb_level (TAO_SHMIOP_Protocol_Properties &
                                                                      ACE_ENV_ARG_DECL_NOT_USED)
{
}

void
TAO_Default_Protocols_Hooks::server_protocol_properties_at_orb_level (TAO_DIOP_Protocol_Properties &
                                                                      ACE_ENV_ARG_DECL_NOT_USED)
{
}

void
TAO_Default_Protocols_Hooks::client_protocol_properties_at_orb_level (TAO_DIOP_Protocol_Properties &
                                                                      ACE_ENV_ARG_DECL_NOT_USED)
{
}

void
TAO_Default_Protocols_Hooks::server_protocol_properties_at_orb_level (TAO_SCIOP_Protocol_Properties &
                                                                      ACE_ENV_ARG_DECL_NOT_USED)
{
}

void
TAO_Default_Protocols_Hooks::client_protocol_properties_at_orb_level (TAO_SCIOP_Protocol_Properties &
                                                                      ACE_ENV_ARG_DECL_NOT_USED)
{
}

void
TAO_Default_Protocols_Hooks::set_dscp_codepoint (CORBA::Long &dscp_codepoint
                                   ACE_ENV_ARG_DECL_NOT_USED)
{
  this->dscp_codepoint_ = dscp_codepoint;
}

CORBA::Long
TAO_Default_Protocols_Hooks::get_dscp_codepoint (void)
{
  return this->dscp_codepoint_;
}

TAO_END_VERSIONED_NAMESPACE_DECL

// ****************************************************************

ACE_STATIC_SVC_DEFINE (TAO_Default_Protocols_Hooks,
                       ACE_TEXT ("Protocols_Hooks"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_Default_Protocols_Hooks),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (TAO, TAO_Default_Protocols_Hooks)
