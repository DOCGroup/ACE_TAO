// $Id$

#include "DynStruct_Handler.h"
#include "DynAny_Handler.h"

#include "Basic_Deployment_Data.hpp"
#include "Common.h"

#include "ace/Null_Mutex.h"

//#include "tao/ORB.h"
#include "tao/IFR_Client/IFR_BasicC.h"
//#include "tao/TypeCodeFactory/TypeCodeFactory_Adapter_Impl.h"

#include "tao/AnyTypeCode/Struct_TypeCode.h"

namespace CIAO
{
  namespace Config_Handlers
  {
    void create_type_map (const DataType &type,
                          std::map <std::string, DataType const *> &dt_map)
    {
      for (StructType::member_const_iterator i = type.struct_ ().begin_member ();
           i != type.struct_ ().end_member (); ++i)
        {
          dt_map[i->name ()] = &(i->type ());
        }
    }

    DynamicAny::DynAny_ptr
    DynStruct_Handler::extract_into_dynany (const DataType &type,
                                            const DataValue &value,
                                            CORBA::TypeCode_ptr req_tc)
    {
      try
        {
          CORBA::TypeCode_ptr tc;

          if (req_tc)
            tc = req_tc;
          else
            tc = DynStruct_Handler::create_typecode (type);

          std::map <std::string, DataType const*> dt_map;
          create_type_map (type, dt_map);
          
          // Make the actual DynStruct
          DynamicAny::DynAny_var temp =
            DYNANY_HANDLER->daf ()->create_dyn_any_from_type_code (tc);
          DynamicAny::DynStruct_var retval = DynamicAny::DynStruct::_narrow (temp.in ());
          
          CORBA::ULong pos (0);
          DynamicAny::NameDynAnyPairSeq values;
          values.length (value.count_member ());
          
          for (DataValue::member_const_iterator i = value.begin_member ();
               i != value.end_member (); ++i)
            {
              values[pos].id = i->name ().c_str ();
              values[pos].value = DYNANY_HANDLER->extract_into_dynany (*dt_map[i->name ()],
                                                                       i->value ());
              pos++;
            }
          
          retval->set_members_as_dyn_any (values);
          
          return retval._retn ();
        }
      catch (Config_Error &ex)
        {
          if (type.struct_p ())
            ex.add_name (type.struct_ ().typeId ());
          throw ex;
        }
      catch (...)
        {
          throw Config_Error (type.struct_ ().typeId (),
                              "Unknown exception");
        }
    }
    
    
    void
    DynStruct_Handler::extract_out_of_dynany (const DynamicAny::DynAny_ptr dyn)
    {
      ACE_UNUSED_ARG (dyn);
      ACE_ERROR ((LM_ERROR, "Extracting Structs not yet supported\n"));
    }
    
    CORBA::TypeCode_ptr 
    DynStruct_Handler::create_typecode (const DataType &type)
    {
      if (!type.struct_p ())
        {
          ACE_ERROR ((LM_ERROR, "ERROR: Struct type descriptioin required"));
          throw Config_Error ("", "Expected struct type information, tc_kind may be incorrect\n");
        }
      
      std::string rid (type.struct_ ().typeId ());
      std::string name (type.struct_ ().name ());
      
      CORBA::StructMemberSeq members;
      members.length (type.struct_ ().count_member ());
      CORBA::ULong pos (0);

      for (StructType::member_const_iterator i = type.struct_ ().begin_member ();
           i != type.struct_ ().end_member (); ++i)
        {
          members[pos].name = i->name ().c_str ();
          members[pos].type = DYNANY_HANDLER->create_typecode (i->type ());
          ++pos;
        }

      // @@ Leak this guy onto the heap to avoid a compile problem.
      CORBA::TypeCode_ptr tc =
        DYNANY_HANDLER->orb ()->create_struct_tc (rid.c_str (),
                                                  name.c_str (),
                                                  members);
      
      DYNANY_HANDLER->register_typecode (type.struct_ ().typeId (),
                                         tc);
      
      return tc;
    }
  }
}


  
