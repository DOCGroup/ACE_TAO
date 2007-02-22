// $Id$

#include "DynSequence_Handler.h"
#include "DynAny_Handler.h"

#include "Basic_Deployment_Data.hpp"
#include "tao/AnyTypeCode/TypeCode.h"
#include "tao/TypeCodeFactory/TypeCodeFactory_Adapter_Impl.h"
#include "tao/AnyTypeCode/AnyTypeCode_methods.h"
#include "tao/AnyTypeCode/Sequence_TypeCode.h"
#include "tao/IFR_Client/IFR_BasicC.h"

namespace CIAO
{
  namespace Config_Handlers
  {
    DynamicAny::DynAny_ptr
    DynSequence_Handler::extract_into_dynany (const Any &any)
    {
      CORBA::TypeCode_ptr tc = DynSequence_Handler::create_typecode (any.type ());
      
      // Make the actual DynSequence
      DynamicAny::DynAny_var temp =
        DYNANY_HANDLER->daf ()->create_dyn_any_from_type_code (tc);
      DynamicAny::DynSequence_var retval = DynamicAny::DynSequence::_narrow (temp.in ());
      
      CORBA::TypeCode_ptr element_tc = 
        DynAny_Handler::create_typecode (any.type ().sequence ().elementType ());
      
      switch (any.type ().kind ().integral ())
        {
          // ========== BASIC TYPES
        case TCKind::tk_null_l:
        case TCKind::tk_void_l:
          ACE_ERROR ((LM_WARNING, "I don't know how to handle null or void types\n"));
          throw 1;

        case TCKind::tk_short_l:
        case TCKind::tk_long_l:
        case TCKind::tk_ushort_l:
        case TCKind::tk_ulong_l:
        case TCKind::tk_float_l:
        case TCKind::tk_double_l:
        case TCKind::tk_boolean_l:
        case TCKind::tk_octet_l:
        case TCKind::tk_string_l:
        case TCKind::tk_longlong_l:
        case TCKind::tk_ulonglong_l:
        case TCKind::tk_longdouble_l:
        case TCKind::tk_wstring_l:
        case TCKind::tk_enum_l:
        case TCKind::tk_objref_l:
        case TCKind::tk_fixed_l:
        case TCKind::tk_wfixed_l:
        case TCKind::tk_component_l:
        case TCKind::tk_home_l:
          // special case where multiplicity of data value iterator is greater than one
          // and is equal to length of sequence.
          for (Any::value_const_iterator i = values;
               values
        case TCKind::tk_char_l:
        case TCKind::tk_wchar_l:
          // special case where value iterataor multiplicity should be one, and should 
          // represent a string, each character of which becomes a element of the sequence.

        case TCKind::tk_sequence_l:
        case TCKind::tk_array_l:
          // Several special cases here.

        case TCKind::tk_any_l:
        case TCKind::tk_TypeCode_l:
        case TCKind::tk_Principal_l:
        case TCKind::tk_struct_l:
        case TCKind::tk_union_l:
        case TCKind::tk_alias_l:
        case TCKind::tk_except_l:
        case TCKind::tk_value_l:
        case TCKind::tk_value_box_l:
        case TCKind::tk_native_l:
        case TCKind::tk_abstract_interface_l:
        case TCKind::tk_local_interface_l:
        case TCKind::tk_event_l:
          // Special case where element association in datavalue contains another datavalue.

          ACE_ERROR ((LM_ERROR, "Type not supported\n"));
          throw 1;
        }

      return retval._retn ();
    }

    void
    DynSequence_Handler::extract_out_of_dynany (const DynamicAny::DynAny_ptr dyn)
    {
      ACE_UNUSED_ARG (dyn);
      ACE_ERROR ((LM_ERROR, "Extracting Sequences not yet supported\n"));
    }
    
    CORBA::TypeCode_ptr 
    DynSequence_Handler::create_typecode (const DataType &type)
    {
      if (!type.sequence_p ())
        {
          ACE_ERROR ((LM_ERROR, "ERROR: Sequence type descriptioin required"));
          throw 1;
        }
      
      CORBA::TypeCode_ptr etc = 
        DYNANY_HANDLER->create_typecode (type.sequence ().elementType ());
      
      // @@ Leak this guy onto the heap to avoid a compile problem.
      CORBA::TypeCode_ptr tc =
        DYNANY_HANDLER->orb ()->create_sequence_tc (type.sequence ().bound ()
                                                    etc);
      
      // Sequences don't have names.
      //      DYNANY_HANDLER->register_typecode (type.sequence_ ().typeId (),
      //                                   tc);
      
      return tc;
    }
    
  }
}


