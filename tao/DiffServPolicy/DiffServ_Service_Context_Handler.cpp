// $Id$

#include "tao/DiffServPolicy/DiffServ_Service_Context_Handler.h"

#include "tao/DiffServPolicy/Client_Network_Priority_Policy.h"
#include "tao/CDR.h"
#include "tao/TAO_Server_Request.h"
#include "tao/Transport.h"
#include "tao/ORB_Core.h"
#include "tao/GIOP_Message_Base.h"
#include "tao/operation_details.h"
#include "tao/Transport_Mux_Strategy.h"
#include "tao/Stub.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

int
TAO_DiffServ_Service_Context_Handler::process_service_context (
  TAO_Transport&,
  const IOP::ServiceContext&)
{
  return 0;
}

int
TAO_DiffServ_Service_Context_Handler::generate_service_context (
  TAO_Stub *stub,
  TAO_Transport&,
  TAO_Operation_Details &opdetails,
  TAO_Target_Specification &,
  TAO_OutputCDR &)
{
  if (stub)
    {
      CORBA::Policy_var cnpp =
        stub->get_cached_policy (TAO_CACHED_POLICY_CLIENT_NETWORK_PRIORITY);

      TAO::NetworkPriorityPolicy_var cnp =
         TAO::NetworkPriorityPolicy::_narrow (cnpp.in ());

      if (!CORBA::is_nil (cnp.in ()))
        {
          TAO::DiffservCodepoint const reply_diffserv_codepoint =
            cnp->reply_diffserv_codepoint ();

          CORBA::Long const rep_dscp_codepoint = reply_diffserv_codepoint;

          TAO_OutputCDR cdr;
          if (!(cdr << ACE_OutputCDR::from_boolean (TAO_ENCAP_BYTE_ORDER))
            || !(cdr << rep_dscp_codepoint))
            {
              throw CORBA::MARSHAL ();
            }

          opdetails.request_service_context ().set_context (IOP::REP_NWPRIORITY, cdr);
        }
    }

  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL
