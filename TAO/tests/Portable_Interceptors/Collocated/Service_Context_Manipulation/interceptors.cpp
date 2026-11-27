#include "interceptors.h"
#include "../../context_match.h"
#include "ace/Log_Msg.h"
#include <cstring>

constexpr IOP::ServiceId service_id { 0xdeadbeef };
char const request_msg[] = "The Echo_Request_Interceptor request message";
char const reply_msg[] = "The Echo_Request_Interceptor reply message";

Echo_Client_Request_Interceptor::Echo_Client_Request_Interceptor (char const *id)
  : orb_id_ (CORBA::string_dup (id))
{
}

char *
Echo_Client_Request_Interceptor::name ()
{
  return CORBA::string_dup (this->myname_);
}

void
Echo_Client_Request_Interceptor::destroy ()
{
}

void
Echo_Client_Request_Interceptor::send_poll (
    PortableInterceptor::ClientRequestInfo_ptr)
{
  // Do Nothing
}

void
Echo_Client_Request_Interceptor::send_request (
    PortableInterceptor::ClientRequestInfo_ptr ri)
{
  if (CORBA::is_nil (this->orb_.in ()))
    {
      int argc {};
      ACE_TCHAR **argv {};
      this->orb_ = CORBA::ORB_init (argc, argv,
                                    this->orb_id_.in ());
    }

  CORBA::String_var operation =
    ri->operation ();

  CORBA::Object_var target = ri->target ();

  CORBA::String_var ior =
    this->orb_->object_to_string (target.in ());

  ACE_DEBUG ((LM_DEBUG,
              "%C.send_request "
              "from \"%C\"\n",
              this->myname_,
              operation.in ()));

  // Populate target member of the ClientRequestInfo.

  // Make the context to send the context to the target
  IOP::ServiceContext sc {};
  sc.context_id = ::service_id;

  CORBA::ULong const string_len = std::strlen (request_msg) + 1;
  // The sequence takes ownership of this buffer in replace ().
  CORBA::Octet *buf = CORBA::OctetSeq::allocbuf (string_len);
  std::strcpy (reinterpret_cast<char *> (buf), request_msg);

  sc.context_data.replace (string_len, string_len, buf, true);

  // Add this context to the service context list.
  ri->add_request_service_context (sc, false);

  // Check that the request service context can be retrieved.
  IOP::ServiceContext_var sc2 =
    ri->get_request_service_context (::service_id);

  if (!context_matches (sc2->context_data, request_msg))
    {
        ACE_ERROR ((LM_ERROR,
                    "ERROR: Expected request service context to be: %C.\n",
                    request_msg));
    }
}

void
Echo_Client_Request_Interceptor::receive_reply (
    PortableInterceptor::ClientRequestInfo_ptr ri)
{
  if (CORBA::is_nil (this->orb_.in ()))
    {
      int argc {};
      ACE_TCHAR **argv {};
      this->orb_ = CORBA::ORB_init (argc, argv,
                                    this->orb_id_.in ());
    }

  CORBA::String_var operation =
    ri->operation ();

  CORBA::Object_var target =
    ri->target ();

  CORBA::String_var ior =
    this->orb_->object_to_string (target.in ());

  ACE_DEBUG ((LM_DEBUG,
              "%C.receive_reply from "
              "\"%C\"\n",
              this->myname_,
              operation.in ()));

  try
    {
      // Check that the reply service context was received as
      // expected.
      IOP::ServiceContext_var sc =
        ri->get_reply_service_context (::service_id);

      if (!context_matches (sc->context_data, reply_msg))
        {
          ACE_ERROR ((LM_ERROR,
                      "ERROR: Expected reply service context to be: %C\n",
                      reply_msg));
        }

      // Check that no one has messed with the request service context.
      IOP::ServiceContext_var sc2 =
        ri->get_request_service_context (::service_id);

      if (!context_matches (sc2->context_data, request_msg))
        {
          ACE_ERROR ((LM_ERROR,
                      "ERROR: Expected request service context to be: %C.\n",
                      request_msg));
        }
    }
  catch (CORBA::BAD_PARAM const &)
    {
      ACE_ERROR ((LM_ERROR,
                  "ERROR: Service Context Error\n"));
    }
}

void
Echo_Client_Request_Interceptor::receive_other (
    PortableInterceptor::ClientRequestInfo_ptr)
{
  ACE_DEBUG ((LM_DEBUG,
              "%C.receive_other\n",
              this->myname_));
}

