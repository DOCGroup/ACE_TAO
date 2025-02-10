#include "ServerRequestInterceptor.h"

ServerRequestInterceptor::ServerRequestInterceptor ()
  : request_count_ (0)
{
}

ServerRequestInterceptor::~ServerRequestInterceptor ()
{
}

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
  ++this->request_count_;
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
  const CORBA::Long r = this->request_count_.value ();
  this->request_count_ = 0;

  return r;
}
