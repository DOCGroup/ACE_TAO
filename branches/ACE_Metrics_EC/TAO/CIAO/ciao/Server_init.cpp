// $Id$

#include "Server_init.h"
#include "CIAO_common.h"
#include "CCM_ComponentC.h"
#include "Cookies.h"

int
CIAO::Server_init (CORBA::ORB_ptr o)
{
  CIAO_REGISTER_VALUE_FACTORY (o, CIAO::Map_Key_Cookie_init,
                               Components::Cookie);
  CIAO_REGISTER_VALUE_FACTORY (o, CIAO::Map_Key_Cookie_init,
                               CIAO::Cookie);
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
