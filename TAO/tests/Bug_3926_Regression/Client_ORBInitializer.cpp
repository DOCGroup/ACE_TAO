// -*- C++ -*-
#include "Client_ORBInitializer.h"

#if TAO_HAS_INTERCEPTORS == 1

#include "Client_Request_Interceptor.h"

#include <cstring>

void
Client_ORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr)
{
}

void
Client_ORBInitializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr info)
{
  CORBA::String_var const orb_id = info->orb_id ();

  CORBA::StringSeq_var const args = info->arguments ();

  CORBA::String_var forward_str;

  // Extract the last forward reference from the argument list.
  CORBA::ULong const args_len = args->length ();
  for (CORBA::ULong i = 0; i < args_len; ++i)
    if (std::strcmp ("-k", args[i]) == 0
        && i < (args_len - 1))
      forward_str = args[i + 1];

  PortableInterceptor::ClientRequestInterceptor_ptr interceptor {};

  // Install the client request interceptor.
  ACE_NEW_THROW_EX (interceptor,
                    Client_Request_Interceptor (orb_id.in (),
                                                forward_str.in ()),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO::VMCID,
                        ENOMEM),
                      CORBA::COMPLETED_NO));

  // The var takes ownership of the interceptor's initial reference.
  PortableInterceptor::ClientRequestInterceptor_var
    client_interceptor = interceptor;

  info->add_client_request_interceptor (client_interceptor.in ());
}

#endif  /* TAO_HAS_INTERCEPTORS == 1 */
