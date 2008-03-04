// $Id$

#include "Req_Handler.h"
#include "Property_Handler.h"
#include "DAnCE/Deployment/DeploymentC.h"
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
    Req_Handler::handle_requirement (const Requirement& desc,
                     Deployment::Requirement& toconfig)
    {
      CIAO_TRACE("Req_Handler::get_Requirement");

      //Map the basic string types to their Deployment::Req
      //counterparts.
      toconfig.name = desc.name ().c_str ();
      toconfig.resourceType = desc.resourceType ().c_str ();

      toconfig.property.length (desc.count_property ());
      std::for_each (desc.begin_property (),
                     desc.end_property (),
                     Property_Functor (toconfig.property));
    }

    Requirement
    Req_Handler::get_requirement (const Deployment::Requirement& src)
    {
      CIAO_TRACE("Req_Handler::get_requirement - reverse");

      //Get the values for name and res
      XMLSchema::string< char > name ((src.name));
      XMLSchema::string< char > res ((src.resourceType));

      //Instantiate the Requirement
      Requirement req (name,res);

      for (CORBA::ULong i = 0; i < src.property.length (); ++i)
        {
          req.add_property (Property_Handler::get_property (src.property[i]));
        }

      return req;
    }
  }
}
