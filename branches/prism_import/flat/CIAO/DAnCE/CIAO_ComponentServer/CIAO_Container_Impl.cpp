#include "CIAO_Container_Impl.h"

#include <ace/Log_Msg.h>


namespace CIAO
{
  namespace Deployment
  {
          // Constructor 
    CIAO_Container_i::CIAO_Container_i (void)
    {
      CIAO_TRACE("CIAO_Container_i::CIAO_Container_i");
    }
    
  
    // Destructor 
    CIAO_Container_i::~CIAO_Container_i (void)
    {
      CIAO_DEBUG("CIAO_Container_i::~CIAO_Container_i");
    }
    
    ::Components::CCMObject_ptr
    CIAO_Container_i::install_component (const char * id,
                                         const char * entrypt,
                                         const ::Components::ConfigValues & config)
    {
      CIAO_TRACE("CIAO_Container_i::install_component");
      return 0;
    }
    
    void 
    CIAO_Container_i::remove_component (::Components::CCMObject_ptr cref)
    {
      CIAO_TRACE("CIAO_Container_i::remove_component");
    }
    
    ::Components::Deployment::CIAO::CCMObjects * 
    CIAO_Container_i::get_components (void)
    {
      CIAO_TRACE("CIAO_Container_i::get_components");
      return 0;
    }
    
    ::Components::ConfigValues * 
    CIAO_Container_i::configuration (void)
    {
      CIAO_TRACE("CIAO_Container_i::configuration");
      return 0;
    }
    
    ::Components::Deployment::ComponentServer_ptr 
    CIAO_Container_i::get_component_server (void)
    {
      CIAO_TRACE("CIAO_Container_i::get_component_server");
      return 0;
    }
    
    ::Components::CCMHome_ptr 
    CIAO_Container_i::install_home (const char * id,
                                    const char * entrypt,
                                    const ::Components::ConfigValues & config)
    {
      CIAO_TRACE("CIAO_Container_i::install_home");
      return  *;
    }
    
    void 
    CIAO_Container_i::remove_home (::Components::CCMHome_ptr href)
    {
      CIAO_TRACE("CIAO_Container_i::remove_home");
    }
    
    ::Components::CCMHomes * 
    CIAO_Container_i::get_homes (void)
    {
      CIAO_TRACE("CIAO_Container_i::get_homes");
      return 0;
    }
    
    void 
    CIAO_Container_i::remove (void)
    {
      CIAO_TRACE("CIAO_Container_i::remove");
    }
    

  }
}
