
// $Id$


#include "MDD_Handler.h"
#include "Basic_Deployment_Data.hpp"
#include "ciao/Deployment_DataC.h"
#include "Singleton_IDREF_Map.h"
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
    MDD_Handler::monolithic_deployment_descr (
                    const MonolithicDeploymentDescription& desc,
                    Deployment::MonolithicDeploymentDescription& toconfig)
    {


      
      toconfig.name=
           CORBA::string_dup (desc.name ().c_str ());
      
      MonolithicDeploymentDescription::source_const_iterator source_end =
        desc.end_source ();
      MonolithicDeploymentDescription::source_const_iterator source_beg =
        desc.begin_source ();  
        
      CORBA::ULong source_length = toconfig.source.length ();
      size_t source_delta = source_end - source_beg;
      source_length += source_delta;
      
      toconfig.source.length (source_length);  
      size_t source_count = 0;     
           
      for (MonolithicDeploymentDescription::source_const_iterator
           item (desc.begin_source ());
           item != source_end;
           ++item)
        {	 
           toconfig.source[source_length - (source_delta - source_count)] =
             CORBA::string_dup (item->c_str ());
           ++source_count;
        }
      
      MonolithicDeploymentDescription::artifact_const_iterator artifact_end =
        desc.end_artifact ();
      MonolithicDeploymentDescription::artifact_const_iterator artifact_beg =
        desc.begin_artifact ();  
        
      CORBA::ULong artifact_length = toconfig.artifactRef.length ();
      size_t artifact_delta = artifact_end - artifact_beg;
      artifact_length += artifact_delta;
      
      toconfig.artifactRef.length (artifact_length);  
      size_t artifact_count = 0;        
      IDREF_Map* artifact_instance = Singleton_IDREF_Map::instance ();
      
      
      for (MonolithicDeploymentDescription::artifact_const_iterator
           item (desc.begin_artifact ());
           item != artifact_end;
           ++item)
        {
          ACE_TString artifact_id (item->id ().c_str ());
          artifact_instance->find_ref (
            artifact_id,
            toconfig.artifactRef[artifact_length - (artifact_delta - artifact_count)]);
          ++artifact_count;  
        }
      
      MonolithicDeploymentDescription::execParameter_const_iterator execParameter_end =
        desc.end_execParameter ();
      MonolithicDeploymentDescription::execParameter_const_iterator execParameter_beg =
        desc.begin_execParameter ();  
        
      CORBA::ULong execParameter_length = toconfig.execParameter.length ();
      size_t execParameter_delta = execParameter_end - execParameter_beg;
      execParameter_length += execParameter_delta;
      
      toconfig.execParameter.length (execParameter_length);  
      size_t execParameter_count = 0;        

      for (MonolithicDeploymentDescription::execParameter_const_iterator
           item (execParameter_beg);
           item != execParameter_end;
           ++item)
        {	        
          Property_Handler::property (
            *item,
            toconfig.execParameter[execParameter_length - (execParameter_delta - execParameter_count)]);
          ++execParameter_count;  
        }
      
      MonolithicDeploymentDescription::deployRequirement_const_iterator deployRequirement_end =
        desc.end_deployRequirement ();
      MonolithicDeploymentDescription::deployRequirement_const_iterator deployRequirement_beg =
        desc.begin_deployRequirement ();  
        
      CORBA::ULong deployRequirement_length = toconfig.deployRequirement.length ();
      size_t deployRequirement_delta = deployRequirement_end - deployRequirement_beg;
      deployRequirement_length += deployRequirement_delta;
      
      toconfig.deployRequirement.length (deployRequirement_length);  
      size_t deployRequirement_count = 0;        

      for (MonolithicDeploymentDescription::deployRequirement_const_iterator
           item (deployRequirement_beg);
           item != deployRequirement_end;
           ++item)
        {	        
          Req_Handler::requirement (
            *item,
            toconfig.deployRequirement[deployRequirement_length - (deployRequirement_delta - deployRequirement_count)]);
          ++deployRequirement_count;  
        }

      
    }

  }

}
