// $Id$

#include "DynEnum_Handler.h"
#include "DynAny_Handler.h"

#include "Basic_Deployment_Data.hpp"
#include "tao/AnyTypeCode/TypeCode.h"
#include "tao/TypeCodeFactory/TypeCodeFactory_Adapter_Impl.h"
#include "tao/AnyTypeCode/AnyTypeCode_methods.h"
#include "tao/AnyTypeCode/Enum_TypeCode.h"
#include "tao/IFR_Client/IFR_BasicC.h"

namespace CIAO
{
  namespace Config_Handlers
  {
    DynamicAny::DynAny_ptr
    DynEnum_Handler::extract_into_dynany (const DataType &type,
                                          const DataValue &value)
    {
      if (!type.enum_p ())
        {
          ACE_ERROR ((LM_ERROR, "ERROR: Enum type descriptioin required"));
          throw 1;
        }

      // Construct TypeCode for the enum
      CORBA::EnumMemberSeq members;
      members.length (type.enum_ ().count_member ());
      CORBA::ULong index (0);

      for (EnumType::member_const_iterator i = type.enum_ ().begin_member ();
           i != type.enum_ ().end_member ();
           ++i)
        {
          members[index++] = CORBA::string_dup (i->c_str ());
        }

      // Grab pointer to the DynAny_Handler to use the orb and any factory.
      //      DynAny_Handler *dah = DynAny_Handler::instance ();

      // @@ Leak this guy onto the heap to avoid a compile problem.
      CORBA::TypeCode_ptr tc =
        DYNANY_HANDLER->orb ()->create_enum_tc (type.enum_ ().typeId ().c_str (),
                                     type.enum_ ().name ().c_str (),
                                     members);

      ACE_ERROR ((LM_ERROR, "Type: %s \nName: %s\nvalue: %s\n",
                  type.enum_ ().typeId ().c_str (),
                  type.enum_ ().name ().c_str (),
                  value.begin_enum ()->c_str ()));

      // Make the actual DynEnum
      DynamicAny::DynAny_var temp =
        DYNANY_HANDLER->daf ()->create_dyn_any_from_type_code (tc);
      DynamicAny::DynEnum_var retval = DynamicAny::DynEnum::_narrow (temp.in ());

      retval->set_as_string (value.begin_enum ()->c_str ());

      return retval._retn ();
    }

    void
    DynEnum_Handler::extract_out_of_dynany (const DynamicAny::DynAny_ptr dyn)
    {
      ACE_UNUSED_ARG (dyn);
      ACE_ERROR ((LM_ERROR, "Extracting Enums not yet supported\n"));
    }
  }
}


