// $Id$

#include "IRDD_Handler.h"
#include "Basic_Deployment_Data.hpp"
#include "DAnCE/Deployment/Deployment_DataC.h"
#include "Property_Handler.h"
#include "DAnCE/Logger/Log_Macros.h"


namespace DAnCE
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
    IRDD_Handler::handle_irdd (const InstanceResourceDeploymentDescription& desc,
                               Deployment::InstanceResourceDeploymentDescription& toconfig)
    {
      DANCE_TRACE("IRDD_Handler::instance_resource_deployment_descr");

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
                              ACE_TEXT ("Unknown ResourceUsageKind."));
          break;
        }

      toconfig.requirementName = ACE_TEXT_ALWAYS_CHAR ( desc.requirementName ().c_str ());

      toconfig.resourceName= ACE_TEXT_ALWAYS_CHAR ( desc.resourceName ().c_str ());

      toconfig.property.length (desc.count_property ());
      std::for_each (desc.begin_property (),
                     desc.end_property (),
                     Property_Functor (toconfig.property));
    }

    InstanceResourceDeploymentDescription
    IRDD_Handler::instance_resource_deployment_descr (const Deployment::InstanceResourceDeploymentDescription& src)
    {
      DANCE_TRACE("IRDD_Handler::instance_resource_deployment_descr - reverse");

      XMLSchema::string< ACE_TCHAR > reqname (ACE_TEXT_CHAR_TO_TCHAR (src.requirementName));
      XMLSchema::string< ACE_TCHAR > resname (ACE_TEXT_CHAR_TO_TCHAR (src.resourceName));

      InstanceResourceDeploymentDescription irdd (ResourceUsageKind::None,
                                                  reqname,
                                                  resname);

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
          throw Config_Error (ACE_TEXT_CHAR_TO_TCHAR (src.resourceName.in ()),
                              ACE_TEXT ("Unknown ResourceUsageKind."));
          break;
        }

      for (CORBA::ULong i = 0; i < src.property.length (); ++i)
        {
          //irdd.add_property (Property_Handler::get_property (src.property[i]));
        }


      return irdd;
    }
  }

}
