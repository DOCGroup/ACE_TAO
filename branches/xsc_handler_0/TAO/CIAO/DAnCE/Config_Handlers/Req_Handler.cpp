
// $Id$


#include "Req_Handler.h"
#include "Basic_Deployment_Data.hpp"
#include "ciao/Deployment_DataC.h"



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
    Req_Handler::requirement (
                    const Requirement& desc,
                    Deployment::Requirement& toconfig)
    {


      
      toconfig.name=
           CORBA::string_dup (desc.name ().c_str ());
      
      toconfig.resourceType=
           CORBA::string_dup (desc.resourceType ().c_str ());

      
    }

  }

}
