// -*- C++ -*-
// $Id$

#include "tao/EndpointPolicy/EndpointPolicy.h"
#include "tao/EndpointPolicy/EndpointPolicy_ORBInitializer.h"
#include "tao/EndpointPolicy/Endpoint_Acceptor_Filter_Factory.h"

#include "tao/ORB_Core.h"
#include "tao/ORBInitializer_Registry.h"

#include "tao/PI/PI.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

int
TAO_EndpointPolicy_Initializer::static_init (void)
{
  ACE_Service_Config::process_directive  (ace_svc_desc_TAO_EndpointPolicy_Initializer);
  return 0;
}

int
TAO_EndpointPolicy_Initializer::init (int, ACE_TCHAR* [])
{
  PortableInterceptor::ORBInitializer_var orb_initializer;

  try
    {
      PortableInterceptor::ORBInitializer_ptr temp_orb_initializer =
        PortableInterceptor::ORBInitializer::_nil ();

      /// Register the EndpointPolicy ORBInitializer.
      ACE_NEW_THROW_EX (temp_orb_initializer,
                        TAO_EndpointPolicy_ORBInitializer,
                        CORBA::NO_MEMORY (
                          CORBA::SystemException::_tao_minor_code (
                            TAO::VMCID,
                            ENOMEM),
                          CORBA::COMPLETED_NO));

      orb_initializer = temp_orb_initializer;

      PortableInterceptor::register_orb_initializer (orb_initializer.in ());
    }
  catch (const ::CORBA::Exception& ex)
    {
      if (TAO_debug_level > 0)
        {
          ex._tao_print_exception ("Caught exception:");
        }
      return -1;
    }

  ACE_Service_Config::process_directive
    (ace_svc_desc_TAO_Endpoint_Acceptor_Filter_Factory,1); // force replacement

  return 0;
}


/////////////////////////////////////////////////////////////////////

ACE_FACTORY_DEFINE (TAO_EndpointPolicy, TAO_EndpointPolicy_Initializer)
ACE_STATIC_SVC_DEFINE (TAO_EndpointPolicy_Initializer,
                       ACE_TEXT ("EndpointPolicy_Initializer"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_EndpointPolicy_Initializer),
                       ACE_Service_Type::DELETE_THIS
                       | ACE_Service_Type::DELETE_OBJ,
                       0)
TAO_END_VERSIONED_NAMESPACE_DECL
