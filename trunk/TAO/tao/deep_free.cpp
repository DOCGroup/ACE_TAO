// ============================================================================
//
// $Id$
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    deep_free.cpp
//
// = DESCRIPTION
//   Code for deep_free
//   The original code had a single static deep_free function that called
//   traverse to interpret the data types. This version defines a static method
//   "deep_free" on each class and avoids calling traverse.
//
//   Helper routine for "Any" destructor.
//
//   This frees all the memory pointed to by any given value held inside
//   of an "Any".  For most data types it does nothing, since most data
//   types don't hold any memory.  For a few, it calls other deep_free methods
//   to free the memory
//
// = AUTHOR
//     Copyright 1994-1995 by Sun Microsystems Inc.
//     and
//     Aniruddha Gokhale
//
// ============================================================================

#include "tao/Marshal.h"
#include "tao/CDR.h"
#include "tao/Environment.h"
#include "tao/Any.h"
#include "tao/Object.h"
#include "tao/Stub.h"
#include "tao/Principal.h"
#include "tao/varout.h"
#include "tao/Union.h"
#include "tao/debug.h"

ACE_RCSID(tao, deep_free, "$Id$")

CORBA::TypeCode::traverse_status
DEEP_FREE (CORBA::TypeCode_ptr  param,
           const void *source,
           const void *dest,
           CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::TypeCode::traverse_status retval = CORBA::TypeCode::TRAVERSE_CONTINUE;

  // Since every exception-throwing function is followed by break and then return,
  // we'll just skip the checking here.
  switch (param->kind_)
    {
    case CORBA::tk_null:
    case CORBA::tk_void:
    case CORBA::tk_short:
    case CORBA::tk_ushort:
    case CORBA::tk_long:
    case CORBA::tk_ulong:
    case CORBA::tk_float:
    case CORBA::tk_enum:
    case CORBA::tk_double:
    case CORBA::tk_longlong:
    case CORBA::tk_ulonglong:
    case CORBA::tk_boolean:
    case CORBA::tk_char:
    case CORBA::tk_octet:
    case CORBA::tk_longdouble:
    case CORBA::tk_wchar:
      break;
    case CORBA::tk_any:
      retval = TAO_Marshal_Any::deep_free (param, source, dest, ACE_TRY_ENV);
      break;
    case CORBA::tk_TypeCode:
      retval = TAO_Marshal_TypeCode::deep_free (param, source, dest, ACE_TRY_ENV);
      break;
    case CORBA::tk_Principal:
      retval = TAO_Marshal_Principal::deep_free (param, source, dest, ACE_TRY_ENV);
      break;
    case CORBA::tk_objref:
      retval = TAO_Marshal_ObjRef::deep_free (param, source, dest, ACE_TRY_ENV);
      break;
    case CORBA::tk_struct:
      retval = TAO_Marshal_Struct::deep_free (param, source, dest, ACE_TRY_ENV);

      // @@ It's likely that other types will need their top-level
      // memory freed as well. --- Carlos (and Seth).
      // delete source;
      break;
    case CORBA::tk_union:
      retval = TAO_Marshal_Union::deep_free (param, source, dest, ACE_TRY_ENV);
      break;
    case CORBA::tk_string:
      retval = TAO_Marshal_String::deep_free (param, source, dest, ACE_TRY_ENV);
      break;
    case CORBA::tk_sequence:
      retval = TAO_Marshal_Sequence::deep_free (param, source, dest, ACE_TRY_ENV);
      break;
    case CORBA::tk_array:
      retval = TAO_Marshal_Array::deep_free (param, source, dest, ACE_TRY_ENV);
      break;
    case CORBA::tk_alias:
      retval = TAO_Marshal_Alias::deep_free (param, source, dest, ACE_TRY_ENV);
      break;
    case CORBA::tk_except:
      retval = TAO_Marshal_Except::deep_free (param, source, dest, ACE_TRY_ENV);
      break;
    case CORBA::tk_wstring:
      retval = TAO_Marshal_WString::deep_free (param, source, dest, ACE_TRY_ENV);
      break;
    default:
      retval = CORBA::TypeCode::TRAVERSE_STOP;
    } // end of switch

  return retval;
}

