
// $Id$


#include "IDD_Handler.h"
#include "Basic_Deployment_Data.hpp"
#include "ciao/Deployment_DataC.h"
#include "MDD_Handler.h"
#include "Property_Handler.h"
#include "IRDD_Handler.h"



namespace CIAO
{
  namespace Config_Handlers
  {

    IDD_Handler::IDD_Handler (void)
    {
    }

    IDD_Handler::~IDD_Handler (void)
    {
    }


    void
    IDD_Handler::instance_deployment_descr (
                    const InstanceDeploymentDescription& desc,
                    Deployment::InstanceDeploymentDescription& toconfig)
    {


      
      toconfig.name=
           CORBA::string_dup (desc.name ().c_str ());
      
      toconfig.node=
           CORBA::string_dup (desc.node ().c_str ());
      
      CORBA::ULong source_length = toconfig.source.length ();
         toconfig.source.length (source_length + 1);
         toconfig.source[source_length - 1]=
           CORBA::string_dup (desc.source ().c_str ());
      
      Singleton_IDREF_Map::instance ()->find_ref (
        toconfig.implementation,
        desc.implementation);
      
      CORBA::ULong configProperty_length = toconfig.configProperty.length ();
     
      toconfig.configProperty.length (configProperty_length + 1);
      Property_Handler::property (
        desc.configProperty (),
        toconfig.configProperty[configProperty_length - 1]);
      
      CORBA::ULong deployedProperty_length = toconfig.deployedProperty.length ();
     
      toconfig.deployedProperty.length (deployedProperty_length + 1);
      IRDD_Handler::instance_resource_deployment_descr (
        desc.deployedProperty (),
        toconfig.deployedProperty[deployedProperty_length - 1]);
      
      CORBA::ULong deployedSharedProperty_length = toconfig.deployedSharedProperty.length ();
     
      toconfig.deployedSharedProperty.length (deployedSharedProperty_length + 1);
      IRDD_Handler::instance_resource_deployment_descr (
        desc.deployedSharedProperty (),
        toconfig.deployedSharedProperty[deployedSharedProperty_length - 1]);

      
    }

  }

}
