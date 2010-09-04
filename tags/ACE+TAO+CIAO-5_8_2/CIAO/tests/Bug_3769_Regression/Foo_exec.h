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
    
    //@}
    
    //@{
    /** Component attributes and port operations. */
    
    
    // TAO_IDL - Generated from
    // c:\middleware\ace_devo\tao\tao_idl\be\be_visitor_operation/operation_ch.cpp:42
    
    virtual ::CORBA::Short
    my_short (void);
    
    // TAO_IDL - Generated from
    // c:\middleware\ace_devo\tao\tao_idl\be\be_visitor_operation/operation_ch.cpp:42
    
    virtual void
    my_short (
      ::CORBA::Short my_short);
    
    // TAO_IDL - Generated from
    // c:\middleware\ace_devo\tao\tao_idl\be\be_visitor_operation/operation_ch.cpp:42
    
    virtual ::CORBA::Long
    my_long (void);
    
    // TAO_IDL - Generated from
    // c:\middleware\ace_devo\tao\tao_idl\be\be_visitor_operation/operation_ch.cpp:42
    
    virtual void
    my_long (
      ::CORBA::Long my_long);
    
    // TAO_IDL - Generated from
    // c:\middleware\ace_devo\tao\tao_idl\be\be_visitor_operation/operation_ch.cpp:42
    
    virtual ::CORBA::Float
    my_float (void);
    
    // TAO_IDL - Generated from
    // c:\middleware\ace_devo\tao\tao_idl\be\be_visitor_operation/operation_ch.cpp:42
    
    virtual void
    my_float (
      ::CORBA::Float my_float);
    
    // TAO_IDL - Generated from
    // c:\middleware\ace_devo\tao\tao_idl\be\be_visitor_operation/operation_ch.cpp:42
    
    virtual ::CORBA::Double
    my_double (void);
    
    // TAO_IDL - Generated from
    // c:\middleware\ace_devo\tao\tao_idl\be\be_visitor_operation/operation_ch.cpp:42
    
    virtual void
    my_double (
      ::CORBA::Double my_double);
    
    // TAO_IDL - Generated from
    // c:\middleware\ace_devo\tao\tao_idl\be\be_visitor_operation/operation_ch.cpp:42
    
    virtual ::short_sequence *
    my_short_sequence (void);
    
    // TAO_IDL - Generated from
    // c:\middleware\ace_devo\tao\tao_idl\be\be_visitor_operation/operation_ch.cpp:42
    
    virtual void
    my_short_sequence (
      const ::short_sequence & my_short_sequence);
    
    // TAO_IDL - Generated from
    // c:\middleware\ace_devo\tao\tao_idl\be\be_visitor_operation/operation_ch.cpp:42
    
    virtual ::long_sequence *
    my_long_sequence (void);
    
    // TAO_IDL - Generated from
    // c:\middleware\ace_devo\tao\tao_idl\be\be_visitor_operation/operation_ch.cpp:42
    
    virtual void
    my_long_sequence (
      const ::long_sequence & my_long_sequence);
    
    // TAO_IDL - Generated from
    // c:\middleware\ace_devo\tao\tao_idl\be\be_visitor_operation/operation_ch.cpp:42
    
    virtual ::float_sequence *
    my_float_sequence (void);
    
    // TAO_IDL - Generated from
    // c:\middleware\ace_devo\tao\tao_idl\be\be_visitor_operation/operation_ch.cpp:42
    
    virtual void
    my_float_sequence (
      const ::float_sequence & my_float_sequence);
    
    // TAO_IDL - Generated from
    // c:\middleware\ace_devo\tao\tao_idl\be\be_visitor_operation/operation_ch.cpp:42
    
    virtual ::double_sequence *
    my_double_sequence (void);
    
    // TAO_IDL - Generated from
    // c:\middleware\ace_devo\tao\tao_idl\be\be_visitor_operation/operation_ch.cpp:42
    
    virtual void my_double_sequence (
      const ::double_sequence & my_double_sequence);
    
    virtual ::Bar my_bar_struct (void);
    
    virtual void my_bar_struct (const ::Bar & my_bar_struct);

    virtual ::Baz *my_baz_struct (void);
    
    virtual void my_baz_struct (const ::Baz & my_baz_struct);

    virtual ::Data my_data_union (void);
    
    virtual void my_data_union (const ::Data & my_data_union);

    virtual ::Data2* my_var_data_union (void);

    virtual void my_var_data_union (const ::Data2 & my_data_union);

    virtual ::string_sequence *my_string_sequence (void);

    virtual void my_string_sequence (
      const ::string_sequence & my_string_sequence);

    virtual char *my_fixed_string (void);

    virtual void my_fixed_string (const char * my_fixed_string);

    virtual char *my_variable_string (void);

    virtual void my_variable_string (const char * my_variable_string);

    virtual ::CORBA::WChar *my_fixed_wstring (void);

    virtual void my_fixed_wstring (const ::CORBA::WChar * my_fixed_wstring);

    virtual ::CORBA::WChar *my_variable_wstring (void);

    virtual void my_variable_wstring (
      const ::CORBA::WChar * my_variable_wstring);

    virtual ::long_array_slice *my_long_array (void);

    virtual void my_long_array (const ::long_array my_long_array);

    virtual ::string_array_slice *my_string_array (void);

    virtual void my_string_array (const ::string_array my_string_array);
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
      my_short_,
      my_long_,
      my_float_,
      my_double_,
      my_short_sequence_,
      my_long_sequence_,
      my_float_sequence_,
      my_double_sequence_,
      my_bar_struct_,
      my_baz_struct_,
      my_string_sequence_,
      my_fixed_string_,
      my_variable_string_;
    
  };
  
  extern "C" FOO_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Foo_Impl (void);
}

#endif /* ifndef */

