// $Id$


#ifndef COMPASS_SERVER_ACTIVATOR_H
#define COMPASS_SERVER_ACTIVATOR_H

#include "ace/Unbounded_Set.h"
#include "ACEXML/compass/CompassTypes.h"

namespace Deployment
{
  class ComponentServer;  // Forward decl.
  typedef ACE_Unbounded_Set<ComponentServer*> ComponentServers;

  class ServerActivator
  {
  public:
    ComponentServer* create_component_server (const ConfigValues* config)
      ACE_THROW_SPEC ((CreateFailure, InvalidConfiguration));

    void remove_component_server (const ComponentServer* server)
      ACE_THROW_SPEC (RemoveFailure);

    ComponentServers* get_component_servers ();
  private:
    ComponentServers* comp_servers_;
  };
  typedef ACE_Singleton <ServerActivator, ACE_SYNCH_MUTEX> SERVER_ACTIVATOR;
};

#endif /* COMPASS_SERVER_ACTIVATOR_H */
