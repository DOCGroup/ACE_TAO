// $Id$

#include "Server_Request_Interceptor.h"
#include "ace/Log_Msg.h"

Server_Request_Interceptor::Server_Request_Interceptor (bool & destroy_called)
  : destroy_called_ (destroy_called)
{
  this->destroy_called_ = false;  // Sanity check.
}

Server_Request_Interceptor::~Server_Request_Interceptor (void)
{
  // ORB::destroy() should have been called, which in turn should have
  // called the Interceptor::destroy() method in all registered
  // interceptors.
  ACE_ASSERT (this->destroy_called_ == true);
}

char *
Server_Request_Interceptor::name (void)
{
  return CORBA::string_dup ("Server_Request_Interceptor");
}

void
Server_Request_Interceptor::destroy (void)
{
  this->destroy_called_ = true;
}

void
Server_Request_Interceptor::receive_request_service_contexts (
    PortableInterceptor::ServerRequestInfo_ptr)
{
  // Interceptor::destroy() should not have been called yet.
  ACE_ASSERT (this->destroy_called_ == false);
}

void
Server_Request_Interceptor::receive_request (
    PortableInterceptor::ServerRequestInfo_ptr)

{
  // Interceptor::destroy() should not have been called yet.
  ACE_ASSERT (this->destroy_called_ == false);
}

void
Server_Request_Interceptor::send_reply (
    PortableInterceptor::ServerRequestInfo_ptr)
{
  // Interceptor::destroy() should not have been called yet.
  ACE_ASSERT (this->destroy_called_ == false);
}

void
Server_Request_Interceptor::send_exception (
    PortableInterceptor::ServerRequestInfo_ptr)
{
  // Interceptor::destroy() should not have been called yet.
  ACE_ASSERT (this->destroy_called_ == false);
}

void
Server_Request_Interceptor::send_other (
    PortableInterceptor::ServerRequestInfo_ptr)
{
  // Interceptor::destroy() should not have been called yet.
  ACE_ASSERT (this->destroy_called_ == false);
}
