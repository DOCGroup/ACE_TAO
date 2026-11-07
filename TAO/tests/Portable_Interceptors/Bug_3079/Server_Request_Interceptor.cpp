// -*- C++ -*-
#include "Server_Request_Interceptor.h"
#include "tao/ORB_Constants.h"
#include "ace/Log_Msg.h"
#include "CrashPoint.h"

char *
Server_Request_Interceptor::name ()
{
  return CORBA::string_dup ("Server_Request_Interceptor");
}

void
Server_Request_Interceptor::destroy ()
{
}

void
Server_Request_Interceptor::receive_request_service_contexts (
    PortableInterceptor::ServerRequestInfo_ptr)
{
  this->request_count_++;
}

void
Server_Request_Interceptor::receive_request (
    PortableInterceptor::ServerRequestInfo_ptr)
{
}

void
Server_Request_Interceptor::send_reply (
    PortableInterceptor::ServerRequestInfo_ptr)
{
  if (crash_point == 1 && this->request_count_ == 2) ACE_OS::exit (1);
}

void
Server_Request_Interceptor::send_exception (
    PortableInterceptor::ServerRequestInfo_ptr)
{
}

void
Server_Request_Interceptor::send_other (
    PortableInterceptor::ServerRequestInfo_ptr)
{
}
