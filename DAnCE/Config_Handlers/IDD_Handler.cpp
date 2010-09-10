// $Id$

#include "ace/UUID.h"
#include "IDD_Handler.h"
#include "IRDD_Handler.h"
#include "MDD_Handler.h"
#include "Property_Handler.h"
#include "Any_Handler.h"
#include "Basic_Deployment_Data.hpp"
#include "DAnCE/Deployment/Deployment_DataC.h"
#include "cdp.hpp"

namespace DAnCE
{
  namespace Config_Handlers
  {
    IDREF_Base<CORBA::ULong> IDD_Handler::IDREF;

    void
    IDD_Handler::instance_deployment_descrs (const deploymentPlan &src,
                                             Deployment::InstanceDeploymentDescriptions& dest)
    {
      DANCE_TRACE("IDD_Handler::instance_deployment_descrs");
      deploymentPlan::instance_const_iterator idd_e =
        src.end_instance ();

      CORBA::ULong pos = 0;
      dest.length (src.count_instance ());
      for (deploymentPlan::instance_const_iterator idd_b =
             src.begin_instance ();
           idd_b != idd_e;
           ++idd_b)
        {
          IDD_Handler::instance_deployment_descr (*(*idd_b),
                                                  dest[pos], pos);
          pos++;
        }
    }

    void
    IDD_Handler::instance_deployment_descr (
                                            const InstanceDeploymentDescription& src,
                                            Deployment::InstanceDeploymentDescription& dest,
                                            CORBA::ULong pos)
    {
      DANCE_TRACE("IDD_Handler::instance_deployment_descr");
      try
        {
          dest.name = ACE_TEXT_ALWAYS_CHAR ( src.name ().c_str ());
          dest.node = ACE_TEXT_ALWAYS_CHAR ( src.node ().c_str ());

          if (src.id_p ())
            {
              ACE_TString cstr (src.id ().c_str ());
              IDD_Handler::IDREF.bind_ref (cstr, pos);
            }
          else
            {
              DANCE_ERROR (2, (LM_WARNING,
                               "(%P|%t) Warning:  IDD %s has no idref\n",
                               ACE_TEXT_ALWAYS_CHAR (src.name ().c_str ())));
            }

          // We know there should be only one element
          dest.source.length (1);
          dest.source [0] =
            ACE_TEXT_ALWAYS_CHAR (src.source ().c_str ());

          CORBA::ULong tmp = 0;
          MDD_Handler::IDREF.find_ref (ACE_TString (src.implementation ().idref ().id ().c_str ()), tmp);

          dest.implementationRef = tmp;

          dest.configProperty.length (src.count_configProperty ());
          std::for_each (src.begin_configProperty (),
                         src.end_configProperty (),
                         Property_Functor (dest.configProperty));

          dest.deployedResource.length (src.count_deployedResource ());
          std::for_each (src.begin_deployedResource (),
                         src.end_deployedResource (),
                         IRDD_Functor (dest.deployedResource));

          if (src.deployedSharedResource_p ())
            {
              dest.deployedSharedResource.length (1);
              IRDD_Handler::handle_irdd (src.deployedSharedResource (),
                                         dest.deployedSharedResource[0]);
            }

        }
      catch (Config_Error &ex)
        {
          ex.name_ = src.name ()  + ACE_TEXT (":") + ex.name_;
          throw ex;
        }
      catch (...)
        {
          DANCE_ERROR (1, (LM_ERROR, ACE_TEXT ("Unknown exception at IDD:%s"),
                           src.name ().c_str ()));
          throw Config_Error (src.name (), ACE_TEXT ("Unknown exception"));
        }

      // Done!
    }

    InstanceDeploymentDescription
    IDD_Handler::instance_deployment_descr (
                                            const Deployment::InstanceDeploymentDescription& src)
    {
      DANCE_TRACE("IDD_Handler::instance_deployment_descr - reverse");
      //Get all the string/IDREFs
      XMLSchema::string < ACE_TCHAR > name (ACE_TEXT_CHAR_TO_TCHAR (src.name));
      XMLSchema::string < ACE_TCHAR > node (ACE_TEXT_CHAR_TO_TCHAR (src.node));
      XMLSchema::string < ACE_TCHAR > source (ACE_TEXT (""));
      if (src.source.length () > 0)
        {
          XMLSchema::string < ACE_TCHAR > source_detail (ACE_TEXT_CHAR_TO_TCHAR (src.source[0]));
          source = source_detail;
        }
      ACE_TString temp;
      MDD_Handler::IDREF.find_ref(src.implementationRef, temp);
      XMLSchema::IDREF< ACE_TCHAR > implementation ((temp.c_str()));

      // Instantiate the IDD
      IdRef xid;
      xid.idref (implementation);
      InstanceDeploymentDescription idd (name, node, source, xid);

      //Get and store the configProperty(s)
      size_t total = src.configProperty.length();
      for(size_t j = 0; j < total; j++)
        {
          //idd.add_configProperty(Property_Handler::get_property (src.configProperty[j]));
        }

      // Get and store the deployedResource(s)
      size_t dp_total = src.deployedResource.length ();
      for (size_t k = 0; k < dp_total; k++)
        {
          //idd.add_deployedResource (IRDD_Handler::instance_resource_deployment_descr (src.deployedResource[k]));
        }

      //Check if there is a deployedSharedResource, if so store it
      if(src.deployedSharedResource.length() != 0)
        idd. deployedSharedResource(IRDD_Handler::instance_resource_deployment_descr(src.deployedSharedResource[0]));

      // Generate a UUID to use for the IDREF.
      ACE_Utils::UUID uuid;
      ACE_Utils::UUID_GENERATOR::instance ()->generate_UUID (uuid);
      ACE_TString idd_id (ACE_TEXT ("_"));
      idd_id += ACE_TEXT_CHAR_TO_TCHAR (uuid.to_string ()->c_str ());

      XMLSchema::ID< ACE_TCHAR > xml_id (idd_id.c_str ());

      // Bind the ref and set it in the IDD
      IDD_Handler::IDREF.bind_next_available (idd_id);

      idd.id (xml_id);

      return idd;
    }

  }
}
