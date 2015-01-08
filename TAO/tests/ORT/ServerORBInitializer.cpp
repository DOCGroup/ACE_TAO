#include "ServerORBInitializer.h"
#include "ServerRequestInterceptor.h"
#include "ORT_test_IORInterceptor.h"

#include "tao/ORB_Constants.h"
#include "tao/PortableServer/PortableServer.h"

void
ServerORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr /* info */)
{
}

void
ServerORBInitializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr info)
{

  CORBA::Object_var obj =
    info->resolve_initial_references ("POACurrent");

  PortableServer::Current_var poa_current =
    PortableServer::Current::_narrow (obj.in ());

  ACE_ASSERT (!CORBA::is_nil (poa_current.in ()));


  CORBA::String_var orb_id = info->orb_id ();

  // Create and register the test's ServerRequestInterceptor

  PortableInterceptor::ServerRequestInterceptor_ptr tmp;
  ACE_NEW_THROW_EX (tmp,
                    ServerRequestInterceptor (orb_id.in (),
                                              poa_current.in ()),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO::VMCID,
                        ENOMEM),
                      CORBA::COMPLETED_NO));

  PortableInterceptor::ServerRequestInterceptor_var server_interceptor = tmp;

  info->add_server_request_interceptor (server_interceptor.in ());


  // Create and register the test's IORInterceptor

  PortableInterceptor::IORInterceptor_ptr ort_test_interceptor;
  ACE_NEW_THROW_EX (ort_test_interceptor,
                    ORT_test_IORInterceptor,
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO::VMCID,
                        ENOMEM),
                      CORBA::COMPLETED_NO));

  PortableInterceptor::IORInterceptor_var ior_interceptor =
    ort_test_interceptor;

  info->add_ior_interceptor (ior_interceptor.in ());
}
