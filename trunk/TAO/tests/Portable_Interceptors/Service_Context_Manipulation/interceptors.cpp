// $Id$

#include "interceptors.h"
#include "tao/OctetSeqC.h"

ACE_RCSID (Service_Context_Manipulation,
           interceptors,
           "$Id$")

const CORBA::ULong request_ctx_id = 0xdead;
const CORBA::ULong reply_ctx_id = 0xbeef;
const char *request_msg = "The Echo_Request_Interceptor request message";
const char *reply_msg = "The Echo_Request_Interceptor reply message";

Echo_Client_Request_Interceptor::
Echo_Client_Request_Interceptor (const char *id)
  : myname_ ("Echo_Client_Interceptor"),
    orb_id_ (CORBA::string_dup (id))
{
}

Echo_Client_Request_Interceptor::~Echo_Client_Request_Interceptor (void)
{
}

void
Echo_Client_Request_Interceptor::_add_ref (void)
{
  this->_incr_refcnt ();
  // This is a TAO specific implementation.
}

void
Echo_Client_Request_Interceptor::_remove_ref (void)
{
  this->_decr_refcnt ();
  // This is a TAO specific implementation.
}

char *
Echo_Client_Request_Interceptor::name (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup (this->myname_);
}

void
Echo_Client_Request_Interceptor::destroy (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
Echo_Client_Request_Interceptor::send_poll (
    PortableInterceptor::ClientRequestInfo_ptr
    TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Do Nothing
}

void
Echo_Client_Request_Interceptor::send_request (
    PortableInterceptor::ClientRequestInfo_ptr ri
    TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
  TAO_ENV_ARG_DEFN;

  if (CORBA::is_nil (this->orb_.in ()))
    {
      int argc = 0;
      this->orb_ = CORBA::ORB_init (argc, 0,
                                    this->orb_id_.in (),
                                    ACE_TRY_ENV);
      ACE_CHECK;
    }

  CORBA::String_var operation = ri->operation (ACE_TRY_ENV);
  ACE_CHECK;

  CORBA::Object_var target = ri->target (ACE_TRY_ENV);
  ACE_CHECK;

  CORBA::String_var ior =
    this->orb_->object_to_string (target.in (), ACE_TRY_ENV);
  ACE_CHECK;

  ACE_DEBUG ((LM_DEBUG,
              "Echo_Client_Request_Interceptor::send_request "
              "from \"%s\" on object: %s\n",
              operation.in (),
              ior.in ()));

  // Populate target member of the ClientRequestInfo.

  // MAke the context to send the context to the target
  IOP::ServiceContext sc;
  sc.context_id = request_ctx_id;

  CORBA::ULong string_len = ACE_OS::strlen (request_msg) + 1;
  CORBA::Octet *buf = CORBA::OctetSeq::allocbuf (string_len);
  ACE_OS::strcpy (ACE_reinterpret_cast (char *, buf), request_msg);

  sc.context_data.replace (string_len, string_len, buf, 1);

  // Add this context to the service context list.
  ri->add_request_service_context (sc, 0, ACE_TRY_ENV);
}

void
Echo_Client_Request_Interceptor::receive_reply (
    PortableInterceptor::ClientRequestInfo_ptr ri
    TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_ENV_ARG_DEFN;

  if (CORBA::is_nil (this->orb_.in ()))
    {
      int argc = 0;
      this->orb_ = CORBA::ORB_init (argc, 0,
                                    this->orb_id_.in (),
                                    ACE_TRY_ENV);
      ACE_CHECK;
    }

  CORBA::String_var operation = ri->operation (ACE_TRY_ENV);
  ACE_CHECK;

  CORBA::Object_var target = ri->target (ACE_TRY_ENV);
  ACE_CHECK;

  CORBA::String_var ior =
    this->orb_->object_to_string (target.in (), ACE_TRY_ENV);
  ACE_CHECK;

  ACE_DEBUG ((LM_DEBUG,
              "Echo_Client_Request_Interceptor::receive_reply from "
              "\"%s\" on object: %s\n",
              operation.in (),
              ior.in ()));

  IOP::ServiceId id = reply_ctx_id;
  IOP::ServiceContext_var sc =
    ri->get_reply_service_context (id, ACE_TRY_ENV);
  ACE_CHECK;

  const char *buf =
    ACE_reinterpret_cast (const char *, sc->context_data.get_buffer ());
  ACE_DEBUG ((LM_DEBUG,
              "  Received reply service context: %s\n",
              buf));
}

void
Echo_Client_Request_Interceptor::receive_other (
    PortableInterceptor::ClientRequestInfo_ptr
    TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
  ACE_DEBUG ((LM_DEBUG,
              "Echo_Client_Request_Interceptor::receive_other\n"));
}

void
Echo_Client_Request_Interceptor::receive_exception (
    PortableInterceptor::ClientRequestInfo_ptr ri
    TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
  TAO_ENV_ARG_DEFN;

  if (CORBA::is_nil (this->orb_.in ()))
    {
      int argc = 0;
      this->orb_ = CORBA::ORB_init (argc, 0,
                                    this->orb_id_.in (),
                                    ACE_TRY_ENV);
      ACE_CHECK;
    }

  CORBA::String_var operation = ri->operation (ACE_TRY_ENV);
  ACE_CHECK;

  CORBA::Object_var target = ri->target (ACE_TRY_ENV);
  ACE_CHECK;

  CORBA::String_var ior =
    this->orb_->object_to_string (target.in (), ACE_TRY_ENV);
  ACE_CHECK;

  ACE_DEBUG ((LM_DEBUG,
              "Echo_Client_Request_Interceptor::received_exception "
              "from \"%s\" on object: %s\n",
              operation.in (), ior.in ()));
  ACE_CHECK;
}

Echo_Server_Request_Interceptor::Echo_Server_Request_Interceptor (void)
  : myname_ ("Echo_Server_Interceptor")
{
}

Echo_Server_Request_Interceptor::~Echo_Server_Request_Interceptor (void)
{
}

void
Echo_Server_Request_Interceptor::_add_ref (void)
{
  this->_incr_refcnt ();
  // This is a TAO specific implementation.
}

void
Echo_Server_Request_Interceptor::_remove_ref (void)
{
  this->_decr_refcnt ();
  // This is a TAO specific implementation.
}

char *
Echo_Server_Request_Interceptor::name (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup (this->myname_);
}

void
Echo_Server_Request_Interceptor::destroy (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
Echo_Server_Request_Interceptor::receive_request_service_contexts (
    PortableInterceptor::ServerRequestInfo_ptr ri
    TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
  TAO_ENV_ARG_DEFN;

  CORBA::String_var operation = ri->operation (ACE_TRY_ENV);
  ACE_CHECK;

  ACE_DEBUG ((LM_DEBUG,
              "Echo_Server_Request_Interceptor::"
              "receive_request_service_contexts from "
              "\"%s\"\n",
              operation.in ()));

  // Ignore the "_is_a" operation since it may have been invoked
  // locally on the server side as a side effect of another call,
  // meaning that the client hasn't added the service context yet.
  if (ACE_OS_String::strcmp ("_is_a", operation.in ()) == 0)
    return;

  IOP::ServiceId id = request_ctx_id;
  IOP::ServiceContext_var sc =
    ri->get_request_service_context (id, ACE_TRY_ENV);
  ACE_CHECK;

  const char *buf =
    ACE_reinterpret_cast (const char *, sc->context_data.get_buffer ());
  ACE_DEBUG ((LM_DEBUG,
              "  Received service context: %s\n",
              buf));

  // Make the context to send the context to the client
  IOP::ServiceContext scc;

  scc.context_id = reply_ctx_id;

  CORBA::ULong string_len = ACE_OS::strlen (reply_msg) + 1;
  CORBA::Octet *buff = CORBA::OctetSeq::allocbuf (string_len);

  ACE_OS::strcpy (ACE_reinterpret_cast (char *, buff), reply_msg);

  scc.context_data.replace (string_len, string_len, buff, 1);

  // Add this context to the service context list.
  ri->add_reply_service_context (scc, 0, ACE_TRY_ENV);
  ACE_CHECK;
}


void
Echo_Server_Request_Interceptor::receive_request (
    PortableInterceptor::ServerRequestInfo_ptr
    TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
  // Do nothing
}

void
Echo_Server_Request_Interceptor::send_reply (
    PortableInterceptor::ServerRequestInfo_ptr ri
    TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_ENV_ARG_DEFN;

  CORBA::String_var operation = ri->operation (ACE_TRY_ENV);
  ACE_CHECK;

  ACE_DEBUG ((LM_DEBUG,
              "Echo_Server_Request_Interceptor::send_reply from \"%s\"",
              ri->operation ()));

  IOP::ServiceId id = reply_ctx_id;
  IOP::ServiceContext_var sc =
    ri->get_reply_service_context (id, ACE_TRY_ENV);
  ACE_CHECK;

  const char *buf = ACE_reinterpret_cast (const char *,
                                          sc->context_data.get_buffer ());
  ACE_DEBUG ((LM_DEBUG,
              "  Replying service context: %s\n",
              buf));
}

void
Echo_Server_Request_Interceptor::send_exception (
    PortableInterceptor::ServerRequestInfo_ptr ri
    TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
  TAO_ENV_ARG_DEFN;

  CORBA::String_var operation = ri->operation (ACE_TRY_ENV);
  ACE_CHECK;

  ACE_DEBUG ((LM_DEBUG,
              "Echo_Server_Request_Interceptor::send_exception from "
              "\"%s\"",
              operation.in ()));
}

void
Echo_Server_Request_Interceptor::send_other (
             PortableInterceptor::ServerRequestInfo_ptr
             TAO_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableInterceptor::ForwardRequest))
{
  // Do Nothing
}
