// @(#) $Id$

#include "RT_Servant_Dispatcher.h"
#include "RT_POA.h"
#include "tao/ORB_Core.h"
#include "tao/TAO_Server_Request.h"
#include "tao/Service_Context.h"
#include "tao/debug.h"

ACE_RCSID(tao, Default_Acceptor_Filter, "$Id$")

RT_Priority_Model_Processing::RT_Priority_Model_Processing (TAO_POA &poa)
  : state_ (NO_ACTION_REQUIRED),
    poa_ (poa),
    original_native_priority_ (0),
    original_CORBA_priority_ (0)
{
}

RT_Priority_Model_Processing::~RT_Priority_Model_Processing (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;

  if (this->state_ == PRIORITY_RESET_REQUIRED)
    {
      this->state_ = NO_ACTION_REQUIRED;

      // Reset the priority of the current thread back to its original
      // value.
      TAO_Protocols_Hooks *tph = poa_.orb_core ().get_protocols_hooks (ACE_TRY_ENV);
      ACE_CHECK;

      if (tph->set_thread_native_priority (this->original_native_priority_,
                                           ACE_TRY_ENV)
          == -1)
        // At this point we cannot throw an exception.  Just log the
        // error.
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("TAO (%P|%t) - Priority_Model_Processing:: ")
                    ACE_TEXT (" Priority_Model_Processing\n")));
    }
}

void
RT_Priority_Model_Processing::pre_invoke (
   TAO_Service_Context &request_service_context,
   TAO_Service_Context &reply_service_context,
   CORBA::Environment &ACE_TRY_ENV)
{
  TAO_POA_Cached_Policies &cached_policies = this->poa_.cached_policies ();

  // CLIENT_PROPAGATED PriorityModel processing.
  if (cached_policies.priority_model () == TAO_POA_Cached_Policies::CLIENT_PROPAGATED
      && cached_policies.server_priority () != TAO_INVALID_PRIORITY)
    {
      // Remember current thread's priority.
      TAO_Protocols_Hooks *tph = poa_.orb_core ().get_protocols_hooks (ACE_TRY_ENV);
      ACE_CHECK;

      if (tph->get_thread_CORBA_and_native_priority (this->original_CORBA_priority_,
                                                     this->original_native_priority_,
                                                     ACE_TRY_ENV)
          == -1)
        ACE_THROW (CORBA::DATA_CONVERSION (1,
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
          cdr.reset_byte_order (ACE_static_cast(int,byte_order));

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
          target_priority = cached_policies.server_priority ();
        }

      // Change the priority of the current thread to the
      // client-propagated value for the duration of
      // request.
      if (this->original_CORBA_priority_ != target_priority)
        {
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("RTCORBA::CLIENT_PROPAGATED processing")
                        ACE_TEXT (" (%P|%t): original thread CORBA/native priority %d/%d;")
                        ACE_TEXT (" temporarily changed to CORBA priority %d\n"),
                        this->original_CORBA_priority_,
                        this->original_native_priority_,
                        target_priority));

          if (tph->set_thread_CORBA_priority (target_priority,
                                              ACE_TRY_ENV)
              == -1)
            ACE_THROW (CORBA::DATA_CONVERSION (1, CORBA::COMPLETED_NO));

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
      TAO_Protocols_Hooks *tph = poa_.orb_core ().get_protocols_hooks (ACE_TRY_ENV);
      ACE_CHECK;

      if (tph->set_thread_native_priority (this->original_native_priority_,
                                           ACE_TRY_ENV)
          == -1)
        ACE_THROW (CORBA::DATA_CONVERSION (1, CORBA::COMPLETED_NO));
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

TAO_POA *
TAO_RT_Servant_Dispatcher::create_POA (const ACE_CString &name,
                                       TAO_POA_Manager &poa_manager,
                                       const TAO_POA_Policy_Set &policies,
                                       TAO_POA *parent,
                                       ACE_Lock &lock,
                                       TAO_SYNCH_MUTEX &thread_lock,
                                       TAO_ORB_Core &orb_core,
                                       TAO_Object_Adapter *object_adapter,
                                       CORBA_Environment &ACE_TRY_ENV)
{
  TAO_RT_POA *poa;

  ACE_NEW_THROW_EX (poa,
                    TAO_RT_POA (name,
                                poa_manager,
                                policies,
                                parent,
                                lock,
                                thread_lock,
                                orb_core,
                                object_adapter,
                                ACE_TRY_ENV),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  return poa;
}
