// $Id$


#ifndef COMPASS_SERVER_ACTIVATOR_H
#define COMPASS_SERVER_ACTIVATOR_H


#include "ace/pre.h"
#include "ACEXML/compass/Compass_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Singleton.h"
#include "ace/Synch.h"
#include "ace/Unbounded_Set.h"
#include "ACEXML/compass/CompassTypes.h"

namespace Deployment
{
  class ComponentServer;  // Forward decl.
  typedef ACE_Unbounded_Set<ComponentServer*> ComponentServers;

  class Compass_Export ServerActivator
  {
  public:
    friend class ACE_Singleton <ServerActivator, ACE_SYNCH_MUTEX>;
    ComponentServer* create_component_server (const ConfigValues& config)
      ACE_THROW_SPEC ((CreateFailure, InvalidConfiguration));

    void remove_component_server (ComponentServer* server)
      ACE_THROW_SPEC ((RemoveFailure));

    ComponentServers* get_component_servers ();

  protected:
    ServerActivator();
    ~ServerActivator();
    ServerActivator (const ServerActivator&);
    ServerActivator& operator= (const ServerActivator&);
  private:
    ComponentServers* comp_servers_;
  };
  typedef ACE_Singleton <ServerActivator, ACE_SYNCH_MUTEX> SERVER_ACTIVATOR;
}

#if defined (__ACE_INLINE__)
#include "ServerActivator.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"

#endif /* COMPASS_SERVER_ACTIVATOR_H */
