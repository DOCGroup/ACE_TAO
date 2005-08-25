// $Id$
#include "ADD_Handler.h"
#include "cdp.hpp"
#include "ciao/Deployment_DataC.h"
#include "Property_Handler.h"


ACE_RCSID (DAnCE,
           ADD_Handler,
           "$Id$")

 
namespace CIAO
{
  namespace Config_Handlers
  {
    IDREF_Base ADD_Handler::IDREF;
    
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
            dest.source.length ();

          dest.source.length (l + 1);

          dest.source[l] = scb->c_str ();
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
          
          bool retval = ADD_Handler::IDREF.bind_ref (cstr,pos);
          if (!retval)
            {
              return false;
            }
        }
      else
        {
          ACE_DEBUG ((LM_ERROR,
                      "(%P|%t) Warning: ADD %s has no idref.\n",
                      src.name ().c_str ()));
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
    
      ArtifactDeploymentDescription
      ADD_Handler::artifact_deployment_descr (
	  const Deployment::ArtifactDeploymentDescription &src)
      {
	  //Get the name and node and store them in the add
	  XMLSchema::string< char > name ((src.name));
	  XMLSchema::string< char > node ((src.node));
	  
	  ArtifactDeploymentDescription add (name,node);

	  //Get the location(s) and store it/them in the add
	  size_t total = src.location.length ();
	  for (size_t i = 0; i < total; ++i)
	  {
	      XMLSchema::string< char > curr ((src.location[i]));
	      add.add_location (curr);
	  }

	  //As above, for the source(s)
	  total = src.source.length ();
	  for (size_t j = 0; j < total; ++j)
	  {
	      XMLSchema::string< char > curr ((src.source[j]));
	      add.add_source (curr);
	  }  

	  //As above for the execParameter(s)
	  total = src.execParameter.length ();
	  for (size_t k = 0; k < total; ++k)
	  {
	      add.add_execParameter (
		  Property_Handler::get_property (
		      src.execParameter[k]));
	  }
        
	  return add;       
    }

    

  }

}
