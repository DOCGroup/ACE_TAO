// $Id$

#include "tao/PortableServer/Default_Servant_Dispatcher.h"
#include "tao/PortableServer/Root_POA.h"
#include "tao/ORB_Core.h"
#include "tao/ORB_Core_TSS_Resources.h"
#include "tao/TAO_Server_Request.h"
#include "tao/Transport.h"
#include "tao/Connection_Handler.h"
#include "tao/Service_Context.h"
#include "tao/Protocols_Hooks.h"
#include "tao/debug.h"
#include "tao/CDR.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"

ACE_RCSID(PortableServer,
          Default_Servant_Dispatcher,
          "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Default_Servant_Dispatcher::~TAO_Default_Servant_Dispatcher (void)
{
}

TAO_Root_POA *
TAO_Default_Servant_Dispatcher::create_Root_POA (const ACE_CString &name,
                                                 PortableServer::POAManager_ptr poa_manager,
                                                 const TAO_POA_Policy_Set &policies,
                                                 ACE_Lock &lock,
                                                 TAO_SYNCH_MUTEX &thread_lock,
                                                 TAO_ORB_Core &orb_core,
                                                 TAO_Object_Adapter *object_adapter
                                                 ACE_ENV_ARG_DECL)
{
  TAO_Root_POA *poa = 0;

  ACE_NEW_THROW_EX (poa,
                    TAO_Root_POA (name,
                                  poa_manager,
                                  policies,
                                  0,
                                  lock,
                                  thread_lock,
                                  orb_core,
                                  object_adapter
                                  ACE_ENV_ARG_PARAMETER), CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  return poa;
}

void
TAO_Default_Servant_Dispatcher::pre_invoke_remote_request (
  TAO_Root_POA &poa,
  CORBA::Short servant_priority,
  TAO_ServerRequest &req,
  TAO::Portable_Server::Servant_Upcall::Pre_Invoke_State &pre_invoke_state
  ACE_ENV_ARG_DECL_NOT_USED)
{
  TAO_Service_Context &request_service_context =
    req.request_service_context ();

  CORBA::Long dscp_codepoint;

  TAO_Connection_Handler *connection_handler = 
    req.transport ()->connection_handler ();

  dscp_codepoint = poa.get_diffserv_codepoint (request_service_context);
  connection_handler->set_dscp_codepoint (dscp_codepoint);

  /*
  if (poa.network_priority_model () ==
      TAO::Portable_Server::Cached_Policies::CLIENT_PROPAGATED_NETWORK_PRIORITY)
    {
      if (request_service_context.get_context (IOP::REP_NWPRIORITY,
                                               &context) == 1)
        {
          TAO_InputCDR cdr (reinterpret_cast
                            <const char*>
                            (context->context_data.get_buffer ()),
                            context->context_data.length ());
          CORBA::Boolean byte_order;
          if ((cdr >> ACE_InputCDR::to_boolean (byte_order)) == 0)
            ACE_THROW (CORBA::MARSHAL ());
          cdr.reset_byte_order (static_cast<int> (byte_order));

          if ((cdr >> dscp_codepoint) == 0)
            ACE_THROW (CORBA::MARSHAL ());

          connection_handler->set_dscp_codepoint (dscp_codepoint);
        }
    }
    else if (poa.network_priority_model () ==
             TAO::Portable_Server::Cached_Policies::
             SERVER_DECLARED_NETWORK_PRIORITY)
      {
        CORBA::Long dscp = poa.reply_diffserv_codepoint ();
        connection_handler->set_dscp_codepoint (dscp);
      }

    */
}

void
TAO_Default_Servant_Dispatcher::pre_invoke_collocated_request (
  TAO_Root_POA &,
  CORBA::Short,
  TAO::Portable_Server::Servant_Upcall::Pre_Invoke_State &
  ACE_ENV_ARG_DECL_NOT_USED)
{
}

void
TAO_Default_Servant_Dispatcher::post_invoke (
  TAO_Root_POA &,
  TAO::Portable_Server::Servant_Upcall::Pre_Invoke_State &)
{
}

TAO_END_VERSIONED_NAMESPACE_DECL
