#include "ServerRequestInterceptor.h"

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
  return this->request_count_.exchange (CORBA::Long {}, std::memory_order_relaxed);
}
