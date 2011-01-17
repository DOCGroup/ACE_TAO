// -*- C++ -*-
// $Id$

#include "Client_ORBInitializer.h"

#if TAO_HAS_INTERCEPTORS == 1

#include "Client_Request_Interceptor.h"
#include "tao/StringSeqC.h"
#include "tao/ORB_Constants.h"
#include "ace/OS_NS_string.h"

void
Client_ORBInitializer::pre_init (PortableInterceptor::ORBInitInfo_ptr)
{
}

void
Client_ORBInitializer::post_init (PortableInterceptor::ORBInitInfo_ptr info)
{
  // Install the client request interceptor.
  ACE_NEW_THROW_EX (client_interceptor_,
                    Client_Request_Interceptor,
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO::VMCID,
                        ENOMEM),
                      CORBA::COMPLETED_NO));

  PortableInterceptor::ClientRequestInterceptor_var
    client_interceptor = client_interceptor_;

  info->add_client_request_interceptor (client_interceptor.in ());
}

#endif  /* TAO_HAS_INTERCEPTORS == 1 */
