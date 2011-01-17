// -*- C++ -*-
// $Id$

#include "Client_Request_Interceptor.h"
#include "testC.h"

#include "ace/Log_Msg.h"

Client_Request_Interceptor::Client_Request_Interceptor (
  const char *orb_id,
  const char *forward_str)
  : orb_id_ (CORBA::string_dup (orb_id)),
    orb_ (),
    request_count_ (0),
    forward_str_ (CORBA::string_dup (forward_str))
{
}

char *
Client_Request_Interceptor::name ()
{
  return CORBA::string_dup ("Client_Request_Interceptor");
}

void
Client_Request_Interceptor::destroy (void)
{
}

void
Client_Request_Interceptor::send_request (
    PortableInterceptor::ClientRequestInfo_ptr ri)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("CRI: in send_request\n")));

  ++this->request_count_;

  CORBA::Boolean response_expected =
    ri->response_expected ();

  if (!response_expected)   // A one-way request.
    return;

  // Request 1 -- non-forwarded
  // Request 2 -- forwarded by this interception point.
  // Request 3 -- non-forwarded

  if (this->request_count_ == 2)
    {
      if (CORBA::is_nil (this->orb_.in ()))
        {
          int argc = 0;
          char **const argv= 0;
          this->orb_ = CORBA::ORB_init (argc,
                                        argv,
                                        this->orb_id_.in ());
        }

      CORBA::Object_var forward =
        this->orb_->string_to_object (this->forward_str_.in ());

      CORBA::String_var forward_str =
        this->orb_->object_to_string (forward.in ());

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("CRI: request %d will be forwarded via send_request()\n"),
                  this->request_count_));

      // Notice that this is not a permanent forward.
      throw PortableInterceptor::ForwardRequest (forward.in ());
    }
}

void
Client_Request_Interceptor::send_poll (
    PortableInterceptor::ClientRequestInfo_ptr)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("CRI: in send_poll\n")));
}

void
Client_Request_Interceptor::receive_reply (
    PortableInterceptor::ClientRequestInfo_ptr)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("CRI: in receive_reply\n")));
}

void
Client_Request_Interceptor::receive_exception (
    PortableInterceptor::ClientRequestInfo_ptr)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("CRI: in receive_exception\n")));
}

void
Client_Request_Interceptor::receive_other (
    PortableInterceptor::ClientRequestInfo_ptr ri)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("CRI: in receive_other\n")));

  CORBA::Boolean response_expected =
    ri->response_expected ();

  // Oneway
  if (!response_expected)
    return;

  PortableInterceptor::ReplyStatus reply_status;

  try
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("CRI: in receive_other, before reply_status\n")));

      reply_status = ri->reply_status ();
    }
  catch(CORBA::BAD_INV_ORDER const &e)
    {
      e._tao_print_exception ("CRI: exception");
    }

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("CRI: in receive_other, after reply_status\n")));
}
