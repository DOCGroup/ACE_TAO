// -*- C++ -*-
// $Id$

#ifndef CIAO_FOO_EXEC_H_
#define CIAO_FOO_EXEC_H_

#include "FooEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "Foo_exec_export.h"
#include "tao/LocalObject.h"

namespace CIAO_Foo_Impl
{
  class FOO_EXEC_Export Foo_exec_i
    : public virtual Foo_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    Foo_exec_i (void);
    virtual ~Foo_exec_i (void);

    //@{
    /** Supported operations and attributes. */

    virtual ::CORBA::Short supported_short (void);

    virtual void supported_short (::CORBA::Short supported_short);
    //@}

    //@{
    /** Component attributes and port operations. */
    virtual ::CORBA::Char my_char (void);

    virtual void my_char (::CORBA::Char my_char);

    virtual ::CORBA::Octet my_octet (void);

    virtual void my_octet (::CORBA::Octet my_octet);

    virtual ::CORBA::Short my_short (void);

    virtual void my_short (::CORBA::Short my_short);

    virtual ::CORBA::Boolean my_boolean (void);

    virtual void my_boolean (::CORBA::Boolean my_boolean);

    virtual ::CORBA::Long my_long (void);

    virtual void my_long (::CORBA::Long my_long);

    virtual ::CORBA::ULong my_ulong (void);

    virtual void my_ulong (::CORBA::ULong my_ulong);

    virtual ::CORBA::LongLong my_longlong (void);

    virtual void my_longlong (::CORBA::LongLong my_longlong);

    virtual ::CORBA::ULongLong my_ulonglong (void);

    virtual void my_ulonglong (::CORBA::ULongLong my_ulonglong);

    virtual ::CORBA::Float my_float (void);

    virtual void my_float (::CORBA::Float my_float);

    virtual ::CORBA::Double my_double (void);

    virtual void my_double (::CORBA::Double my_double);

    virtual ::CORBA::LongDouble my_longdouble (void);

    virtual void my_longdouble (::CORBA::LongDouble my_longdouble);

    virtual ::CORBA::Char my_default_char (void);

    virtual void my_default_char (::CORBA::Char my_default_char);

    virtual ::CORBA::Octet my_default_octet (void);

    virtual void my_default_octet (::CORBA::Octet my_default_octet);

    virtual ::CORBA::Short my_default_short (void);

    virtual void my_default_short (::CORBA::Short my_default_short);

    virtual ::CORBA::Boolean my_default_boolean (void);

    virtual void my_default_boolean (::CORBA::Boolean my_default_boolean);

    virtual ::CORBA::Long my_default_long (void);

    virtual void my_default_long (::CORBA::Long my_default_long);

    virtual ::CORBA::ULong my_default_ulong (void);

    virtual void my_default_ulong (::CORBA::ULong my_default_ulong);

    virtual ::CORBA::LongLong my_default_longlong (void);

    virtual void my_default_longlong (::CORBA::LongLong my_default_longlong);

    virtual ::CORBA::ULongLong my_default_ulonglong (void);

    virtual void my_default_ulonglong (::CORBA::ULongLong my_default_ulonglong);

    virtual ::CORBA::Float my_default_float (void);

    virtual void my_default_float (::CORBA::Float my_default_float);

    virtual ::CORBA::Double my_default_double (void);

    virtual void my_default_double (::CORBA::Double my_default_double);

    virtual ::CORBA::LongDouble my_default_longdouble (void);

    virtual void my_default_longdouble (::CORBA::LongDouble my_default_longdouble);

    virtual ::short_sequence * my_short_sequence (void);

    virtual void my_short_sequence (const ::short_sequence & my_empty_sequence);

    virtual ::short_sequence * my_empty_sequence (void);

    virtual void my_empty_sequence (const ::short_sequence & my_empty_sequence);

    virtual ::long_sequence * my_long_sequence (void);

    virtual void my_long_sequence (const ::long_sequence & my_long_sequence);

    virtual ::float_sequence * my_float_sequence (void);

    virtual void my_float_sequence (const ::float_sequence & my_float_sequence);

    virtual ::double_sequence * my_double_sequence (void);

    virtual void my_double_sequence (const ::double_sequence & my_double_sequence);

    virtual ::Bar my_bar_struct (void);

    virtual void my_bar_struct (const ::Bar & my_bar_struct);

    virtual ::Bar my_default_bar_struct (void);

    virtual void my_default_bar_struct (const ::Bar & my_default_bar_struct);

    virtual ::BarArray_slice* my_bar_array (void);

    virtual void my_bar_array (const ::BarArray my_bar_array);

    virtual ::BarArray2_slice * my_bar_array_2 (void);

    virtual void my_bar_array_2 (const ::BarArray2 my_bar_array_2);

    virtual ::SequenceArray_slice * my_sequence_array (void);

    virtual void my_sequence_array (const ::SequenceArray my_sequence_array);

    virtual ::BarStruct my_bar_struct_array (void);

    virtual void my_bar_struct_array (const ::BarStruct & my_bar_struct_array);

    virtual ::Baz *my_baz_struct (void);

    virtual void my_baz_struct (const ::Baz & my_baz_struct);

    virtual ::Data my_data_union (void);

    virtual void my_data_union (const ::Data & my_data_union);

