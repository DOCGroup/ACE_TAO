// $Id$

#include "Req_Handler.h"
#include "Property_Handler.h"
#include "ciao/DeploymentC.h"
#include "ciao/CIAO_common.h"
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
      CIAO_TRACE("Req_Handler::get_Requirement");
      
      //Map the basic string types to their Deployment::Req
      //counterparts.
      toconfig.name = CORBA::string_dup (desc.name ().c_str ());
      toconfig.resourceType =
        CORBA::string_dup (desc.resourceType ().c_str ());

      //Map the XSC Req's property into the next
      //position in the IDL Req's sequence.
      /// @@ MAJO:
      Property_Handler prophandler;
      toconfig.property.length (toconfig.property.length () + 1);
      Property_Handler::get_property (
        desc.property (),
        toconfig.property[toconfig.property.length () - 1]);
    }
    
    Requirement
    Req_Handler::get_requirement (const Deployment::Requirement& src)
    {
      CIAO_TRACE("Req_Handler::get_requirement - reverse")
      //Get the values for name and res
      XMLSchema::string< char > name ((src.name));
      XMLSchema::string< char > res ((src.resourceType));
      
      //Get the Property
      Property prop (Property_Handler::get_property (src.property[0]));
      
      //Instantiate the Requirement
      Requirement req (name,res,prop);
      
      return req;
    }
  }
}
