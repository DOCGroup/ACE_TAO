// -*- C++ -*-

#include "Client_ORBInitializer.h"

ACE_RCSID (ForwardRequest,
           Client_ORBInitializer,
           "$Id$")

#if TAO_HAS_INTERCEPTORS == 1

#include "Client_Request_Interceptor.h"

#include "tao/StringSeqC.h"

void
Client_ORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr
    TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
Client_ORBInitializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr info
    TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_ENV_ARG_DEFN;

  CORBA::String_var orb_id = info->orb_id (ACE_TRY_ENV);
  ACE_CHECK;

  CORBA::StringSeq_var args = info->arguments (ACE_TRY_ENV);
  ACE_CHECK;

  CORBA::String_var forward_str;

  // Extract the last forward reference from the argument list.
  CORBA::ULong args_len = args->length ();
  for (CORBA::ULong i = 0; i < args_len; ++i)
    if (ACE_OS_String::strcmp ("-k", args[i].in ()) == 0
        && i < (args_len - 1))
      forward_str = args[i + 1];

  PortableInterceptor::ClientRequestInterceptor_ptr interceptor =
    PortableInterceptor::ClientRequestInterceptor::_nil ();

  // Install the client request interceptor.
  ACE_NEW_THROW_EX (interceptor,
                    Client_Request_Interceptor (orb_id.in (),
                                                forward_str.in ()),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK;

  PortableInterceptor::ClientRequestInterceptor_var
    client_interceptor = interceptor;

  info->add_client_request_interceptor (client_interceptor.in ()
                                        TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

}

#endif  /* TAO_HAS_INTERCEPTORS == 1 */
