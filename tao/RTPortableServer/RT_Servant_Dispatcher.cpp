// -*- C++ -*-
// $Id$

#include "tao/RTPortableServer/RT_Servant_Dispatcher.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#include "tao/RTPortableServer/RT_POA.h"

#include "tao/ORB_Core.h"
#include "tao/ORB_Core_TSS_Resources.h"
#include "tao/TAO_Server_Request.h"
#include "tao/Transport.h"
#include "tao/Connection_Handler.h"
#include "tao/Service_Context.h"
#include "tao/Protocols_Hooks.h"
#include "tao/Network_Priority_Protocols_Hooks.h"
#include "tao/PortableServer/Network_Priority_Hook.h"
#include "tao/debug.h"
#include "tao/CDR.h"

#include "tao/RTCORBA/Thread_Pool.h"

#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_RT_Servant_Dispatcher::~TAO_RT_Servant_Dispatcher (void)
{
}

void
TAO_RT_Servant_Dispatcher::pre_invoke_remote_request (
  TAO_Root_POA &poa,
  CORBA::Short servant_priority,
  TAO_ServerRequest &req,
  TAO::Portable_Server::Servant_Upcall::Pre_Invoke_State &pre_invoke_state
  )
{
  TAO_Service_Context &request_service_context = req.request_service_context ();
  TAO_Service_Context &reply_service_context = req.reply_service_context ();

  TAO_Thread_Pool *thread_pool =
    static_cast <TAO_Thread_Pool *> (poa.thread_pool ());

  if (thread_pool != 0 &&
      thread_pool->with_lanes ())
    {
      //
      // We don't mess with the priority of threads in lanes.
      //

      if (TAO_debug_level > 0)
        {
          // Get the ORB_Core's TSS resources.
          TAO_ORB_Core_TSS_Resources *tss =
            poa.orb_core ().get_tss_resources ();

          /// Get the lane attribute in TSS.
          TAO_Thread_Lane *lane =
            static_cast<TAO_Thread_Lane *> (tss->lane_);

          ACE_ASSERT (lane->pool ().id () ==
                      thread_pool->id ());

          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("Using thread pool with lane ")
                      ACE_TEXT ("(%P|%t|%d|%d): original thread ")
                      ACE_TEXT ("CORBA/native priority %d/%d not changed\n"),
                      lane->pool ().id (),
                      lane->id (),
                      lane->lane_priority (),
                      lane->native_priority ()));
        }

      return;
    }

  // Remember current thread's priority.
  TAO_Protocols_Hooks *tph = poa.orb_core ().get_protocols_hooks ();

  if (tph != 0)
    {
      const char *priority_model = 0;
      RTCORBA::Priority target_priority = TAO_INVALID_PRIORITY;

      // NOT_SPECIFIED PriorityModel processing.
      if (poa.priority_model () ==
          TAO::Portable_Server::Cached_Policies::NOT_SPECIFIED)
        {
          priority_model = "RTCORBA::NOT_SPECIFIED";
        }

      // CLIENT_PROPAGATED PriorityModel processing.
      else if (poa.priority_model () ==
          TAO::Portable_Server::Cached_Policies::CLIENT_PROPAGATED)
        {
          priority_model = "RTCORBA::CLIENT_PROPAGATED";

          // Attempt to extract client-propagated priority from the
          // ServiceContextList of the request.
          const IOP::ServiceContext *context = 0;

          if (request_service_context.get_context (IOP::RTCorbaPriority,
                                                   &context) == 1)
            {
              // Extract the target priority
              TAO_InputCDR cdr (reinterpret_cast
                                <const char*>
                                 (context->context_data.get_buffer ()),
                                context->context_data.length ());
              CORBA::Boolean byte_order;
              if (!(cdr >> ACE_InputCDR::to_boolean (byte_order)))
                throw ::CORBA::MARSHAL ();
              cdr.reset_byte_order (static_cast<int> (byte_order));

              if (!(cdr >> target_priority))
                throw ::CORBA::MARSHAL ();

              // Save the target priority in the response service
              // context to propagate back to the client as specified
              // by the RTCORBA specification.
              reply_service_context.set_context (*context);
            }
          else
            {
              // Use default priority if none came in the request.
              // (Request must have come from a non-RT ORB.)
              target_priority = poa.server_priority ();
            }
        }
      else
        // SERVER_DECLARED PriorityModel processing.
        {
          priority_model = "RTCORBA::SERVER_DECLARED";

          // Use the request associated with the servant.
          target_priority = servant_priority;
        }

      char thread_pool_id[BUFSIZ];
      if (TAO_debug_level > 0)
        {
          if (thread_pool == 0)
            ACE_OS::strcpy (thread_pool_id,
                            "default thread pool");
          else
            ACE_OS::sprintf (thread_pool_id,
                             "thread pool %d",
                             thread_pool->id ());
        }

      // Target priority is invalid.
      if (target_priority == TAO_INVALID_PRIORITY)
        {
          if (TAO_debug_level > 0)
            {

    // If we are in a multi-threaded configuration, print out the current
    // thread priority.
    #if defined (ACE_HAS_THREADS)

              if (tph->get_thread_CORBA_and_native_priority (
                    pre_invoke_state.original_CORBA_priority_,
                    pre_invoke_state.original_native_priority_) == -1)
                throw ::CORBA::DATA_CONVERSION (CORBA::OMGVMCID | 2,
                                                CORBA::COMPLETED_NO);

              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("(%P|%t): %C processing using %C ")
                          ACE_TEXT ("original thread CORBA/native priority %d/%d ")
                          ACE_TEXT ("not changed\n"),
                          priority_model,
                          thread_pool_id,
                          pre_invoke_state.original_CORBA_priority_,
                          pre_invoke_state.original_native_priority_));

    // If we are in a single-threaded configuration, we cannot get the
    // current thread priority.  Therefore, print out a simpler message.
    #else /* ACE_HAS_THREADS */

              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("(%P|%t): %C processing using %C ")
                          ACE_TEXT ("original thread CORBA/native priority ")
                          ACE_TEXT ("not changed\n"),
                          priority_model,
                          thread_pool_id));

    #endif /* ACE_HAS_THREADS */

            }
        }
      else
        {
          // Get the current thread's priority.

          if (tph->get_thread_CORBA_and_native_priority (
                pre_invoke_state.original_CORBA_priority_,
                pre_invoke_state.original_native_priority_) == -1)
            throw ::CORBA::DATA_CONVERSION (CORBA::OMGVMCID | 2,
                                            CORBA::COMPLETED_NO);

          // Priority needs to be temporarily changed for the
          // duration of request.
          if (target_priority != pre_invoke_state.original_CORBA_priority_)
            {
              if (tph->set_thread_CORBA_priority (target_priority) == -1)
                throw ::CORBA::DATA_CONVERSION (CORBA::OMGVMCID | 2,
                                                CORBA::COMPLETED_NO);

              pre_invoke_state.state_ =
                TAO::Portable_Server::Servant_Upcall::Pre_Invoke_State::PRIORITY_RESET_REQUIRED;

              if (TAO_debug_level > 0)
                {
                  CORBA::Short native_priority = 0;
                  tph->get_thread_native_priority (native_priority);

                  ACE_DEBUG ((LM_DEBUG,
                              ACE_TEXT ("%C processing using %C ")
                              ACE_TEXT ("(%P|%t): original thread CORBA/native priority %d/%d ")
                              ACE_TEXT ("temporarily changed to CORBA/native priority %d/%d\n"),
                              priority_model,
                              thread_pool_id,
                              pre_invoke_state.original_CORBA_priority_,
                              pre_invoke_state.original_native_priority_,
                              target_priority,
                              native_priority));
                }
            }
          // No change in priority required.
          else
            {
              if (TAO_debug_level > 0)
                {
                  ACE_DEBUG ((LM_DEBUG,
                              ACE_TEXT ("%C processing using %C ")
                              ACE_TEXT ("(%P|%t): original thread CORBA/native priority %d/%d ")
                              ACE_TEXT ("is the same as the target priority\n"),
                              priority_model,
                              thread_pool_id,
                              pre_invoke_state.original_CORBA_priority_,
                              pre_invoke_state.original_native_priority_));
                }
            }
        }
    }

  TAO_Network_Priority_Protocols_Hooks *nph =
    poa.orb_core ().get_network_priority_protocols_hooks ();

  if (nph != 0)
    {
      poa.network_priority_hook ()-> set_dscp_codepoint (req, poa);
    }
  else if (tph != 0)
    {
      CORBA::Policy_var policy =
        poa.policies ().get_cached_policy (
          TAO_CACHED_POLICY_RT_SERVER_PROTOCOL);
      CORBA::Boolean set_server_network_priority =
        tph->set_server_network_priority (
          req.transport ()->tag (), policy.in ());
      TAO_Connection_Handler *connection_handler =
        req.transport ()->connection_handler ();
      connection_handler->set_dscp_codepoint (set_server_network_priority);
    }
}

