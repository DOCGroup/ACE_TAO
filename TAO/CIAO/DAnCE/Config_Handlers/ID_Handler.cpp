
// $Id$


#include "ID_Handler.h"
#include "Basic_Deployment_Data.hpp"
#include "ciao/Deployment_DataC.h"



namespace CIAO
{
  namespace Config_Handlers
  {

    ID_Handler::ID_Handler (void)
    {
    }

    ID_Handler::~ID_Handler (void)
    {
    }


    void
    ID_Handler::implementation_dependency (
                    const ImplementationDependency& desc,
                    Deployment::ImplementationDependency& toconfig)
    {


      
      toconfig.requiredType=
           CORBA::string_dup (desc.requiredType ().c_str ());

      
    }

  }

}
