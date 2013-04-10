
//=============================================================================
/**
 *  @file    Marshal.cpp
 *
 * $Id$
 *
 *  Implements the Marshal_Object class and the factory
 *  The original encoder and decoder code now appears in files encode.cpp and
 *  decode.cpp
 *
 *  @author Aniruddha S. Gokhale
 *  @author Copyright 1994-1995 by Sun Microsystems Inc.
 */
//=============================================================================

#include "tao/AnyTypeCode/Marshal.h"
#include "tao/AnyTypeCode/TypeCode.h"

#if !defined (__ACE_INLINE__)
# include "tao/AnyTypeCode/Marshal.inl"
#endif /* ! __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Marshal_Object::~TAO_Marshal_Object (void)
{
}

TAO::traverse_status
TAO_Marshal_Object::perform_skip (CORBA::TypeCode_ptr tc, TAO_InputCDR *stream)
{
  CORBA::ULong const kind = tc->kind ();

  switch (kind)
    {
    default:
    case CORBA::tk_fixed:
    case CORBA::tk_native:
    case CORBA::tk_abstract_interface:
    case CORBA::tk_component:
    case CORBA::tk_home:
      // @@ We don't know how to handle any of them yet.
      return TAO::TRAVERSE_STOP;

    case CORBA::tk_null:
    case CORBA::tk_void:
    case CORBA::tk_short:
    case CORBA::tk_long:
    case CORBA::tk_ushort:
    case CORBA::tk_ulong:
    case CORBA::tk_float:
    case CORBA::tk_double:
    case CORBA::tk_boolean:
    case CORBA::tk_char:
    case CORBA::tk_octet:
    case CORBA::tk_longlong:
    case CORBA::tk_ulonglong:
    case CORBA::tk_longdouble:
    case CORBA::tk_wchar:
    case CORBA::tk_enum:
      {
        TAO_Marshal_Primitive marshal;
        return marshal.skip (tc, stream);
      }

    case CORBA::tk_any:
      {
        TAO_Marshal_Any marshal;
        return marshal.skip (tc, stream);
      }
    case CORBA::tk_TypeCode:
      {
        TAO_Marshal_TypeCode marshal;
        return marshal.skip (tc, stream);
      }
    case CORBA::tk_Principal:
      {
        TAO_Marshal_Principal marshal;
        return marshal.skip (tc, stream);
      }
    case CORBA::tk_objref:
      {
        TAO_Marshal_ObjRef marshal;
        return marshal.skip (tc, stream);
      }
    case CORBA::tk_struct:
      {
        TAO_Marshal_Struct marshal;
        return marshal.skip (tc, stream);
      }
    case CORBA::tk_union:
      {
        TAO_Marshal_Union marshal;
        return marshal.skip (tc, stream);
      }
    case CORBA::tk_string:
      {
        TAO_Marshal_String marshal;
        return marshal.skip (tc, stream);
      }
    case CORBA::tk_sequence:
      {
        TAO_Marshal_Sequence marshal;
        return marshal.skip (tc, stream);
      }
    case CORBA::tk_array:
      {
        TAO_Marshal_Array marshal;
        return marshal.skip (tc, stream);
      }
    case CORBA::tk_alias:
      {
        TAO_Marshal_Alias marshal;
        return marshal.skip (tc, stream);
      }
    case CORBA::tk_except:
      {
        TAO_Marshal_Except marshal;
        return marshal.skip (tc, stream);
      }
    case CORBA::tk_wstring:
      {
        TAO_Marshal_WString marshal;
        return marshal.skip (tc, stream);
      }
    case CORBA::tk_value_box:
    case CORBA::tk_value:
    case CORBA::tk_event:
      {
        TAO_Marshal_Value marshal;
        return marshal.skip (tc, stream);
      }
    }
}

TAO::traverse_status
TAO_Marshal_Object::perform_append (CORBA::TypeCode_ptr tc,
                                    TAO_InputCDR *src,
                                    TAO_OutputCDR *dest)
{
  CORBA::ULong kind = tc->kind ();

  switch (kind)
    {
    default:
    case CORBA::tk_fixed:
    case CORBA::tk_native:
    case CORBA::tk_abstract_interface:
    case CORBA::tk_component:
    case CORBA::tk_home:
      // @@ We don't know how to handle any of them yet.
      return TAO::TRAVERSE_STOP;

    case CORBA::tk_null:
    case CORBA::tk_void:
    case CORBA::tk_short:
    case CORBA::tk_long:
    case CORBA::tk_ushort:
    case CORBA::tk_ulong:
    case CORBA::tk_float:
    case CORBA::tk_double:
    case CORBA::tk_boolean:
    case CORBA::tk_char:
    case CORBA::tk_octet:
    case CORBA::tk_longlong:
    case CORBA::tk_ulonglong:
    case CORBA::tk_longdouble:
    case CORBA::tk_wchar:
    case CORBA::tk_enum:
      {
        TAO_Marshal_Primitive marshal;
        return marshal.append (tc, src, dest);
      }

    case CORBA::tk_any:
      {
        TAO_Marshal_Any marshal;
        return marshal.append (tc, src, dest);
      }
    case CORBA::tk_TypeCode:
      {
        TAO_Marshal_TypeCode marshal;
        return marshal.append (tc, src, dest);
      }
    case CORBA::tk_Principal:
      {
        TAO_Marshal_Principal marshal;
        return marshal.append (tc, src, dest);
      }
    case CORBA::tk_objref:
      {
        TAO_Marshal_ObjRef marshal;
        return marshal.append (tc, src, dest);
      }
    case CORBA::tk_struct:
      {
        TAO_Marshal_Struct marshal;
        return marshal.append (tc, src, dest);
      }
    case CORBA::tk_union:
      {
        TAO_Marshal_Union marshal;
        return marshal.append (tc, src, dest);
      }
    case CORBA::tk_string:
      {
        TAO_Marshal_String marshal;
        return marshal.append (tc, src, dest);
      }
    case CORBA::tk_sequence:
      {
        TAO_Marshal_Sequence marshal;
        return marshal.append (tc, src, dest);
      }
    case CORBA::tk_array:
      {
        TAO_Marshal_Array marshal;
        return marshal.append (tc, src, dest);
      }
    case CORBA::tk_alias:
      {
        TAO_Marshal_Alias marshal;
        return marshal.append (tc, src, dest);
      }
    case CORBA::tk_except:
      {
        TAO_Marshal_Except marshal;
        return marshal.append (tc, src, dest);
      }
    case CORBA::tk_wstring:
      {
        TAO_Marshal_WString marshal;
        return marshal.append (tc, src, dest);
      }
    case CORBA::tk_value_box:
    case CORBA::tk_value:
    case CORBA::tk_event:
      {
        TAO_Marshal_Value marshal;
        return marshal.append (tc, src, dest);
      }
    }
}

TAO_END_VERSIONED_NAMESPACE_DECL