// deep free for primitives
CORBA::TypeCode::traverse_status
TAO_Marshal_Primitive::deep_free (CORBA::TypeCode_ptr  tc,
                                  const void *,
                                  const void *,
                                  CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::TCKind my_kind;

  if (!tc)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO_Marshal_Primitive::deep_free detected error\n"));
      ACE_THROW_RETURN (CORBA::BAD_TYPECODE (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_MAYBE), CORBA::TypeCode::TRAVERSE_STOP);
    }

  my_kind = tc->kind (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::TypeCode::TRAVERSE_STOP);

  switch (my_kind)
    {
    case CORBA::tk_null:
    case CORBA::tk_void:
    case CORBA::tk_char:
    case CORBA::tk_octet:
    case CORBA::tk_short:
    case CORBA::tk_ushort:
    case CORBA::tk_wchar:
    case CORBA::tk_long:
    case CORBA::tk_ulong:
    case CORBA::tk_float:
    case CORBA::tk_enum:
    case CORBA::tk_longlong:
    case CORBA::tk_ulonglong:
    case CORBA::tk_double:
    case CORBA::tk_longdouble:
    case CORBA::tk_boolean:
      break;
    default:
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO_Marshal_Primitive::deep_free detected error\n"));
      ACE_THROW_RETURN (CORBA::BAD_TYPECODE (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_MAYBE),
                        CORBA::TypeCode::TRAVERSE_STOP);
    }
  return CORBA::TypeCode::TRAVERSE_CONTINUE;
}

// deep_free structs
CORBA::TypeCode::traverse_status
TAO_Marshal_Struct::deep_free (CORBA::TypeCode_ptr  tc,
                               const void *source,
                               const void *dest,
                               CORBA::Environment &ACE_TRY_ENV)
{
  if (!tc)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO_Marshal_Struct::deep_free detected error\n"));
      ACE_THROW_RETURN (CORBA::BAD_TYPECODE (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_MAYBE), CORBA::TypeCode::TRAVERSE_STOP);
    }

  CORBA::TypeCode::traverse_status retval = CORBA::TypeCode::TRAVERSE_CONTINUE;
  CORBA::TypeCode_ptr param;
  CORBA::Long size;

  // compute the number of fields in the struct
  int member_count = tc->member_count (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::TypeCode::TRAVERSE_STOP);

  for (int i = 0; i < member_count && retval ==
         CORBA::TypeCode::TRAVERSE_CONTINUE; i++)
    {
      // get the typecode for the ith field
      param = tc->member_type (i, ACE_TRY_ENV);
      ACE_CHECK_RETURN (CORBA::TypeCode::TRAVERSE_STOP);

      // get the size of the field
      size = param->size (ACE_TRY_ENV);
      ACE_CHECK_RETURN (CORBA::TypeCode::TRAVERSE_STOP);

      // Since every exception-throwing function is followed by a break, we'll just
      // check for exception after the switch.
      switch (param->kind_)
        {
        case CORBA::tk_null:
        case CORBA::tk_void:
        case CORBA::tk_short:
        case CORBA::tk_ushort:
        case CORBA::tk_long:
        case CORBA::tk_ulong:
        case CORBA::tk_float:
        case CORBA::tk_enum:
        case CORBA::tk_double:
        case CORBA::tk_longlong:
        case CORBA::tk_ulonglong:
        case CORBA::tk_boolean:
        case CORBA::tk_char:
        case CORBA::tk_octet:
        case CORBA::tk_longdouble:
        case CORBA::tk_wchar:
          break;
        case CORBA::tk_any:
          retval = TAO_Marshal_Any::deep_free (param, source, dest, ACE_TRY_ENV);
          break;
        case CORBA::tk_TypeCode:
          retval = TAO_Marshal_TypeCode::deep_free (param, source, dest, ACE_TRY_ENV);
          break;
        case CORBA::tk_Principal:
          retval = TAO_Marshal_Principal::deep_free (param, source, dest, ACE_TRY_ENV);
          break;

        case CORBA::tk_objref:
          {
            typedef TAO_Object_Field_T<CORBA::Object,CORBA::Object_var>
              TAO_Object_Field_Class;
            TAO_Object_Field_Class* field =
              ACE_reinterpret_cast (TAO_Object_Field_Class *,
                                    ACE_const_cast (void *, source));
            field->_release ();
            retval = CORBA::TypeCode::TRAVERSE_CONTINUE;
          }
        break;

        case CORBA::tk_struct:
          retval = TAO_Marshal_Struct::deep_free (param, source, dest, ACE_TRY_ENV);
          break;
        case CORBA::tk_union:
          retval = TAO_Marshal_Union::deep_free (param, source, dest, ACE_TRY_ENV);
          break;
        case CORBA::tk_string:
          retval = TAO_Marshal_String::deep_free (param, source, dest, ACE_TRY_ENV);
          break;
        case CORBA::tk_sequence:
          retval = TAO_Marshal_Sequence::deep_free (param, source, dest, ACE_TRY_ENV);
          break;
        case CORBA::tk_array:
          retval = TAO_Marshal_Array::deep_free (param, source, dest, ACE_TRY_ENV);
          break;
        case CORBA::tk_alias:
          retval = TAO_Marshal_Alias::deep_free (param, source, dest, ACE_TRY_ENV);
          break;
        case CORBA::tk_except:
          retval = TAO_Marshal_Except::deep_free (param, source, dest, ACE_TRY_ENV);
          break;
        case CORBA::tk_wstring:
          retval = TAO_Marshal_WString::deep_free (param, source, dest, ACE_TRY_ENV);
          break;
        default:
          retval = CORBA::TypeCode::TRAVERSE_STOP;
        } // end of switch
      source = (char *)source + size;
    } // end of loop
  ACE_CHECK_RETURN (CORBA::TypeCode::TRAVERSE_STOP);

  if (retval != CORBA::TypeCode::TRAVERSE_CONTINUE)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO_Marshal_Struct::deep_free detected error\n"));
      ACE_THROW_RETURN (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_MAYBE),
                        CORBA::TypeCode::TRAVERSE_STOP);
    }
  return CORBA::TypeCode::TRAVERSE_CONTINUE;
}

