// $Id$

#include "interceptors.h"
#include "tao/OctetSeqC.h"

ACE_RCSID (Service_Context_Manipulation,
           interceptors,
           "$Id$")

const IOP::ServiceId service_id = 0xdeadbeef;
const char *request_msg = "REQUEST message";
const char *reply_msg = "REPLY message";
const char *forward_msg = "FORWARD message";
int send_request_count = 0;
int exception_count = 0;

Echo_Client_Request_Interceptor::
Echo_Client_Request_Interceptor (const char *id)
  : myname_ ("Echo_Client_Interceptor"),
    orb_id_ (CORBA::string_dup (id))
{
}

Echo_Client_Request_Interceptor::~Echo_Client_Request_Interceptor (void)
{
}

char *
Echo_Client_Request_Interceptor::name (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup (this->myname_);
}

void
Echo_Client_Request_Interceptor::destroy (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
Echo_Client_Request_Interceptor::send_poll (
    PortableInterceptor::ClientRequestInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Do Nothing
}

void
Echo_Client_Request_Interceptor::send_request (
    PortableInterceptor::ClientRequestInfo_ptr ri
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
  send_request_count++;

  if (CORBA::is_nil (this->orb_.in ()))
    {
      int argc = 0;
      this->orb_ = CORBA::ORB_init (argc, 0,
                                    this->orb_id_.in ()
                                    ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }

  CORBA::String_var operation = ri->operation (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::Object_var target = ri->target (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::String_var ior =
    this->orb_->object_to_string (target.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_DEBUG ((LM_DEBUG,
              "%s.send_request "
              "from \"%s\" on object: %s\n",
              this->myname_,
              operation.in (),
              ior.in ()));

  ACE_TRY_NEW_ENV
    {
      ri->get_request_service_context (::service_id ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA::BAD_PARAM, error)
    {
      // This is supposed to happen
      exception_count++;
    }
  ACE_ENDTRY;

  if (send_request_count == 1)
    {
      ACE_ASSERT (exception_count == 1);
      // Populate target member of the ClientRequestInfo.

      // Make the context to send the context to the target
      IOP::ServiceContext sc;
      sc.context_id = ::service_id;

      CORBA::ULong string_len = ACE_OS::strlen (request_msg) + 1;
      CORBA::Octet *buf = CORBA::OctetSeq::allocbuf (string_len);
      ACE_OS::strcpy (ACE_reinterpret_cast (char *, buf), request_msg);

      sc.context_data.replace (string_len, string_len, buf, 1);

      // Add this context to the service context list.
      ri->add_request_service_context (sc, 0 ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      // Check that the request service context can be retrieved.
      IOP::ServiceContext_var sc2 =
        ri->get_request_service_context (::service_id ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      const char *buf2 =
        ACE_reinterpret_cast (const char *, sc2->context_data.get_buffer ());

      if (ACE_OS::strcmp (buf2, request_msg) != 0)
        {
            ACE_ERROR ((LM_ERROR,
                        "ERROR: Expected request service context to be: %s.\n"
                        "  Got: %s\n",
                        request_msg,
                        buf2));
        }
    }
  else if (send_request_count == 2)
    {
      ACE_ASSERT (exception_count == 2);
      // Populate target member of the ClientRequestInfo.

      // Make the context to send the context to the target
      IOP::ServiceContext sc;
      sc.context_id = ::service_id;

      CORBA::ULong string_len = ACE_OS::strlen (forward_msg) + 1;
      CORBA::Octet *buf = CORBA::OctetSeq::allocbuf (string_len);
      ACE_OS::strcpy (ACE_reinterpret_cast (char *, buf), forward_msg);

      sc.context_data.replace (string_len, string_len, buf, 1);
      // Add this context to the service context list.
      ri->add_request_service_context (sc, 0 ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      // Check that the request service context can be retrieved.
      IOP::ServiceContext_var sc2 =
        ri->get_request_service_context (::service_id ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      const char *buf2 =
        ACE_reinterpret_cast (const char *, sc2->context_data.get_buffer ());

      if (ACE_OS::strcmp (buf2, forward_msg) != 0)
        {
            ACE_ERROR ((LM_ERROR,
                        "ERROR: Expected request service context to be: %s.\n"
                        "  Got: %s\n",
                        forward_msg,
                        buf2));
        }
    }
  else
    {
      // Something's wrong
    }

}

void
Echo_Client_Request_Interceptor::receive_reply (
    PortableInterceptor::ClientRequestInfo_ptr ri
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{

  if (CORBA::is_nil (this->orb_.in ()))
    {
      int argc = 0;
      this->orb_ = CORBA::ORB_init (argc, 0,
                                    this->orb_id_.in ()
                                    ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }

  CORBA::String_var operation = ri->operation (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::Object_var target = ri->target (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::String_var ior =
    this->orb_->object_to_string (target.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_DEBUG ((LM_DEBUG,
              "%s.receive_reply from "
              "\"%s\" on object: %s\n",
              this->myname_,
              operation.in (),
              ior.in ()));

  // Check that the reply service context was received as
  // expected.
  IOP::ServiceContext_var sc =
    ri->get_reply_service_context (::service_id ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  const char *buf =
    ACE_reinterpret_cast (const char *, sc->context_data.get_buffer ());
  ACE_DEBUG ((LM_DEBUG,
              "  Received reply service context: %s\n",
              buf));

  if (ACE_OS::strcmp (buf, reply_msg) != 0)
    {
        ACE_ERROR ((LM_ERROR,
                    "ERROR: Expected reply service context to be: %s\n",
                    reply_msg));
    }

  // Check that no one has messed with the request service context.
  IOP::ServiceContext_var sc2 =
    ri->get_request_service_context (::service_id ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  const char *buf2 =
    ACE_reinterpret_cast (const char *, sc2->context_data.get_buffer ());

  if (ACE_OS::strcmp (buf2, forward_msg) != 0)
    {
        ACE_ERROR ((LM_ERROR,
                    "ERROR: Expected request service context to be: %s.\n"
                    "  Got: %s\n",
                    forward_msg,
                    buf2));
    }
  send_request_count = 0;
  exception_count = 0;
}

void
Echo_Client_Request_Interceptor::receive_other (
    PortableInterceptor::ClientRequestInfo_ptr ri
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
  if (CORBA::is_nil (this->orb_.in ()))
    {
      int argc = 0;
      this->orb_ = CORBA::ORB_init (argc, 0,
                                    this->orb_id_.in ());
    }

  CORBA::String_var operation = ri->operation (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::Object_var target = ri->target (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::String_var ior =
    this->orb_->object_to_string (target.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_DEBUG ((LM_DEBUG,
              "%s.receive_other from "
              "\"%s\" on object: %s\n",
              this->myname_,
              operation.in (),
              ior.in ()));

  // Check that no one has messed with the request service context.
  IOP::ServiceContext_var sc =
    ri->get_request_service_context (::service_id ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  const char *buf =
    ACE_reinterpret_cast (const char *, sc->context_data.get_buffer ());

  if (ACE_OS::strcmp (buf, request_msg) != 0)
    {
        ACE_ERROR ((LM_ERROR,
                    "ERROR: Expected request service context to be: %s.\n"
                    "  Got: %s\n",
                    request_msg,
                    buf));
    }
}

void
Echo_Client_Request_Interceptor::receive_exception (
    PortableInterceptor::ClientRequestInfo_ptr ri
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{

  if (CORBA::is_nil (this->orb_.in ()))
    {
      int argc = 0;
      this->orb_ = CORBA::ORB_init (argc, 0,
                                    this->orb_id_.in ()
                                    ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }

  CORBA::String_var operation = ri->operation (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::Object_var target = ri->target (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::String_var ior =
    this->orb_->object_to_string (target.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_DEBUG ((LM_DEBUG,
              "%s.received_exception "
              "from \"%s\" on object: %s\n",
              this->myname_,
              operation.in (),
              ior.in ()));

  // Check that the reply service context was received as
  // expected.
  IOP::ServiceContext_var sc =
    ri->get_reply_service_context (::service_id ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  const char *buf =
    ACE_reinterpret_cast (const char *, sc->context_data.get_buffer ());
  ACE_DEBUG ((LM_DEBUG,
              "  Received reply service context: %s\n",
              buf));

  if (ACE_OS::strcmp (buf, reply_msg) != 0)
    {
        ACE_ERROR ((LM_ERROR,
                    "ERROR: Expected reply service context to be: %s\n",
                    reply_msg));
    }

  // Check that no one has messed with the request service context.
  IOP::ServiceContext_var sc2 =
    ri->get_request_service_context (::service_id ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  const char *buf2 =
    ACE_reinterpret_cast (const char *, sc2->context_data.get_buffer ());

  if (ACE_OS::strcmp (buf2, forward_msg) != 0)
    {
        ACE_ERROR ((LM_ERROR,
                    "ERROR: Expected request service context to be: %s.\n"
                    "  Got: %s\n",
                    forward_msg,
                    buf2));
    }
  send_request_count = 0;
  exception_count = 0;
}

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
Echo_Server_Request_Interceptor::forward_reference (CORBA::Object_ptr forward_location
                                                    ACE_ENV_ARG_DECL)
                                                    ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (CORBA::is_nil (forward_location))
    ACE_THROW (CORBA::INV_OBJREF (
                 CORBA::SystemException::_tao_minor_code (
                   TAO_DEFAULT_MINOR_CODE,
                   EINVAL),
                 CORBA::COMPLETED_NO));
  this->forward_location_ = CORBA::Object::_duplicate (forward_location);
}

char *
Echo_Server_Request_Interceptor::name (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup (this->myname_);
}

void
Echo_Server_Request_Interceptor::destroy (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
Echo_Server_Request_Interceptor::receive_request_service_contexts (
    PortableInterceptor::ServerRequestInfo_ptr ri
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{

  CORBA::String_var operation = ri->operation (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  ACE_DEBUG ((LM_DEBUG,
              "%s.receive_request_service_contexts from "
              "\"%s\"\n",
              this->myname_,
              operation.in ()));

  // Ignore the "_is_a" operation since it may have been invoked
  // locally on the server side as a side effect of another call,
  // meaning that the client hasn't added the service context yet.
  if (ACE_OS_String::strcmp ("_is_a", operation.in ()) == 0)
    return;

  IOP::ServiceId id = ::service_id;
  IOP::ServiceContext_var sc =
    ri->get_request_service_context (id ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  const char *buf =
    ACE_reinterpret_cast (const char *, sc->context_data.get_buffer ());
  ACE_DEBUG ((LM_DEBUG,
              "  Received service context: %s\n",
              buf));
  if (ACE_OS::strcmp (buf, request_msg) == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "Sending LOCATION_FORWARD\n"));
      ACE_THROW (PortableInterceptor::ForwardRequest (this->forward_location_, 0));
    }
  else if (ACE_OS::strcmp (buf, forward_msg) == 0)
    {
      // Make the context to send the context to the client
      IOP::ServiceContext scc;

      scc.context_id = ::service_id;

      CORBA::ULong string_len = ACE_OS::strlen (reply_msg) + 1;
      CORBA::Octet *buff = CORBA::OctetSeq::allocbuf (string_len);

      ACE_OS::strcpy (ACE_reinterpret_cast (char *, buff), reply_msg);

      scc.context_data.replace (string_len, string_len, buff, 1);

      // Add this context to the service context list.
      ri->add_reply_service_context (scc, 0 ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
  else
    {
      ACE_ERROR ((LM_ERROR,
                  "ERROR: Echo_Server_Request_Interceptor::receive_request_service_contexts: "
                  "Expected request service context to be: %s or %s\n",
                  request_msg,
                  forward_msg));
    }
}


void
Echo_Server_Request_Interceptor::receive_request (
    PortableInterceptor::ServerRequestInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
  // Do nothing
}

void
Echo_Server_Request_Interceptor::send_reply (
    PortableInterceptor::ServerRequestInfo_ptr ri
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{

  CORBA::String_var operation = ri->operation (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  ACE_DEBUG ((LM_DEBUG,
              "%s.send_reply from \"%s\"\n",
              this->myname_,
              ri->operation ()));

  // Check that the reply service context is set as expected.
  IOP::ServiceContext_var sc =
    ri->get_reply_service_context (::service_id ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  const char *buf = ACE_reinterpret_cast (const char *,
                                          sc->context_data.get_buffer ());
  ACE_DEBUG ((LM_DEBUG,
              "  Reply service context: %s\n",
              buf));

  if (ACE_OS::strcmp (buf, reply_msg) != 0)
    {
        ACE_ERROR ((LM_ERROR,
                    "ERROR: Echo_Server_Request_Interceptor::send_reply: "
                    "Expected reply service context to be: %s\n",
                    reply_msg));
    }

  // Check that the request service context hasn't been changed.
  IOP::ServiceContext_var sc2 =
    ri->get_request_service_context (::service_id ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  const char *buf2 = ACE_reinterpret_cast (const char *,
                                           sc2->context_data.get_buffer ());

  if (ACE_OS::strcmp (buf2, forward_msg) != 0)
    {
        ACE_ERROR ((LM_ERROR,
                    "ERROR: Echo_Server_Request_Interceptor::send_reply: "
                    "Expected request service context to be: %s.\n"
                    "  Got: %s\n",
                    forward_msg,
                    buf2));
    }
}

void
Echo_Server_Request_Interceptor::send_exception (
    PortableInterceptor::ServerRequestInfo_ptr ri
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{

  CORBA::String_var operation = ri->operation (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  ACE_DEBUG ((LM_DEBUG,
              "%s.send_exception from \"%s\"\n",
              this->myname_,
              operation.in ()));

  // Check that the reply service context is set as expected.
  IOP::ServiceContext_var sc =
    ri->get_reply_service_context (::service_id ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  const char *buf = ACE_reinterpret_cast (const char *,
                                          sc->context_data.get_buffer ());
  ACE_DEBUG ((LM_DEBUG,
              "  Reply service context: %s\n",
              buf));

  if (ACE_OS::strcmp (buf, reply_msg) != 0)
    {
        ACE_ERROR ((LM_ERROR,
                    "ERROR: Echo_Server_Request_Interceptor::send_exception: "
                    "Expected reply service context to be: %s\n",
                    reply_msg));
    }

  // Check that the request service context hasn't been changed.
  IOP::ServiceContext_var sc2 =
    ri->get_request_service_context (::service_id ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  const char *buf2 = ACE_reinterpret_cast (const char *,
                                           sc2->context_data.get_buffer ());

  if (ACE_OS::strcmp (buf2, forward_msg) != 0)
    {
        ACE_ERROR ((LM_ERROR,
                    "ERROR: Echo_Server_Request_Interceptor::send_exception: "
                    "Expected request service context to be: %s.\n"
                    "  Got: %s\n",
                    forward_msg,
                    buf2));
    }
}

void
Echo_Server_Request_Interceptor::send_other (
             PortableInterceptor::ServerRequestInfo_ptr ri
             ACE_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableInterceptor::ForwardRequest))
{
  CORBA::String_var operation = ri->operation (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  ACE_DEBUG ((LM_DEBUG,
              "%s.send_other from \"%s\"\n",
              this->myname_,
              operation.in ()));

  // Check that the request service context hasn't been changed.
  IOP::ServiceContext_var sc =
    ri->get_request_service_context (::service_id ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  const char *buf = ACE_reinterpret_cast (const char *,
                                           sc->context_data.get_buffer ());

  if (ACE_OS::strcmp (buf, request_msg) != 0)
    {
        ACE_ERROR ((LM_ERROR,
                    "ERROR: Echo_Server_Request_Interceptor::send_reply: "
                    "Expected request service context to be: %s.\n"
                    "  Got: %s\n",
                    request_msg,
                    buf));
    }

  // If we get this far then we should have received a
  // LOCATION_FORWARD reply.

  // This will throw an exception if a location forward has not
  // occured.  If an exception is thrown then something is wrong with
  // the PortableInterceptor::ForwardRequest support.
  CORBA::Object_var forward = ri->forward_reference (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (CORBA::is_nil (forward.in ()))
    ACE_THROW (CORBA::INTERNAL ());
}
