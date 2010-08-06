// $Id$

#include "MDD_Handler.h"
#include "Basic_Deployment_Data.hpp"
#include "DAnCE/Deployment/Deployment_DataC.h"
#include "ADD_Handler.h"
#include "Property_Handler.h"
#include "Req_Handler.h"
#include "cdp.hpp"
#include "ace/UUID.h"

namespace CIAO
{
  namespace Config_Handlers
  {
    IDREF_Base<CORBA::ULong> MDD_Handler::IDREF;

    void
    MDD_Handler::mono_deployment_descriptions (
                                               const DeploymentPlan& src,
                                               Deployment::MonolithicDeploymentDescriptions& dest)
    {
      CIAO_TRACE("MDD_Handler::mono_deployment_descriptions");

      DeploymentPlan::implementation_const_iterator imp_e =
        src.end_implementation ();
      CORBA::ULong pos = 0;
      dest.length (src.count_implementation ());
      for (DeploymentPlan::implementation_const_iterator imp_b =
             src.begin_implementation ();
           imp_b != imp_e;
           ++imp_b)
        {
          MDD_Handler::mono_deployment_description (*imp_b,
                                                    dest[pos],
                                                    pos);
          pos++;
        }
    }

    void
    MDD_Handler::mono_deployment_description (
                                              const MonolithicDeploymentDescription& desc,
                                              Deployment::MonolithicDeploymentDescription& toconfig,
                                              CORBA::ULong pos)
    {
      toconfig.name =
        CORBA::string_dup (desc.name ().c_str ());

      MonolithicDeploymentDescription::source_const_iterator me =
        desc.end_source ();

      CORBA::ULong len = 0;
      toconfig.source.length (desc.count_source ());
      for (MonolithicDeploymentDescription::source_const_iterator se =
             desc.begin_source ();
           se != me;
           ++se)
        {
          toconfig.source[len++] =
            CORBA::string_dup ((*se).c_str ());
        }

      MonolithicDeploymentDescription::artifact_const_iterator ae =
        desc.end_artifact ();
      len = 0;
      toconfig.artifactRef.length (desc.count_artifact ());
      for (MonolithicDeploymentDescription::artifact_const_iterator
             ab = desc.begin_artifact ();
           ae != ab;
           ++ab)
        {
          CORBA::ULong tmp = 0;

          ADD_Handler::IDREF.find_ref (ACE_CString (ab->id ().c_str ()),
                                       tmp);

          toconfig.artifactRef[len++] = tmp;
        }

      MonolithicDeploymentDescription::execParameter_const_iterator epce =
        desc.end_execParameter ();
      len = 0;
      toconfig.execParameter.length (desc.count_execParameter ());
      for (MonolithicDeploymentDescription::execParameter_const_iterator epcb =
             desc.begin_execParameter ();
           epcb != epce;
           ++epcb)
        {
          Property_Handler::handle_property ((*epcb),
                                          toconfig.execParameter[len++]);
        }

      toconfig.deployRequirement.length (desc.count_deployRequirement ());
      std::for_each (desc.begin_deployRequirement (),
                     desc.end_deployRequirement (),
                     Requirement_Functor (toconfig.deployRequirement));

      // Handle the idref
      if (desc.id_p ())
        {
          ACE_CString cstr (desc.id ().c_str ());

          MDD_Handler::IDREF.bind_ref (cstr, pos);
        }
      else
        {
          ACE_DEBUG((LM_ERROR,
                     "(%P|%t) Warning:  MDD %s has no idref \n",
                     desc.name ().c_str ()));
        }
    }


    MonolithicDeploymentDescription
    MDD_Handler::mono_deployment_description(
                                             const Deployment::MonolithicDeploymentDescription &src)
    {
      CIAO_TRACE("mono_deployment_description - reverse");

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
                                 Property_Handler::get_property (src.execParameter[k]));
        }

      //Get the deployRequirement(s) from the IDL and store them
      total = src.deployRequirement.length();
      for(size_t l = 0; l < total; l++)
        {
          mdd.add_deployRequirement(
                                    Req_Handler::get_requirement (src.deployRequirement[l]));
        }

      // Generate a UUID to use for the IDREF.
      ACE_Utils::UUID uuid;
      ACE_Utils::UUID_GENERATOR::instance ()->generate_UUID (uuid);
      ACE_CString mdd_id ("_");
      mdd_id += *uuid.to_string ();

      XMLSchema::ID< ACE_TCHAR > xml_id (mdd_id.c_str ());

      // Bind the ref and set it in the IDD
      MDD_Handler::IDREF.bind_next_available (mdd_id);

      mdd.id (xml_id);

      return mdd;
    }
  }

}

