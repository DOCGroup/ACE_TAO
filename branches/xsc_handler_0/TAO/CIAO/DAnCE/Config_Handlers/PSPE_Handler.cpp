// $Id$

#include "PSPE_Handler.h"
#include "Basic_Deployment_Data.hpp"
#include "ciao/Deployment_DataC.h"

namespace CIAO
{
  namespace Config_Handlers
  {

    PSPE_Handler::PSPE_Handler (void)
    {
    }

    PSPE_Handler::~PSPE_Handler (void)
    {
    }
 
    ///This method takes a <Deployment::PlanSubcomponentPortEndpoint>
    ///and maps the values from the passed in XSC 
    ///PlanSubcomponentPortEndpoint to its members.
    void PSPE_Handler::get_PlanSubcomponentPortEndpoint (
                Deployment::PlanSubcomponentPortEndpoint& toconfig,
                PlanSubcomponentPortEndpoint& desc)
    {
      toconfig.portName = CORBA::string_dup (desc.portName ().c_str ());
      
      if (desc.provider_p ())
        {
          toconfig.provider = !(desc.provider ().empty ());
        }
      else
        {
          toconfig.provider = 0; 
        }  
      
    }
    
  }
}
