// $Id$

#include "IDD_Handler.h"
#include "MDD_Handler.h"
#include "Property_Handler.h"
#include "Any_Handler.h"
#include "Basic_Deployment_Data.hpp"
#include "ciao/Deployment_DataC.h"
#include "cdp.hpp"

namespace CIAO
{
  namespace Config_Handlers
  {
    IDREF_Base IDD_Handler::IDREF;
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
          /* @@ Can you say n^2? */
          CORBA::ULong len =
            dest.length ();
          dest.length (len + 1);

          bool retval =
            IDD_Handler::instance_deployment_descr ((*idd_b),
                                                    dest[len], len);

          if (!retval)
            return false;
        }

      return true;
    }

    bool
    IDD_Handler::instance_deployment_descr (
        const InstanceDeploymentDescription& src,
        Deployment::InstanceDeploymentDescription& dest,
        CORBA::ULong pos)
    {
      dest.name =
        src.name ().c_str ();
      dest.node =
        src.node ().c_str ();

      if (src.id_p ())
        {
          ACE_CString cstr (src.id ().c_str ());
          if (!IDD_Handler::IDREF.bind_ref (cstr, pos))
           {
             return false;
           }
        }
      else
        {
          ACE_DEBUG((LM_ERROR,
                    "(%P|%t) Warning:  IDD %s has no idref \n",
                    src.name ().c_str ()));
        }

      // We know there should be only one element
      dest.source.length (1);
      dest.source [0] =
        src.source ().c_str ();

      // @@ MAJO:This is where the MDD should be? Need to look into
      // this later.
      // ACE_DEBUG ((LM_DEBUG, "string is %s\n", 
      //            src.implementation ().id ().c_str ()));
      size_t tmp = 0;
      bool r = MDD_Handler::IDREF.find_ref 
          (src.implementation ().id ().c_str (), tmp);
      
      // @@ MAJO:  What shouold we do if find_ref fails??
      dest.implementationRef = tmp;

      InstanceDeploymentDescription::configProperty_const_iterator pend =
        src.end_configProperty ();

      for (InstanceDeploymentDescription::configProperty_const_iterator pstart =
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

#if 0 /* MAJO:  Handle this next round */
      InstanceDeploymentDescription::deployedResource_const_iterator drend =
        src.end_deployedResource ();

      for (InstanceDeploymentDescription::deployedResource_const_iterator drstart =
             src.begin_deployedResource ();
           drstart != drend;
           ++drstart)
        {
          const CORBA::ULong len =
            dest.deployedResource.length ();

          dest.deployedResource.length (len + 1);

          IDD_Handler::instance_resource_depl_descr (
            (*drstart),
            dest.deployedResource[len]);

        }

#endif
#if 0
     // @@ MAJO: Need to handle this in the next round
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

    void
    IDD_Handler::instance_resource_depl_descr (
        const InstanceResourceDeploymentDescription &src,
        ::Deployment::InstanceResourceDeploymentDescription &dest
        )
    {
      // resourceUsage is an enumerated type
      switch (src.resourceUsage ().integral ())
        {
        case ResourceUsageKind::None_l:
          dest.resourceUsage = Deployment::None;
          break;

        case ResourceUsageKind::InstanceUsesResource_l:
          dest.resourceUsage = Deployment::InstanceUsesResource;
          break;

        case ResourceUsageKind::ResourceUsesInstance_l:
          dest.resourceUsage = Deployment::ResourceUsesInstance;
          break;

        case ResourceUsageKind::PortUsesResource_l:
          dest.resourceUsage = Deployment::PortUsesResource;
          break;

        case ResourceUsageKind::ResourceUsesPort_l:
          dest.resourceUsage = Deployment::ResourceUsesPort;
          break;
        }

      // requirementName and resourceName are strings
      dest.requirementName =
        src.requirementName ().c_str ();

      dest.resourceName =
        src.resourceName ().c_str ();

      Any_Handler::extract_into_any (src.resourceValue (),
                                     dest.resourceValue);

    }

  }
}
