
// $Id$


#include "ADD_Handler.h"
#include "Basic_Deployment_Data.hpp"
#include "ciao/Deployment_DataC.h"
#include "Prop_Handler.h"
#include "Req_Handler.h"
#include "RDD_Handler.h"



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
                    Deployment::ArtifactDeploymentDescription& toconfig, 
                    ArtifactDeploymentDescription& desc)
    {


      
      toconfig.name=
           CORBA::string_dup (desc.name ().c_str ());
      
      toconfig.source.length (
           toconfig.source.length () + 1);
         toconfig.source[toconfig.source.length () - 1]=
           CORBA::string_dup (desc.source ().c_str ());
      
      if (desc.node_p ())
       {
         
         toconfig.node=
           CORBA::string_dup (desc.node ().c_str ());
       }
      
      for (ArtifactDeploymentDescription::location_iterator
           item (desc.begin_location ());
           item != desc.end_location ();
           ++item)
        {	 
           toconfig.location.length (
             toconfig.location.length () + 1);
           toconfig.location[toconfig.location.length () - 1] =
             CORBA::string_dup (item->c_str ());
        }
      
      if (desc.execParameter_p ())
        {
	 			  Prop_Handler handler;
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
      
      if (desc.deployedResource_p ())
        {
	 			  RDD_Handler handler;
          toconfig.deployedResource.length (
            toconfig.deployedResource.length () + 1);
          handler.get_ResourceDeploymentDescription (
            toconfig.deployedResource[toconfig.deployedResource.length () - 1],
            desc.deployedResource ());
        }

      
    }

  }

}
