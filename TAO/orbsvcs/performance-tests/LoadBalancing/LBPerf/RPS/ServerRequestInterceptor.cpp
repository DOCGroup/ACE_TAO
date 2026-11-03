#include "ServerRequestInterceptor.h"

ServerRequestInterceptor::ServerRequestInterceptor () = default;

char *
ServerRequestInterceptor::name ()
{
  return CORBA::string_dup ("ServerRequestInterceptor");
}

void
ServerRequestInterceptor::destroy ()
{
}

void
ServerRequestInterceptor::receive_request_service_contexts (
    PortableInterceptor::ServerRequestInfo_ptr /* ri */)
{
  this->request_count_.fetch_add (1, std::memory_order_relaxed);
}

void
ServerRequestInterceptor::receive_request (
    PortableInterceptor::ServerRequestInfo_ptr)
{
}

void
ServerRequestInterceptor::send_reply (
    PortableInterceptor::ServerRequestInfo_ptr /* ri */)
{
}

void
ServerRequestInterceptor::send_exception (
    PortableInterceptor::ServerRequestInfo_ptr)
{
}

void
ServerRequestInterceptor::send_other (
    PortableInterceptor::ServerRequestInfo_ptr)
{
}

CORBA::Long
ServerRequestInterceptor::request_count ()
{
  CORBA::Long const r = static_cast<CORBA::Long> (
    this->request_count_.exchange (0, std::memory_order_relaxed));

  return r;
}