// deep_free for union
CORBA::TypeCode::traverse_status
TAO_Marshal_Union::deep_free (CORBA::TypeCode_ptr  tc,
                              const void *data,
                              const void *data2,
                              CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::TypeCode::traverse_status retval = CORBA::TypeCode::TRAVERSE_CONTINUE;
  CORBA::TypeCode_ptr discrim_tc;
  CORBA::TypeCode_ptr member_tc;
  CORBA::Any_ptr member_label;
  const void *discrim_val;
  CORBA::ULong member_count;
  CORBA::Long  default_index;
  CORBA::ULong i;
  CORBA::TypeCode_ptr default_tc = 0;
  CORBA::Boolean discrim_matched = 0;
  TAO_Base_Union *base_union;
  void *member_val;

  // Get a base pointer so we can use the union's virtual functions.
  base_union = ACE_reinterpret_cast (TAO_Base_Union *,
                                     ACE_const_cast (void *,
                                                     data));

  discrim_tc = tc->discriminator_type (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::TypeCode::TRAVERSE_STOP);

  discrim_val = base_union->_discriminant ();
  // Get a pointer to the discriminator value.

  member_val = base_union->_access (0);
  // Get a pointer to the member, but don't allocate new storage.

  default_index = tc->default_index (ACE_TRY_ENV);
  // now get ready to marshal the actual union value
  ACE_CHECK_RETURN ( CORBA::TypeCode::TRAVERSE_STOP);

  member_count = tc->member_count (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::TypeCode::TRAVERSE_STOP);

  // check which label value matches with the discriminator
  // value. Accordingly, marshal the corresponding
  // member_type. If none match, check if default exists
  // and marshal accordingly. Otherwise it is an error.
  for (i = 0; member_count-- != 0; i++)
    {
      member_label = tc->member_label (i, ACE_TRY_ENV);
      ACE_CHECK_RETURN ( CORBA::TypeCode::TRAVERSE_STOP);

      // do the matching
      CORBA::TypeCode_var type = member_label->type ();
      switch (type->kind (ACE_TRY_ENV))
        {
        case CORBA::tk_short:
          if (*(CORBA::Short *)member_label->_tao_get_cdr ()->base () ==
              *(CORBA::Short *)discrim_val)
            discrim_matched = 1;
          break;
        case CORBA::tk_ushort:
          if (*(CORBA::UShort *)member_label->_tao_get_cdr ()->base () ==
              *(CORBA::UShort *)discrim_val)
            discrim_matched = 1;
          break;
        case CORBA::tk_long:
          if (*(CORBA::Long *)member_label->_tao_get_cdr ()->base () ==
              *(CORBA::Long *)discrim_val)
            discrim_matched = 1;
          break;
        case CORBA::tk_ulong:
        case CORBA::tk_enum:
          if (*(CORBA::ULong *)member_label->_tao_get_cdr ()->base () ==
              *(CORBA::ULong *)discrim_val)
            discrim_matched = 1;
          break;
        case CORBA::tk_char:
          if (*(CORBA::Char *)member_label->_tao_get_cdr ()->base () ==
              *(CORBA::Char *)discrim_val)
            discrim_matched = 1;
          break;
        case CORBA::tk_wchar:
          if (*(CORBA::WChar *)member_label->_tao_get_cdr ()->base () ==
              *(CORBA::WChar *)discrim_val)
            discrim_matched = 1;
          break;
        case CORBA::tk_boolean:
          if (*(CORBA::Boolean *)member_label->_tao_get_cdr ()->base () ==
              *(CORBA::Boolean *)discrim_val)
            discrim_matched = 1;
          break;
        default:
          ACE_THROW_RETURN (CORBA::BAD_TYPECODE (),
                            CORBA::TypeCode::TRAVERSE_STOP);
        }// end of switch

      // get the member typecode
      member_tc = tc->member_type (i, ACE_TRY_ENV);
      ACE_CHECK_RETURN ( CORBA::TypeCode::TRAVERSE_STOP);

      if (default_index >= 0 && default_index-- == 0)
        {
          // have we reached the default label?, if so,
          // save a handle to the typecode for the default
          default_tc = member_tc;
        }
      if (discrim_matched)
        {
          // deep_free the discriminator value
          retval = DEEP_FREE (discrim_tc,
                              discrim_val,
                              data2,
                              ACE_TRY_ENV);

          ACE_CHECK_RETURN (CORBA::TypeCode::TRAVERSE_STOP);

          if (retval != CORBA::TypeCode::TRAVERSE_CONTINUE)
            ACE_THROW_RETURN (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_MAYBE),
                              CORBA::TypeCode::TRAVERSE_STOP);

          // marshal according to the matched typecode
          return DEEP_FREE (member_tc,
                            member_val,
                            data2,
                            ACE_TRY_ENV);
        } // end of if
    } // end of for

  // we are here only if there was no match

  // deep_free the discriminator value
  retval = DEEP_FREE (discrim_tc,
                      discrim_val,
                      data2,
                      ACE_TRY_ENV);

  ACE_CHECK_RETURN (CORBA::TypeCode::TRAVERSE_STOP);

  if (retval != CORBA::TypeCode::TRAVERSE_CONTINUE)
    ACE_THROW_RETURN (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_MAYBE),
                      CORBA::TypeCode::TRAVERSE_STOP);

  if (default_tc)
    return DEEP_FREE (default_tc,
                      member_val,
                      data2,
                      ACE_TRY_ENV);
  else
    return CORBA::TypeCode::TRAVERSE_CONTINUE;
}

