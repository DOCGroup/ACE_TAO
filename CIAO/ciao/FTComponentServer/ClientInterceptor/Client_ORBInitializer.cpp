// -*- C++ -*-

#include "Client_ORBInitializer.h"

ACE_RCSID (Hello,
           Client_ORBInitializer,
           "$Id$")

#if TAO_HAS_INTERCEPTORS == 1

#include "Client_Request_Interceptor.h"

#include "tao/StringSeqC.h"
#include "tao/ORB_Constants.h"
#include "ace/OS_NS_string.h"

Client_ORBInitializer::Client_ORBInitializer (Agent_i *agent)
  : agent_ (agent)
{
}

Client_ORBInitializer::~Client_ORBInitializer (void)
{
}


void
Client_ORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr)
{
}

void
Client_ORBInitializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr info)
{
  CORBA::String_var orb_id = info->orb_id ();

  PortableInterceptor::ClientRequestInterceptor_ptr interceptor =
    PortableInterceptor::ClientRequestInterceptor::_nil ();

  // Install the client request interceptor.
  ACE_NEW_THROW_EX (interceptor,
                    Client_Request_Interceptor (orb_id.in (), agent_),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO::VMCID,
                        ENOMEM),
                      CORBA::COMPLETED_NO));

  PortableInterceptor::ClientRequestInterceptor_var
    client_interceptor = interceptor;

  info->add_client_request_interceptor (client_interceptor.in ());

}

#endif  /* TAO_HAS_INTERCEPTORS == 1 */
