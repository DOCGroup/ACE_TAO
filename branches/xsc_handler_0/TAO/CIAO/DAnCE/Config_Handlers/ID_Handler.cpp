
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
    ID_Handler::get_ImplementationDependency (
                    Deployment::ImplementationDependency& toconfig, 
                    ImplementationDependency& desc)
    {


      
      toconfig.requiredType=
           CORBA::string_dup (desc.requiredType ().c_str ());

      
    }

  }

}
