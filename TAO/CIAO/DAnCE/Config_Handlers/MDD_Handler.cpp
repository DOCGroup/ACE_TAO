// $Id$
#include "MDD_Handler.h"
#include "Basic_Deployment_Data.hpp"
#include "ciao/Deployment_DataC.h"
#include "ADD_Handler.h"
#include "Property_Handler.h"
#include "Req_Handler.h"



namespace CIAO
{
  namespace Config_Handlers
  {

    MDD_Handler::MDD_Handler (void)
    {
    }

    MDD_Handler::~MDD_Handler (void)
    {
    }


    void
    MDD_Handler::get_MonolithicDeploymentDescription (
        Deployment::MonolithicDeploymentDescription& toconfig,
        MonolithicDeploymentDescription& desc)
    {

      toconfig.name=
           CORBA::string_dup (desc.name ().c_str ());

      toconfig.source.length (
        toconfig.source.length () + 1);

      toconfig.source[toconfig.source.length () - 1] =
        CORBA::string_dup (desc.source ().c_str ());

      ADD_Handler artifact_handler;
      ACE_TString artifact_id;

      for (MonolithicDeploymentDescription::artifact_iterator
             item (desc.begin_artifact ());
           item != desc.end_artifact ();
           ++item)
        {
          toconfig.artifactRef.length (
            toconfig.artifactRef.length () + 1);

          artifact_id = item->id ().c_str ();
          artifact_handler.get_ref (
            artifact_id,
            toconfig.artifactRef[toconfig.artifactRef.length () - 1]);

        }

      if (desc.execParameter_p ())
        {
          Property_Handler handler;
          toconfig.execParameter.length (
            toconfig.execParameter.length () + 1);
          handler.get_Property (
            toconfig.execParameter[toconfig.execParameter.length () - 1],
            desc.execParameter ());
        }

      if (desc.deployRequirement_p ())
        {
                                  Req_Handler handler;
          toconfig.deployRequirement.length (
            toconfig.deployRequirement.length () + 1);
          handler.get_Requirement (
            toconfig.deployRequirement[toconfig.deployRequirement.length () - 1],
            desc.deployRequirement ());
        }


    }

  }

}
