// -*- C++ -*-
#include "ServerRequest_Interceptor2.h"
#include "orbsvcs/FT_CORBA_ORBC.h"
#include "tao/IOPC.h"
#include "tao/CDR.h"
#include "tao/Exception.h"
#include <cstring>

std::atomic<bool> ServerRequest_Interceptor2::has_ft_request_sc_ {};

char *
ServerRequest_Interceptor2::name ()
{
  return CORBA::string_dup ("ServerRequest_Interceptor2");
}

void
ServerRequest_Interceptor2::destroy ()
{
}

void
ServerRequest_Interceptor2::receive_request_service_contexts (
  PortableInterceptor::ServerRequestInfo_ptr)
{
}

void
ServerRequest_Interceptor2::receive_request (
  PortableInterceptor::ServerRequestInfo_ptr ri)
{
  CORBA::String_var op = ri->operation ();

  if (std::strcmp (op.in (), "has_ft_request_service_context") != 0)
    return;

  try
    {
      IOP::ServiceContext_var const sc =
        ri->get_request_service_context (IOP::FT_REQUEST);

      // No exception therefore there was a context
      has_ft_request_sc_ = true;
    }
  catch (CORBA::BAD_PARAM const &)
    {
      // No group version context
      has_ft_request_sc_ = false;
    }
}

void
ServerRequest_Interceptor2::send_reply (
  PortableInterceptor::ServerRequestInfo_ptr)
{
}

void
ServerRequest_Interceptor2::send_exception (
  PortableInterceptor::ServerRequestInfo_ptr)
{
}

void
ServerRequest_Interceptor2::send_other (
  PortableInterceptor::ServerRequestInfo_ptr)
{
}

