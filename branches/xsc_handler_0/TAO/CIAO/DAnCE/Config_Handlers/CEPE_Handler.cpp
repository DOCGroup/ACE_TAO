// $Id$

#include "CEPE_Handler.h"
#include "Basic_Deployment_Data.hpp"
#include "ciao/Deployment_DataC.h"

namespace CIAO
{
  namespace Config_Handlers
  {

    CEPE_Handler::CEPE_Handler (void)
    {
    }

    CEPE_Handler::~CEPE_Handler (void)
    {
    }
 
    ///This method takes a <Deployment::ComponentExternalPortEndpoint>
    ///and maps the values from the passed in XSC 
    ///ComponentExternalPortEndpoint to its members.
    void CEPE_Handler::get_ComponentExternalPortEndpoint (
                Deployment::ComponentExternalPortEndpoint& toconfig,
                ComponentExternalPortEndpoint& desc)
    {
      toconfig.portName = CORBA::string_dup (desc.portName ().c_str ());
    }
    
  }
}

