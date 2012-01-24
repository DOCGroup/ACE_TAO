// -*- C++ -*-
// $Id$

#include "Foo_exec.h"

namespace CIAO_Foo_Impl
{
  //============================================================
  // Component Executor Implementation Class: Foo_exec_i
  //============================================================

  Foo_exec_i::Foo_exec_i (void)
    : my_short_ (false),
      my_long_ (false),
      my_float_ (false),
      my_double_ (false),
      my_short_sequence_ (false),
      my_empty_sequence_ (false),
      my_long_sequence_ (false),
      my_float_sequence_ (false),
      my_double_sequence_ (false),
      my_bar_struct_ (false),
      my_baz_struct_ (false),
      my_string_sequence_ (false),
      my_fixed_string_ (false),
      my_variable_string_ (false),
      supported_short_ (false)
  {
  }

  Foo_exec_i::~Foo_exec_i (void)
  {
  }

  // Supported operations and attributes.
  
  ::CORBA::Short
  Foo_exec_i::supported_short (void)
  {
    /* Your code here. */
    return 0;
  }

  void
  Foo_exec_i::supported_short (
    const ::CORBA::Short supported_short)
  {
    if(supported_short != 11)
      ACE_ERROR ((LM_ERROR, "ERROR: supported_short != 11, it is %d\n", supported_short));
    else supported_short_ = true;

  }

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
    else my_short_ = true;

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
    else my_long_ = true;

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
    else my_float_ = true;

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
    else my_double_ = true;
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

    my_short_sequence_ = true;

    if(my_short_sequence[0] != 11)
      {
        my_short_sequence_ = false;
        ACE_ERROR ((LM_ERROR, "ERROR: my_short_sequence[0] != 11, it is %d\n", my_short_sequence[0]));
      }

    if(my_short_sequence[1] != 12)
      {
        my_short_sequence_ = false;
        ACE_ERROR ((LM_ERROR, "ERROR: my_short_sequence[1] != 12, it is %d\n", my_short_sequence[1]));
      }