    virtual ::Data2* my_var_data_union (void);

    virtual void my_var_data_union (const ::Data2 & my_data_union);

    virtual ::string_sequence *my_string_sequence (void);

    virtual void my_string_sequence (
      const ::string_sequence & my_string_sequence);

    virtual ::string_sequence2 * my_string_sequence_2 (void);

    virtual void my_string_sequence_2 (const ::string_sequence2 & my_string_sequence_2);

    virtual ::array_sequence * my_array_sequence (void);

    virtual void my_array_sequence (const ::array_sequence & my_array_sequence);

    virtual char *my_bounded_string (void);

    virtual void my_bounded_string (const char * my_bounded_string);

    virtual char *my_variable_string (void);

    virtual void my_variable_string (const char * my_variable_string);

    virtual char *my_default_variable_string (void);

    virtual void my_default_variable_string (const char * my_default_variable_string);

    virtual ::CORBA::WChar *my_bounded_wstring (void);

    virtual void my_bounded_wstring (const ::CORBA::WChar * my_bounded_wstring);

    virtual ::CORBA::WChar *my_variable_wstring (void);

    virtual void my_variable_wstring (
      const ::CORBA::WChar * my_variable_wstring);

    virtual ::long_array_slice *my_long_array (void);

    virtual void my_long_array (const ::long_array my_long_array);

    virtual ::ulong_array_slice *my_ulong_array (void);

    virtual void my_ulong_array (const ::ulong_array my_ulong_array);

    virtual ::longlong_array_slice * my_longlong_array (void);

    virtual void my_longlong_array (const ::longlong_array my_longlong_array);

    virtual ::ulonglong_array_slice * my_ulonglong_array (void);

    virtual void my_ulonglong_array (const ::ulonglong_array my_ulonglong_array);

    virtual ::short_array_slice * my_short_array (void);

    virtual void my_short_array (const ::short_array my_short_array);

    virtual ::ushort_array_slice * my_ushort_array (void);

    virtual void my_ushort_array (const ::ushort_array my_ushort_array);

    virtual ::string_array_slice * my_string_array (void);

    virtual void my_string_array (const ::string_array my_string_array);

    virtual ::wstring_array_slice * my_wstring_array (void);

    virtual void my_wstring_array (const ::wstring_array my_wstring_array);

    virtual ::char_array_slice * my_char_array (void);

    virtual void my_char_array (const ::char_array my_char_array);

    virtual ::wchar_array_slice * my_wchar_array (void);

    virtual void my_wchar_array (const ::wchar_array my_wchar_array);

    virtual ::double_array_slice * my_double_array (void);

    virtual void my_double_array (const ::double_array my_double_array);

    virtual ::longdouble_array_slice *my_longdouble_array (void);

    virtual void my_longdouble_array (const ::longdouble_array my_longdouble_array);

    virtual ::float_array_slice * my_float_array (void);

    virtual void my_float_array (const ::float_array my_float_array);

    virtual ::boolean_array_slice * my_boolean_array (void);

    virtual void my_boolean_array (const ::boolean_array my_boolean_array);

    virtual ::octet_array_slice * my_octet_array (void);

    virtual void my_octet_array (const ::octet_array my_octet_array);

    virtual StructModule::StructStruct my_struct_struct (void);

    virtual void my_struct_struct (const StructModule::StructStruct & my_struct_struct);

    //@}

    //@{
    /** Operations from Components::SessionComponent. */

    virtual void
    set_session_context (
      ::Components::SessionContext_ptr ctx);

    virtual void configuration_complete (void);

    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);
    //@}


  private:
    ::CCM_Foo_Context_var context_;

    bool
      my_octet_success_,
      my_short_success_,
      my_boolean_success_,
      my_long_success_,
      my_ulong_success_,
      my_longlong_success_,
      my_ulonglong_success_,
      my_float_success_,
      my_double_success_,
      my_default_octet_success_,
      my_default_short_success_,
      my_default_boolean_success_,
      my_default_long_success_,
      my_default_ulong_success_,
      my_default_longlong_success_,
      my_default_ulonglong_success_,
      my_default_float_success_,
      my_default_double_success_,
      my_short_sequence_success_,
      my_empty_sequence_success_,
      my_long_sequence_success_,
      my_long_array_success_,
      my_ulong_array_success_,
      my_longlong_array_success_,
      my_ulonglong_array_success_,
      my_short_array_success_,
      my_ushort_array_success_,
      my_string_array_success_,
      my_double_array_success_,
      my_float_array_success_,
      my_boolean_array_success_,
      my_octet_array_success_,
      my_float_sequence_success_,
      my_double_sequence_success_,
      my_bar_struct_success_,
      my_default_bar_struct_success_,
      my_bar_array_success_,
      my_bar_array_2_success_,
      my_sequence_array_success_,
      my_bar_struct_array_success_,
      my_baz_struct_success_,
      my_string_sequence_success_,
      my_string_sequence_2_success_,
      my_array_sequence_success_,
      my_bounded_string_success_,
      my_variable_string_success_,
      my_default_variable_string_success_,
      supported_short_success_,
      my_struct_struct_success_;

  };

  extern "C" FOO_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Foo_Impl (void);
}

#endif /* ifndef */

