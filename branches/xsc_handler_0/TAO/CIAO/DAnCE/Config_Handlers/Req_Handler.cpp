// $Id$

#include "Req_Handler.h"
#include "Prop_Handler.h"
#include "ciao/DeploymentC.h"

namespace CIAO
{
  namespace Config_Handlers
  {

    Requirement_Handler::Requirement_Handler (void)
    {
    }

    Requirement_Handler::~Requirement_Handler (void)
    {
    }

    void
    Requirement_Handler::get_Requirement (
                         Deployment::Requirement& toconfig,
                         Requirement& desc)
    {
      //Map the basic string types to their Deployment::Requirement
      //counterparts.
      toconfig.name = CORBA::string_dup (desc.name ().c_str ());
      toconfig.resourceType = 
        CORBA::string_dup (desc.resourceType ().c_str ()); 
        
      //Map the XSC Requirement's property into the next
      //position in the IDL Requirement's sequence.
      Prop_Handler prophandler;
      toconfig.property.length (toconfig.property.length () + 1);
      prophandler.get_Property (
        toconfig.property[toconfig.property.length () - 1],
        desc.property ());
    }
    
  }
}
