//$Id$

#include "ComponentPropertyDescription_Handler.h"
#include "DataType_Handler.h"
#include "DAnCE/Deployment/Deployment_DataC.h"
#include "ciao/CIAO_common.h"
namespace CIAO
{
  namespace Config_Handlers
  {
    void
    ComponentPropertyDescription_Handler::component_property_description (
                                                                          const ComponentPropertyDescription& desc,
                                                                          ::Deployment::ComponentPropertyDescription& toconfig)
    {
      CIAO_TRACE("ComponentPropertyDescription_Handler::component_property_description");
      toconfig.name = CORBA::string_dup (desc.name ().c_str ());

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
      CIAO_TRACE("ComponentPropertyDescription_Handler::component_property_description - reverse");
      XMLSchema::string< char > name ((src.name));
      DataType dt (DataType_Handler::data_type (src.type));
      ComponentPropertyDescription cpd (name,dt);
      return cpd;
    }

  }
}
