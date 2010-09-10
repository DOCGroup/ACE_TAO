//$Id$

#include "Property_Handler.h"
#include "Any_Handler.h"
#include "Basic_Deployment_Data.hpp"
#include "DAnCE/Deployment/Deployment_DataC.h"
#include "DAnCE/Logger/Log_Macros.h"
namespace DAnCE
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
    Property_Handler::handle_property (const Property& desc,
                                       Deployment::Property& toconfig)
    {
      DANCE_TRACE("Property_Handler::get_property");

      try
        {
          toconfig.name =
            CORBA::string_dup (ACE_TEXT_ALWAYS_CHAR (desc.name ().c_str ()));

          Any_Handler::extract_into_any (desc.value (),
                                         toconfig.value);
        }
      catch (Config_Error &ex)
        {
          ex.name_ = desc.name ();
          throw ex;
        }
    }

    Property
    Property_Handler::get_property (const Deployment::Property& src)
    {
      DANCE_TRACE("Property_Handler::get_property - reverse");

      ::XMLSchema::string< ACE_TCHAR > name (ACE_TEXT_CHAR_TO_TCHAR(src.name));
      Any value (Any_Handler::get_any (src.value));

      Property prop (name,value);

      return prop;
    }

  }
}
