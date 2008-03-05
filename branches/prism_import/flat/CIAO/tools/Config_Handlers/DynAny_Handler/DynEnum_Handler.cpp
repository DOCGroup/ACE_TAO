// $Id$

#include "DynEnum_Handler.h"
#include "DynAny_Handler.h"

#include "Basic_Deployment_Data.hpp"
#include "Common.h"

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
                                          const DataValue &value,
                                          CORBA::TypeCode_ptr req_tc)
    {
      try
        {
          CORBA::TypeCode_ptr tc;

          if (req_tc)
            tc = req_tc;
          else
            tc = DynEnum_Handler::create_typecode (type);

          // Make the actual DynEnum
          DynamicAny::DynAny_var temp =
            DYNANY_HANDLER->daf ()->create_dyn_any_from_type_code (tc);
          DynamicAny::DynEnum_var retval = DynamicAny::DynEnum::_narrow (temp.in ());

          retval->set_as_string (value.begin_enum ()->c_str ());

          return retval._retn ();
        }
      catch (DynamicAny::DynAny::InvalidValue)
        {
          ACE_ERROR ((LM_ERROR, "Invalid value provided in XML when trying to "
                      "initialize an instance of enumerated type %s\n",
                      type.enum_ ().typeId ().c_str ()));
          throw Config_Error (type.enum_ ().typeId (),
                              "Invalid value provided in XML");
        }
      catch (Config_Error &ex)
        {
          ACE_ERROR ((LM_ERROR, "DynEnum_Handler caught Config_Error\n"));
          if (type.enum_p ())
            ex.add_name (type.enum_ ().typeId ());

          throw ex;
        }
      catch (...)
        {
          ACE_ERROR ((LM_ERROR, "DynEnum_Handler caught unknown exception\n"));
          throw Config_Error (type.enum_ ().typeId (),
                              "Unknown exception");
        }
    }

    void
    DynEnum_Handler::extract_out_of_dynany (const DynamicAny::DynAny_ptr dyn)
    {
      ACE_UNUSED_ARG (dyn);
      ACE_ERROR ((LM_ERROR, "Extracting Enums not yet supported\n"));
    }
    
    CORBA::TypeCode_ptr
    DynEnum_Handler::create_typecode (const DataType &type)
    {
      if (!type.enum_p ())
        {
          ACE_ERROR ((LM_ERROR, "ERROR: Enum type descriptioin required"));
          throw Config_Error ("", "Did not find expected enum type description, tk_kind may be wrong.");
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

      // @@ Leak this guy onto the heap to avoid a compile problem.
      CORBA::TypeCode_ptr tc =
        DYNANY_HANDLER->orb ()->create_enum_tc (type.enum_ ().typeId ().c_str (),
                                     type.enum_ ().name ().c_str (),
                                     members);
      
      DYNANY_HANDLER->register_typecode (type.enum_ ().typeId (),
                                         tc);
      
      return tc;
    }
    
  }
}


