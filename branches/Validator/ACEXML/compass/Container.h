// $Id$

#ifndef COMPASS_CONTAINER_H
#define COMPASS_CONTAINER_H

#include "ace/pre.h"
#include "ACEXML/compass/Compass_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ACEXML/compass/CompassTypes.h"
#include "ACEXML/compass/CCMHome.h"

namespace Deployment
{
  class Compass_Export Container
  {
  public:
    Container(ComponentServer* server);

    ~Container();

    CCMHome* install_home (const UUID& id, const string& entrypt,
                           const ConfigValues* config = 0)
      ACE_THROW_SPEC ((UnknownImplId, ImplEntryPointNotFound,
                       InstallationFailure, InvalidConfiguration));

    void remove_home (const CCMHome* href)
      ACE_THROW_SPEC ((RemoveFailure));

    void remove ()
      ACE_THROW_SPEC ((RemoveFailure));

    CCMHomes* get_homes ();

    ComponentServer* get_component_server ();

    ConfigValues* get_configuration (void);

    void set_configuration (const ConfigValues* config);

  private:
    ConfigValues*   config_;
    CCMHomes*       homes_;
    ComponentServer* parent_;
  };

}

#if defined (__ACE_INLINE__)
#include "Container.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"

#endif /* COMPASS_CONTAINER_H */
