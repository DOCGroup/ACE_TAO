// $Id$
#include "MDD_Handler.h"
#include "Basic_Deployment_Data.hpp"
#include "ciao/Deployment_DataC.h"
#include "ADD_Handler.h"
#include "Property_Handler.h"
#include "Req_Handler.h"
#include "cdp.hpp"

namespace CIAO
{
  namespace Config_Handlers
  {
    IDREF_Base<CORBA::ULong> MDD_Handler::IDREF;
    
    bool
    MDD_Handler::mono_deployment_descriptions (
        const DeploymentPlan& src,
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
                                                                  dest[len],
                                                                  len);
          if (!retval)
            {
              ACE_DEBUG ((LM_ERROR,
                          "(%P|%t) MDD_Handler: Error parsing element %i\n",
                          len));
              return false;
            }
          
        }
      return true;
    }  
    
    bool
    MDD_Handler::mono_deployment_description (
        const MonolithicDeploymentDescription& desc,
        Deployment::MonolithicDeploymentDescription& toconfig,
        CORBA::ULong pos)
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
            ADD_Handler::IDREF.find_ref (ACE_CString (ab->id ().c_str ()),
                                         tmp);

          if (!r)
            {
              // @@MAJO: What should we do if find_ref fails?
              ACE_DEBUG ((LM_ERROR,
                          "(%P|%t) MDD_Handler:  Find ref failed!  Uh oh!\n"));
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

      // Handle the idref
      if (desc.id_p ())
        {
          ACE_CString cstr (desc.id ().c_str ());
          
          if (!MDD_Handler::IDREF.bind_ref (cstr, pos))
            return false;
        }
      else
        {
          ACE_DEBUG((LM_ERROR,
                     "(%P|%t) Warning:  MDD %s has no idref \n",
                     desc.name ().c_str ()));
        }
      
      return true;
    }  


      MonolithicDeploymentDescription
      MDD_Handler::mono_deployment_description(
	  const Deployment::MonolithicDeploymentDescription &src)
      {
	  //Get the name and instantiate the mdd
	  XMLSchema::string < char > name ((src.name));
	  MonolithicDeploymentDescription mdd (name);

	  //Get the source(s) from the IDL and store them
	  size_t total = src.source.length();
	  for(size_t i = 0; i < total; i++)
	  {
	      XMLSchema::string< char > curr ((src.source[i]));
	      mdd.add_source(curr);
	  }

	  //Get the artifactRef(s) from the IDL and store them
	  total = src.artifactRef.length();
	  for(size_t j = 0; j < total; j++)
	  {
	      ACE_CString tmp;
	      ADD_Handler::IDREF.find_ref(src.artifactRef[j], tmp);
	      XMLSchema::IDREF< ACE_TCHAR > curr(tmp.c_str());
	      mdd.add_artifact (curr);
	  }

	  //Get the execParameter(s) from the IDL and store them
	  total = src.execParameter.length();
	  for(size_t k = 0; k < total; k++)
	  {
	      mdd.add_execParameter (
		  Property_Handler::get_property (
		      src.execParameter[k]));
	  }

	  //Get the deployRequirement(s) from the IDL and store them
	  total = src.deployRequirement.length();
	  for(size_t l = 0; l < total; l++)
	  {
	      mdd.add_deployRequirement(
		  Req_Handler::get_requirement (
		      src.deployRequirement[l]));
	  }

	  return mdd;
      }

  }

}
      
