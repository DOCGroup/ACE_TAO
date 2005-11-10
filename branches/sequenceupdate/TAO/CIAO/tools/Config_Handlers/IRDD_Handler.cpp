
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
      throw (Config_Error)
    {
      
      switch (desc.resourceUsage ().integral ())
	{
	case ResourceUsageKind::None_l:
	  toconfig.resourceUsage = Deployment::None;
	  break;
	  
	case ResourceUsageKind::InstanceUsesResource_l:
	  toconfig.resourceUsage = Deployment::InstanceUsesResource;
	  break;
	  
	case ResourceUsageKind::ResourceUsesInstance_l:
	  toconfig.resourceUsage = Deployment::ResourceUsesInstance;
	  break;
	  
	case ResourceUsageKind::PortUsesResource_l:
	  toconfig.resourceUsage = Deployment::PortUsesResource;
	  break;
	  
	case ResourceUsageKind::ResourceUsesPort_l:
	  toconfig.resourceUsage = Deployment::ResourceUsesPort;
	  break;
	  
	default:
	  throw Config_Error (desc.requirementName (),
			      "Unknown ResourceUsageKind.");
	  break;
	}
      
      toconfig.requirementName = desc.requirementName ().c_str ();
      
      toconfig.resourceName= desc.resourceName ().c_str ();
      
      Any_Handler::extract_into_any (desc.resourceValue (),
				     toconfig.resourceValue);
      
    }

    InstanceResourceDeploymentDescription
    IRDD_Handler::instance_resource_deployment_descr (
      const Deployment::InstanceResourceDeploymentDescription& src)
      throw (Config_Error)
    {
      XMLSchema::string< char > reqname ((src.requirementName));
      XMLSchema::string< char > resname ((src.resourceName));
      Any resval (Any_Handler::get_any (src.resourceValue));
      
      InstanceResourceDeploymentDescription irdd (ResourceUsageKind::None,
						  reqname,
						  resname,
						  resval);
      
      switch (src.resourceUsage)
	{
	case Deployment::None:
	  irdd.resourceUsage (ResourceUsageKind::None);
	  break;
	  
	case Deployment::InstanceUsesResource:
	  irdd.resourceUsage (ResourceUsageKind::InstanceUsesResource);
	  break;
	  
	case Deployment::ResourceUsesInstance:
	  irdd.resourceUsage (ResourceUsageKind::ResourceUsesInstance);
	  break;
	  
	case Deployment::PortUsesResource:
	  irdd.resourceUsage (ResourceUsageKind::PortUsesResource);
	  break;

	case Deployment::ResourceUsesPort:
         irdd.resourceUsage (ResourceUsageKind::ResourceUsesPort);
	 break;
	 
	default:
	  throw Config_Error (src.resourceName.in (),
			      "Unknown ResourceUsageKind.");
	  break;
	}
      
      return irdd;
    }
  }

}
