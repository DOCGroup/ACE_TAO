// $Id$

#include "tao/Strategies/OC_Endpoint_Selector_Factory.h"
#include "tao/Strategies/Optimized_Connection_Endpoint_Selector.h"
#include "ace/Log_Msg.h"
#include "tao/ORB_Core.h"
#include "ace/OS.h"

ACE_RCSID (tao,
           OC_Endpoint_Selector_Factory,
           "$Id$")

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
  ACE_Time_Value timeout(0,0);

  TAO_ORB_Core::set_endpoint_selector_factory ("OC_Endpoint_Selector_Factory");
  for (int count = 0; count < argc; count++)
    {
      if ((ACE_OS::strcasecmp (argv[count],
                               ACE_TEXT ("-connect_timeout")) == 0) &&
          count < argc-1)
        {
          count++;
          long ms = ACE_OS::strtol(argv[count],0,10);
          timeout.msec (ms);
        }
    }

  ACE_NEW_RETURN (this->oc_endpoint_selector_,
                  TAO_Optimized_Connection_Endpoint_Selector(timeout),
                  -1);
  return 0;
}


TAO_Invocation_Endpoint_Selector *
TAO_OC_Endpoint_Selector_Factory::get_selector ( ACE_ENV_SINGLE_ARG_DECL_NOT_USED )
{
  // Trivial endpoint selector.  Just return the default selector.
  return this->oc_endpoint_selector_;
}

TAO_END_VERSIONED_NAMESPACE_DECL

// ****************************************************************

ACE_STATIC_SVC_DEFINE (TAO_OC_Endpoint_Selector_Factory,
                       ACE_TEXT ("OC_Endpoint_Selector_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_OC_Endpoint_Selector_Factory),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (TAO_Strategies, TAO_OC_Endpoint_Selector_Factory)
