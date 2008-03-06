#include "CIAO_ServerActivator_Impl.h"

#include <ace/Log_Msg.h>

namespace CIAO
{
  namespace Deployment
  {
    CIAO_ServerActivator_i::CIAO_ServerActivator_i (void)
    {
      CIAO_TRACE ("CIAO_ServerActivator_i::CIAO_ServerActivator_i");
    }
    
    void
    CIAO_ServerActivator_i::component_server_callback (::Components::Deployment::ComponentServer_ptr serverref,
                                                       const char * server_UUID,
                                                       ::Components::ConfigValues_out config)
    {
      CIAO_TRACE("CIAO_ServerActivator_i::component_server_callback");
    }
    
    ::Components::Deployment::ComponentServer_ptr 
    CIAO_ServerActivator_i::create_component_server (const ::Components::ConfigValues & config)
    {
      CIAO_TRACE("CIAO_ServerActivator_i::create_component_server");
    }
    
    
    
    void
    CIAO_ServerActivator_i::remove_component_server (::Components::Deployment::ComponentServer_ptr server)
    {
      CIAO_TRACE("CIAO_ServerActivator_i::remove_component_server");
    }
    
    
    ::Components::Deployment::ComponentServers
    CIAO_ServerActivator_i::get_component_servers (void)
    {
      CIAO_TRACE("CIAO_ServerActivator_i::get_component_servers");
    }
    
