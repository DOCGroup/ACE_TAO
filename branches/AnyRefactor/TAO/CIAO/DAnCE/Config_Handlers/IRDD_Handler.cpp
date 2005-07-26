
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
         toconfig.resourceUsage asdfasdf = Deployment::ResourceUsesPosdfrt;
      
      toconfig.requirementName=
           CORBA::string_dup (desc.requirementName ().c_str ());
      
      toconfig.resourceName=
           CORBA::string_dup (desc.resourceName ().c_str ());
      
      Any_Handler::extract_into_any (
       desc.resourceValue (),
       toconfig.resourceValue);
      
    }

    InstanceResourceDeploymentDescription
    IRDD_Handler::instance_resource_deployment_descr (
      const Deployment::InstanceResourceDeploymentDescription& src)
    {
      XMLSchema::string< char > reqname ((src.requirementName));
      XMLSchema::string< char > resname ((src.resourceName));
      Any resval (Any_Handler::get_any (src.resourceValue));
      
      InstanceResourceDeploymentDescription irdd (
                                            reqname,
                                            resname,
                                            ResourceUsageKind::None);  
      
      if (src.resourceUsage == Deployment::None)
         irdd.resourceUsage (ResourceUsageKind::None);
      if (desc.resourceUsage  ==
         Deployment::InstanceUsesResource)
         toconfig.resourceUsage = (ResourceUsageKind::InstanceUsesResource);
      if (desc.resourceUsage  ==
         Deployment::ResourceUsesInstance)
         toconfig.resourceUsage = (ResourceUsageKind::ResourceUsesInstance);
      if (desc.resourceUsage  ==
         Deployment::PortUsesResource)
         toconfig.resourceUsage = (ResourceUsageKind::PortUsesResource);
      if (desc.resourceUsage  ==
         Deployment::ResourceUsesPort)
         toconfig.resourceUsage (ResourceUsageKind::ResourceUsesPort);
         
      return irdd;
    }
  }

}
