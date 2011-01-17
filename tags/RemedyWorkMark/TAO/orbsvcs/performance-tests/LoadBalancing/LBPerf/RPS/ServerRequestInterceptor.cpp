// $Id$

#include "ServerRequestInterceptor.h"

ServerRequestInterceptor::ServerRequestInterceptor (void)
  : request_count_ (0)
{
}

ServerRequestInterceptor::~ServerRequestInterceptor (void)
{
}

char *
ServerRequestInterceptor::name (void)
{
  return CORBA::string_dup ("ServerRequestInterceptor");
}

void
ServerRequestInterceptor::destroy (void)
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
ServerRequestInterceptor::request_count (void)
{
  const CORBA::Long r = this->request_count_.value ();
  this->request_count_ = 0;

  return r;
}
