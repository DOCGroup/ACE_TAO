// $Id$


#ifndef COMPASS_COMPONENT_SERVER_H
#define COMPASS_COMPONENT_SERVER_H

#include "ace/pre.h"
#include "ACEXML/compass/Compass_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Unbounded_Set.h"
#include "ACEXML/compass/CompassTypes.h"

namespace Deployment
{
  class Container;        // Forward decl.
  typedef ACE_Unbounded_Set<Container*> Containers;

  class Compass_Export ComponentServer
  {
  public:
    ComponentServer (const ConfigValues& config);
    ~ComponentServer();

    ServerActivator* get_server_activator ();

    Container* create_container (const ConfigValues& config)
      ACE_THROW_SPEC ((CreateFailure, InvalidConfiguration));

    void remove_container (Container* cref)
      ACE_THROW_SPEC ((RemoveFailure));

    Containers* get_containers ();

    void remove ()
      ACE_THROW_SPEC ((RemoveFailure));

    const ConfigValues& get_configuration (void);

    void set_configuration (const ConfigValues& config);

  private:
    Containers* containers_;
    ServerActivator* activator_;
    ConfigValues* config_;
  };

}

#if defined (__ACE_INLINE__)
#include "ComponentServer.inl"
#endif /* __ACE_INLINE__ */

#endif /* COMPASS_COMPONENT_SERVER_H */
