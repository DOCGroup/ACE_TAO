// $Id$

#include "Req_Handler.h"
#include "Property_Handler.h"
#include "ciao/DeploymentC.h"

namespace CIAO
{
  namespace Config_Handlers
  {

    Req_Handler::Req_Handler (void)
    {
    }

    Req_Handler::~Req_Handler (void)
    {
    }

    void
    Req_Handler::get_Requirement (
                         Deployment::Requirement& toconfig,
                         Requirement& desc)
    {
      //Map the basic string types to their Deployment::Req
      //counterparts.
      toconfig.name = CORBA::string_dup (desc.name ().c_str ());
      toconfig.resourceType =
        CORBA::string_dup (desc.resourceType ().c_str ());

      //Map the XSC Req's property into the next
      //position in the IDL Req's sequence.
#if 0
      /// @@ MAJO:
      Property_Handler prophandler;
      toconfig.property.length (toconfig.property.length () + 1);
      Property_Handler::get_property (
        toconfig.property[toconfig.property.length () - 1],
        desc.property ());
#endif /*if 0*/
    }

  }
}
