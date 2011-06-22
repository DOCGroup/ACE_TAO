#include "ServerORBInitializer.h"
#include "IOR_Interceptor.h"
#include "tao/ORB_Constants.h"
#include "tao/PortableServer/PortableServer.h"

ACE_RCSID (Hello,
           ServerORBInitializer,
           "$Id$")

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

  // Create and register the test's IORInterceptor

  PortableInterceptor::IORInterceptor_ptr ior_intercept;
  ACE_NEW_THROW_EX (ior_intercept,
                    IOR_Interceptor,
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO::VMCID,
                        ENOMEM),
                      CORBA::COMPLETED_NO));

  PortableInterceptor::IORInterceptor_var ior_interceptor =
    ior_intercept;

  info->add_ior_interceptor (ior_interceptor.in ());
}
