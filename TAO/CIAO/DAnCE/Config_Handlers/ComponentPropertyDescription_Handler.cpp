//$Id$

#include "ComponentPropertyDescription_Handler.h"
#include "DataType_Handler.h"
#include "ciao/Deployment_DataC.h"

namespace CIAO
{
  namespace Config_Handlers
  {
    void
    ComponentPropertyDescription_Handler::component_property_description (
        const ComponentPropertyDescription& desc,
        ::Deployment::ComponentPropertyDescription& toconfig)
    {
      toconfig.name = CORBA::string_dup (desc.name ().c_str ());

      // Delegate the DataType to the
      // DataType_Handler.

      // @@ There is a lurking bug here.
#if 0
      DataType_Handler dtypehandler;
      CORBA::TypeCode_ptr tcptr = toconfig.type.in ();
      dtypehandler.get_DataType (tcptr,desc.type ());
#endif /*if 0*/
    }

  }
}
