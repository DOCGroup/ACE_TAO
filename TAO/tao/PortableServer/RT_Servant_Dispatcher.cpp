// $Id$

#include "RT_Servant_Dispatcher.h"
#include "POA.h"
#include "tao/ORB_Core.h"
#include "tao/TAO_Server_Request.h"
#include "tao/debug.h"

ACE_RCSID (tao,
           Default_Acceptor_Filter,
           "$Id$")

#if (TAO_HAS_RT_CORBA == 1)

RT_Priority_Model_Processing::RT_Priority_Model_Processing (TAO_POA &poa)
  : state_ (NO_ACTION_REQUIRED),
    poa_ (poa),
    original_priority_ (0)
{
}

RT_Priority_Model_Processing::~RT_Priority_Model_Processing (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      if (this->state_ == PRIORITY_RESET_REQUIRED)
        {
          this->state_ = NO_ACTION_REQUIRED;

          // Reset the priority of the current thread back to its original
          // value.
          int result =
            poa_.orb_core ().get_protocols_hooks ()->
            set_thread_priority (&poa_.orb_core (),
                                 this->original_priority_,
                                 ACE_TRY_ENV);
          ACE_TRY_CHECK;

          if (result == -1)
            ACE_DEBUG ((LM_ERROR,
                        ACE_TEXT ("TAO (%P|%t) - Priority_Model_Processing:: ")
                        ACE_TEXT (" Priority_Model_Processing\n")));
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "~RT_Priority_Model_Processing:");
    }
  ACE_ENDTRY;
}

void
RT_Priority_Model_Processing::pre_invoke (
   TAO_Service_Context &request_service_context,
   TAO_Service_Context &reply_service_context,
   CORBA::Environment &ACE_TRY_ENV)
{
  TAO_POA_Policies &poa_policies = this->poa_.policies ();

  // CLIENT_PROPAGATED PriorityModel processing.
  if (poa_policies.priority_model ()
      == TAO_POA_Policies::CLIENT_PROPAGATED
      && poa_policies.server_priority ()
      != TAO_INVALID_PRIORITY)
    {
      // Remember current thread's priority.
      int result =
        poa_.orb_core ().get_protocols_hooks ()->
          get_thread_priority (&poa_.orb_core (),
                               this->original_priority_,
                               ACE_TRY_ENV);
      ACE_CHECK;

      if (result == -1)
        ACE_THROW (CORBA::DATA_CONVERSION (TAO_OMG_VMCID | 1, // @@ Correct?
                                           CORBA::COMPLETED_NO));

      // Attempt to extract client-propagated priority from the
      //  ServiceContextList of the request.
      RTCORBA::Priority target_priority;
      const IOP::ServiceContext *context;

      if (request_service_context.get_context (IOP::RTCorbaPriority,
                                               &context) == 1)
        {
          // Extract the target priority
          TAO_InputCDR cdr (ACE_reinterpret_cast
                            (const char*,
                             context->context_data.get_buffer ()),
                            context->context_data.length ());
          CORBA::Boolean byte_order;
          if ((cdr >> ACE_InputCDR::to_boolean (byte_order)) == 0)
            ACE_THROW (CORBA::MARSHAL ());
          cdr.reset_byte_order (ACE_static_cast (int, byte_order));

          if ((cdr >> target_priority) == 0)
            ACE_THROW (CORBA::MARSHAL ());

          // Save the target priority in the response service
          // context to propagate back to the client as specified
          // by the RTCORBA specification.
          reply_service_context.set_context (*context);
        }
      else
        {
          // Use default priority if none came in the request.  (Request
          // must have come from a non-RT ORB.)
          target_priority = poa_policies.server_priority ();
        }

      // Change the priority of the current thread to the
      // client-propagated value for the duration of
      // request.
      if (original_priority_ != target_priority)
        {
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("RTCORBA::CLIENT_PROPAGATED processing")
                        ACE_TEXT (" (%P|%t): original thread priority %d")
                        ACE_TEXT (" temporarily changed to %d\n"),
                        original_priority_, target_priority));

          result =
            poa_.orb_core ().get_protocols_hooks ()->
              set_thread_priority (&poa_.orb_core (),
                                   target_priority,
                                   ACE_TRY_ENV);
          ACE_CHECK;

          if (result == -1)
            ACE_THROW (CORBA::DATA_CONVERSION (TAO_OMG_VMCID | 1, //@@ Correct?
                                               CORBA::COMPLETED_NO));

          this->state_ = PRIORITY_RESET_REQUIRED;
        }
      else if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("RTCORBA::CLIENT_PROPAGATED processing")
                    ACE_TEXT (" (%P|%t): original thread priority =")
                    ACE_TEXT (" requested priority = %d\n"),
                    target_priority));
    }
}

void
RT_Priority_Model_Processing::post_invoke (
    CORBA::Environment &ACE_TRY_ENV)
{
  if (this->state_ == PRIORITY_RESET_REQUIRED)
    {
      this->state_ = NO_ACTION_REQUIRED;

      // Reset the priority of the current thread back to its original
      // value.
      int result =
        poa_.orb_core ().get_protocols_hooks ()->
          set_thread_priority (&poa_.orb_core (),
                               this->original_priority_,
                               ACE_TRY_ENV);
      ACE_CHECK;

      if (result == -1)
        ACE_THROW (CORBA::DATA_CONVERSION (TAO_OMG_VMCID | 1, //@@ Correct?
                                           CORBA::COMPLETED_NO));
    }
}

/////////////////////////////////////////////////////////////////////////

TAO_RT_Servant_Dispatcher::~TAO_RT_Servant_Dispatcher (void)
{
}

void
TAO_RT_Servant_Dispatcher::dispatch (TAO_Object_Adapter::Servant_Upcall &servant_upcall,
                                     TAO_ServerRequest &req,
                                     CORBA::Environment &ACE_TRY_ENV)
{
  // RTCORBA PriorityModelPolicy processing (may need to be
  // moved/adjusted when POA threadpools are added).
  // This is the earliest place we can do the processing, since only
  // at this point we know the target POA.

  RT_Priority_Model_Processing priority_processing (servant_upcall.poa ());

  // Set thread's priority.
  priority_processing.pre_invoke (req.request_service_context (),
                                  req.reply_service_context (),
                                  ACE_TRY_ENV);
  ACE_CHECK;

  // Servant dispatch.
  servant_upcall.servant ()->_dispatch (req,
                                        &servant_upcall,
                                        ACE_TRY_ENV);
  ACE_CHECK;

  // Reset thread's priority to its original value.  If this method
  // isn't reached, i.e., because of an exception, the reset takes
  // place in Priority_Model_Processing destructor.
  priority_processing.post_invoke (ACE_TRY_ENV);
  ACE_CHECK;
}

#endif /* TAO_HAS_RT_CORBA == 1 */
