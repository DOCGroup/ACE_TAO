// $Id$
#include "STD_CID_Handler.h"
#include "STD_CPD_Handler.h"
#include "STD_IAD_Handler.h"
#include "Basic_Deployment_Data.hpp"
#include "DAnCE/Deployment/Deployment_DataC.h"
#include "Utils/XML_Helper.h"
#include "Deployment.hpp"
#include "Property_Handler.h"

namespace DAnCE
{
  namespace Config_Handlers
  {
    IDREF_Base<CORBA::ULong> STD_CID_Handler::IDREF;

    bool
    STD_CID_Handler::component_impl_descr (
      const ComponentImplementationDescription &desc,
      ::Deployment::ComponentImplementationDescription &toconfig)
    {
      toconfig.label =
        desc.label ().c_str ();

      toconfig.UUID =
        desc.UUID ().c_str ();


      if (desc.assemblyImpl_p ())
      {
        Deployment::ComponentAssemblyDescription idl_cad;
        ComponentAssemblyDescription cad =
          desc.assemblyImpl ();

        // @@: handle <assemblyImpl>
        for (ComponentAssemblyDescription::instance_const_iterator iter =
               cad.begin_instance ();
             iter != cad.end_instance ();
             iter++)
        {
          CORBA::ULong len =
            idl_cad.instance.length ();
          idl_cad.instance.length (len + 1);
          idl_cad.instance [len].name =
            (*iter).name ().c_str ();

          // Bind the instance id with the index
          if ((*iter).id_p ())
          {
            ACE_CString cstr ((*iter).id ().c_str ());
            
        STD_CID_Handler::IDREF.bind_ref (cstr,len);
      }

          if (!this->xml_helper_->is_initialized ())
            return false;

          // parse the .cpd (ComponentPackageDescription) file
          // that <package> links to
          xercesc::DOMDocument* dom =
            this->xml_helper_->create_dom (((*iter).package ()).href ().c_str ());

          if (!dom)
            return false;

          ComponentPackageDescription cpd =
            componentPackageDescription (dom);

          Deployment::ComponentPackageDescription idl_cpd;

          STD_CPD_Handler cpd_handler (this->xml_helper_);

          cpd_handler.component_package_descr (cpd,
                                               idl_cpd);

          idl_cad.instance [len].package.length (1);
          idl_cad.instance [len].package [0] = idl_cpd;
        }

        // @@: handle <connection>
        for (ComponentAssemblyDescription::connection_const_iterator conn_iter =
               cad.begin_connection ();
             conn_iter != cad.end_connection ();
             conn_iter++)
        {
          CORBA::ULong conn_len =
            idl_cad.connection.length ();

          idl_cad.connection.length (conn_len + 1);
          idl_cad.connection [conn_len].name =
            (*conn_iter).name ().c_str ();


          for (AssemblyConnectionDescription::internalEndpoint_const_iterator ep_iter =
                 (*conn_iter).begin_internalEndpoint ();
               ep_iter != (*conn_iter).end_internalEndpoint ();
               ep_iter++)
          {
            CORBA::ULong endp_len =
              idl_cad.connection [conn_len].internalEndpoint.length ();
            idl_cad.connection [conn_len].internalEndpoint.length (endp_len + 1);
            idl_cad.connection [conn_len].internalEndpoint [endp_len].portName =
              (*ep_iter).portName ().c_str ();

            CORBA::ULong tmp = 0;
        STD_CID_Handler::IDREF.find_ref (ACE_CString ((((*ep_iter).instance ()).id ()).c_str ()),
                         tmp);
            CORBA::ULong ref = tmp;
            idl_cad.connection[conn_len].internalEndpoint [endp_len].instanceRef = ref;
          }
        }

        toconfig.assemblyImpl.length (1);
        toconfig.assemblyImpl [0] = idl_cad;
      }

      // handle <monolithicImpl>
      if (desc.monolithicImpl_p ())
      {
        Deployment::MonolithicImplementationDescription idl_mid;
        MonolithicImplementationDescription mid =
          desc.monolithicImpl ();

        for (MonolithicImplementationDescription::primaryArtifact_const_iterator iter =
               mid.begin_primaryArtifact ();
             iter != mid.end_primaryArtifact ();
             iter++)
        {
          CORBA::ULong len =
            idl_mid.primaryArtifact.length ();
          idl_mid.primaryArtifact.length (len + 1);
          idl_mid.primaryArtifact [len].name = (*iter).name ().c_str ();

          if (!this->xml_helper_->is_initialized ())
            return false;

          // parse the .iad (ImplementationArtifactDescription) file
          // that the <referencedArtifact> links to
          xercesc::DOMDocument* dom =
            this->xml_helper_->create_dom (((*iter).referencedArtifact ()).href ().c_str ()); // here location indicates .iad file

          if (!dom)
            return false;

          ImplementationArtifactDescription iad =
            implementationArtifactDescription (dom);

          Deployment::ImplementationArtifactDescription idl_iad;

          STD_IAD_Handler::impl_artifact_descr (iad,
                                                idl_iad);

          idl_mid.primaryArtifact [len].referencedArtifact = idl_iad;

        }
        toconfig.monolithicImpl.length (1);
        toconfig.monolithicImpl [0] = idl_mid;
      }

      // handle <configProperty>
      if (desc.configProperty_p ())
      {
        Property p = desc.configProperty ();
        Deployment::Property idl_p;
        Property_Handler::get_property (p, idl_p);
        toconfig.configProperty.length (1);
        toconfig.configProperty [0] = idl_p;
      }

      return true;
    }
  }
}
