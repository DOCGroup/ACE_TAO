// $Id$

#include "ID_Handler.h"
#include "Basic_Deployment_Data.hpp"
#include "DAnCE/Deployment/Deployment_DataC.h"
#include "DAnCE/Logger/Log_Macros.h"
namespace DAnCE
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
    ID_Handler::get_ImplementationDependency (const ImplementationDependency& desc,
                                              Deployment::ImplementationDependency& toconfig)
    {

      DANCE_TRACE("ID_Handler::get_ImplementationDependency");

      toconfig.requiredType=
        CORBA::string_dup (ACE_TEXT_ALWAYS_CHAR (desc.requiredType ().c_str ()));
    }

    ImplementationDependency
    ID_Handler::impl_dependency (
                                 const ::Deployment::ImplementationDependency& src)
    {
      DANCE_TRACE("ID_Handler::get_ImplementationDependency - reverse");
      XMLSchema::string< ACE_TCHAR > reqtype (ACE_TEXT_CHAR_TO_TCHAR (src.requiredType));

      ImplementationDependency id (reqtype);

      return id;
    }

  }

}
