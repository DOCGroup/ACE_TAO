// -*- C++ -*-

#include "Server_Request_Interceptor.h"

ACE_RCSID (ForwardRequest,
           Server_Request_Interceptor,
           "$Id$")

Server_Request_Interceptor::Server_Request_Interceptor (void)
  : request_count_ (0)
{
  this->obj_[0] = CORBA::Object::_nil ();
  this->obj_[1] = CORBA::Object::_nil ();
}

Server_Request_Interceptor::~Server_Request_Interceptor (void)
{
}

void
Server_Request_Interceptor::forward_references (
  CORBA::Object_ptr obj1,
  CORBA::Object_ptr obj2
  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (CORBA::is_nil (obj1) || CORBA::is_nil (obj2))
    ACE_THROW (CORBA::INV_OBJREF (
                 CORBA::SystemException::_tao_minor_code (
                   TAO_DEFAULT_MINOR_CODE,
                   EINVAL),
                 CORBA::COMPLETED_NO));

  this->obj_[0] = CORBA::Object::_duplicate (obj1);
  this->obj_[1] = CORBA::Object::_duplicate (obj2);
}

char *
Server_Request_Interceptor::name (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup ("Server_Request_Interceptor");
}

void
Server_Request_Interceptor::destroy (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::release (this->obj_[0]);
  CORBA::release (this->obj_[1]);
}

void
Server_Request_Interceptor::receive_request_service_contexts (
    PortableInterceptor::ServerRequestInfo_ptr ri
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
  this->request_count_++;

  CORBA::Boolean response_expected =
    ri->response_expected (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (!response_expected)   // A one-way request.
    return;

  // Request 1 -- non-forwarded
  // Request 2 -- forwarded by client request interceptor
  // Request 3 -- forwarded by this interception point

  if (this->request_count_ == 3)
    {
      // The client request interceptor should have already forwarded
      // the request to obj_[1], so we re-forward the request back to
      // obj_[0].

      ACE_DEBUG ((LM_DEBUG,
                  "SERVER (%P|%t) Request %d will be forwarded "
                  "to object 1\n"        // "object 1" as in "obj_[0]"
                  "SERVER (%P|%t) via "
                  "receive_request_service_contexts().\n",
                  this->request_count_));

      // Notice that this is not a permanent forward.
      ACE_THROW (PortableInterceptor::ForwardRequest (
                   this->obj_[0],
                   0));
    }
}

void
Server_Request_Interceptor::receive_request (
    PortableInterceptor::ServerRequestInfo_ptr ri
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
  CORBA::Boolean response_expected =
    ri->response_expected (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (!response_expected)   // A one-way request.
    return;

  // Request 1 -- non-forwarded
  // Request 2 -- forwarded by client request interceptor
  // Request 3 -- forwarded by receive_request_service_contexts()
  // Request 4 -- non-forwarded (give client chance to print result)
  // Request 5 -- forwarded by this interception point

  if (this->request_count_ == 5)
    {
      // This interceptor should have already forwarded the request to
      // obj_[0] so re-forward it to obj_[1].  This will be the last
      // location forward.


      ACE_DEBUG ((LM_DEBUG,
                  "SERVER (%P|%t) Request %d will be forwarded "
                  "to object 2\n"  // "object 2" as in "obj_[1]"
                  "SERVER (%P|%t) via receive_request().\n",
                  this->request_count_ - 1));
      // "request_count_ - 1" is used above since there was a location
      // forward.

      // Notice that this is not a permanent forward.
      ACE_THROW (PortableInterceptor::ForwardRequest (
                   this->obj_[1],
                   0));
    }
}

void
Server_Request_Interceptor::send_reply (
    PortableInterceptor::ServerRequestInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
Server_Request_Interceptor::send_exception (
    PortableInterceptor::ServerRequestInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
}

void
Server_Request_Interceptor::send_other (
    PortableInterceptor::ServerRequestInfo_ptr ri
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
  CORBA::Boolean response_expected =
    ri->response_expected (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (!response_expected)   // A one-way request.
    return;

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