void
TAO_RT_Servant_Dispatcher::pre_invoke_collocated_request (TAO_Root_POA &poa,
                                                          CORBA::Short servant_priority,
                                                          TAO::Portable_Server::Servant_Upcall::Pre_Invoke_State &pre_invoke_state)
{
  TAO_Thread_Pool *thread_pool =
    static_cast <TAO_Thread_Pool *> (poa.thread_pool ());

  if (thread_pool == 0 || thread_pool->with_lanes ())
    {
      //
      // We don't mess with the priority of threads in lanes or for
      // the default thread pool.
      //
      return;
    }

  if (poa.priority_model () !=
      TAO::Portable_Server::Cached_Policies::SERVER_DECLARED ||
      servant_priority == TAO_INVALID_PRIORITY)
    {
      //
      // We either don't have server declared model or servant
      // priority is invalid.
      //
      return;
    }

  //
  // SERVER_DECLARED PriorityModel processing.
  //

  // Remember current thread's priority.
  TAO_Protocols_Hooks *tph = poa.orb_core ().get_protocols_hooks ();

  if (tph != 0)
    {
      if (tph->get_thread_CORBA_and_native_priority (
            pre_invoke_state.original_CORBA_priority_,
            pre_invoke_state.original_native_priority_) == -1)
        throw ::CORBA::DATA_CONVERSION (CORBA::OMGVMCID | 2, CORBA::COMPLETED_NO);

      // Change the priority of the current thread for the duration of
      // request.
      if (servant_priority != pre_invoke_state.original_CORBA_priority_)
        {
          if (tph->set_thread_CORBA_priority (servant_priority) == -1)
            throw ::CORBA::DATA_CONVERSION (CORBA::OMGVMCID | 2, CORBA::COMPLETED_NO);

          pre_invoke_state.state_ =
            TAO::Portable_Server::Servant_Upcall::Pre_Invoke_State::PRIORITY_RESET_REQUIRED;
        }
    }
}

