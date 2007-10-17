// $Id$

#include "Client_init.h"
#include "CCM_ComponentC.h"
#include "CCM_StandardConfiguratorC.h"
#include "CIAO_common.h"

#include "ace/Env_Value_T.h"

int
CIAO::Client_init (CORBA::ORB_ptr o)
{
  CIAO_REGISTER_VALUE_FACTORY (o, Components::Cookie_init,
                               Components::Cookie);
  CIAO_REGISTER_VALUE_FACTORY (o, Components::PortDescription_init,
                               Components::PortDescription);
  CIAO_REGISTER_VALUE_FACTORY (o, Components::FacetDescription_init,
                               Components::FacetDescription);
  CIAO_REGISTER_VALUE_FACTORY (o, Components::ConnectionDescription_init,
                               Components::ConnectionDescription);
  CIAO_REGISTER_VALUE_FACTORY (o, Components::ReceptacleDescription_init,
                               Components::ReceptacleDescription);
  CIAO_REGISTER_VALUE_FACTORY (o, Components::ConsumerDescription_init,
                               Components::ConsumerDescription);
  CIAO_REGISTER_VALUE_FACTORY (o, Components::EmitterDescription_init,
                               Components::EmitterDescription);
  CIAO_REGISTER_VALUE_FACTORY (o, Components::SubscriberDescription_init,
                               Components::SubscriberDescription);
  CIAO_REGISTER_VALUE_FACTORY (o, Components::PublisherDescription_init,
                               Components::PublisherDescription);
  CIAO_REGISTER_VALUE_FACTORY (o, Components::ConfigValue_init,
                               Components::ConfigValue);
  CIAO_REGISTER_VALUE_FACTORY (o, Components::ComponentPortDescription_init,
                               Components::ComponentPortDescription);
  return 0;
}

/// This should really be an anonymous namespace, but some compilers
/// still don't support this features.  Therefore, just use a long
/// namespace name here.
namespace ciao_anonymous_namespace
{
  int debug_level = -1;
}

int
CIAO::debug_level (void)
{
  if (ciao_anonymous_namespace::debug_level == -1)
    {
      // Initialize the thing.
      ACE_Env_Value<int> envar ("CIAO_DEBUG_LEVEL", 1);
      ciao_anonymous_namespace::debug_level = envar;
    }

  return ciao_anonymous_namespace::debug_level;
}