// deep_free for Sequence
CORBA::TypeCode::traverse_status
TAO_Marshal_Sequence::deep_free (CORBA::TypeCode_ptr  tc,
                                 const void *source,
                                 const void *,
                                 CORBA::Environment &)
{
  // TAO implements sequences using inheritance from a base
  // class. That class allocate  and deallocates the buffer, using
  // virtual methods.

  if (tc)
    {
      TAO_Base_Sequence *src =
        ACE_reinterpret_cast(TAO_Base_Sequence*,ACE_const_cast(void*,source));

      src->_deallocate_buffer ();
      return CORBA::TypeCode::TRAVERSE_CONTINUE;
    }
  return CORBA::TypeCode::TRAVERSE_STOP;
}

// deep_free for Array
CORBA::TypeCode::traverse_status
TAO_Marshal_Array::deep_free (CORBA::TypeCode_ptr  tc,
                              const void *source,
                              const void *dest,
                              CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::TypeCode::traverse_status retval =
    CORBA::TypeCode::TRAVERSE_CONTINUE;  // return status
  CORBA::TypeCode_ptr   tc2;  // typecode of the element
  size_t  size; // size of element
  CORBA::ULong  bounds;

  // Rely on binary format of sequences -- all are the same
  // except for the type pointed to by "buffer"

  if (!tc)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO_Marshal_Struct::deep_free detected error\n"));
      ACE_THROW_RETURN (CORBA::BAD_TYPECODE (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_MAYBE),
                        CORBA::TypeCode::TRAVERSE_STOP);
    }

  bounds = tc->length (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::TypeCode::TRAVERSE_STOP);
 // exception getting bounds

  // get element typecode
  tc2 = tc->content_type (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::TypeCode::TRAVERSE_STOP);
  // exception computing content type

  // get the size of the element type
  size = tc2->size (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::TypeCode::TRAVERSE_STOP);

  switch (tc2->kind_)
    {
    case CORBA::tk_null:
    case CORBA::tk_void:
    case CORBA::tk_short:
    case CORBA::tk_ushort:
    case CORBA::tk_long:
    case CORBA::tk_ulong:
    case CORBA::tk_float:
    case CORBA::tk_double:
    case CORBA::tk_longlong:
    case CORBA::tk_ulonglong:
    case CORBA::tk_boolean:
    case CORBA::tk_char:
    case CORBA::tk_octet:
    case CORBA::tk_longdouble:
    case CORBA::tk_wchar:
    case CORBA::tk_enum:
      //                      CORBA::release (tc2);
      return CORBA::TypeCode::TRAVERSE_CONTINUE;

      // handle all aggregate types here
    case CORBA::tk_any:
      while (bounds-- && retval == CORBA::TypeCode::TRAVERSE_CONTINUE)
        {
          retval = TAO_Marshal_Any::deep_free (tc2, source, dest, ACE_TRY_ENV);
          ACE_CHECK_RETURN (CORBA::TypeCode::TRAVERSE_STOP);
          source = (char *)source + size;
        }
      break;
    case CORBA::tk_TypeCode:
      while (bounds-- && retval == CORBA::TypeCode::TRAVERSE_CONTINUE)
        {
          retval = TAO_Marshal_TypeCode::deep_free (tc2, source, dest, ACE_TRY_ENV);
          ACE_CHECK_RETURN (CORBA::TypeCode::TRAVERSE_STOP);
          source = (char *)source + size;
        }
      break;
    case CORBA::tk_Principal:
      while (bounds-- && retval == CORBA::TypeCode::TRAVERSE_CONTINUE)
        {
          retval = TAO_Marshal_Principal::deep_free (tc2, source, dest, ACE_TRY_ENV);
          ACE_CHECK_RETURN (CORBA::TypeCode::TRAVERSE_STOP);
          source = (char *)source + size;
        }
      break;
    case CORBA::tk_objref:
      while (bounds-- && retval == CORBA::TypeCode::TRAVERSE_CONTINUE)
        {
          retval = TAO_Marshal_ObjRef::deep_free (tc2, source, dest, ACE_TRY_ENV);
          ACE_CHECK_RETURN (CORBA::TypeCode::TRAVERSE_STOP);
          source = (char *)source + size;
        }
      break;
    case CORBA::tk_struct:
      while (bounds-- && retval == CORBA::TypeCode::TRAVERSE_CONTINUE)
        {
          retval = TAO_Marshal_Struct::deep_free (tc2, source, dest, ACE_TRY_ENV);
          ACE_CHECK_RETURN (CORBA::TypeCode::TRAVERSE_STOP);
          source = (char *)source + size;
        }
      break;
    case CORBA::tk_union:
      while (bounds-- && retval == CORBA::TypeCode::TRAVERSE_CONTINUE)
        {
          retval = TAO_Marshal_Union::deep_free (tc2, source, dest, ACE_TRY_ENV);
          ACE_CHECK_RETURN (CORBA::TypeCode::TRAVERSE_STOP);
          source = (char *)source + size;
        }
      break;
    case CORBA::tk_string:
      while (bounds-- && retval == CORBA::TypeCode::TRAVERSE_CONTINUE)
        {
          retval = TAO_Marshal_String::deep_free (tc2, source, dest, ACE_TRY_ENV);
          ACE_CHECK_RETURN (CORBA::TypeCode::TRAVERSE_STOP);
          source = (char *)source + size;
        }
      break;
    case CORBA::tk_sequence:
      while (bounds-- && retval == CORBA::TypeCode::TRAVERSE_CONTINUE)
        {
          retval = TAO_Marshal_Sequence::deep_free (tc2, source, dest, ACE_TRY_ENV);
          ACE_CHECK_RETURN (CORBA::TypeCode::TRAVERSE_STOP);
          source = (char *)source + size;
        }
      break;
    case CORBA::tk_array:
      while (bounds-- && retval == CORBA::TypeCode::TRAVERSE_CONTINUE)
        {
          retval = TAO_Marshal_Array::deep_free (tc2, source, dest, ACE_TRY_ENV);
          ACE_CHECK_RETURN (CORBA::TypeCode::TRAVERSE_STOP);
          source = (char *)source + size;
        }
      break;
    case CORBA::tk_alias:
      while (bounds-- && retval == CORBA::TypeCode::TRAVERSE_CONTINUE)
        {
          retval = TAO_Marshal_Alias::deep_free (tc2, source, dest, ACE_TRY_ENV);
          ACE_CHECK_RETURN (CORBA::TypeCode::TRAVERSE_STOP);
          source = (char *)source + size;
        }
      break;
    case CORBA::tk_except:
      while (bounds-- && retval == CORBA::TypeCode::TRAVERSE_CONTINUE)
        {
          retval = TAO_Marshal_Except::deep_free (tc2, source, dest, ACE_TRY_ENV);
          ACE_CHECK_RETURN (CORBA::TypeCode::TRAVERSE_STOP);
          source = (char *)source + size;
        }
      break;
    case CORBA::tk_wstring:
      while (bounds-- && retval == CORBA::TypeCode::TRAVERSE_CONTINUE)
        {
          retval = TAO_Marshal_WString::deep_free (tc2, source, dest, ACE_TRY_ENV);
          ACE_CHECK_RETURN (CORBA::TypeCode::TRAVERSE_STOP);
          source = (char *)source + size;
        }
      break;
    default:
      retval = CORBA::TypeCode::TRAVERSE_STOP;
      break;
    } // end of switch
  //              CORBA::release (tc2);
  if (retval != CORBA::TypeCode::TRAVERSE_CONTINUE)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO_Marshal_Sequence::deep_free detected error\n"));
      // error exit
      ACE_THROW_RETURN (CORBA::MARSHAL (), CORBA::TypeCode::TRAVERSE_STOP);
    }
  return CORBA::TypeCode::TRAVERSE_CONTINUE;
}

