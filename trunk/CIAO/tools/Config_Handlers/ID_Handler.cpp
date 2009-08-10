// $Id$

#include "ID_Handler.h"
#include "Basic_Deployment_Data.hpp"
#include "DAnCE/Deployment/Deployment_DataC.h"
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
    ID_Handler::get_ImplementationDependency (const ImplementationDependency& desc,
                                              Deployment::ImplementationDependency& toconfig)
    {

      CIAO_TRACE("ID_Handler::get_ImplementationDependency");
      
      toconfig.requiredType=
        CORBA::string_dup (ACE_TEXT_ALWAYS_CHAR (desc.requiredType ().c_str ()));
    }

    ImplementationDependency
    ID_Handler::impl_dependency (
                                 const ::Deployment::ImplementationDependency& src)
    {
      CIAO_TRACE("ID_Handler::get_ImplementationDependency - reverse");
      XMLSchema::string< ACE_TCHAR > reqtype (ACE_TEXT_CHAR_TO_TCHAR (src.requiredType));

      ImplementationDependency id (reqtype);

      return id;
    }

  }

}
