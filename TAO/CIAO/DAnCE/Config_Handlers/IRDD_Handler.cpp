
// $Id$


#include "IRDD_Handler.h"
#include "Basic_Deployment_Data.hpp"
#include "ciao/Deployment_DataC.h"
#include "Any_Handler.h"



namespace CIAO
{
  namespace Config_Handlers
  {

    IRDD_Handler::IRDD_Handler (void)
    {
    }

    IRDD_Handler::~IRDD_Handler (void)
    {
    }


    void
    IRDD_Handler::instance_resource_deployment_descr (
                    const InstanceResourceDeploymentDescription& desc,
                    Deployment::InstanceResourceDeploymentDescription& toconfig)
    {


      
      if (desc.resourceUsage () ==
         ResourceUsageKind::None)        
         toconfig.resourceUsage = Deployment::None;
      if (desc.resourceUsage () ==
         ResourceUsageKind::InstanceUsesResource)        
         toconfig.resourceUsage = Deployment::InstanceUsesResource;
      if (desc.resourceUsage () ==
         ResourceUsageKind::ResourceUsesInstance)        
         toconfig.resourceUsage = Deployment::ResourceUsesInstance;
      if (desc.resourceUsage () ==
         ResourceUsageKind::PortUsesResource)        
         toconfig.resourceUsage = Deployment::PortUsesResource;
      if (desc.resourceUsage () ==
         ResourceUsageKind::ResourceUsesPort)        
         toconfig.resourceUsage = Deployment::ResourceUsesPort;
      
      toconfig.requirementName=
           CORBA::string_dup (desc.requirementName ().c_str ());
      
      toconfig.resourceName=
           CORBA::string_dup (desc.resourceName ().c_str ());
      
      Any_Handler::extract_into_any (
       desc.resourceValue (),
       toconfig.resourceValue);

      
    }

  }

}