// deep_free alias
CORBA::TypeCode::traverse_status
TAO_Marshal_Alias::deep_free (CORBA::TypeCode_ptr  tc,
                              const void *source,
                              const void *dest,
                              CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::TypeCode_ptr   tc2;  // typecode of the aliased type
  CORBA::TypeCode::traverse_status   retval =
    CORBA::TypeCode::TRAVERSE_CONTINUE; // status of encode operation

  if (!tc)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO_Marshal_Alias::deep_free detected error\n"));
      ACE_THROW_RETURN (CORBA::BAD_TYPECODE (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_MAYBE),
                        CORBA::TypeCode::TRAVERSE_STOP);
    }

  // get element type
  tc2 = tc->content_type (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::TypeCode::TRAVERSE_STOP);
  // exception getting content_type

  // switch on the data type and handle the cases for primitives here for
  // efficiency
  switch (tc2->kind_)
    {
    case CORBA::tk_null:
    case CORBA::tk_void:
    case CORBA::tk_short:
    case CORBA::tk_ushort:
    case CORBA::tk_long:
    case CORBA::tk_ulong:
    case CORBA::tk_float:
    case CORBA::tk_enum:
    case CORBA::tk_double:
    case CORBA::tk_longlong:
    case CORBA::tk_ulonglong:
    case CORBA::tk_boolean:
    case CORBA::tk_char:
    case CORBA::tk_octet:
    case CORBA::tk_longdouble:
    case CORBA::tk_wchar:
      //              CORBA::release (tc2);
      return CORBA::TypeCode::TRAVERSE_CONTINUE;
    case CORBA::tk_any:
      retval = TAO_Marshal_Any::deep_free (tc2, source, dest, ACE_TRY_ENV);
      break;
    case CORBA::tk_TypeCode:
      retval = TAO_Marshal_TypeCode::deep_free (tc2, source, dest, ACE_TRY_ENV);
      break;
    case CORBA::tk_Principal:
      retval = TAO_Marshal_Principal::deep_free (tc2, source, dest, ACE_TRY_ENV);
      break;
    case CORBA::tk_objref:
      retval = TAO_Marshal_ObjRef::deep_free (tc2, source, dest, ACE_TRY_ENV);
      break;
    case CORBA::tk_struct:
      retval = TAO_Marshal_Struct::deep_free (tc2, source, dest, ACE_TRY_ENV);
      break;
    case CORBA::tk_union:
      retval = TAO_Marshal_Union::deep_free (tc2, source, dest, ACE_TRY_ENV);
      break;
    case CORBA::tk_string:
      retval = TAO_Marshal_String::deep_free (tc2, source, dest, ACE_TRY_ENV);
      break;
    case CORBA::tk_sequence:
      retval = TAO_Marshal_Sequence::deep_free (tc2, source, dest, ACE_TRY_ENV);
      break;
    case CORBA::tk_array:
      retval = TAO_Marshal_Array::deep_free (tc2, source, dest, ACE_TRY_ENV);
      break;
    case CORBA::tk_alias:
      retval = TAO_Marshal_Alias::deep_free (tc2, source, dest, ACE_TRY_ENV);
      break;
    case CORBA::tk_except:
      retval = TAO_Marshal_Except::deep_free (tc2, source, dest, ACE_TRY_ENV);
      break;
    case CORBA::tk_wstring:
      retval = TAO_Marshal_WString::deep_free (tc2, source, dest, ACE_TRY_ENV);
      break;
    default:
      // anything else is an error
      retval = CORBA::TypeCode::TRAVERSE_STOP;
    }
  //      CORBA::release (tc2);
  ACE_CHECK_RETURN (CORBA::TypeCode::TRAVERSE_STOP);

  if (retval != CORBA::TypeCode::TRAVERSE_CONTINUE)
    {
      // We should never reach here.
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO_Marshal_Alias::decode detected error\n"));
      ACE_THROW_RETURN (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_MAYBE),
                        CORBA::TypeCode::TRAVERSE_STOP);
    }
  return CORBA::TypeCode::TRAVERSE_CONTINUE;
}

