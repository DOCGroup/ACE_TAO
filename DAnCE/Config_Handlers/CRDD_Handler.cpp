// $Id$

#include "CRDD_Handler.h"
#include "Any_Handler.h"
#include "Basic_Deployment_Data.hpp"
#include "DAnCE/Deployment/Deployment_DataC.h"
#include "Property_Handler.h"

#include "DAnCE/Logger/Log_Macros.h"

namespace DAnCE
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
      DANCE_TRACE("CRDD_Handler::get_ConnectionResourceDD");

      toconfig.targetName = ACE_TEXT_ALWAYS_CHAR ( desc.targetName ().c_str ());
      toconfig.requirementName = ACE_TEXT_ALWAYS_CHAR ( desc.requirementName ().c_str ());
      toconfig.resourceName = ACE_TEXT_ALWAYS_CHAR ( desc.resourceName ().c_str ());

      std::for_each (desc.begin_property (),
                     desc.end_property (),
                     Property_Functor (toconfig.property));
    }

    ConnectionResourceDeploymentDescription
    CRDD_Handler::connection_resource_depl_desc (
                                                 const ::Deployment::ConnectionResourceDeploymentDescription& src)
    {
      DANCE_TRACE("CRDD_Handler::get_ConnectionResourceDeploymentDescription- reverse");
      XMLSchema::string< ACE_TCHAR > tname (ACE_TEXT_CHAR_TO_TCHAR (src.targetName));
      XMLSchema::string< ACE_TCHAR > reqname (ACE_TEXT_CHAR_TO_TCHAR(src.requirementName));
      XMLSchema::string <ACE_TCHAR> resname (ACE_TEXT_CHAR_TO_TCHAR (src.resourceName));

      ConnectionResourceDeploymentDescription crdd (tname,
                                                    reqname,
                                                    resname);

      for (CORBA::ULong i = 0;
           i != src.property.length ();
           ++i)
        {
          //crdd.add_property (Property_Handler::get_property (src.property[i]));
        }


      return crdd;
    }

  }
}
