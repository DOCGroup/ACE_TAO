// $Id$

#include "PSPE_Handler.h"
#include "Basic_Deployment_Data.hpp"


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
          toconfig.provider.length (
            toconfig.provider.length () + 1);
          toconfig.provider[toconfig.provider.length () - 1] = 
            CORBA::string_dup (desc.provider ().c_str ());
        }
        
        
        
        
      
    }
    
  }
}
