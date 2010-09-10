//$Id$

#include "ComponentPropertyDescription_Handler.h"
#include "DataType_Handler.h"
#include "DAnCE/Deployment/Deployment_DataC.h"
#include "DAnCE/Logger/Log_Macros.h"
namespace DAnCE
{
  namespace Config_Handlers
  {
    void
    ComponentPropertyDescription_Handler::component_property_description (
                                                                          const ComponentPropertyDescription& desc,
                                                                          ::Deployment::ComponentPropertyDescription& toconfig)
    {
      DANCE_TRACE("ComponentPropertyDescription_Handler::component_property_description");
      toconfig.name = CORBA::string_dup (ACE_TEXT_ALWAYS_CHAR (desc.name ().c_str ()));

      // Delegate the DataType to the
      // DataType_Handler.

      // @@ There is a lurking bug here.
      //#if 0
      CORBA::TypeCode_ptr tcptr = toconfig.type.in ();
      DataType_Handler::data_type (desc.type (), tcptr);
      //#endif /*if 0*/
    }

    ComponentPropertyDescription
    ComponentPropertyDescription_Handler::component_property_description (
                                                                          const ::Deployment::ComponentPropertyDescription &src)
    {
      DANCE_TRACE("ComponentPropertyDescription_Handler::component_property_description - reverse");
      XMLSchema::string< ACE_TCHAR > name (ACE_TEXT_CHAR_TO_TCHAR (src.name));
      DataType dt (DataType_Handler::data_type (src.type));
      ComponentPropertyDescription cpd (name,dt);
      return cpd;
    }

  }
}
