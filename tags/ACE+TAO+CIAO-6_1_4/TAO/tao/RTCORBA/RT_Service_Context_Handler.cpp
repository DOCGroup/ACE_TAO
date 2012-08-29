// $Id$

#include "tao/RTCORBA/RT_Service_Context_Handler.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#include "tao/RTCORBA/RT_Policy_i.h"
#include "tao/RTCORBA/RT_Protocols_Hooks.h"
#include "tao/RTCORBA/RT_Stub.h"
#include "tao/CDR.h"
#include "tao/TAO_Server_Request.h"
#include "tao/Transport.h"
#include "tao/ORB_Core.h"
#include "tao/GIOP_Message_Base.h"
#include "tao/operation_details.h"
#include "tao/Transport_Mux_Strategy.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

int
TAO_RT_Service_Context_Handler::process_service_context (
  TAO_Transport&,
  const IOP::ServiceContext&,
  TAO_ServerRequest*)
{
  return 0;
}

int
TAO_RT_Service_Context_Handler::generate_service_context (
  TAO_Stub *stub,
  TAO_Transport&,
  TAO_Operation_Details &opdetails,
  TAO_Target_Specification &,
  TAO_OutputCDR &)
{
  TAO_RT_Stub *rt_stub =
    dynamic_cast<TAO_RT_Stub *> (stub);

  if (rt_stub)
    {
      CORBA::Policy_var priority_model_policy =
        rt_stub->get_cached_policy (TAO_CACHED_POLICY_PRIORITY_MODEL);

      RTCORBA::PriorityModelPolicy_var model_policy_ptr =
        RTCORBA::PriorityModelPolicy::_narrow (priority_model_policy.in ());

      if (!CORBA::is_nil (model_policy_ptr.in ()))
        {
          TAO_PriorityModelPolicy *priority_model =
            static_cast<TAO_PriorityModelPolicy *> (model_policy_ptr.in ());

          if (priority_model->get_priority_model () == RTCORBA::CLIENT_PROPAGATED)
            {
              CORBA::Short client_priority = -1;
              TAO_Protocols_Hooks *protocol_hooks = stub->orb_core ()->get_protocols_hooks ();
              // Get client thread priority from 'Current' or if not set by implying one
              // from the native thread priority via the mapping.
              if (protocol_hooks &&
                  (protocol_hooks->get_thread_CORBA_priority (client_priority) != -1 ||
                   protocol_hooks->get_thread_implicit_CORBA_priority (client_priority) != -1))
                {
                  // OK
                }
              else
                {
                  if (TAO_debug_level > 0)
                    ACE_ERROR ((LM_ERROR, "ERROR: TAO_RT_Protocols_Hooks::rt_service_context. "
                                "Unable to access RT CORBA Priority in client thread "
                                "accessing object with CLIENT_PROPAGATED priority model.\n"));
                  throw CORBA::DATA_CONVERSION (CORBA::OMGVMCID | 2, CORBA::COMPLETED_NO);
                }

              // Encapsulate the priority of the current thread into
              // a service context.
              TAO_OutputCDR cdr;
              if (!(cdr << ACE_OutputCDR::from_boolean (TAO_ENCAP_BYTE_ORDER))
                  || !(cdr << client_priority))
                {
                  throw CORBA::MARSHAL ();
                }

              opdetails.request_service_context ().set_context (IOP::RTCorbaPriority, cdr);
            }
        }
      else
        {
          // The Object does not contain PriorityModel policy in its IOR.
          // We must be talking to a non-RT ORB.  Do nothing.
        }
    }
  return 0;
}


TAO_END_VERSIONED_NAMESPACE_DECL

#endif
