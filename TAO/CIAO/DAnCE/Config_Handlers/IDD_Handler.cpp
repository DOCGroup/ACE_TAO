// $Id$

#include "IDD_Handler.h"
#include "Basic_Deployment_Data.hpp"
#include "ciao/Deployment_DataC.h"

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
 
    ///This method takes a <Deployment::InstanceDeploymentDescription>
    ///and maps the values from the passed in XSC 
    ///InstanceDeploymentDescription to its members.
    void IDD_Handler::get_InstanceDeploymentDescription (
                Deployment::InstanceDeploymentDescription& toconfig,
                InstanceDeploymentDescription& desc)
    {

    }
    
  }
}
