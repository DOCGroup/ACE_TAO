//$Id$

#include "ComponentPropertyDescription_Handler.h"
#include "DataType_Handler.h"


namespace CIAO{

  namespace Config_Handlers{

    CIAO::Config_Handlers::ComponentPropertyDescription_Handler::ComponentPropertyDescription_Handler(void)
    {
    }
    CIAO::Config_Handlers::ComponentPropertyDescription_Handler::~ComponentPropertyDescription_Handler(void)
    {
    }

    void
    CIAO::Config_Handlers::ComponentPropertyDescription_Handler::get_ComponentPropertyDescription (
            Deployment::ComponentPropertyDescription& toconfig,
            ComponentPropertyDescription& desc)
    {
      //Copy the name to the CPD.
      toconfig.name = CORBA::string_dup (desc.name ().c_str ());

      //Delegate the DataType to the
      //DataType_Handler.
      DataType_Handler dtypehandler;
      CORBA::TypeCode_ptr tcptr = toconfig.type.in ();
      dtypehandler.get_DataType (tcptr,desc.type ());

    }

  }
}