void
TAO_RT_Servant_Dispatcher::post_invoke (TAO_Root_POA &poa,
                                        TAO::Portable_Server::Servant_Upcall::Pre_Invoke_State &pre_invoke_state)

{
  if (pre_invoke_state.state_ ==
      TAO::Portable_Server::Servant_Upcall::Pre_Invoke_State::PRIORITY_RESET_REQUIRED)
    {
      pre_invoke_state.state_ =
        TAO::Portable_Server::Servant_Upcall::Pre_Invoke_State::NO_ACTION_REQUIRED;

      try
        {
          // Reset the priority of the current thread back to its original
          // value.
          TAO_Protocols_Hooks *tph = poa.orb_core ().get_protocols_hooks ();

          if (tph != 0)
            {
              if (tph->restore_thread_CORBA_and_native_priority (
                       pre_invoke_state.original_CORBA_priority_,
                       pre_invoke_state.original_native_priority_) == -1)
                throw ::CORBA::DATA_CONVERSION (CORBA::OMGVMCID | 2,
                                                CORBA::COMPLETED_NO);
            }
        }
      catch (const ::CORBA::Exception& ex)
        {
          // Eat up the exception.
          ex._tao_print_exception (
            "Exception caught: TAO - "
            "Priority_Model_Processing::"
            "~Priority_Model_Processing");
        }
    }
}

TAO_Root_POA *
TAO_RT_Servant_Dispatcher::create_Root_POA (const ACE_CString &name,
                                            PortableServer::POAManager_ptr poa_manager,
                                            const TAO_POA_Policy_Set &policies,
                                            ACE_Lock &lock,
                                            TAO_SYNCH_MUTEX &thread_lock,
                                            TAO_ORB_Core &orb_core,
                                            TAO_Object_Adapter *object_adapter)
{
  TAO_RT_POA *poa = 0;

  ACE_NEW_THROW_EX (poa,
                    TAO_RT_POA (name,
                                poa_manager,
                                policies,
                                0,
                                lock,
                                thread_lock,
                                orb_core,
                                object_adapter),
                    CORBA::NO_MEMORY ());

  return poa;
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */
