// $Id$
#include "ADD_Handler.h"
#include "cdp.hpp"
#include "ciao/Deployment_DataC.h"
#include "Property_Handler.h"
#include "Singleton_IDREF_Map.h"

ACE_RCSID (DAnCE,
           ADD_Handler,
           "$Id$")

namespace CIAO
{
  namespace Config_Handlers
  {
    bool
    ADD_Handler::artifact_deployment_descrs (
        const DeploymentPlan &src,
        ::Deployment::ArtifactDeploymentDescriptions &dest)
    {
      DeploymentPlan::artifact_const_iterator aci_e =
        src.end_artifact ();

      for (DeploymentPlan::artifact_const_iterator aci_b =
             src.begin_artifact ();
           aci_e != aci_b;
           ++aci_b)
        {
          CORBA::ULong len =
            dest.length ();

          dest.length (len + 1);

          bool retval =
            ADD_Handler::artifact_deployment_descr (
             (*aci_b),
             dest[len],
             len);

          if (!retval)
            return retval;
        }

      return true;
    }

    bool
    ADD_Handler::artifact_deployment_descr (
        const ArtifactDeploymentDescription &src,
        Deployment::ArtifactDeploymentDescription &dest,
        CORBA::ULong pos)
    {
      dest.name =
        CORBA::string_dup (src.name ().c_str ());

      dest.node =
        CORBA::string_dup (src.node ().c_str ());

      ArtifactDeploymentDescription::location_const_iterator end =
        src.end_location ();

      for (ArtifactDeploymentDescription::location_const_iterator
           start = src.begin_location ();
           start != end;
           ++start)
        {
          CORBA::ULong l =
            dest.location.length ();

          dest.location.length (l + 1);

          dest.location[l] = start->c_str ();
        }

      ArtifactDeploymentDescription::source_const_iterator sce =
        src.end_source ();

      for (ArtifactDeploymentDescription::source_const_iterator
           scb = src.begin_source ();
           scb != sce;
           ++scb)
        {
          CORBA::ULong l =
            dest.location.length ();

          dest.location.length (l + 1);

          dest.location[l] = scb->c_str ();
        }

      // @@TODO: See this loop is repeated
      ArtifactDeploymentDescription::execParameter_const_iterator adce =
        src.end_execParameter ();

      for (ArtifactDeploymentDescription::execParameter_const_iterator adcb =
             src.begin_execParameter ();
           adcb != adce;
           ++adcb)
        {
          CORBA::ULong len =
            dest.execParameter.length ();

          dest.execParameter.length (len + 1);

          Property_Handler::get_property ((*adcb),
                                          dest.execParameter[len]);
        }


      if (src.id_p ())
        {
          ACE_CString cstr (src.id ().c_str ());

          bool retval =
            Singleton_IDREF_Map::instance ()->bind_ref (cstr,
                                                        pos);
          if (!retval)
            {
              return false;
            }
        }
#if 0
      // @@ MAJO: Don't know how to handle this.
      if (src.deployRequirement_p ())
        {
          Req_Handler handler;
          add.deployRequirement.length (
            add.deployRequirement.length () + 1);
          handler.get_Requirement (
            add.deployRequirement[add.deployRequirement.length () - 1],
            src.deployRequirement ());
        }

      if (src.deployedResource_p ())
        {
          RDD_Handler handler;
          add.deployedResource.length (
            add.deployedResource.length () + 1);
          handler.get_ResourceDeploymentDescription (
            add.deployedResource[add.deployedResource.length () - 1],
            src.deployedResource ());
        }
#endif /* if 0*/

      return true;
    }

  }

}
