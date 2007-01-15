// -*- C++ -*-
//
// $Id$
//

#include "Client_ORBInitializer.h"
#include "client_interceptor.h"

ACE_RCSID (Bug_2510_Regression, Client_ORBInitializer, "$Id$")

Client_ORBInitializer::Client_ORBInitializer (void)
{
}

void
Client_ORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
Client_ORBInitializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr info
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  PortableInterceptor::SlotId slot_id = info->allocate_slot_id ();

  PortableInterceptor::ClientRequestInterceptor_ptr interceptor =
    PortableInterceptor::ClientRequestInterceptor::_nil ();

  // Install the Echo client request interceptor
  ACE_NEW_THROW_EX (interceptor,
                    Echo_Client_Request_Interceptor (),
                    CORBA::NO_MEMORY ());
  ACE_CHECK;

  PortableInterceptor::ClientRequestInterceptor_var
    client_interceptor = interceptor;

  info->add_client_request_interceptor (client_interceptor.in ()
                                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_UNUSED_ARG (slot_id);
}

