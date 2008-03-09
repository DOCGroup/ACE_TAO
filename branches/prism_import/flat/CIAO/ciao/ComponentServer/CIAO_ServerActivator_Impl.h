/**
 * @file CIAO_ServerActivator.h
 * @author William R. Otte
 */

#ifndef CIAO_SERVERACTIVATOR_H_
#define CIAO_SERVERACTIVATOR_H_

#include "CIAO_ComponentServerS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace CIAO
{
  namespace Deployment
  {
    /**
     * @class CIAO_ServerActivator_i
     * @author William R. Otte <wotte@dre.vanderbilt.edu>
     * @brief Default server activator for CIAO component servers.
     *
     * Implements the default component server activation strategy
     * which is to spawn new processes.
     */
    class  CIAO_ServerActivator_i
      : public virtual POA_Components::Deployment::CIAO::ServerActivator
    {
    public:
      // Constructor 
      CIAO_ServerActivator_i (void);
      
      // Destructor 
      virtual ~CIAO_ServerActivator_i (void);
      
      virtual
      void component_server_callback (::Components::Deployment::ComponentServer_ptr serverref,
				      const char * server_UUID,
				      ::Components::ConfigValues_out config);
      
      virtual
      ::Components::Deployment::ComponentServer_ptr 
      create_component_server (const ::Components::ConfigValues & config);
      
      virtual
      void remove_component_server (::Components::Deployment::ComponentServer_ptr server);
      
      virtual
      ::Components::Deployment::ComponentServers * get_component_servers (void);
    };

  }
}
#endif /* CIAO_SERVERACTIVATOR_H_ */
