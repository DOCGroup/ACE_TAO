// $Id$


#include "ID_Handler.h"
#include "Basic_Deployment_Data.hpp"
#include "ciao/Deployment_DataC.h"
#include "ciao/CIAO_common.h"
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
      CIAO_TRACE("ID_Handler::get_ImplementationDependency");
      
      toconfig.requiredType=
           CORBA::string_dup (desc.requiredType ().c_str ());
    }

    ImplementationDependency
    ID_Handler::impl_dependency (
        const ::Deployment::ImplementationDependency& src)
    {
      CIAO_TRACE("ID_Handler::get_ImplementationDependency - reverse");
      XMLSchema::string< char > reqtype ((src.requiredType));

      ImplementationDependency id (reqtype);

      return id;
    }

  }

}
