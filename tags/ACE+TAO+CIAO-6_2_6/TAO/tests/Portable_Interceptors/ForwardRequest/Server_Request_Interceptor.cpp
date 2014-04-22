// -*- C++ -*-
// $Id$

#include "tao/AnyTypeCode/TypeCode.h"
#include "Server_Request_Interceptor.h"
#include "tao/ORB_Constants.h"
#include "ace/Log_Msg.h"

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
  CORBA::Object_ptr obj2)
{
  if (CORBA::is_nil (obj1) || CORBA::is_nil (obj2))
    throw CORBA::INV_OBJREF (
      CORBA::SystemException::_tao_minor_code (
        TAO::VMCID,
        EINVAL),
      CORBA::COMPLETED_NO);

  this->obj_[0] = CORBA::Object::_duplicate (obj1);
  this->obj_[1] = CORBA::Object::_duplicate (obj2);
}

char *
Server_Request_Interceptor::name (void)
{
  return CORBA::string_dup ("Server_Request_Interceptor");
}

void
Server_Request_Interceptor::destroy (void)
{
  CORBA::release (this->obj_[0]);
  CORBA::release (this->obj_[1]);
}

void
Server_Request_Interceptor::receive_request_service_contexts (
    PortableInterceptor::ServerRequestInfo_ptr ri)
{
  this->request_count_++;

  CORBA::Boolean response_expected =
    ri->response_expected ();

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

      throw PortableInterceptor::ForwardRequest (this->obj_[0]);
    }
}

void
Server_Request_Interceptor::receive_request (
    PortableInterceptor::ServerRequestInfo_ptr ri)
{
  CORBA::Boolean response_expected =
    ri->response_expected ();

  if (!response_expected)   // A one-way request.
    return;

  // Request 1 -- non-forwarded
  // Request 2 -- forwarded by client request interceptor
  // Request 3 -- forwarded by receive_request_service_contexts()
  // Request 4 -- non-forwarded (give client chance to print result)
  // Request 5 -- forwarded by this interception point
  // Request 6 -- non-forwarded (request 5 gets forwarded here)
  // Request 7 -- throw exception to initiate forwarding from sent_exception

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

      throw PortableInterceptor::ForwardRequest (this->obj_[1]);
    }

  if (this->request_count_ == 7)
    {
      // Throw an exception to force the invocation of send_exception.
      ACE_DEBUG ((LM_DEBUG,
                  "SERVER (%P|%t) OBJ_NOT_EXIST exception thrown for request %d\n"
                  "SERVER (%P|%t) via receive_request().\n",
                  this->request_count_ - 2));

      throw CORBA::OBJECT_NOT_EXIST (
        CORBA::SystemException::_tao_minor_code (
          TAO::VMCID,
          EINVAL),
        CORBA::COMPLETED_NO);
    }
}

void
Server_Request_Interceptor::send_reply (
    PortableInterceptor::ServerRequestInfo_ptr)
{
}

void
Server_Request_Interceptor::send_exception (
    PortableInterceptor::ServerRequestInfo_ptr ri)
{
  CORBA::Any_var exception = ri->sending_exception ();
  CORBA::TypeCode_var tc = exception->type ();
  const char *id = tc->id ();

  CORBA::OBJECT_NOT_EXIST nonexist_exception;

  if (ACE_OS::strcmp (id, nonexist_exception._rep_id ()) == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "SERVER (%P|%t) OBJ_NOT_EXIST exception caught for request %d\n"
                  "SERVER (%P|%t) will be forwarded to object 1\n"
                  "SERVER (%P|%t) via send_exception().\n",
                  this->request_count_ - 2));

      throw PortableInterceptor::ForwardRequest (this->obj_[0]);

    }

}

void
Server_Request_Interceptor::send_other (
    PortableInterceptor::ServerRequestInfo_ptr ri)
{
  CORBA::Boolean response_expected =
    ri->response_expected ();

  if (!response_expected)   // A one-way request.
    return;

  // If we get this far then we should have received a
  // LOCATION_FORWARD reply.

  // This will throw an exception if a location forward has not
  // occured.  If an exception is thrown then something is wrong with
  // the PortableInterceptor::ForwardRequest support.
  CORBA::Object_var forward = ri->forward_reference ();

  if (CORBA::is_nil (forward.in ()))
    throw CORBA::INTERNAL ();
}
