// -*- C++ -*-

//=============================================================================
/**
 *  @file    Factory_Map.h
 *
 *  $Id$
 *
 *  Header file for TAO TypeCode factory map.
 *
 *  @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef TAO_TYPECODE_FACTORY_MAP_H
#define TAO_TYPECODE_FACTORY_MAP_H

#include /**/ "ace/pre.h"

#include "tao/Typecode_typesC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


namespace TAO
{
  namespace TypeCodeFactory
  {

    typedef bool (*factory) (CORBA::TCKind,
                             TAO_InputCDR &,
                             CORBA::TypeCode_ptr &);

    bool tc_null_factory       (CORBA::TCKind kind,
                                TAO_InputCDR & cdr,
                                CORBA::TypeCode_ptr & tc);
    bool tc_void_factory       (CORBA::TCKind kind,
                                TAO_InputCDR & cdr,
                                CORBA::TypeCode_ptr & tc);
    bool tc_short_factory      (CORBA::TCKind kind,
                                TAO_InputCDR & cdr,
                                CORBA::TypeCode_ptr & tc);
    bool tc_long_factory       (CORBA::TCKind kind,
                                TAO_InputCDR & cdr,
                                CORBA::TypeCode_ptr & tc);
    bool tc_ushort_factory     (CORBA::TCKind kind,
                                TAO_InputCDR & cdr,
                                CORBA::TypeCode_ptr & tc);
    bool tc_ulong_factory      (CORBA::TCKind kind,
                                TAO_InputCDR & cdr,
                                CORBA::TypeCode_ptr & tc);
    bool tc_float_factory      (CORBA::TCKind kind,
                                TAO_InputCDR & cdr,
                                CORBA::TypeCode_ptr & tc);
    bool tc_double_factory     (CORBA::TCKind kind,
                                TAO_InputCDR & cdr,
                                CORBA::TypeCode_ptr & tc);
    bool tc_boolean_factory    (CORBA::TCKind kind,
                                TAO_InputCDR & cdr,
                                CORBA::TypeCode_ptr & tc);
    bool tc_char_factory       (CORBA::TCKind kind,
                                TAO_InputCDR & cdr,
                                CORBA::TypeCode_ptr & tc);
    bool tc_octet_factory      (CORBA::TCKind kind,
                                TAO_InputCDR & cdr,
                                CORBA::TypeCode_ptr & tc);
    bool tc_any_factory        (CORBA::TCKind kind,
                                TAO_InputCDR & cdr,
                                CORBA::TypeCode_ptr & tc);
    bool tc_TypeCode_factory   (CORBA::TCKind kind,
                                TAO_InputCDR & cdr,
                                CORBA::TypeCode_ptr & tc);
    bool tc_Principal_factory  (CORBA::TCKind kind,
                                TAO_InputCDR & cdr,
                                CORBA::TypeCode_ptr & tc);
    bool tc_objref_factory     (CORBA::TCKind kind,
                                TAO_InputCDR & cdr,
                                CORBA::TypeCode_ptr & tc);
    bool tc_struct_factory     (CORBA::TCKind kind,
                                TAO_InputCDR & cdr,
                                CORBA::TypeCode_ptr & tc);
    bool tc_union_factory      (CORBA::TCKind kind,
                                TAO_InputCDR & cdr,
                                CORBA::TypeCode_ptr & tc);
    bool tc_enum_factory       (CORBA::TCKind kind,
                                TAO_InputCDR & cdr,
                                CORBA::TypeCode_ptr & tc);
    bool tc_string_factory     (CORBA::TCKind kind,
                                TAO_InputCDR & cdr,
                                CORBA::TypeCode_ptr & tc);
    bool tc_sequence_factory   (CORBA::TCKind kind,
                                TAO_InputCDR & cdr,
                                CORBA::TypeCode_ptr & tc);
    bool tc_array_factory      (CORBA::TCKind kind,
                                TAO_InputCDR & cdr,
                                CORBA::TypeCode_ptr & tc);
    bool tc_alias_factory      (CORBA::TCKind kind,
                                TAO_InputCDR & cdr,
                                CORBA::TypeCode_ptr & tc);
    bool tc_except_factory     (CORBA::TCKind kind,
                                TAO_InputCDR & cdr,
                                CORBA::TypeCode_ptr & tc);
    bool tc_longlong_factory   (CORBA::TCKind kind,
                                TAO_InputCDR & cdr,
                                CORBA::TypeCode_ptr & tc);
    bool tc_ulonglong_factory  (CORBA::TCKind kind,
                                TAO_InputCDR & cdr,
                                CORBA::TypeCode_ptr & tc);
    bool tc_longdouble_factory (CORBA::TCKind kind,
                                TAO_InputCDR & cdr,
                                CORBA::TypeCode_ptr & tc);
    bool tc_wchar_factory      (CORBA::TCKind kind,
                                TAO_InputCDR & cdr,
                                CORBA::TypeCode_ptr & tc);
    bool tc_wstring_factory    (CORBA::TCKind kind,
                                TAO_InputCDR & cdr,
                                CORBA::TypeCode_ptr & tc);
    bool tc_fixed_factory      (CORBA::TCKind kind,
                                TAO_InputCDR & cdr,
                                CORBA::TypeCode_ptr & tc);
    bool tc_value_factory      (CORBA::TCKind kind,
                                TAO_InputCDR & cdr,
                                CORBA::TypeCode_ptr & tc);
    bool tc_value_box_factory  (CORBA::TCKind kind,
                                TAO_InputCDR & cdr,
                                CORBA::TypeCode_ptr & tc);
    bool tc_native_factory     (CORBA::TCKind kind, TAO_InputCDR & cdr,
                                CORBA::TypeCode_ptr & tc);
    bool tc_abstract_interface_factory (CORBA::TCKind kind,
                                        TAO_InputCDR & cdr,
                                        CORBA::TypeCode_ptr & tc);
    bool tc_local_interface_factory (CORBA::TCKind kind,
                                     TAO_InputCDR & cdr,
                                     CORBA::TypeCode_ptr & tc);
    bool tc_component_factory  (CORBA::TCKind kind,
                                TAO_InputCDR & cdr,
                                CORBA::TypeCode_ptr & tc);
    bool tc_home_factory       (CORBA::TCKind kind,
                                TAO_InputCDR & cdr, CORBA::TypeCode_ptr & tc);
    bool tc_event_factory      (CORBA::TCKind kind,
                                TAO_InputCDR & cdr, CORBA::TypeCode_ptr & tc);

    factory const factory_map[] =
      {
        tc_null_factory,
        tc_void_factory,
        tc_short_factory,
        tc_long_factory,
        tc_ushort_factory,
        tc_ulong_factory,
        tc_float_factory,
        tc_double_factory,
        tc_boolean_factory,
        tc_char_factory,
        tc_octet_factory,
        tc_any_factory,
        tc_TypeCode_factory,
        tc_Principal_factory,
        tc_objref_factory,
        tc_struct_factory,
        tc_union_factory,
        tc_enum_factory,
        tc_string_factory,
        tc_sequence_factory,
        tc_array_factory,
        tc_alias_factory,
        tc_except_factory,
        tc_longlong_factory,
        tc_ulonglong_factory,
        tc_longdouble_factory,
        tc_wchar_factory,
        tc_wstring_factory,
        tc_fixed_factory,
        tc_value_factory,
        tc_value_box_factory,
        tc_native_factory,
        tc_abstract_interface_factory,
        tc_local_interface_factory,
        tc_component_factory,
        tc_home_factory,
        tc_event_factory

      };


  }  // End namespace TypeCodeFactory
}  // End namespace TAO


#include /**/ "ace/post.h"

#endif /* TAO_TYPECODE_FACTORY_MAP_H */
