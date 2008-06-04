// $Id$

#include "server_interceptor.h"
#include "tao/OctetSeqC.h"
#include "ace/Log_Msg.h"
#include "tao/ORB_Constants.h"
#include "ace/OS_NS_string.h"

const IOP::ServiceId service_id = 0x08154711;
const char * const request_msg = "TEST 0123456789 TEST";
PortableInterceptor::SlotId slotId = 0;

Echo_Server_Request_Interceptor::Echo_Server_Request_Interceptor (void)
  : myname_ ("Echo_Server_Interceptor")
{
}

Echo_Server_Request_Interceptor::~Echo_Server_Request_Interceptor (void)
{
}

char *
Echo_Server_Request_Interceptor::name (void)
{
  return CORBA::string_dup (this->myname_);
}

void
Echo_Server_Request_Interceptor::destroy (void)
{
}

void
Echo_Server_Request_Interceptor::receive_request_service_contexts (
    PortableInterceptor::ServerRequestInfo_ptr ri)
{

  CORBA::String_var operation = ri->operation ();

  ACE_DEBUG ((LM_DEBUG,
              "%s.receive_request_service_contexts from "
              "\"%s\"\n",
              this->myname_,
              operation.in ()));

  // Ignore the shutdown operation.
  if (ACE_OS_String::strcmp ("shutdown", operation.in ()) == 0)
    return;

  // retrieve the context
  IOP::ServiceId id = ::service_id;
  IOP::ServiceContext_var sc =
    ri->get_request_service_context (id);

  const char *buf =
    reinterpret_cast<const char *> (sc->context_data.get_buffer ());
  ACE_DEBUG ((LM_DEBUG,
              "  Received service context: %s\n",
              buf));

  if (ACE_OS::strcmp (buf, request_msg) != 0)
  {
    ACE_ERROR ((LM_ERROR,
                "ERROR: Echo_Server_Request_Interceptor::receive_request_service_contexts: "
                "Expected request service context to be: %s\n",
                request_msg ));
  }
  else
  {
    // put the context into s slot
    CORBA::Any data;
    data <<= buf;

    try
    {
        ri->set_slot (slotId, data);
        ACE_DEBUG ((LM_DEBUG, "receive_request_service_contexts filled Slot %d\n",(int)slotId));
    }
    catch (const CORBA::Exception& ex)
    {
        ex._tao_print_exception (
          "Exception thrown in receive_request_service_contexts()\n");
        throw CORBA::INTERNAL ();
    }

  }
}


void
Echo_Server_Request_Interceptor::receive_request (
    PortableInterceptor::ServerRequestInfo_ptr)
{
  // Do nothing
}

void
Echo_Server_Request_Interceptor::send_reply (
    PortableInterceptor::ServerRequestInfo_ptr)
{
  // Do nothing
}

void
Echo_Server_Request_Interceptor::send_exception (
    PortableInterceptor::ServerRequestInfo_ptr)
{
  // Do nothing
}

void
Echo_Server_Request_Interceptor::send_other (
             PortableInterceptor::ServerRequestInfo_ptr)
{
  // Do nothing
}
