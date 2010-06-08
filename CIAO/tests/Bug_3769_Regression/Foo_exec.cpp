// -*- C++ -*-
// $Id$

#include "Foo_exec.h"

namespace CIAO_Foo_Impl
{
  //============================================================
  // Component Executor Implementation Class: Foo_exec_i
  //============================================================
  
  Foo_exec_i::Foo_exec_i (void)
  {
  }
  
  Foo_exec_i::~Foo_exec_i (void)
  {
  }
  
  // Supported operations and attributes.
  
  // Component attributes and port operations.
  
  ::CORBA::Short
  Foo_exec_i::my_short (void)
  {
    /* Your code here. */
    return 0;
  }
  
  void
  Foo_exec_i::my_short (
    ::CORBA::Short my_short)
  {
    if(my_short != 22)
      ACE_ERROR ((LM_ERROR, "ERROR: my_short != 22, it is %d\n", my_short));
  }
  
  ::CORBA::Long
  Foo_exec_i::my_long (void)
  {
    /* Your code here. */
    return 0;
  }
  
  void
  Foo_exec_i::my_long (
    ::CORBA::Long my_long)
  {
    if(my_long != 33)
      ACE_ERROR ((LM_ERROR, "ERROR: my_long != 33, it is %d\n", my_long));
  }
  
  ::CORBA::Float
  Foo_exec_i::my_float (void)
  {
    /* Your code here. */
    return 0.0f;
  }
  
  void
  Foo_exec_i::my_float (
    ::CORBA::Float my_float)
  {
    if(my_float != 45.67F)
      ACE_ERROR ((LM_ERROR, "ERROR: my_float != 45.67, it is %f\n", my_float));
  }
  
  ::CORBA::Double
  Foo_exec_i::my_double (void)
  {
    /* Your code here. */
    return 0.0;
  }
  
  void
  Foo_exec_i::my_double (
    ::CORBA::Double my_double)
  {
    if(my_double != 56.78)
      ACE_ERROR ((LM_ERROR, "ERROR: my_double != 56.78, it is %f\n", my_double));
  }
  
  ::short_sequence *
  Foo_exec_i::my_short_sequence (void)
  {
    /* Your code here. */
    return 0;
  }
  
  void
  Foo_exec_i::my_short_sequence (
    const ::short_sequence & my_short_sequence)
  {
    if(my_short_sequence.length() != 3)
    {
      ACE_ERROR ((LM_ERROR, "ERROR: my_short_sequence does not have the correct length\n"));
      return;
    }

    if(my_short_sequence[0] != 11)
      ACE_ERROR ((LM_ERROR, "ERROR: my_short_sequence[0] != 11, it is %d\n", my_short_sequence[0]));

    if(my_short_sequence[1] != 12)
      ACE_ERROR ((LM_ERROR, "ERROR: my_short_sequence[1] != 12, it is %d\n", my_short_sequence[1]));

    if(my_short_sequence[2] != 13)
      ACE_ERROR ((LM_ERROR, "ERROR: my_short_sequence[2] != 13, it is %d\n", my_short_sequence[2]));
  }
  
  ::long_sequence *
  Foo_exec_i::my_long_sequence (void)
  {
    /* Your code here. */
    return 0;
  }
  
  void
  Foo_exec_i::my_long_sequence (
    const ::long_sequence & my_long_sequence)
  {
    if(my_long_sequence.length() != 3)
    {
      ACE_ERROR ((LM_ERROR, "ERROR: my_long_sequence does not have the correct length\n"));
      return;
    }

    if(my_long_sequence[0] != 21)
      ACE_ERROR ((LM_ERROR, "ERROR: my_long_sequence[0] != 21, it is %d\n", my_long_sequence[0]));

    if(my_long_sequence[1] != 22)
      ACE_ERROR ((LM_ERROR, "ERROR: my_long_sequence[1] != 22, it is %d\n", my_long_sequence[1]));

    if(my_long_sequence[2] != 23)
      ACE_ERROR ((LM_ERROR, "ERROR: my_long_sequence[2] != 23, it is %d\n", my_long_sequence[2]));
  }
  
  ::float_sequence *
  Foo_exec_i::my_float_sequence (void)
  {
    /* Your code here. */
    return 0;
  }
  
  void
  Foo_exec_i::my_float_sequence (
    const ::float_sequence & my_float_sequence)
  {
    if(my_float_sequence.length() != 3)
    {
      ACE_ERROR ((LM_ERROR, "ERROR: my_float_sequence does not have the correct length\n"));
      return;
    }

    if(my_float_sequence[0] != 21.12F)
      ACE_ERROR ((LM_ERROR, "ERROR: my_float_sequence[0] != 21.12, it is %f\n", my_float_sequence[0]));

    if(my_float_sequence[1] != 22.22F)
      ACE_ERROR ((LM_ERROR, "ERROR: my_float_sequence[1] != 22.22, it is %f\n", my_float_sequence[1]));

    if(my_float_sequence[2] != 23.32F)
      ACE_ERROR ((LM_ERROR, "ERROR: my_float_sequence[2] != 23.32, it is %f\n", my_float_sequence[2]));
  }
  
  ::double_sequence *
  Foo_exec_i::my_double_sequence (void)
  {
    /* Your code here. */
    return 0;
  }
  
