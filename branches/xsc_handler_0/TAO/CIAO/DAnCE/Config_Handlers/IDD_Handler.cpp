// $Id$

#include "IDD_Handler.h"
#include "Prop_Handler.h"
#include "ANY_Handler.h"
#include "Basic_Deployment_Data.hpp"
#include "ciao/Deployment_DataC.h"

namespace CIAO
{
  namespace Config_Handlers
  {

    IDD_Handler::IDD_Handler (void)
    {
    }

    IDD_Handler::~IDD_Handler (void)
    {
    }
 
    ///This method takes a <Deployment::InstanceDeploymentDescription>
    ///and maps the values from the passed in XSC 
    ///InstanceDeploymentDescription to its members.
    void 
    IDD_Handler::get_InstanceDeploymentDescription (
         Deployment::InstanceDeploymentDescription& toconfig,
         InstanceDeploymentDescription& desc)
    {
      // name, node, and source are required elements,
      // and will be present in any valid instance document.
      toconfig.name = CORBA::string_dup (desc.name ().c_str ());
      toconfig.node = CORBA::string_dup (desc.node ().c_str ());
      
      toconfig.source.length (toconfig.source.length () + 1);
      toconfig.source[toconfig.source.length () - 1] = 
        CORBA::string_dup (desc.source ().c_str ());
      
      // @@TODO:  What to do with the implementationRef?
      
      if (desc.configProperty_p ())
        {
          // Create a property handler to fill in a property struct
          Prop_Handler property_handler;
          Deployment::Property prop;
          property_handler.get_Property (prop,
                                         desc.configProperty ());
          
          // Make room in the sequence for the strtuct that we have
          // created.
          toconfig.configProperty.length (toconfig.configProperty.length () + 1);
          toconfig.configProperty[toconfig.configProperty.length () - 1] = 
            prop;
        }

      if (desc.deployedResource_p ())
        {
          CORBA::ULong length = toconfig.deployedResource.length ();
          toconfig.deployedResource.length (length + 1);

          this->get_InstanceResourceDeploymentDescription
            (toconfig.deployedResource[length - 1],
             desc.deployedResource ());
        }
      
      if (desc.deployedSharedResource_p ())
        {
          CORBA::ULong length = toconfig.deployedSharedResource.length ();
          toconfig.deployedSharedResource.length (length + 1);

          this->get_InstanceResourceDeploymentDescription
            (toconfig.deployedResource[length],
             desc.deployedResource ());
        }
      
      // Done!
    }

    void 
    IDD_Handler::get_InstanceResourceDeploymentDescription (
            Deployment::InstanceResourceDeploymentDescription &toconfig,
            InstanceResourceDeploymentDescription &desc)
    {
      // resourceUsage is an enumerated type
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
        }
      
      // requirementName and resourceName are strings
      toconfig.requirementName = 
        CORBA::string_dup (desc.requirementName ().c_str ());
      toconfig.resourceName = 
        CORBA::string_dup (desc.resourceName ().c_str ());
      
      ANY_Handler::get_Any (toconfig.resourceValue,
                            desc.resourceValue ());
      
      // Done!
    }
    
  }
}
