// $Id$
#include "ADD_Handler.h"
#include "cdp.hpp"
#include "DAnCE/Deployment/Deployment_DataC.h"
#include "Property_Handler.h"
#include "ace/UUID.h"

namespace CIAO
{
  namespace Config_Handlers
  {
    IDREF_Base<CORBA::ULong> ADD_Handler::IDREF;

      void
      ADD_Handler::artifact_deployment_descrs (const deploymentPlan &src,
                                               ::Deployment::ArtifactDeploymentDescriptions &dest)
      {
        DANCE_TRACE("ADD_Handler::atrifact_deployment_descrs");
        deploymentPlan::artifact_const_iterator aci_e =
          src.end_artifact ();
        dest.length (src.count_artifact ());
        CORBA::ULong pos = 0;
        for (deploymentPlan::artifact_const_iterator aci_b =
               src.begin_artifact ();
             aci_e != aci_b;
             ++aci_b)
          {
            ADD_Handler::artifact_deployment_descr (*(*aci_b),
                                                    dest[pos],
                                                    pos);
            pos++;
          }
      }

      void
      ADD_Handler::artifact_deployment_descr (
                                              const ArtifactDeploymentDescription &src,
                                              Deployment::ArtifactDeploymentDescription &dest,
                                              CORBA::ULong pos)
      {
        DANCE_TRACE("ADD_Handler::atrifact_deployment_descr");
        dest.name = ACE_TEXT_ALWAYS_CHAR ( src.name ().c_str ());


        dest.node = ACE_TEXT_ALWAYS_CHAR ( src.node ().c_str ());

        ArtifactDeploymentDescription::location_const_iterator end =
          src.end_location ();

        dest.location.length (src.count_location ());
        CORBA::ULong len = 0;
        for (ArtifactDeploymentDescription::location_const_iterator
               start = src.begin_location ();
             start != end;
             ++start)
          {
            dest.location[len++] = ACE_TEXT_ALWAYS_CHAR ( (*start)->c_str ());
          }

        ArtifactDeploymentDescription::source_const_iterator sce =
          src.end_source ();
        len = 0;
        dest.source.length (src.count_source ());
        for (ArtifactDeploymentDescription::source_const_iterator
               scb = src.begin_source ();
             scb != sce;
             ++scb)
          {
            dest.source[len++] = ACE_TEXT_ALWAYS_CHAR ( (*scb)->c_str ());
          }

        // @@TODO: See this loop is repeated
        ArtifactDeploymentDescription::execParameter_const_iterator adce =
          src.end_execParameter ();
        len = 0;
        dest.execParameter.length (src.count_execParameter ());
        for (ArtifactDeploymentDescription::execParameter_const_iterator adcb =
               src.begin_execParameter ();
             adcb != adce;
             ++adcb)
          {
            Property_Handler::handle_property (*(*adcb),
                                               dest.execParameter[len++]);
          }

        if (src.id_p ())
          {
            ACE_TString cstr (src.id ().c_str ());

            ADD_Handler::IDREF.bind_ref (cstr,pos);
          }
        else
          {
            DANCE_DEBUG (1, (LM_ERROR,
                        "(%P|%t) Warning: ADD %s has no idref.\n",
 ACE_TEXT_ALWAYS_CHAR (                        src.name ().c_str ())));
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
#endif /* 0  */
      }

      ArtifactDeploymentDescription
      ADD_Handler::artifact_deployment_descr (
                                              const Deployment::ArtifactDeploymentDescription &src)
      {
        DANCE_TRACE("ADD_Handler::atrifact_deployment_descr - reverse");
        //Get the name and node and store them in the add
        XMLSchema::string< ACE_TCHAR > name (ACE_TEXT_CHAR_TO_TCHAR (src.name));
        XMLSchema::string< ACE_TCHAR > node (ACE_TEXT_CHAR_TO_TCHAR (src.node));

        ArtifactDeploymentDescription add (name,node);

        //Get the location(s) and store it/them in the add
        size_t total = src.location.length ();
        for (size_t i = 0; i < total; ++i)
          {
            XMLSchema::string< ACE_TCHAR > curr (ACE_TEXT_CHAR_TO_TCHAR (src.location[i]));
            //add.add_location (curr);
          }

        //As above, for the source(s)
        total = src.source.length ();
        for (size_t j = 0; j < total; ++j)
          {
            XMLSchema::string< ACE_TCHAR > curr (ACE_TEXT_CHAR_TO_TCHAR (src.source[j]));
            //add.add_source (curr);
          }

        //As above for the execParameter(s)
        total = src.execParameter.length ();
        for (size_t k = 0; k < total; ++k)
          {
            //add.add_execParameter (Property_Handler::get_property (src.execParameter[k]));
          }

        // Generate a UUID to use for the IDREF.
        ACE_Utils::UUID uuid;
        ACE_Utils::UUID_GENERATOR::instance ()->generate_UUID (uuid);
        ACE_TString add_id (ACE_TEXT ("_"));
        add_id += ACE_TEXT_CHAR_TO_TCHAR (uuid.to_string ()->c_str ());

        XMLSchema::ID< ACE_TCHAR > xml_id (add_id.c_str ());

        // Bind the ref and set it in the IDD
        ADD_Handler::IDREF.bind_next_available (add_id);

        add.id (xml_id);

        return add;
      }
    }
  }
