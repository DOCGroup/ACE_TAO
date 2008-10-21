#include "ClientInterceptor.h"
#include <tao/OctetSeqC.h>
#include <tao/PI/ClientRequestInfo.h>
#include <ace/OS_NS_string.h>
#include <iostream>

const CORBA::ULong service_ctx_id = 0xdeed;

ClientInterceptor::
ClientInterceptor (void)
  : myname_ ("Client_Authentication_Interceptor")
{
  std::cout << "Calling ClientInterceptor constructor." << std::endl;
}

ClientInterceptor::~ClientInterceptor (void)
{
}

char *
ClientInterceptor::name ()
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  std::cout << "Calling ClientInterceptor name() method" << std::endl;
  return CORBA::string_dup (this->myname_);
}

void
ClientInterceptor::destroy ()
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
ClientInterceptor::send_poll (
                              PortableInterceptor::ClientRequestInfo_ptr)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  std::cout << "Calling send_poll()." << std::endl;
}


void
ClientInterceptor::send_request (
                                 PortableInterceptor::ClientRequestInfo_ptr ri)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
  std::cout << "Calling send_request()." << std::endl;

  IOP::ServiceContext sc;
  sc.context_id = service_ctx_id;

  const char user_name[] = "Ron Klein";
  std::cout << "User's Name: " << user_name << std::endl;
  CORBA::ULong string_len = sizeof (user_name) + 1;
  CORBA::Octet *buf = 0;

  buf = new CORBA::Octet [string_len];

  ACE_OS::strcpy (reinterpret_cast<char*> (buf), user_name);

  sc.context_data.replace (string_len, string_len, buf, 1);

  // Add this context to the service context list.
  ri->add_request_service_context (sc, 0);

}

void
ClientInterceptor::receive_reply (
                                  PortableInterceptor::ClientRequestInfo_ptr ri)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_UNUSED_ARG(ri);
  std::cout << "Calling receive_reply()." << std::endl;
}

void
ClientInterceptor::receive_other (
                                  PortableInterceptor::ClientRequestInfo_ptr)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
  std::cout << "Calling receive_other()." << std::endl;
}

void
ClientInterceptor::receive_exception (
                                      PortableInterceptor::ClientRequestInfo_ptr ri)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
  ACE_UNUSED_ARG(ri);
  std::cout << "Calling receive_exception()." << std::endl;
}
