// $Id$

#include "server_interceptor.h"
#include "tao/OctetSeqC.h"
#include "ace/Log_Msg.h"
#include "tao/ORB_Constants.h"
#include "ace/OS_NS_string.h"

const IOP::ServiceId service_id = 0xdeadbeef;
const char *request_msg = "REQUEST message";
const char *reply_msg = "REPLY message";
const char *forward_msg = "FORWARD message";
int send_request_count = 0;
int exception_count = 0;

Echo_Server_Request_Interceptor::Echo_Server_Request_Interceptor (void)
  : myname_ ("Echo_Server_Interceptor")
{
}

Echo_Server_Request_Interceptor::Echo_Server_Request_Interceptor (CORBA::Object_ptr forward_location)
  : myname_ ("Echo_Server_Interceptor"),
    forward_location_ (CORBA::Object::_duplicate (forward_location))
{
}

Echo_Server_Request_Interceptor::~Echo_Server_Request_Interceptor (void)
{
}

void
Echo_Server_Request_Interceptor::forward_reference (CORBA::Object_ptr forward_location)
{
  if (CORBA::is_nil (forward_location))
    throw CORBA::INV_OBJREF (
      CORBA::SystemException::_tao_minor_code (
        TAO::VMCID,
        EINVAL),
      CORBA::COMPLETED_NO);
  this->forward_location_ = CORBA::Object::_duplicate (forward_location);
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

  // Ignore the "_is_a" operation since it may have been invoked
  // locally on the server side as a side effect of another call,
  // meaning that the client hasn't added the service context yet.
  // Same goes for the shutdown call
  if (ACE_OS_String::strcmp ("shutdown", operation.in ()) == 0)
    return;

  IOP::ServiceId id = ::service_id;
  IOP::ServiceContext_var sc =
    ri->get_request_service_context (id);

  const char *buf =
    reinterpret_cast<const char *> (sc->context_data.get_buffer ());
  ACE_DEBUG ((LM_DEBUG,
              "  Received service context: %C\n",
              buf));
  if (ACE_OS::strcmp (buf, request_msg) == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "Sending LOCATION_FORWARD\n"));
      throw PortableInterceptor::ForwardRequest (this->forward_location_.in ());
    }
  else if (ACE_OS::strcmp (buf, forward_msg) == 0)
    {
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
  else
    {
      ACE_ERROR ((LM_ERROR,
                  "ERROR: Echo_Server_Request_Interceptor::receive_request_service_contexts: "
                  "Expected request service context to be: %C or %C\n",
                  request_msg,
                  forward_msg));
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
  ACE_DEBUG ((LM_DEBUG,
              "  Reply service context: %C\n",
              buf));

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

  if (ACE_OS::strcmp (buf2, forward_msg) != 0)
    {
        ACE_ERROR ((LM_ERROR,
                    "ERROR: Echo_Server_Request_Interceptor::send_reply: "
                    "Expected request service context to be: %C.\n"
                    "  Got: %C\n",
                    forward_msg,
                    buf2));
    }
}

void
Echo_Server_Request_Interceptor::send_exception (
    PortableInterceptor::ServerRequestInfo_ptr ri)
{

  CORBA::String_var operation = ri->operation ();

  ACE_DEBUG ((LM_DEBUG,
              "%C.send_exception from \"%C\"\n",
              this->myname_,
              operation.in ()));

  // Check that the reply service context is set as expected.
  IOP::ServiceContext_var sc =
    ri->get_reply_service_context (::service_id);

  const char *buf = reinterpret_cast<const char *> (sc->context_data.get_buffer ());
  ACE_DEBUG ((LM_DEBUG,
              "  Reply service context: %C\n",
              buf));

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

  if (ACE_OS::strcmp (buf2, forward_msg) != 0)
    {
        ACE_ERROR ((LM_ERROR,
                    "ERROR: Echo_Server_Request_Interceptor::send_exception: "
                    "Expected request service context to be: %C.\n"
                    "  Got: %C\n",
                    forward_msg,
                    buf2));
    }
}

void
Echo_Server_Request_Interceptor::send_other (
             PortableInterceptor::ServerRequestInfo_ptr ri)
{
  CORBA::String_var operation = ri->operation ();

  ACE_DEBUG ((LM_DEBUG,
              "%C.send_other from \"%C\"\n",
              this->myname_,
              operation.in ()));

  // Check that the request service context hasn't been changed.
  IOP::ServiceContext_var sc =
    ri->get_request_service_context (::service_id);

  const char *buf = reinterpret_cast<const char *> (sc->context_data.get_buffer ());

  if (ACE_OS::strcmp (buf, request_msg) != 0)
    {
        ACE_ERROR ((LM_ERROR,
                    "ERROR: Echo_Server_Request_Interceptor::send_reply: "
                    "Expected request service context to be: %C.\n"
                    "  Got: %C\n",
                    request_msg,
                    buf));
    }

  // If we get this far then we should have received a
  // LOCATION_FORWARD reply.

  // This will throw an exception if a location forward has not
  // occured.  If an exception is thrown then something is wrong with
  // the PortableInterceptor::ForwardRequest support.
  CORBA::Object_var forward = ri->forward_reference ();

  if (CORBA::is_nil (forward.in ()))
    throw CORBA::INTERNAL ();
}
