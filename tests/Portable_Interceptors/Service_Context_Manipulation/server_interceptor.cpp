// $Id$

#include "server_interceptor.h"
#include "tao/OctetSeqC.h"

#include "ace/Log_Msg.h"
#include "ace/OS_NS_string.h"

const IOP::ServiceId service_id = 0xdeadbeef;
const char *request_msg = "The Echo_Request_Interceptor request message";
const char *reply_msg = "The Echo_Request_Interceptor reply message";

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
              "%C.receive_request_service_contexts from "
              "\"%C\"\n",
              this->myname_,
              operation.in ()));

  IOP::ServiceId id = ::service_id;
  IOP::ServiceContext_var sc =
    ri->get_request_service_context (id);

  const char *buf =
    reinterpret_cast<const char *> (sc->context_data.get_buffer ());
#if 0
  ACE_DEBUG ((LM_DEBUG,
              "  Received service context: %C\n",
              buf));
#endif /*if 0*/

  if (ACE_OS::strcmp (buf, request_msg) != 0)
    {
        ACE_ERROR ((LM_ERROR,
                    "ERROR: Echo_Server_Request_Interceptor::receive_request_service_contexts: "
                    "Expected request service context to be: %C\n",
                    request_msg));
    }

  // Make the context to send the context to the client
  IOP::ServiceContext scc;

  scc.context_id = ::service_id;

  CORBA::ULong string_len = ACE_OS::strlen (reply_msg) + 1;
  CORBA::Octet *buff = CORBA::OctetSeq::allocbuf (string_len);

  ACE_OS::strcpy (reinterpret_cast<char *> (buff), reply_msg);

  scc.context_data.replace (string_len, string_len, buff, 1);

  // Add this context to the service context list.
  ri->add_reply_service_context (scc, 0);
}


void
Echo_Server_Request_Interceptor::receive_request (
    PortableInterceptor::ServerRequestInfo_ptr)
{
  // Do nothing
}

void
Echo_Server_Request_Interceptor::send_reply (
    PortableInterceptor::ServerRequestInfo_ptr ri)
{

  CORBA::String_var operation = ri->operation ();

  ACE_DEBUG ((LM_DEBUG,
              "%C.send_reply from \"%C\"\n",
              this->myname_,
              operation.in ()));

  // Check that the reply service context is set as expected.
  IOP::ServiceContext_var sc =
    ri->get_reply_service_context (::service_id);

  const char *buf = reinterpret_cast<const char *> (sc->context_data.get_buffer ());

#if 0
  ACE_DEBUG ((LM_DEBUG,
              "  Reply service context: %C\n",
              buf));
#endif /*if 0*/

  if (ACE_OS::strcmp (buf, reply_msg) != 0)
    {
        ACE_ERROR ((LM_ERROR,
                    "ERROR: Echo_Server_Request_Interceptor::send_reply: "
                    "Expected reply service context to be: %C\n",
                    reply_msg));
    }

  // Check that the request service context hasn't been changed.
  IOP::ServiceContext_var sc2 =
    ri->get_request_service_context (::service_id);

  const char *buf2 = reinterpret_cast<const char *> (sc2->context_data.get_buffer ());

  if (ACE_OS::strcmp (buf2, request_msg) != 0)
    {
        ACE_ERROR ((LM_ERROR,
                    "ERROR: Echo_Server_Request_Interceptor::send_reply: "
                    "Expected request service context to be: %C.\n"
                    "  Got: %C\n",
                    request_msg,
                    buf2));
    }
}

void
Echo_Server_Request_Interceptor::send_exception (
    PortableInterceptor::ServerRequestInfo_ptr ri)
{

  CORBA::String_var operation = ri->operation ();

#if 0
  ACE_DEBUG ((LM_DEBUG,
              "%C.send_exception from \"%C\"\n",
              this->myname_,
              operation.in ()));
#endif /*if 0*/

  // Check that the reply service context is set as expected.
  IOP::ServiceContext_var sc =
    ri->get_reply_service_context (::service_id);

  const char *buf = reinterpret_cast<const char *> (sc->context_data.get_buffer ());

#if 0
  ACE_DEBUG ((LM_DEBUG,
              "  Reply service context: %C\n",
              buf));
#endif /*if 0*/

  if (ACE_OS::strcmp (buf, reply_msg) != 0)
    {
        ACE_ERROR ((LM_ERROR,
                    "ERROR: Echo_Server_Request_Interceptor::send_exception: "
                    "Expected reply service context to be: %C\n",
                    reply_msg));
    }

  // Check that the request service context hasn't been changed.
  IOP::ServiceContext_var sc2 =
    ri->get_request_service_context (::service_id);

  const char *buf2 = reinterpret_cast<const char *> (sc2->context_data.get_buffer ());

  if (ACE_OS::strcmp (buf2, request_msg) != 0)
    {
        ACE_ERROR ((LM_ERROR,
                    "ERROR: Echo_Server_Request_Interceptor::send_exception: "
                    "Expected request service context to be: %C.\n"
                    "  Got: %C\n",
                    request_msg,
                    buf2));
    }
}

void
Echo_Server_Request_Interceptor::send_other (
             PortableInterceptor::ServerRequestInfo_ptr)
{
  // Do Nothing
}
