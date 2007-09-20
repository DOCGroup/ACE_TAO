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

namespace CIAO
{
  namespace Config_Handlers
  {
    IDREF_Base<CORBA::ULong> IDD_Handler::IDREF;

    void
    IDD_Handler::instance_deployment_descrs (const DeploymentPlan &src,
                                             Deployment::InstanceDeploymentDescriptions& dest)
    {
      CIAO_TRACE("IDD_Handler::instance_deployment_descrs");
      DeploymentPlan::instance_const_iterator idd_e =
        src.end_instance ();

      CORBA::ULong pos = 0;
      dest.length (src.count_instance ());
      for (DeploymentPlan::instance_const_iterator idd_b =
             src.begin_instance ();
           idd_b != idd_e;
           ++idd_b)
        {
          IDD_Handler::instance_deployment_descr ((*idd_b),
                                                  dest[pos], pos);
          pos++;
        }
    }

    void
    IDD_Handler::instance_deployment_descr (const InstanceDeploymentDescription& src,
                                            Deployment::InstanceDeploymentDescription& dest,
                                            CORBA::ULong pos)
    {
      CIAO_TRACE("IDD_Handler::instance_deployment_descr");
      try
        {
          dest.name = src.name ().c_str ();
          dest.node = src.node ().c_str ();

          if (src.id_p ())
            {
              ACE_CString cstr (src.id ().c_str ());
              IDD_Handler::IDREF.bind_ref (cstr, pos);
            }
          else
            {
              ACE_DEBUG((LM_ERROR,
                         "(%P|%t) Warning:  IDD %s has no idref \n",
                         src.name ().c_str ()));
            }

          // We know there should be only one element
          dest.source.length (1);
          dest.source [0] =
            src.source ().c_str ();

          CORBA::ULong tmp = 0;
          MDD_Handler::IDREF.find_ref
            (ACE_CString (src.implementation ().id ().c_str ()), tmp);

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
          ex.name_ = src.name ()  + ":" + ex.name_;
          throw ex;
        }
      // Done!
    }

    InstanceDeploymentDescription
    IDD_Handler::instance_deployment_descr (const Deployment::InstanceDeploymentDescription& src)
    {
      CIAO_TRACE("IDD_Handler::instance_deployment_descr - reverse");
      //Get all the string/IDREFs
      XMLSchema::string < ACE_TCHAR > name ((src.name));
      XMLSchema::string < ACE_TCHAR > node ((src.node));
      XMLSchema::string < ACE_TCHAR > source ("");
      if (src.source.length () > 0)
        {
          XMLSchema::string < ACE_TCHAR > source_detail (src.source[0]);
          source = source_detail;
        }
      ACE_CString temp;
      MDD_Handler::IDREF.find_ref(src.implementationRef, temp);
      XMLSchema::IDREF< ACE_TCHAR > implementation ((temp.c_str()));

      // Instantiate the IDD
      InstanceDeploymentDescription idd (name, node, source, implementation);

      //Get and store the configProperty(s)
      size_t total = src.configProperty.length();
      for(size_t j = 0; j < total; j++)
        {
          idd.add_configProperty(Property_Handler::get_property (src.configProperty[j]));
        }

      // Get and store the deployedResource(s)
      size_t dp_total = src.deployedResource.length ();
      for (size_t k = 0; k < dp_total; k++)
        {
          idd.add_deployedResource (IRDD_Handler::instance_resource_deployment_descr (src.deployedResource[k]));
        }

      //Check if there is a deployedSharedResource, if so store it
      if(src.deployedSharedResource.length() != 0)
        idd. deployedSharedResource(IRDD_Handler::instance_resource_deployment_descr(src.deployedSharedResource[0]));

      // Generate a UUID to use for the IDREF.
      ACE_Utils::UUID uuid;
      ACE_Utils::UUID_GENERATOR::instance ()->generate_UUID (uuid);
      ACE_CString idd_id ("_");
      idd_id += *uuid.to_string ();

      XMLSchema::ID< ACE_TCHAR > xml_id (idd_id.c_str ());

      // Bind the ref and set it in the IDD
      IDD_Handler::IDREF.bind_next_available (idd_id);

      idd.id (xml_id);

      return idd;
    }
  }
}
