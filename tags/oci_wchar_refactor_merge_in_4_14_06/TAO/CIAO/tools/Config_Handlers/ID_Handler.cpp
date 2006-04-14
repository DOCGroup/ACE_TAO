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
                    const ImplementationDependency& desc)
    {
      toconfig.requiredType=
           CORBA::string_dup (desc.requiredType ().c_str ());
    }

    ImplementationDependency
    ID_Handler::impl_dependency (
        const ::Deployment::ImplementationDependency& src)
    {
      XMLSchema::string< char > reqtype ((src.requiredType));

      ImplementationDependency id (reqtype);

      return id;
    }

  }

}
