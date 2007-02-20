// $Id$

#include "CRDD_Handler.h"
#include "Any_Handler.h"
#include "Basic_Deployment_Data.hpp"
#include "DAnCE/Deployment/Deployment_DataC.h"
#include "Property_Handler.h"

#include "ciao/CIAO_common.h"

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
    void CRDD_Handler::get_ConnectionResourceDeploymentDescription (Deployment::ConnectionResourceDeploymentDescription& toconfig,
                                                                    const ConnectionResourceDeploymentDescription& desc)
    {
      CIAO_TRACE("CRDD_Handler::get_ConnectionResourceDD");

      toconfig.targetName = desc.targetName ().c_str ();
      toconfig.requirementName = desc.requirementName ().c_str ();
      toconfig.resourceName = desc.resourceName ().c_str ();

      std::for_each (desc.begin_property (),
                     desc.end_property (),
                     Property_Functor (toconfig.property));
    }

    ConnectionResourceDeploymentDescription
    CRDD_Handler::connection_resource_depl_desc (
                                                 const ::Deployment::ConnectionResourceDeploymentDescription& src)
    {
      CIAO_TRACE("CRDD_Handler::get_ConnectionResourceDeploymentDescription- reverse");
      XMLSchema::string< ACE_TCHAR > tname ((src.targetName));
      XMLSchema::string< ACE_TCHAR > reqname ((src.requirementName));
      XMLSchema::string <ACE_TCHAR> resname ((src.resourceName));

      ConnectionResourceDeploymentDescription crdd (tname,
                                                    reqname,
                                                    resname);

      for (CORBA::ULong i = 0;
           i != src.property.length ();
           ++i)
        {
          crdd.add_property (Property_Handler::get_property (src.property[i]));
        }


      return crdd;
    }

  }
}