// deep_free structs
CORBA::TypeCode::traverse_status
TAO_Marshal_Except::deep_free (CORBA::TypeCode_ptr  tc,
                               const void *,
                               const void *,
                               CORBA::Environment &ACE_TRY_ENV)
{
#if 0
  // temporarily commented out to make compiler happy
  CORBA::Long i,
    member_count;      // number of fields in the struct
  CORBA::TypeCode::traverse_status retval = CORBA::TypeCode::TRAVERSE_CONTINUE;
  CORBA::TypeCode_ptr param;
  CORBA::Long size, alignment;
#endif /* 0 */

  if (!tc)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO_Marshal_Except::deep_free detected error\n"));
      ACE_THROW_RETURN (CORBA::BAD_TYPECODE (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_MAYBE),
                        CORBA::TypeCode::TRAVERSE_STOP);
    }
    // XXX: Exceptions are currently leaked because of bugs lurking
    // in this area.  Keep in mind that there are two things to
    // free: (a) the typecode in the exception base class; (b) any
    // pointers held by a user-defined exception, such as an objref
    // or string.
    //
    // Since this code does nothing, it should leak BOTH of those
    // kinds of memory.  Since it's not supposed to be called except
    // when the exception really is being freed, it should only be
    // called when the reference count in the exception base class
    // is zero.
    //
    // It's not clear which of those assertions actually hold.
    //
    // The code SHOULD be just like the traverse () call for a
    // structure, with (a) a precondition that the reference count
    // is zero, (b) an assertion that the typecode in the exception
    // and "tc" are equivalent, (c) releasing that typecode found
    // within the exception.
    //
  return CORBA::TypeCode::TRAVERSE_CONTINUE;
}
