// $Id$

#include "CRDD_Handler.h"
#include "Any_Handler.h"
#include "Basic_Deployment_Data.hpp"
#include "ciao/Deployment_DataC.h"

namespace CIAO
{
  namespace Config_Handlers
  {

    CRDD_Handler::CRDD_Handler (void)
    {
    }

    CRDD_Handler::~CRDD_Handler (void)
    {
    }

    ///This method takes a <Deployment::ConnectionResourceDeploymentDescription>
    ///and maps the values from the passed in XSC
    ///ConnectionResourceDeploymentDescription to its members.
    void CRDD_Handler::get_ConnectionResourceDeploymentDescription (
                Deployment::ConnectionResourceDeploymentDescription& toconfig,
                ConnectionResourceDeploymentDescription& desc)
    {
      toconfig.targetName = CORBA::string_dup (desc.targetName ().c_str ());
      toconfig.requirementName =
        CORBA::string_dup (desc.requirementName ().c_str ());
      toconfig.resourceName =
        CORBA::string_dup (desc.resourceName ().c_str ());

      Any_Handler::extract_into_any (desc.resourceValue (),
                                     toconfig.resourceValue);
    }
    
    ConnectionResourceDeploymentDescription
    CRDD_Handler::connection_resource_depl_desc (
      const ::Deployment::ConnectionResourceDeploymentDescription& src)
    {
      XMLSchema::string< char > tname ((src.targetName));
      XMLSchema::string< char > reqname ((src.requirementName));
      XMLSchema::string< char > resname ((src.resourceName));
      Any resval (Any_Handler::get_any (src.resourceValue));
      
      ConnectionResourceDeploymentDescription crdd (
                                              tname,
                                              reqname,
                                              resname,
                                              resval);
      return crdd;
    }

  }
}
