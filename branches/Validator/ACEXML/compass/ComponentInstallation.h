// $Id$


#ifndef COMPASS_COMPONENT_INSTALLATION_H
#define COMPASS_COMPONENT_INSTALLATION_H

#include "ACEXML/compass/CompassTypes.h"

namespace Deployment
{

  class ComponentInstallation
  {
  public:
    void install(const UUID& implUUID, const Location& component_loc)
      ACE_THROW_SPEC ((InvalidLocation,InstallationFailure));

    void replace(const UUID& implUUID, const Location& component_loc)
      ACE_THROW_SPEC ((InvalidLocation, InstallationFailure));

    void remove(const UUID& implUUID)
      ACE_THROW_SPEC ((UnknownImplId, RemoveFailure));

    Location get_implementation (const UUID& implUUID)
      ACE_THROW_SPEC ((UnknownImplId, InstallationFailure));
  };

};

#endif /* COMPASS_COMPONENT_INSTALLATION_H */
