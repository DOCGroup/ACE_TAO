// $Id$
#include "ADD_Handler.h"
#include "Basic_Deployment_Data.hpp"
#include "ciao/Deployment_DataC.h"
#include "Prop_Handler.h"
#include "Req_Handler.h"
#include "RDD_Handler.h"

ACE_RCSID (DAnCE,
           ADD_Handler,
           "$Id$")

namespace CIAO
{
  namespace Config_Handlers
  {

    ADD_Handler::ADD_Handler (void)
    {
    }

    ADD_Handler::~ADD_Handler (void)
    {
    }


    void
    ADD_Handler::get_ArtifactDeploymentDescription (
        Deployment::ArtifactDeploymentDescription& add,
        ArtifactDeploymentDescription& desc)
    {
      add.name=
        CORBA::string_dup (desc.name ().c_str ());

      add.source.length (add.source.length () + 1);

      add.source[add.source.length () - 1]=
        CORBA::string_dup (desc.source ().c_str ());

      if (desc.node_p ())
       {
         add.node=
           CORBA::string_dup (desc.node ().c_str ());
       }

      for (ArtifactDeploymentDescription::location_iterator
           item (desc.begin_location ());
           item != desc.end_location ();
           ++item)
        {
          add.location.length (
              add.location.length () + 1);
          add.location[add.location.length () - 1] =
            CORBA::string_dup (item->c_str ());
        }

      if (desc.execParameter_p ())
        {
          Prop_Handler handler;

          add.execParameter.length (
            add.execParameter.length () + 1);

          handler.get_Property (
            add.execParameter[add.execParameter.length () - 1],
            desc.execParameter ());
        }

      if (desc.deployRequirement_p ())
        {
          Req_Handler handler;
          add.deployRequirement.length (
            add.deployRequirement.length () + 1);
          handler.get_Requirement (
            add.deployRequirement[add.deployRequirement.length () - 1],
            desc.deployRequirement ());
        }

      if (desc.deployedResource_p ())
        {
          RDD_Handler handler;
          add.deployedResource.length (
            add.deployedResource.length () + 1);
          handler.get_ResourceDeploymentDescription (
            add.deployedResource[add.deployedResource.length () - 1],
            desc.deployedResource ());
        }
    }

  }

}
