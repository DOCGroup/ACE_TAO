// -*- C++ -*-

#include "Client_Request_Interceptor.h"
#include "testC.h"

ACE_RCSID (ForwardRequest,
           Client_Request_Interceptor,
           "$Id$")

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
Client_Request_Interceptor::name (
    ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup ("Client_Request_Interceptor");
}

void
Client_Request_Interceptor::destroy (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
Client_Request_Interceptor::send_request (
      PortableInterceptor::ClientRequestInfo_ptr ri
      ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
  ++this->request_count_;

  CORBA::Boolean response_expected =
    ri->response_expected (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (!response_expected)   // A one-way request.
    return;

  // Request 1 -- non-forwarded
  // Request 2 -- forwarded by this interception point.

  if (this->request_count_ == 2)
    {
      if (CORBA::is_nil (this->orb_.in ()))
        {
          int argc = 0;
          this->orb_ = CORBA::ORB_init (argc,
                                        0,
                                        this->orb_id_.in ()
                                        ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;
        }

      CORBA::Object_var forward =
        this->orb_->string_to_object (this->forward_str_.in ()
                                      ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      CORBA::String_var forward_str =
        this->orb_->object_to_string (forward.in ()
                                      ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      ACE_DEBUG ((LM_DEBUG,
                  "CLIENT (%P|%t) Request %d will be forwarded "
                  "to object 2\n"
                  "CLIENT (%P|%t) via send_request().\n",
                  this->request_count_));

      // Notice that this is not a permanent forward.
      ACE_THROW (PortableInterceptor::ForwardRequest (forward.in (),
                                                      0));
    }
}

void
Client_Request_Interceptor::send_poll (
    PortableInterceptor::ClientRequestInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
Client_Request_Interceptor::receive_reply (
    PortableInterceptor::ClientRequestInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
Client_Request_Interceptor::receive_exception (
    PortableInterceptor::ClientRequestInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
}

void
Client_Request_Interceptor::receive_other (
    PortableInterceptor::ClientRequestInfo_ptr ri
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{

  CORBA::Boolean response_expected =
    ri->response_expected (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  PortableInterceptor::ReplyStatus reply_status =
    ri->reply_status (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (!response_expected   // A one-way or asynchronous request.
      || reply_status == PortableInterceptor::TRANSPORT_RETRY)  // A retry.
    return;

  // If we get this far then we should have received a
  // LOCATION_FORWARD reply, either from another client request
  // interceptor (not this one) or from the server.

  // This will throw an exception if a location forward has not
  // occured.  If an exception is thrown then something is wrong with
  // the PortableInterceptor::ForwardRequest support.
  CORBA::Object_var forward = ri->forward_reference (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (CORBA::is_nil (forward.in ()))
    ACE_THROW (CORBA::INTERNAL ());

  ACE_DEBUG ((LM_DEBUG,
              "CLIENT (%P|%t) Received LOCATION_FORWARD reply.\n"));
}
