// -*- C++ -*-

#include "Client_Request_Interceptor.h"
#include "testC.h"

ACE_RCSID (Request_Interceptor_Flow,
           Client_Request_Interceptor,
           "$Id$")

Client_Request_Interceptor::Client_Request_Interceptor (void)
  : request_id_ (0)
{
}

char *
Client_Request_Interceptor::name (
    ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup ("Client_Request_Interceptor");
}

void
Client_Request_Interceptor::destroy (
  ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{

}

void
Client_Request_Interceptor::send_request (
    PortableInterceptor::ClientRequestInfo_ptr ri
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
  IOP::ServiceContext sc;
  sc.context_id = Replay_Reply::SPECIFIC_CONTEXT;

  Replay_Reply::tagged_component tc;

  if ((this->request_id_%4) == 0)
    {
      tc.id = this->request_id_;
      ++this->request_id_;
    }
  else
    tc.id = ++this->request_id_;

  TAO_OutputCDR ocdr;

  if (!(ocdr << ACE_OutputCDR::from_boolean (TAO_ENCAP_BYTE_ORDER)))
    ACE_THROW (CORBA::MARSHAL ());

  if (!(ocdr << tc))
    ACE_THROW (CORBA::MARSHAL ());

  CORBA::ULong length =
    ACE_static_cast (CORBA::ULong, ocdr.total_length ());
  sc.context_data.length (length);

  CORBA::Octet *buf = sc.context_data.get_buffer ();

  for (const ACE_Message_Block *i = ocdr.begin ();
       i != 0;
       i = i->cont ())
    {
      ACE_OS::memcpy (buf, i->rd_ptr (), i->length ());
      buf += i->length ();
    }

  // Add this context to the service context list.
  ri->add_request_service_context (sc,
                                   0
                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
Client_Request_Interceptor::send_poll (
    PortableInterceptor::ClientRequestInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
Client_Request_Interceptor::receive_reply (
    PortableInterceptor::ClientRequestInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{

}

void
Client_Request_Interceptor::receive_exception (
    PortableInterceptor::ClientRequestInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{

}

void
Client_Request_Interceptor::receive_other (
    PortableInterceptor::ClientRequestInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{

}