void
Echo_Client_Request_Interceptor::receive_exception (
    PortableInterceptor::ClientRequestInfo_ptr ri)
{
  if (CORBA::is_nil (this->orb_.in ()))
    {
      int argc {};
      ACE_TCHAR **argv {};
      this->orb_ = CORBA::ORB_init (argc, argv,
                                    this->orb_id_.in ());
    }

  CORBA::String_var operation = ri->operation ();

  CORBA::Object_var target = ri->target ();

  CORBA::String_var ior =
    this->orb_->object_to_string (target.in ());

  ACE_DEBUG ((LM_DEBUG,
              "%C.received_exception "
              "from \"%C\"\n",
              this->myname_,
              operation.in ()));

    try
    {
      // Check that the reply service context was received as
      // expected.
      IOP::ServiceContext_var sc =
        ri->get_reply_service_context (::service_id);

      if (!context_matches (sc->context_data, reply_msg))
        {
          ACE_ERROR ((LM_ERROR,
                      "ERROR: Expected reply service context to be: %C\n",
                      reply_msg));
        }

      // Check that no one has messed with the request service context.
      IOP::ServiceContext_var sc2 =
        ri->get_request_service_context (::service_id);

      if (!context_matches (sc2->context_data, request_msg))
        {
          ACE_ERROR ((LM_ERROR,
                      "ERROR: Expected request service context to be: %C.\n",
                      request_msg));
        }
    }
  catch (CORBA::BAD_PARAM const &)
    {
      ACE_ERROR ((LM_ERROR,
                  "ERROR: Service Context Error\n"));
    }
}

Echo_Server_Request_Interceptor::~Echo_Server_Request_Interceptor ()
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) ~Echo_Server_Request_Interceptor\n"));
}

char *
Echo_Server_Request_Interceptor::name ()
{
  return CORBA::string_dup (this->myname_);
}

void
Echo_Server_Request_Interceptor::destroy ()
{
}

void
Echo_Server_Request_Interceptor::receive_request_service_contexts (
    PortableInterceptor::ServerRequestInfo_ptr ri)
{
  CORBA::String_var operation = ri->operation ();

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t)%C.receive_request_service_contexts from "
              "\"%C\"\n",
              this->myname_,
              operation.in ()));

  IOP::ServiceId id = ::service_id;
  IOP::ServiceContext_var sc =
    ri->get_request_service_context (id);

  if (!context_matches (sc->context_data, request_msg))
    {
        ACE_ERROR ((LM_ERROR,
                    "ERROR: Echo_Server_Request_Interceptor::receive_request_service_contexts: "
                    "Expected request service context to be: %C\n",
                    request_msg));
    }

  // Make the context to send the context to the client
  IOP::ServiceContext scc {};

  scc.context_id = ::service_id;

  CORBA::ULong const string_len = std::strlen (reply_msg) + 1;
  // The sequence takes ownership of this buffer in replace ().
  CORBA::Octet *buff = CORBA::OctetSeq::allocbuf (string_len);

  std::strcpy (reinterpret_cast<char *> (buff), reply_msg);

  scc.context_data.replace (string_len, string_len, buff, true);

  // Add this context to the service context list.
  ri->add_reply_service_context (scc, false);
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
              "(%P|%t)%C.send_reply from \"%C\"\n",
              this->myname_,
              operation.in ()));

  // Check that the reply service context is set as expected.
  IOP::ServiceContext_var sc =
    ri->get_reply_service_context (::service_id);

  if (!context_matches (sc->context_data, reply_msg))
    {
        ACE_ERROR ((LM_ERROR,
                    "ERROR: Echo_Server_Request_Interceptor::send_reply: "
                    "Expected reply service context to be: %C\n",
                    reply_msg));
    }

  // Check that the request service context hasn't been changed.
  IOP::ServiceContext_var sc2 =
    ri->get_request_service_context (::service_id);

  if (!context_matches (sc2->context_data, request_msg))
    {
        ACE_ERROR ((LM_ERROR,
                    "ERROR: Echo_Server_Request_Interceptor::send_reply: "
                    "Expected request service context to be: %C.\n",
                    request_msg));
    }
}

void
Echo_Server_Request_Interceptor::send_exception (
    PortableInterceptor::ServerRequestInfo_ptr ri)
{
  CORBA::String_var operation = ri->operation ();

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t)%C.send_exception from \"%C\"\n",
              this->myname_,
              operation.in ()));

  // Check that the reply service context is set as expected.
  IOP::ServiceContext_var sc =
    ri->get_reply_service_context (::service_id);

  if (!context_matches (sc->context_data, reply_msg))
    {
        ACE_ERROR ((LM_ERROR,
                    "ERROR: Echo_Server_Request_Interceptor::send_exception: "
                    "Expected reply service context to be: %C\n",
                    reply_msg));
    }

  // Check that the request service context hasn't been changed.
  IOP::ServiceContext_var sc2 =
    ri->get_request_service_context (::service_id);

  if (!context_matches (sc2->context_data, request_msg))
    {
        ACE_ERROR ((LM_ERROR,
                    "ERROR: Echo_Server_Request_Interceptor::send_exception: "
                    "Expected request service context to be: %C.\n",
                    request_msg));
    }
}

void
Echo_Server_Request_Interceptor::send_other (
             PortableInterceptor::ServerRequestInfo_ptr)
{
  // Do Nothing
}
