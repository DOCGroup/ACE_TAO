// -*- C++ -*-
#include "Client_Request_Interceptor.h"
#include "testC.h"
#include "ace/Log_Msg.h"

Client_Request_Interceptor::Client_Request_Interceptor ()
  : request_count_ (0),
    receive_exception_count_ (0)
{
}

CORBA::ULong
Client_Request_Interceptor::receive_exception_count () const
{
  return this->receive_exception_count_;
}

CORBA::ULong
Client_Request_Interceptor::request_count () const
{
  return this->request_count_;
}

char *
Client_Request_Interceptor::name ()
{
  return CORBA::string_dup ("Client_Request_Interceptor");
}

void
Client_Request_Interceptor::destroy ()
{
}

void
Client_Request_Interceptor::send_request (
      PortableInterceptor::ClientRequestInfo_ptr)
{
  ++this->request_count_;
}

void
Client_Request_Interceptor::send_poll (
    PortableInterceptor::ClientRequestInfo_ptr)
{
}

void
Client_Request_Interceptor::receive_reply (
    PortableInterceptor::ClientRequestInfo_ptr)
{
}

void
Client_Request_Interceptor::receive_exception (
    PortableInterceptor::ClientRequestInfo_ptr)
{
  ++this->receive_exception_count_;
}

void
Client_Request_Interceptor::receive_other (
    PortableInterceptor::ClientRequestInfo_ptr)
{
}
