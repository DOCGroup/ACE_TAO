
// $Id$


#include "ADD_Handler.h"
#include "Basic_Deployment_Data.hpp"
#include "ciao/Deployment_DataC.h"
#include "Property_Handler.h"
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
    ADD_Handler::artifact_deployment_descr (
                    const ArtifactDeploymentDescription& desc,
                    Deployment::ArtifactDeploymentDescription& toconfig)
    {


      
      toconfig.name=
           CORBA::string_dup (desc.name ().c_str ());
      
      ArtifactDeploymentDescription::source_const_iterator source_end =
        desc.end_source ();
      ArtifactDeploymentDescription::source_const_iterator source_beg =
        desc.begin_source ();  
        
      CORBA::ULong source_length = toconfig.source.length ();
      size_t source_delta = source_end - source_beg;
      source_length += source_delta;
      
      toconfig.source.length (source_length);  
      size_t source_count = 0;     
           
      for (ArtifactDeploymentDescription::source_const_iterator
           item (desc.begin_source ());
           item != source_end;
           ++item)
        {	 
           toconfig.source[source_length - (source_delta - source_count)] =
             CORBA::string_dup (item->c_str ());
           ++source_count;
        }
      
      toconfig.node=
           CORBA::string_dup (desc.node ().c_str ());
      
      ArtifactDeploymentDescription::location_const_iterator location_end =
        desc.end_location ();
      ArtifactDeploymentDescription::location_const_iterator location_beg =
        desc.begin_location ();  
        
      CORBA::ULong location_length = toconfig.location.length ();
      size_t location_delta = location_end - location_beg;
      location_length += location_delta;
      
      toconfig.location.length (location_length);  
      size_t location_count = 0;     
           
      for (ArtifactDeploymentDescription::location_const_iterator
           item (desc.begin_location ());
           item != location_end;
           ++item)
        {	 
           toconfig.location[location_length - (location_delta - location_count)] =
             CORBA::string_dup (item->c_str ());
           ++location_count;
        }
      
      ArtifactDeploymentDescription::execParameter_const_iterator execParameter_end =
        desc.end_execParameter ();
      ArtifactDeploymentDescription::execParameter_const_iterator execParameter_beg =
        desc.begin_execParameter ();  
        
      CORBA::ULong execParameter_length = toconfig.execParameter.length ();
      size_t execParameter_delta = execParameter_end - execParameter_beg;
      execParameter_length += execParameter_delta;
      
      toconfig.execParameter.length (execParameter_length);  
      size_t execParameter_count = 0;        

      for (ArtifactDeploymentDescription::execParameter_const_iterator
           item (execParameter_beg);
           item != execParameter_end;
           ++item)
        {	        
          Property_Handler::property (
            *item,
            toconfig.execParameter[execParameter_length - (execParameter_delta - execParameter_count)]);
          ++execParameter_count;  
        }
      
      ArtifactDeploymentDescription::deployRequirement_const_iterator deployRequirement_end =
        desc.end_deployRequirement ();
      ArtifactDeploymentDescription::deployRequirement_const_iterator deployRequirement_beg =
        desc.begin_deployRequirement ();  
        
      CORBA::ULong deployRequirement_length = toconfig.deployRequirement.length ();
      size_t deployRequirement_delta = deployRequirement_end - deployRequirement_beg;
      deployRequirement_length += deployRequirement_delta;
      
      toconfig.deployRequirement.length (deployRequirement_length);  
      size_t deployRequirement_count = 0;        

      for (ArtifactDeploymentDescription::deployRequirement_const_iterator
           item (deployRequirement_beg);
           item != deployRequirement_end;
           ++item)
        {	        
          Req_Handler::requirement (
            *item,
            toconfig.deployRequirement[deployRequirement_length - (deployRequirement_delta - deployRequirement_count)]);
          ++deployRequirement_count;  
        }
      
      ArtifactDeploymentDescription::deployedResource_const_iterator deployedResource_end =
        desc.end_deployedResource ();
      ArtifactDeploymentDescription::deployedResource_const_iterator deployedResource_beg =
        desc.begin_deployedResource ();  
        
      CORBA::ULong deployedResource_length = toconfig.deployedResource.length ();
      size_t deployedResource_delta = deployedResource_end - deployedResource_beg;
      deployedResource_length += deployedResource_delta;
      
      toconfig.deployedResource.length (deployedResource_length);  
      size_t deployedResource_count = 0;        

      for (ArtifactDeploymentDescription::deployedResource_const_iterator
           item (deployedResource_beg);
           item != deployedResource_end;
           ++item)
        {	        
          RDD_Handler::resource_deployment_descr (
            *item,
            toconfig.deployedResource[deployedResource_length - (deployedResource_delta - deployedResource_count)]);
          ++deployedResource_count;  
        }

      
    }

  }

}
