// $Id$


#ifndef COMPASS_COMPONENT_INSTALLATION_H
#define COMPASS_COMPONENT_INSTALLATION_H

#include "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ACEXML/compass/CompassTypes.h"

namespace Deployment
{
  class Compass_Export ComponentInstallation
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

}

#if defined (__ACE_INLINE__)
#include "ComponentInstallation.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"

#endif /* COMPASS_COMPONENT_INSTALLATION_H */
