#include "ServerORBInitializer.h"
#include "ServerRequestInterceptor.h"
#include "ORT_test_IORInterceptor.h"


ACE_RCSID (ORT,
           ServerORBInitializer,
           "$Id$")


void
ServerORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr /* info */
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
ServerORBInitializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr info
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{

  CORBA::Object_var obj =
    info->resolve_initial_references ("POACurrent"
                                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  PortableServer::Current_var poa_current =
    PortableServer::Current::_narrow (obj.in ()
                                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_ASSERT (!CORBA::is_nil (poa_current.in ()));


  CORBA::String_var orb_id = info->orb_id (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // Create and register the test's ServerRequestInterceptor

  PortableInterceptor::ServerRequestInterceptor_ptr tmp;
  ACE_NEW_THROW_EX (tmp,
                    ServerRequestInterceptor (orb_id.in (),
                                              poa_current.in ()),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK;

  PortableInterceptor::ServerRequestInterceptor_var server_interceptor = tmp;

  info->add_server_request_interceptor (server_interceptor.in ()
                                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;


  // Create and register the test's IORInterceptor

  PortableInterceptor::IORInterceptor_ptr ort_test_interceptor;
  ACE_NEW_THROW_EX (ort_test_interceptor,
                    ORT_test_IORInterceptor,
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK;

  PortableInterceptor::IORInterceptor_var ior_interceptor =
    ort_test_interceptor;

  info->add_ior_interceptor (ior_interceptor.in ()
                             ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}