  void
  Foo_exec_i::my_double_sequence (
    const ::double_sequence & my_double_sequence)
  {
    if(my_double_sequence.length() != 3)
    {
      ACE_ERROR ((LM_ERROR, "ERROR: my_double_sequence does not have the correct length\n"));
      return;
    }

    if(my_double_sequence[0] != 621.12)
      ACE_ERROR ((LM_ERROR, "ERROR: my_double_sequence[0] != 621.12, it is %f\n", my_double_sequence[0]));

    if(my_double_sequence[1] != 622.22)
      ACE_ERROR ((LM_ERROR, "ERROR: my_double_sequence[1] != 622.22, it is %f\n", my_double_sequence[1]));

    if(my_double_sequence[2] != 623.32)
      ACE_ERROR ((LM_ERROR, "ERROR: my_double_sequence[2] != 623.32, it is %f\n", my_double_sequence[2]));
  }
  
  ::Bar
  Foo_exec_i::my_bar_struct (void)
  {
    /* Your code here. */
    return ::Bar ();
  }
  
  void
  Foo_exec_i::my_bar_struct (
    const ::Bar & my_bar_struct)
  {
    if(my_bar_struct.s != 3)
      ACE_ERROR ((LM_ERROR, "ERROR: short value != 3, it is %d\n", my_bar_struct.s));

    if(my_bar_struct.l != 4)
      ACE_ERROR ((LM_ERROR, "ERROR: long value != 4, it is %d\n", my_bar_struct.l));

    if(my_bar_struct.f != 5.6F)
      ACE_ERROR ((LM_ERROR, "ERROR: float value != 5.6, it is %f\n", my_bar_struct.f));

    if(my_bar_struct.d != 7.8)
      ACE_ERROR ((LM_ERROR, "ERROR: double value != 7.8, it is %f\n", my_bar_struct.d));
  }

  ::Baz *
  Foo_exec_i::my_baz_struct (void)
  {
    /* Your code here. */
    return 0;
  }
  
  void
  Foo_exec_i::my_baz_struct (
    const ::Baz & my_baz_struct)
  {
    if (my_baz_struct.my_bar_sequence.length () != 5)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: my_bar_sequence is not 5\n"));
        return;
      }
    
    if (ACE_OS::strcmp (my_baz_struct.name.in (),
                        "My Baz Struct") != 0)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: my_baz struct has incorrect name %C\n",
                    my_baz_struct.name.in ()));
      }
      
    CORBA::Short inc_s = 0;
    CORBA::Long inc_l = 0;
    CORBA::Float inc_f = 0;
    CORBA::Double inc_d = 0;
    
    for (CORBA::ULong i = 0;
         i < my_baz_struct.my_bar_sequence.length ();
         ++i)
      {
        if(my_baz_struct.my_bar_sequence[i].s != 3 + inc_s)
          ACE_ERROR ((LM_ERROR, "ERROR: short value != %d, it is %d\n", 
                      3 + inc_s,
                      my_baz_struct.my_bar_sequence[i].s));
        inc_s += 10;
        if(my_baz_struct.my_bar_sequence[i].l != 4 + inc_l)
          ACE_ERROR ((LM_ERROR, "ERROR: long value != %d, it is %d\n", 
                      4 + inc_l,
                      my_baz_struct.my_bar_sequence[i].l));
        inc_l += 10;
        if(my_baz_struct.my_bar_sequence[i].f != 5.6F + inc_f)
          ACE_ERROR ((LM_ERROR, "ERROR: float value != %f, it is %f\n", 
                      5.6F + inc_f,
                      my_baz_struct.my_bar_sequence[i].f));
        inc_f += 10.0F;

        if(my_baz_struct.my_bar_sequence[i].s != 7.8 + inc_d)
          ACE_ERROR ((LM_ERROR, "ERROR: short value != %f, it is %f\n", 
                      7.8 + inc_d,
                      my_baz_struct.my_bar_sequence[i].d));
        inc_d += 10.0;
      }
  }

  ::Data Foo_exec_i::my_data_union (void)
  {
    Data d;
    return d;
  }
  

  void
  Foo_exec_i::my_data_union (const ::Data & /*my_data_union*/)
  {
  }

  ::Data2* Foo_exec_i::my_var_data_union (void)
  {
    return 0;
  }

  void Foo_exec_i::my_var_data_union (const ::Data2 & /*my_data_union*/)
  {
  }

  ::string_sequence * Foo_exec_i::my_string_sequence (void)
  {
    return 0;
  }
  
  void Foo_exec_i::my_string_sequence (
      const ::string_sequence & /*my_string_sequence*/)
  {
  }

  char * Foo_exec_i::my_fixed_string (void)
  {
    return 0;
  }

  void Foo_exec_i::my_fixed_string (const char * /*my_fixed_string*/)
  {
  }

  char *Foo_exec_i::my_variable_string (void)
  {
    return 0;
  }

  void Foo_exec_i::my_variable_string (const char * /*my_variable_string*/)
  {
  }

  ::CORBA::WChar *Foo_exec_i::my_fixed_wstring (void)
  {
    return 0;
  }

  void Foo_exec_i::my_fixed_wstring (const ::CORBA::WChar * /*my_fixed_wstring*/)
  {
  }

  ::CORBA::WChar *Foo_exec_i::my_variable_wstring (void)
  {
    return 0;
  }

  void Foo_exec_i::my_variable_wstring (
      const ::CORBA::WChar * /*my_variable_wstring*/)
  {
  }
 
  // Operations from Components::SessionComponent.
 
  void
  Foo_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::CCM_Foo_Context::_narrow (ctx);
    
    if ( ::CORBA::is_nil (this->context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }
  
  void
  Foo_exec_i::configuration_complete (void)
  {
  }
  
  void
  Foo_exec_i::ccm_activate (void)
  {
  }
  
  void
  Foo_exec_i::ccm_passivate (void)
  {
  }
  
  void
  Foo_exec_i::ccm_remove (void)
  {
  }
  
  extern "C" FOO_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Foo_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();
    
    ACE_NEW_NORETURN (
      retval,
      Foo_exec_i);
    
    return retval;
  }
}

