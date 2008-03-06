#include "CIAO_ComponentServer_Impl.h"

#include <ciao/CIAO_common.h>

namespace CIAO
{
  namespace Deployment
  {
    CIAO_ComponentServer_i::CIAO_ComponentServer_i (const ACE_CString &/*uuid*/)
    {
      CIAO_TRACE("CIAO_ComponentServer_i::CIAO_ComponentServer_i");
    }
    
    // Destructor 
    CIAO_ComponentServer_i::~CIAO_ComponentServer_i (void)
    {
      CIAO_TRACE("CIAO_ComponentServer_i::~CIAO_ComponentServer_i");
    }
      
      
    void 
    CIAO_ComponentServer_i::shutdown (void)
    {
      CIAO_TRACE("CIAO_ComponentServer_i::shutdown");
    }
      
      
    ::Components::ConfigValues * 
    CIAO_ComponentServer_i::configuration (void)
    {
      CIAO_TRACE("CIAO_ComponentServer_i::configuration");
      return 0;
    }
      
      
    ::Components::Deployment::ServerActivator_ptr 
    CIAO_ComponentServer_i::get_server_activator (void)
    {
      CIAO_TRACE("CIAO_ComponentServer_i::get_server_activator");
      return 0;
    }
      
      
    ::Components::Deployment::Container_ptr 
    CIAO_ComponentServer_i::create_container (const ::Components::ConfigValues & /*config*/)
    {
      CIAO_TRACE("CIAO_ComponentServer_i::create_container");
      return 0;
    }
      
      
    void 
    CIAO_ComponentServer_i::remove_container (::Components::Deployment::Container_ptr /*cref*/)
    {
      CIAO_TRACE("CIAO_ComponentServer_i::remove_container");
    }
      
      
    ::Components::Deployment::Containers * 
    CIAO_ComponentServer_i::get_containers (void)
    {
      CIAO_TRACE("CIAO_ComponentServer_i::get_containers");
      return 0;
    }
      
      
    void 
    CIAO_ComponentServer_i::remove (void)
    {
      CIAO_TRACE("CIAO_ComponentServer_i::remove");
    }
  }
}
