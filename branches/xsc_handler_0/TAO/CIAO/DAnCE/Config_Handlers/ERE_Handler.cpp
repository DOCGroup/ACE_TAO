// $Id$

#include "ERE_Handler.h"
#include "Basic_Deployment_Data.hpp"
#include "ciao/Deployment_DataC.h"

namespace CIAO
{
  namespace Config_Handlers
  {

    ERE_Handler::ERE_Handler (void)
    {
    }

    ERE_Handler::~ERE_Handler (void)
    {
    }
 
    ///This method takes a <Deployment::ExternalReferenceEndpoint>
    ///and maps the values from the passed in XSC 
    ///ExternalReferenceEndpoint to its members.
    void ERE_Handler::get_ExternalReferenceEndpoint (
                Deployment::ExternalReferenceEndpoint& toconfig,
                ExternalReferenceEndpoint& desc)
    {
      toconfig.location = CORBA::string_dup (desc.location ().c_str ());
    }
    
  }
}