    if(my_short_sequence[2] != 13)
      {
        my_short_sequence_ = false;
        ACE_ERROR ((LM_ERROR, "ERROR: my_short_sequence[2] != 13, it is %d\n", my_short_sequence[2]));
      }
  }

  ::short_sequence *
  Foo_exec_i::my_empty_sequence (void)
  {
    /* Your code here. */
    return 0;
  }

  void
  Foo_exec_i::my_empty_sequence (
    const ::short_sequence & my_empty_sequence)
  {
    if(my_empty_sequence.length() != 0)
    {
      ACE_ERROR ((LM_ERROR, "ERROR: my_empty_sequence does not have the correct length\n"));
      return;
    }

    my_empty_sequence_ = true;
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

    my_long_sequence_ = true;

    if(my_long_sequence[0] != 21)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: my_long_sequence[0] != 21, it is %d\n", my_long_sequence[0]));
        my_long_sequence_ = false;
      }

    if(my_long_sequence[1] != 22)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: my_long_sequence[1] != 22, it is %d\n", my_long_sequence[1]));
        my_long_sequence_ = false;
      }

    if(my_long_sequence[2] != 23)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: my_long_sequence[2] != 23, it is %d\n", my_long_sequence[2]));
        my_long_sequence_ = false;
      }
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

    my_float_sequence_ = true;

    if(my_float_sequence[0] != 21.12F)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: my_float_sequence[0] != 21.12, it is %f\n", my_float_sequence[0]));
        my_float_sequence_ = false;
      }

    if(my_float_sequence[1] != 22.22F)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: my_float_sequence[1] != 22.22, it is %f\n", my_float_sequence[1]));
        my_float_sequence_ = false;
      }

    if(my_float_sequence[2] != 23.32F)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: my_float_sequence[2] != 23.32, it is %f\n", my_float_sequence[2]));
        my_float_sequence_ = false;
      }
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

    this->my_double_sequence_ = true;

    if(my_double_sequence[0] != 621.12)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: my_double_sequence[0] != 621.12, it is %f\n", my_double_sequence[0]));
        this->my_double_sequence_ = false;
      }


    if(my_double_sequence[1] != 622.22)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: my_double_sequence[1] != 622.22, it is %f\n", my_double_sequence[1]));
        this->my_double_sequence_ = false;
      }

    if(my_double_sequence[2] != 623.32)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: my_double_sequence[2] != 623.32, it is %f\n", my_double_sequence[2]));
        this->my_double_sequence_ = false;
      }
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
    my_bar_struct_ = true;

    if(my_bar_struct.s != 3)
      {
        my_bar_struct_ = false;
        ACE_ERROR ((LM_ERROR, "ERROR: short value != 3, it is %d\n", my_bar_struct.s));
      }

    if(my_bar_struct.l != 4)
      {
        my_bar_struct_ = false;
        ACE_ERROR ((LM_ERROR, "ERROR: long value != 4, it is %d\n", my_bar_struct.l));
      }

    if(my_bar_struct.f != 5.6F)
      {
        my_bar_struct_ = false;
        ACE_ERROR ((LM_ERROR, "ERROR: float value != 5.6, it is %f\n", my_bar_struct.f));
      }

    if(my_bar_struct.d != 7.8)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: double value != 7.8, it is %f\n", my_bar_struct.d));
        my_bar_struct_ = false;
      }
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

    my_baz_struct_ = true;

    if (ACE_OS::strcmp (my_baz_struct.name.in (),
                        "My Baz Struct") != 0)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: my_baz struct has incorrect name %C\n",
                    my_baz_struct.name.in ()));
        my_baz_struct_ = false;
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
          {
            my_baz_struct_ = false;
            ACE_ERROR ((LM_ERROR, "ERROR: short value != %d, it is %d\n",
                        3 + inc_s,
                        my_baz_struct.my_bar_sequence[i].s));
          }

        inc_s += 10;
        if(my_baz_struct.my_bar_sequence[i].l != 4 + inc_l)
          {
            my_baz_struct_ = false;
            ACE_ERROR ((LM_ERROR, "ERROR: long value != %d, it is %d\n",
                        4 + inc_l,
                        my_baz_struct.my_bar_sequence[i].l));
          }

        inc_l += 10;
        if(my_baz_struct.my_bar_sequence[i].f != 5.6F + inc_f)
          {
            my_baz_struct_ = false;
            ACE_ERROR ((LM_ERROR, "ERROR: float value != %f, it is %f\n",
                        5.6F + inc_f,
                        my_baz_struct.my_bar_sequence[i].f));
          }

        inc_f += 10.0F;

        if(my_baz_struct.my_bar_sequence[i].d != 7.8 + inc_d)
          {
            my_baz_struct_ = false;
            ACE_ERROR ((LM_ERROR, "ERROR: double value != %f, it is %f\n",
                        7.8 + inc_d,
                        my_baz_struct.my_bar_sequence[i].d));
          }

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
      const ::string_sequence & my_string_sequence)
  {
    if(my_string_sequence.length() != 2)
    {
      ACE_ERROR ((LM_ERROR, "ERROR: my_short_sequence does not have the correct length\n"));
      return;
    }

    my_string_sequence_ = true;

    if(ACE_OS::strcmp (my_string_sequence[0], "Hi") != 0)
      {
        my_string_sequence_ = false;
        ACE_ERROR ((LM_ERROR, "ERROR: my_string_sequence[0] != Hi, it is %C\n",
                    my_string_sequence[0].in ()));
      }

    if(ACE_OS::strcmp (my_string_sequence[1], "World") != 0)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: my_string_sequence[1] != World, it is %C\n",
                    my_string_sequence[1].in ()));
        my_string_sequence_ = false;
      }
  }

  char * Foo_exec_i::my_fixed_string (void)
  {
    return 0;
  }

  void Foo_exec_i::my_fixed_string (const char * my_fixed_string)
  {
    if(ACE_OS::strcmp (my_fixed_string, "Hi") != 0)
      ACE_ERROR ((LM_ERROR, "ERROR: my_fixed_string != Hi, it is %C\n",
                  my_fixed_string));
    else my_fixed_string_ = true;
  }

  char *Foo_exec_i::my_variable_string (void)
  {
    return 0;
  }

  void Foo_exec_i::my_variable_string (const char * my_variable_string)
  {
    if(ACE_OS::strcmp (my_variable_string, "Hi") != 0)
      ACE_ERROR ((LM_ERROR, "ERROR: my_variable_string != Hi, it is %C\n",
                  my_variable_string));
    else my_variable_string_ = true;
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

  ::long_array_slice *Foo_exec_i::my_long_array (void)
  {
    return 0;
  }

  void Foo_exec_i::my_long_array (const ::long_array /*my_long_array*/)
  {
  }

  ::string_array_slice *Foo_exec_i::my_string_array (void)
  {
    return 0;
  }

  void Foo_exec_i::my_string_array (const ::string_array /*my_string_array*/)
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
    if (!(my_short_ &&
          my_long_ &&
          my_float_ &&
          my_double_ &&
          my_short_sequence_ &&
          my_empty_sequence_ &&
          my_long_sequence_ &&
          my_float_sequence_ &&
          my_double_sequence_ &&
          my_bar_struct_ &&
          my_baz_struct_ &&
          my_string_sequence_ &&
          my_fixed_string_ &&
          my_variable_string_ &&
          supported_short_))
      {
        ACE_ERROR ((LM_ERROR, "ERROR: Not all expected attributes were initialized\n"));
      }
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

