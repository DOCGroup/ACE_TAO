// $Id$

#include "Server_Request_Interceptor.h"

CORBA::Boolean receive_request_service_contexts_called = 0;
CORBA::Boolean receive_request_called = 0;
CORBA::Boolean ending_interception_point_called = 0;


Server_Request_Interceptor::Server_Request_Interceptor (void)
{
}

Server_Request_Interceptor::~Server_Request_Interceptor (void)
{
}

char *
Server_Request_Interceptor::name (void)
{
  return CORBA::string_dup ("");  // Anonymous
}

void
Server_Request_Interceptor::destroy (void)
{
}

void
Server_Request_Interceptor::receive_request_service_contexts (
    PortableInterceptor::ServerRequestInfo_ptr ri)
{
  CORBA::String_var op = ri->operation ();

  if (ACE_OS::strcmp (op.in (), "shutdown") != 0)
    {
      if (::receive_request_service_contexts_called != 0)
        {
          throw CORBA::INTERNAL ();
        }

      ::receive_request_service_contexts_called = 1;
    }
}

void
Server_Request_Interceptor::receive_request (
    PortableInterceptor::ServerRequestInfo_ptr ri)
{
  CORBA::String_var op = ri->operation ();

  if (ACE_OS::strcmp (op.in (), "shutdown") != 0)
    {
      if (::receive_request_called != 0)
        {
          throw CORBA::INTERNAL ();
        }

      ::receive_request_called = 1;
    }
}

void
Server_Request_Interceptor::send_reply (
    PortableInterceptor::ServerRequestInfo_ptr ri)
{
  CORBA::String_var op = ri->operation ();

  if (ACE_OS::strcmp (op.in (), "shutdown") != 0)
    {
      if (::ending_interception_point_called != 0)
        {
          throw CORBA::INTERNAL ();
        }

      ::ending_interception_point_called = 1;
    }
}

void
Server_Request_Interceptor::send_exception (
    PortableInterceptor::ServerRequestInfo_ptr ri)
{
  CORBA::String_var op = ri->operation ();

  if (ACE_OS::strcmp (op.in (), "shutdown") != 0)
    {
      if (::ending_interception_point_called != 0)
        {
          throw CORBA::INTERNAL ();
        }

      ::ending_interception_point_called = 1;
    }
}

void
Server_Request_Interceptor::send_other (
    PortableInterceptor::ServerRequestInfo_ptr ri)
{
  CORBA::String_var op = ri->operation ();

  if (ACE_OS::strcmp (op.in (), "shutdown") != 0)
    {
      if (::ending_interception_point_called != 0)
        {
          throw CORBA::INTERNAL ();
        }

      ::ending_interception_point_called = 1;
    }
}
