// $Id$
#include "MDD_Handler.h"
#include "Basic_Deployment_Data.hpp"
#include "ciao/Deployment_DataC.h"
#include "ADD_Handler.h"
#include "Property_Handler.h"
#include "Req_Handler.h"
#include "Singleton_IDREF_Map.h"

namespace CIAO
{
  namespace Config_Handlers
  {
    bool
    MDD_Handler::mono_deployment_descriptions (
        const MonolithicDeploymentDescription& src,
        Deployment::MonolithicDeploymentDescriptions& dest)
    {
      /* @@ This has changed.  The schema has maxoccurred = unbounded */
      
      // We know there should be only one..
      //dest.length (1);

      DeploymentPlan::implementation_const_iterator imp_e =
        src.end_implementation ();

      for (DeploymentPlan::implementation_const_iterator imp_b =
             src.begin_implementation ();
           imp_b != imp_e;
           ++imp_b)
        {
          CORBA::ULong len =
            dest.length ();
          dest.length (len + 1);

          bool retval = MDD_Handler::mono_deployment_description (*imp_b,
                                                                  dest[len]);
          if (!retval)
            return false;
        }
      return true;
    }

    bool
    MDD_Handler::mono_deployment_description (
        const MonolithicDeploymentDescription& desc,
        Deployment::MonolithicDeploymentDescription& toconfig)
    {
      toconfig.name =
        CORBA::string_dup (desc.name ().c_str ());

      MonolithicDeploymentDescription::source_const_iterator me =
        desc.end_source ();

      for (MonolithicDeploymentDescription::source_const_iterator se =
             desc.begin_source ();
           se != me;
           ++se)
        {
          CORBA::ULong len =
            toconfig.source.length ();

          toconfig.source.length (len + 1);

          toconfig.source[len] =
            CORBA::string_dup ((*se).c_str ());
        }

      MonolithicDeploymentDescription::artifact_const_iterator ae =
        desc.end_artifact ();

      for (MonolithicDeploymentDescription::artifact_const_iterator
             ab = desc.begin_artifact ();
           ae != ab;
           ++ab)
        {
          CORBA::ULong tmp = 0;

          bool r =
            Singleton_IDREF_Map::instance ()->find_ref (
              ab->id ().c_str (),
              tmp);

          if (!r)
            {
              // @@MAJO: What should we do if find_ref fails?
              return false;
            }

          CORBA::ULong len =
            toconfig.artifactRef.length ();

          toconfig.artifactRef.length (len + 1);

          toconfig.artifactRef[len] = tmp;
        }

      MonolithicDeploymentDescription::execParameter_const_iterator epce =
        desc.end_execParameter ();

      for (MonolithicDeploymentDescription::execParameter_const_iterator epcb =
             desc.begin_execParameter ();
           epcb != epce;
           ++epcb)
        {
          CORBA::ULong len =
            toconfig.execParameter.length ();

          toconfig.execParameter.length (len + 1);

          Property_Handler::get_property ((*epcb),
                                          toconfig.execParameter[len]);
        }

#if 0
      // @@ MAJO: Don't know how to handle this
      if (desc.deployRequirement_p ())
        {
                                  Req_Handler handler;
          toconfig.deployRequirement.length (
            toconfig.deployRequirement.length () + 1);
          handler.get_Requirement (
            toconfig.deployRequirement[toconfig.deployRequirement.length () - 1],
            desc.deployRequirement ());
        }
#endif /*if 0*/

      return true;
    }

  }

}
