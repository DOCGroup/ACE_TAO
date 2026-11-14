#include "Server_Request_Interceptor.h"

#include <cstring>

CORBA::Boolean receive_request_service_contexts_called = false;
CORBA::Boolean receive_request_called = false;
CORBA::Boolean ending_interception_point_called = false;

Server_Request_Interceptor::Server_Request_Interceptor () = default;

char *
Server_Request_Interceptor::name ()
{
  return CORBA::string_dup ("");  // Anonymous
}

void
Server_Request_Interceptor::destroy ()
{
}

void
Server_Request_Interceptor::receive_request_service_contexts (
    PortableInterceptor::ServerRequestInfo_ptr ri)
{
  CORBA::String_var op = ri->operation ();

  if (std::strcmp (op.in (), "shutdown") != 0)
    {
      if (::receive_request_service_contexts_called)
        {
          throw CORBA::INTERNAL ();
        }

      ::receive_request_service_contexts_called = true;
    }
}

void
Server_Request_Interceptor::receive_request (
    PortableInterceptor::ServerRequestInfo_ptr ri)
{
  CORBA::String_var op = ri->operation ();

  if (std::strcmp (op.in (), "shutdown") != 0)
    {
      if (::receive_request_called)
        {
          throw CORBA::INTERNAL ();
        }

      ::receive_request_called = true;
    }
}

void
Server_Request_Interceptor::send_reply (
    PortableInterceptor::ServerRequestInfo_ptr ri)
{
  CORBA::String_var op = ri->operation ();

  if (std::strcmp (op.in (), "shutdown") != 0)
    {
      if (::ending_interception_point_called)
        {
          throw CORBA::INTERNAL ();
        }

      ::ending_interception_point_called = true;
    }
}

void
Server_Request_Interceptor::send_exception (
    PortableInterceptor::ServerRequestInfo_ptr ri)
{
  CORBA::String_var op = ri->operation ();

  if (std::strcmp (op.in (), "shutdown") != 0)
    {
      if (::ending_interception_point_called)
        {
          throw CORBA::INTERNAL ();
        }

      ::ending_interception_point_called = true;
    }
}

void
Server_Request_Interceptor::send_other (
    PortableInterceptor::ServerRequestInfo_ptr ri)
{
  CORBA::String_var op = ri->operation ();

  if (std::strcmp (op.in (), "shutdown") != 0)
    {
      if (::ending_interception_point_called)
        {
          throw CORBA::INTERNAL ();
        }

      ::ending_interception_point_called = true;
    }
}
