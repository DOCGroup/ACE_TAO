// $Id$

#ifndef COMPASS_CONTAINER_H
#define COMPASS_CONTAINER_H

#include "ACEXML/compass/CompassTypes.h"
#include "ACEXML/compass/CCMHome.h"

namespace Deployment
{
  class Container
  {
  public:
    Container(ConfigValues* config = 0);
    ~Container();

    ComponentServer* get_component_server ();

    CCMHome* install_home (const UUID& id, const string& entrypt,
                           const ConfigValues* config)
      ACE_THROW_SPEC ((UnknownImplId, ImplEntryPointNotFound,
                       InstallationFailure, InvalidConfiguration));

    void remove_home (const CCMHome* href)
      ACE_THROW_SPEC ((RemoveFailure));

    CCMHomes* get_homes ();

    void remove ()
      ACE_THROW_SPEC ((RemoveFailure));

    ConfigValues* get_configuration (void);

    void set_configuration (const ConfigValues* config);

  private:
    ConfigValues*   config_;
    CCMHomes*       homes_;
    ComponentServer* comp_server_;
  };

};

#endif /* COMPASS_CONTAINER_H */
