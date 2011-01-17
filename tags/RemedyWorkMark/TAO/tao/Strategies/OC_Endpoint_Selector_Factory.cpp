// $Id$

#include "tao/Strategies/OC_Endpoint_Selector_Factory.h"
#include "tao/Strategies/Optimized_Connection_Endpoint_Selector.h"
#include "tao/Strategies/Strategies_ORBInitializer.h"
#include "tao/ORBInitializer_Registry.h"
#include "tao/PI/ORBInitInfo.h"
#include "ace/Log_Msg.h"
#include "tao/ORB_Core.h"
#include "ace/OS_NS_strings.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_OC_Endpoint_Selector_Factory::TAO_OC_Endpoint_Selector_Factory (void)
  : oc_endpoint_selector_(0)
{
}

TAO_OC_Endpoint_Selector_Factory::~TAO_OC_Endpoint_Selector_Factory (void)
{
  delete this->oc_endpoint_selector_;
}

int
TAO_OC_Endpoint_Selector_Factory::init (int argc, ACE_TCHAR *argv[])
{
  ACE_Time_Value connect_timeout(0,0);

  for (int count = 0; count < argc; count++)
    {
      if ((ACE_OS::strcasecmp (argv[count],
                               ACE_TEXT ("-connect_timeout")) == 0) &&
          count < argc-1)
        {
          ++count;
          long const ms = ACE_OS::strtol(argv[count],0,10);
          connect_timeout.msec (ms);
        }
    }

  if (this->register_orb_initializer () == -1)
    return -1;

  ACE_NEW_RETURN (this->oc_endpoint_selector_,
                  TAO_Optimized_Connection_Endpoint_Selector(connect_timeout),
                  -1);
  return 0;
}

int
TAO_OC_Endpoint_Selector_Factory::register_orb_initializer (void)
{
  // Register the ORB initializer.
  try
    {
      PortableInterceptor::ORBInitializer_ptr temp_orb_initializer =
        PortableInterceptor::ORBInitializer::_nil ();

      /// Register the RTCORBA ORBInitializer.
      ACE_NEW_THROW_EX (temp_orb_initializer,
                        TAO_Strategies_ORBInitializer,
                        CORBA::NO_MEMORY (
                          CORBA::SystemException::_tao_minor_code (
                            TAO::VMCID,
                            ENOMEM),
                          CORBA::COMPLETED_NO));

      PortableInterceptor::ORBInitializer_var orb_initializer;
      orb_initializer = temp_orb_initializer;

      PortableInterceptor::register_orb_initializer (orb_initializer.in ());
    }
  catch (const ::CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "Unexpected exception caught while "
        "initializing the Strategies library");
      return -1;
    }

  return 0;
}

TAO_Invocation_Endpoint_Selector *
TAO_OC_Endpoint_Selector_Factory::get_selector (void)
{
  // Trivial endpoint selector.  Just return the default selector.
  return this->oc_endpoint_selector_;
}


// ****************************************************************

ACE_STATIC_SVC_DEFINE (TAO_OC_Endpoint_Selector_Factory,
                       ACE_TEXT ("OC_Endpoint_Selector_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_OC_Endpoint_Selector_Factory),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (TAO_Strategies, TAO_OC_Endpoint_Selector_Factory)

TAO_END_VERSIONED_NAMESPACE_DECL
