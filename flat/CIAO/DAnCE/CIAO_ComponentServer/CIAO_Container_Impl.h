/**
 * @file CIAO_Container.h
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 */

#ifndef CIAO_CONTAINER_H_
#define CIAO_CONTAINER_H_

#include "CIAO_ComponentServerS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace CIAO
{
  namespace Deployment
  {
    /**
     * @class CIAO_Container
     * @brief Implements external interface for CIAO container. 
     */
    class  CIAO_Container_i
      : public virtual POA_Components::Deployment::CIAO::Container
    {
    public:
      // Constructor 
      CIAO_Container_i (void);
  
      // Destructor 
      virtual ~CIAO_Container_i (void);
  
      virtual
      ::Components::CCMObject_ptr install_component (const char * id,
						     const char * entrypt,
						     const ::Components::ConfigValues & config);
  
      virtual
      void remove_component (::Components::CCMObject_ptr cref);
  
      virtual
      ::Components::Deployment::CIAO::CCMObjects * get_components (void);
  
      virtual
      ::Components::ConfigValues * configuration (void);
  
      virtual
      ::Components::Deployment::ComponentServer_ptr get_component_server (void);
  
      virtual
      ::Components::CCMHome_ptr install_home (const char * id,
					      const char * entrypt,
					      const ::Components::ConfigValues & config);
  
      virtual
      void remove_home (::Components::CCMHome_ptr href);
  
      virtual
      ::Components::CCMHomes * get_homes (void);
  
      virtual
      void remove (void);
    };
  }
}
#endif /* CIAO_CONTAINER_H_ */
