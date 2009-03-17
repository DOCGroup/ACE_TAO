// -*- C++ -*-

#include "Server_ORBInitializer.h"
#include "tao/ORB_Constants.h"

ACE_RCSID (Collocated_Forwarding,
           Server_ORBInitializer,
           "$Id$")

#if TAO_HAS_INTERCEPTORS == 1

#include "Server_Request_Interceptor.h"

Server_ORBInitializer::Server_ORBInitializer (CORBA::ULong request_pass_count)
  : request_pass_count_ (request_pass_count)
  , server_interceptor_ ()
{
}

void
Server_ORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr)
{
}

void
Server_ORBInitializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr info)
{
  PortableInterceptor::ServerRequestInterceptor_ptr interceptor;
  // Install the server request interceptor.
  ACE_NEW_THROW_EX (interceptor,
                    Server_Request_Interceptor (this->request_pass_count_),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO::VMCID,
                        ENOMEM),
                      CORBA::COMPLETED_NO));

  this->server_interceptor_ = interceptor;

  info->add_server_request_interceptor (interceptor);
}

PortableInterceptor::ServerRequestInterceptor_ptr
Server_ORBInitializer::server_interceptor (void)
{
  return
    PortableInterceptor::ServerRequestInterceptor::_duplicate (
      this->server_interceptor_.in ());
}

#endif  /* TAO_HAS_INTERCEPTORS == 1 */
