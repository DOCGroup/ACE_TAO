#include "ServerRequestInterceptor.h"

ACE_RCSID (LoadBalancing,
           ServerRequestInterceptor,
           "$Id$")


ServerRequestInterceptor::ServerRequestInterceptor (void)
  : request_count_ (0)
{
}

ServerRequestInterceptor::~ServerRequestInterceptor (void)
{
}

char *
ServerRequestInterceptor::name (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup ("ServerRequestInterceptor");
}

void
ServerRequestInterceptor::destroy (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
ServerRequestInterceptor::receive_request_service_contexts (
    PortableInterceptor::ServerRequestInfo_ptr /* ri */)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
  ++this->request_count_;
}

void
ServerRequestInterceptor::receive_request (
    PortableInterceptor::ServerRequestInfo_ptr)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
}

void
ServerRequestInterceptor::send_reply (
    PortableInterceptor::ServerRequestInfo_ptr /* ri */)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
ServerRequestInterceptor::send_exception (
    PortableInterceptor::ServerRequestInfo_ptr)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
}

void
ServerRequestInterceptor::send_other (
    PortableInterceptor::ServerRequestInfo_ptr)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
}

CORBA::Long
ServerRequestInterceptor::request_count (void)
{
  const CORBA::Long r = this->request_count_.value ();
  this->request_count_ = 0;

  return r;
}
