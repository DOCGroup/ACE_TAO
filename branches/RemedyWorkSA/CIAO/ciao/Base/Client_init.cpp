// $Id$

#include "Client_init.h"
#include <ccm/CCM_CookieC.h>

#if !defined (CCM_LW)
#include <ccm/CCM_ReceptacleC.h>
#include <ccm/CCM_EventsC.h>
#include <ccm/CCM_ObjectC.h>
#endif

#include "ciao/Valuetype_Factories/ConfigValue.h"

namespace CIAO
{
  int
  Client_init (CORBA::ORB_ptr o)
  {
    CIAO_REGISTER_VALUE_FACTORY (o, Components::Cookie_init,
                                 Components::Cookie);
#if !defined (CCM_LW)
    CIAO_REGISTER_VALUE_FACTORY (o, Components::PortDescription_init,
                                 Components::PortDescription);
    CIAO_REGISTER_VALUE_FACTORY (o, Components::FacetDescription_init,
                                 Components::FacetDescription);
    CIAO_REGISTER_VALUE_FACTORY (o, Components::ConnectionDescription_init,
                                 Components::ConnectionDescription);
    CIAO_REGISTER_VALUE_FACTORY (o, Components::ReceptacleDescription_init,
                                 Components::ReceptacleDescription);
#if !defined (CCM_NOEVENT)
    CIAO_REGISTER_VALUE_FACTORY (o, Components::ConsumerDescription_init,
                                 Components::ConsumerDescription);
    CIAO_REGISTER_VALUE_FACTORY (o, Components::EmitterDescription_init,
                                 Components::EmitterDescription);
    CIAO_REGISTER_VALUE_FACTORY (o, Components::SubscriberDescription_init,
                                 Components::SubscriberDescription);
    CIAO_REGISTER_VALUE_FACTORY (o, Components::PublisherDescription_init,
                                 Components::PublisherDescription);
#endif
    CIAO_REGISTER_VALUE_FACTORY (o, Components::ComponentPortDescription_init,
                                 Components::ComponentPortDescription);
#endif
    CIAO_REGISTER_VALUE_FACTORY (o, Components::ConfigValue_init,
                                 Components::ConfigValue);
    return 0;
  }
}
