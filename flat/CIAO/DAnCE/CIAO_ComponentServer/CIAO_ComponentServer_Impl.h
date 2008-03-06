/**
 * @file CIAO_ComponentServer.h
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 */

#ifndef CIAO_COMPONENTSERVER_IMPL_H_
#define CIAO_COMPONENTSERVER_IMPL_H_

#include "CIAO_ComponentServerS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include <ace/String_Base.h>

namespace CIAO
{
  namespace Deployment
  {
    class  CIAO_ComponentServer_i
      : public virtual POA_Components::Deployment::CIAO::ComponentServer
    {
    public:
      // Constructor 
      CIAO_ComponentServer_i (const ACE_CString &uuid);
      
      // Destructor 
      virtual ~CIAO_ComponentServer_i (void);
      
      virtual
      void shutdown (void);
      
      virtual
      ::Components::ConfigValues * configuration (void);
      
      virtual
      ::Components::Deployment::ServerActivator_ptr get_server_activator (void);
      
      virtual
      ::Components::Deployment::Container_ptr create_container (const ::Components::ConfigValues & config);
      
      virtual
      void remove_container (::Components::Deployment::Container_ptr cref);
      
      virtual
      ::Components::Deployment::Containers * get_containers (void);
      
      virtual
      void remove (void);
    };
  }
}

#endif
