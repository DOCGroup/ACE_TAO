// $Id$

#include "IDD_Handler.h"
#include "Property_Handler.h"
#include "Any_Handler.h"
#include "Basic_Deployment_Data.hpp"
#include "ciao/Deployment_DataC.h"
#include "cdp.hpp"

namespace CIAO
{
  namespace Config_Handlers
  {
    bool
    IDD_Handler::instance_deployment_descrs (
        const DeploymentPlan &src,
        Deployment::InstanceDeploymentDescriptions& dest)
    {
      DeploymentPlan::instance_const_iterator idd_e =
        src.end_instance ();

      for (DeploymentPlan::instance_const_iterator idd_b =
             src.begin_instance ();
           idd_b != idd_e;
           ++idd_b)
        {
          CORBA::ULong len =
            dest.length ();
          dest.length (len + 1);

          bool retval =
            IDD_Handler::instance_deployment_descr ((*idd_b),
                                                    dest[len]);

          if (!retval)
            return false;
        }

      return true;
    }

    bool
    IDD_Handler::instance_deployment_descr (
        const InstanceDeploymentDescription& src,
        Deployment::InstanceDeploymentDescription& dest)
    {
      dest.name =
        src.name ().c_str ();
      dest.node =
        src.node ().c_str ();

      // We know there should be only one element
      dest.source.length (1);
      dest.source [0] =
        src.source ().c_str ();

      // @@ MAJO:This is where the MDD should be? Need to look into
      // this later.
      dest.implementationRef = 0;

      InstanceDeploymentDescription::configProperty_const_iterator pend =
        src.end_configProperty ();

      for (ComponentInterfaceDescription::configProperty_const_iterator pstart =
             src.begin_configProperty ();
           pstart != pend;
           ++pstart)
        {
          // Need to improve this. This is clearly O(n^2).
          CORBA::ULong len =
            dest.configProperty.length ();

          dest.configProperty.length (len + 1);

          Property_Handler::get_property (*pstart,
                                          dest.configProperty[len]);
        }

#if 0
     // @@ MAJO: Need to handle this in the next round
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
#endif /*if 0*/
      return true;
      // Done!
    }

#if 0
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
#endif /*if 0*/

  }
}
