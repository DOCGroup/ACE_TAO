//$Id$

#include "Property_Handler.h"
#include "Any_Handler.h"
#include "Basic_Deployment_Data.hpp"
#include "DAnCE/Deployment/Deployment_DataC.h"
#include "ciao/CIAO_common.h"
namespace CIAO
{
  namespace Config_Handlers
  {

    Property_Handler::Property_Handler (void)
    {
    }

    Property_Handler::~Property_Handler (void)
    {
    }

    void
    Property_Handler::handle_property (
                                    const Property& desc,
                                    Deployment::Property& toconfig)
    {
      CIAO_TRACE("Property_Handler::get_property");

      toconfig.name =
        CORBA::string_dup (ACE_TEXT_ALWAYS_CHAR (desc.name ().c_str ()));

      Any_Handler::extract_into_any (desc.value (),
                                     toconfig.value);

    }

    Property
    Property_Handler::get_property (
                                    const Deployment::Property& src)
    {
      CIAO_TRACE("Property_Handler::get_property - reverse");

      ::XMLSchema::string< ACE_TCHAR > name (ACE_TEXT_CHAR_TO_TCHAR (src.name.in ()));
      Any value (Any_Handler::get_any (src.value));

      Property prop (name,value);

      return prop;
    }

  }
}
