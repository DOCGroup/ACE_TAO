
// $Id$


#include "RDD_Handler.h"
#include "Basic_Deployment_Data.hpp"
#include "ciao/Deployment_DataC.h"
#include "Any_Handler.h"



namespace CIAO
{
  namespace Config_Handlers
  {

    RDD_Handler::RDD_Handler (void)
    {
    }

    RDD_Handler::~RDD_Handler (void)
    {
    }


    void
    RDD_Handler::get_ResourceDeploymentDescription (
                    Deployment::ResourceDeploymentDescription& toconfig, 
                    ResourceDeploymentDescription& desc)
    {


      
      toconfig.requirementName=
           CORBA::string_dup (desc.requirementName ().c_str ());
      
      toconfig.resourceName=
           CORBA::string_dup (desc.resourceName ().c_str ());
      
      Any_Handler resourceValue_handler;
         resourceValue_handler.get_Any (
           toconfig.resourceValue,
           desc.resourceValue ());

      
    }

  }

}
