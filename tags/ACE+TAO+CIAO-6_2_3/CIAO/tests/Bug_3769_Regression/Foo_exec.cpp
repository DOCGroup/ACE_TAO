// -*- C++ -*-
// $Id$

#include "Foo_exec.h"

// Helper methods.
// Indicates which test fails.
void check_error (bool success, const char * test)
{
  if (!success)
    {
      ACE_ERROR ((LM_ERROR, "ERROR: Found errors in the <%C> attribute test.\n", test));
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG, "OK: No errors found in the <%C> attribute test.\n", test));
    }
}

template<typename ARRAY, typename ITER>
class check_array
{
public:
  check_array (void) {}

  static bool check (ARRAY array, const char * name,
                     ITER multiplier, int dimension)
  {
    bool ret = true;
    for (int i = 0; i < dimension; ++i)
      {
        if (array[i] != (i+1)*multiplier)
          {
            ACE_ERROR ((LM_ERROR, "ERROR: %C[%d] != %d, it is %d\n",
                        name, i, (i+1)*multiplier, array[i]));
            ret = false;
          }
      }
    return ret;
  }
};



namespace CIAO_Foo_Impl
{
  //============================================================
  // Component Executor Implementation Class: Foo_exec_i
  //============================================================

  Foo_exec_i::Foo_exec_i (void)
    : my_octet_success_ (false),
      my_short_success_ (false),
      my_boolean_success_ (false),
      my_long_success_ (false),
      my_ulong_success_ (false),
      my_longlong_success_ (false),
      my_ulonglong_success_ (false),
      my_float_success_ (false),
      my_double_success_ (false),
      my_default_octet_success_ (false),
      my_default_short_success_ (false),
      my_default_boolean_success_ (false),
      my_default_long_success_ (false),
      my_default_ulong_success_ (false),
      my_default_longlong_success_ (false),
      my_default_ulonglong_success_ (false),
      my_default_float_success_ (false),
      my_default_double_success_ (false),
      my_short_sequence_success_ (false),
      my_empty_sequence_success_ (false),
      my_long_sequence_success_ (false),
      my_long_array_success_ (false),
      my_ulong_array_success_ (false),
      my_longlong_array_success_ (false),
      my_ulonglong_array_success_ (false),
      my_short_array_success_ (false),
      my_ushort_array_success_ (false),
      my_string_array_success_ (false),
      my_double_array_success_ (false),
      my_float_array_success_ (false),
      my_boolean_array_success_ (false),
      my_octet_array_success_ (false),
      my_float_sequence_success_ (false),
      my_double_sequence_success_ (false),
      my_bar_struct_success_ (false),
      my_default_bar_struct_success_ (false),
      my_bar_array_success_ (false),
      my_bar_array_2_success_ (false),
      my_sequence_array_success_ (false),
      my_bar_struct_array_success_ (false),
      my_baz_struct_success_ (false),
      my_string_sequence_success_ (false),
      my_string_sequence_2_success_ (false),
      my_array_sequence_success_ (false),
      my_bounded_string_success_ (false),
      my_variable_string_success_ (false),
      my_default_variable_string_success_ (false),
      supported_short_success_ (false),
      my_struct_struct_success_ (false)
  {
  }

  Foo_exec_i::~Foo_exec_i (void)
  {
  }

  // Supported operations and attributes.

  ::CORBA::Short
  Foo_exec_i::supported_short (void)
  {
    return 0;
  }

  void
  Foo_exec_i::supported_short (
    const ::CORBA::Short supported_short)
  {
    if (supported_short != 11)
      ACE_ERROR ((LM_ERROR, "ERROR: supported_short != 11, it is %d\n", supported_short));
    else this->supported_short_success_ = true;

  }

  // Component attributes and port operations.
  ::CORBA::Char
  Foo_exec_i::my_char (void)
  {
    return 0;
  }

  void
  Foo_exec_i::my_char (
    ::CORBA::Char /*my_char*/)
  {
    // Not supported since it's not configured in the xsd.
  }

  ::CORBA::Octet
  Foo_exec_i::my_octet (void)
  {
    return 0;
  }

  void
  Foo_exec_i::my_octet (
    ::CORBA::Octet my_octet)
  {
    if (my_octet != 20)
      ACE_ERROR ((LM_ERROR, "ERROR: my_octet != 20, it is %d\n", my_octet));
    else this->my_octet_success_ = true;
  }

  ::CORBA::Short
  Foo_exec_i::my_short (void)
  {
    return 0;
  }

  void
  Foo_exec_i::my_short (
    ::CORBA::Short my_short)
  {
    if (my_short != 22)
      ACE_ERROR ((LM_ERROR, "ERROR: my_short != 22, it is %d\n", my_short));
    else this->my_short_success_ = true;

  }

  ::CORBA::Boolean
  Foo_exec_i::my_boolean (void)
  {
    return 0;
  }

  void
  Foo_exec_i::my_boolean (
    ::CORBA::Boolean my_boolean)
  {
    if (!my_boolean)
      ACE_ERROR ((LM_ERROR, "ERROR: my_boolean != true, it is false\n"));
    else this->my_boolean_success_ = true;

  }

  ::CORBA::Long
  Foo_exec_i::my_long (void)
  {
    return 0;
  }

  void
  Foo_exec_i::my_long (
    ::CORBA::Long my_long)
  {
    if (my_long != 33)
      ACE_ERROR ((LM_ERROR, "ERROR: my_long != 33, it is %d\n", my_long));
    else this->my_long_success_ = true;

  }

  ::CORBA::ULong
  Foo_exec_i::my_ulong (void)
  {
    return 0;
  }

  void
  Foo_exec_i::my_ulong (
    ::CORBA::ULong my_ulong)
  {
    if (my_ulong != 33)
      ACE_ERROR ((LM_ERROR, "ERROR: my_ulong != 33, it is %d\n", my_ulong));
    else this->my_ulong_success_ = true;

  }

  ::CORBA::LongLong
  Foo_exec_i::my_longlong (void)
  {
    return 0;
  }

  void
  Foo_exec_i::my_longlong (
    ::CORBA::LongLong my_longlong)
  {
    if (my_longlong != 33)
      ACE_ERROR ((LM_ERROR, "ERROR: my_longlong != 33, it is %d\n", my_longlong));
    else this->my_longlong_success_ = true;

  }

  ::CORBA::ULongLong
  Foo_exec_i::my_ulonglong (void)
  {
    return 0;
  }

  void
  Foo_exec_i::my_ulonglong (
    ::CORBA::ULongLong my_ulonglong)
  {
    if (my_ulonglong != 33)
      ACE_ERROR ((LM_ERROR, "ERROR: my_ulonglong != 33, it is %d\n", my_ulonglong));
    else this->my_ulonglong_success_ = true;

  }

  ::CORBA::Float
  Foo_exec_i::my_float (void)
  {
    return 0.0f;
  }

  void
  Foo_exec_i::my_float (
    ::CORBA::Float my_float)
  {
    if (ACE::is_inequal (my_float, 45.67F))
      ACE_ERROR ((LM_ERROR, "ERROR: my_float != 45.67, it is %f\n", my_float));
    else this->my_float_success_ = true;

  }

  ::CORBA::Double
  Foo_exec_i::my_double (void)
  {
    return 0.0;
  }

  void
  Foo_exec_i::my_double (
    ::CORBA::Double my_double)
  {
    if (ACE::is_inequal (my_double, 56.78))
      ACE_ERROR ((LM_ERROR, "ERROR: my_double != 56.78, it is %f\n", my_double));
    else this->my_double_success_ = true;
  }

  ::CORBA::LongDouble
  Foo_exec_i::my_longdouble (void)
  {
    CORBA::LongDouble ld;
    ACE_CDR_LONG_DOUBLE_ASSIGNMENT (ld, 0.0);
    return ld;
  }

  void
  Foo_exec_i::my_longdouble (
    ::CORBA::LongDouble /*my_longdouble*/)
  {
    // Not supported since a longdouble is defined in the xsd as double.
    // We are then creating a long double initialized with a regular
    // double. This is a very tricky conversion and doesn't work in
    // combination with certain (versions of) compilers.
  }

  // Component attributes and port operations.
  ::CORBA::Char
  Foo_exec_i::my_default_char (void)
  {
    return 0;
  }

  void
  Foo_exec_i::my_default_char (
    ::CORBA::Char /*my_default_char*/)
  {
    // Not supported since it's not configured in the xsd.
  }

  ::CORBA::Octet
  Foo_exec_i::my_default_octet (void)
  {
    return 0;
  }

  void
  Foo_exec_i::my_default_octet (
    ::CORBA::Octet my_default_octet)
  {
    if (my_default_octet != 0)
      ACE_ERROR ((LM_ERROR, "ERROR: my_default_octet != 0, it is %d\n", my_default_octet));
    else this->my_default_octet_success_ = true;
  }

  ::CORBA::Short
  Foo_exec_i::my_default_short (void)
  {
    return 0;
  }

  void
  Foo_exec_i::my_default_short (
    ::CORBA::Short my_default_short)
  {
    if (my_default_short != 0)
      ACE_ERROR ((LM_ERROR, "ERROR: my_default_short != 0, it is %d\n", my_default_short));
    else this->my_default_short_success_ = true;

  }

  ::CORBA::Boolean
  Foo_exec_i::my_default_boolean (void)
  {
    return 0;
  }

  void
  Foo_exec_i::my_default_boolean (
    ::CORBA::Boolean my_default_boolean)
  {
    if (my_default_boolean)
      ACE_ERROR ((LM_ERROR, "ERROR: my_default_boolean != false, it is true\n"));
    else this->my_default_boolean_success_ = true;

  }

  ::CORBA::Long
  Foo_exec_i::my_default_long (void)
  {
    return 0;
  }

  void
  Foo_exec_i::my_default_long (
    ::CORBA::Long my_default_long)
  {
    if (my_default_long != 0)
      ACE_ERROR ((LM_ERROR, "ERROR: my_default_long != 0, it is %d\n", my_default_long));
    else this->my_default_long_success_ = true;

  }

  ::CORBA::ULong
  Foo_exec_i::my_default_ulong (void)
  {
    return 0;
  }

  void
  Foo_exec_i::my_default_ulong (
    ::CORBA::ULong my_default_ulong)
  {
    if (my_default_ulong != 0)
      ACE_ERROR ((LM_ERROR, "ERROR: my_default_ulong != 0, it is %d\n", my_default_ulong));
    else this->my_default_ulong_success_ = true;

  }

  ::CORBA::LongLong
  Foo_exec_i::my_default_longlong (void)
  {
    return 0;
  }

  void
  Foo_exec_i::my_default_longlong (
    ::CORBA::LongLong my_default_longlong)
  {
    if (my_default_longlong != 0)
      ACE_ERROR ((LM_ERROR, "ERROR: my_default_longlong != 0, it is %d\n", my_default_longlong));
    else this->my_default_longlong_success_ = true;

  }

  ::CORBA::ULongLong
  Foo_exec_i::my_default_ulonglong (void)
  {
    return 0;
  }

  void
  Foo_exec_i::my_default_ulonglong (
    ::CORBA::ULongLong my_default_ulonglong)
  {
    if (my_default_ulonglong != 0)
      ACE_ERROR ((LM_ERROR, "ERROR: my_default_ulonglong != 0, it is %d\n", my_default_ulonglong));
    else this->my_default_ulonglong_success_ = true;

  }

  ::CORBA::Float
  Foo_exec_i::my_default_float (void)
  {
    return 0.0f;
  }

  void
  Foo_exec_i::my_default_float (
    ::CORBA::Float my_default_float)
  {
    if (ACE::is_inequal (my_default_float, 0.0f))
      ACE_ERROR ((LM_ERROR, "ERROR: my_default_float != 0.0, it is %f\n", my_default_float));
    else this->my_default_float_success_ = true;

  }

  ::CORBA::Double
  Foo_exec_i::my_default_double (void)
  {
    return 0.0;
  }

  void
  Foo_exec_i::my_default_double (
    ::CORBA::Double my_default_double)
  {
    if (ACE::is_inequal (my_default_double, 0.0))
      ACE_ERROR ((LM_ERROR, "ERROR: my_default_double != 0.0, it is %f\n", my_default_double));
    else this->my_default_double_success_ = true;
  }

  ::CORBA::LongDouble
  Foo_exec_i::my_default_longdouble (void)
  {
    CORBA::LongDouble ld;
    ACE_CDR_LONG_DOUBLE_ASSIGNMENT (ld, 0.0);
    return ld;
  }

  void
  Foo_exec_i::my_default_longdouble (
    ::CORBA::LongDouble /*my_default_longdouble*/)
  {
    // Not supported since a longdouble is defined in the xsd as double.
    // We are then creating a long double initialized with a regular
    // double. This is a very tricky conversion and doesn't work in
    // combination with certain (versions of) compilers.
  }

  ::short_sequence *
  Foo_exec_i::my_short_sequence (void)
  {
    return 0;
  }

  void
  Foo_exec_i::my_short_sequence (
    const ::short_sequence & my_short_sequence)
  {
    if (my_short_sequence.length() != 3)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: my_short_sequence does not have the correct length\n"));
        return;
      }

    this->my_short_sequence_success_ = true;

    if (my_short_sequence[0] != 11)
      {
        this->my_short_sequence_success_ = false;
        ACE_ERROR ((LM_ERROR, "ERROR: my_short_sequence[0] != 11, it is %d\n",
                    my_short_sequence[0]));
      }

    if (my_short_sequence[1] != 12)
      {
        this->my_short_sequence_success_ = false;
        ACE_ERROR ((LM_ERROR, "ERROR: my_short_sequence[1] != 12, it is %d\n",
                    my_short_sequence[1]));
      }

    if (my_short_sequence[2] != 13)
      {
        this->my_short_sequence_success_ = false;
        ACE_ERROR ((LM_ERROR, "ERROR: my_short_sequence[2] != 13, it is %d\n",
                    my_short_sequence[2]));
      }
  }

  ::short_sequence *
  Foo_exec_i::my_empty_sequence (void)
  {
    return 0;
  }

  void
  Foo_exec_i::my_empty_sequence (
    const ::short_sequence & my_empty_sequence)
  {
    if (my_empty_sequence.length() != 0)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: my_empty_sequence does not have the correct length\n"));
        return;
      }

    this->my_empty_sequence_success_ = true;
  }

  ::long_sequence *
  Foo_exec_i::my_long_sequence (void)
  {
    return 0;
  }

  void
  Foo_exec_i::my_long_sequence (
    const ::long_sequence & my_long_sequence)
  {
    if (my_long_sequence.length() != 3)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: my_long_sequence does not have the correct length\n"));
        return;
      }

    this->my_long_sequence_success_ = true;

    if (my_long_sequence[0] != 21)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: my_long_sequence[0] != 21, it is %d\n",
                    my_long_sequence[0]));
        this->my_long_sequence_success_ = false;
      }

    if (my_long_sequence[1] != 22)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: my_long_sequence[1] != 22, it is %d\n",
                    my_long_sequence[1]));
        this->my_long_sequence_success_ = false;
      }

    if (my_long_sequence[2] != 23)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: my_long_sequence[2] != 23, it is %d\n",
                    my_long_sequence[2]));
        this->my_long_sequence_success_ = false;
      }
  }

  ::float_sequence *
  Foo_exec_i::my_float_sequence (void)
  {
    return 0;
  }

  void
  Foo_exec_i::my_float_sequence (
    const ::float_sequence & my_float_sequence)
  {
    if (my_float_sequence.length() != 3)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: my_float_sequence does not have the correct length\n"));
        return;
      }

    this->my_float_sequence_success_ = true;

    if (ACE::is_inequal (my_float_sequence[0], 21.12F))
      {
        ACE_ERROR ((LM_ERROR, "ERROR: my_float_sequence[0] != 21.12, it is %f\n",
                    my_float_sequence[0]));
        this->my_float_sequence_success_ = false;
      }

    if (ACE::is_inequal (my_float_sequence[1], 22.22F))
      {
        ACE_ERROR ((LM_ERROR, "ERROR: my_float_sequence[1] != 22.22, it is %f\n",
                    my_float_sequence[1]));
        this->my_float_sequence_success_ = false;
      }

    if (ACE::is_inequal (my_float_sequence[2], 23.32F))
      {
        ACE_ERROR ((LM_ERROR, "ERROR: my_float_sequence[2] != 23.32, it is %f\n",
                    my_float_sequence[2]));
        this->my_float_sequence_success_ = false;
      }
  }

  ::double_sequence *
  Foo_exec_i::my_double_sequence (void)
  {
    return 0;
  }

  void
  Foo_exec_i::my_double_sequence (
    const ::double_sequence & my_double_sequence)
  {
    if (my_double_sequence.length() != 3)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: my_double_sequence does not have the correct length\n"));
        return;
      }

    this->my_double_sequence_success_ = true;

    if (ACE::is_inequal (my_double_sequence[0], 621.12))
      {
        ACE_ERROR ((LM_ERROR, "ERROR: my_double_sequence[0] != 621.12, it is %f\n",
                    my_double_sequence[0]));
        this->my_double_sequence_success_ = false;
      }


    if (ACE::is_inequal (my_double_sequence[1], 622.22))
      {
        ACE_ERROR ((LM_ERROR, "ERROR: my_double_sequence[1] != 622.22, it is %f\n",
                    my_double_sequence[1]));
        this->my_double_sequence_success_ = false;
      }

    if (ACE::is_inequal (my_double_sequence[2], 623.32))
      {
        ACE_ERROR ((LM_ERROR, "ERROR: my_double_sequence[2] != 623.32, it is %f\n",
                    my_double_sequence[2]));
        this->my_double_sequence_success_ = false;
      }
  }

  ::Bar
  Foo_exec_i::my_bar_struct (void)
  {
    return ::Bar ();
  }

  void
  Foo_exec_i::my_bar_struct (
    const ::Bar & my_bar_struct)
  {
    this->my_bar_struct_success_ = true;

    if (my_bar_struct.s != 3)
      {
        this->my_bar_struct_success_ = false;
        ACE_ERROR ((LM_ERROR, "ERROR: short value != 3, it is %d\n", my_bar_struct.s));
      }

    if (my_bar_struct.l != 4)
      {
        this->my_bar_struct_success_ = false;
        ACE_ERROR ((LM_ERROR, "ERROR: long value != 4, it is %d\n", my_bar_struct.l));
      }

    if (ACE::is_inequal (my_bar_struct.f, 5.6F))
      {
        this->my_bar_struct_success_ = false;
        ACE_ERROR ((LM_ERROR, "ERROR: float value != 5.6, it is %f\n", my_bar_struct.f));
      }

    if (ACE::is_inequal (my_bar_struct.d, 7.8))
      {
        this->my_bar_struct_success_ = false;
        ACE_ERROR ((LM_ERROR, "ERROR: double value != 7.8, it is %f\n", my_bar_struct.d));
      }
  }

  ::Bar
  Foo_exec_i::my_default_bar_struct (void)
  {
    return ::Bar ();
  }

  void
  Foo_exec_i::my_default_bar_struct (
    const ::Bar & my_bar_struct)
  {
    this->my_default_bar_struct_success_ = true;

    if (my_bar_struct.s != 0)
      {
        this->my_default_bar_struct_success_ = false;
        ACE_ERROR ((LM_ERROR, "ERROR: short value != 0, it is %d\n", my_bar_struct.s));
      }

    if (my_bar_struct.l != 0)
      {
        this->my_default_bar_struct_success_ = false;
        ACE_ERROR ((LM_ERROR, "ERROR: long value != 0, it is %d\n", my_bar_struct.l));
      }

    if (ACE::is_inequal (my_bar_struct.f, 0.0F))
      {
        this->my_default_bar_struct_success_ = false;
        ACE_ERROR ((LM_ERROR, "ERROR: float value != 0.0, it is %f\n", my_bar_struct.f));
      }

    if (ACE::is_inequal (my_bar_struct.d, 0.0))
      {
        this->my_default_bar_struct_success_ = false;
        ACE_ERROR ((LM_ERROR, "ERROR: double value != 0.0, it is %f\n", my_bar_struct.d));
      }
  }

  ::BarArray_slice*
  Foo_exec_i::my_bar_array (void)
  {
    return 0;
  }

  void
  Foo_exec_i::my_bar_array (
    const ::BarArray my_bar_array)
  {
    this->my_bar_array_success_ = true;
    for (int i = 0; i < 3; ++i)
      {
        if (my_bar_array[i].s != i+1)
          {
            this->my_bar_array_success_ = false;
            ACE_ERROR ((LM_ERROR, "ERROR for index <%d>: short value != %d, it is %d\n",
                        i, i+1, my_bar_array[i].s));
          }

        if (my_bar_array[i].l != i+2)
          {
            this->my_bar_array_success_ = false;
            ACE_ERROR ((LM_ERROR, "ERROR for index <%d>: short value != %d, it is %d\n",
                        i, i+2, my_bar_array[i].l));
          }
      }

    if (ACE::is_inequal (my_bar_array[0].f, 3.4F))
      {
        this->my_bar_array_success_ = false;
        ACE_ERROR ((LM_ERROR, "ERROR for index <0>: float value != 3.4, it is %f\n",
                    my_bar_array[0].f));
      }

    if (ACE::is_inequal (my_bar_array[0].d, 5.6))
      {
        this->my_bar_array_success_ = false;
        ACE_ERROR ((LM_ERROR, "ERROR for index <0>: double value != 5.6, it is %f\n",
                    my_bar_array[0].d));
      }

    if (ACE::is_inequal (my_bar_array[1].f, 4.5F))
      {
        this->my_bar_array_success_ = false;
        ACE_ERROR ((LM_ERROR, "ERROR for index <1>: float value != 4.5, it is %f\n",
                    my_bar_array[1].f));
      }

    if (ACE::is_inequal (my_bar_array[1].d, 6.7))
      {
        this->my_bar_array_success_ = false;
        ACE_ERROR ((LM_ERROR, "ERROR for index <1>: double value != 6.7, it is %f\n",
                    my_bar_array[1].d));
      }

    if (ACE::is_inequal (my_bar_array[2].f, 5.6F))
      {
        this->my_bar_array_success_ = false;
        ACE_ERROR ((LM_ERROR, "ERROR for index <2>: float value != 5.6, it is %f\n",
                    my_bar_array[2].f));
      }

    if (ACE::is_inequal (my_bar_array[2].d, 7.8))
      {
        this->my_bar_array_success_ = false;
        ACE_ERROR ((LM_ERROR, "ERROR for index <2>: double value != 7.8, it is %f\n",
                    my_bar_array[2].d));
      }
  }

  ::BarArray2_slice *
  Foo_exec_i::my_bar_array_2 (void)
  {
    return 0;
  }

  void
  Foo_exec_i::my_bar_array_2 (
    const ::BarArray2 my_bar_array_2)
  {
    this->my_bar_array_2_success_ = true;
    for (int h = 0; h < 2; ++h)
      {
        for (int i = 0; i < 3; ++i)
          {
            if (my_bar_array_2[h][i].s != (h*10)+(i+1))
              {
                this->my_bar_array_2_success_ = false;
                ACE_ERROR ((LM_ERROR, "ERROR for index <%d>: short value != %d, it is %d\n",
                            i, (h*10)+(i+1), my_bar_array_2[h][i].s));
              }

            if (my_bar_array_2[h][i].l != (h*10)+(i+2))
              {
                this->my_bar_array_2_success_ = false;
                ACE_ERROR ((LM_ERROR, "ERROR for index <%d>: short value != %d, it is %d\n",
                            i, (h*10)+(i+2), my_bar_array_2[h][i].l));
              }
          }
        if (ACE::is_inequal (my_bar_array_2[h][0].f, (h*10)+3.4F))
          {
            this->my_bar_array_success_ = false;
            ACE_ERROR ((LM_ERROR, "ERROR for index <0>: float value != %f, it is %f\n",
                        (h*10)+3.4F, my_bar_array_2[h][0].f));
          }

        if (ACE::is_inequal (my_bar_array_2[h][0].d, (h*10)+5.6))
          {
            this->my_bar_array_success_ = false;
            ACE_ERROR ((LM_ERROR, "ERROR for index <0>: double value != %f, it is %f\n",
                        (h*10)+5.6, my_bar_array_2[h][0].d));
          }

        if (ACE::is_inequal (my_bar_array_2[h][1].f, (h*10)+4.5F))
          {
            this->my_bar_array_success_ = false;
            ACE_ERROR ((LM_ERROR, "ERROR for index <1>: float value != %f, it is %f\n",
                        (h*10)+4.5F, my_bar_array_2[h][1].f));
          }

        if (ACE::is_inequal (my_bar_array_2[h][1].d, (h*10)+6.7))
          {
            this->my_bar_array_success_ = false;
            ACE_ERROR ((LM_ERROR, "ERROR for index <1>: double value != %f, it is %f\n",
                        (h*10)+6.7, my_bar_array_2[h][1].d));
          }

        if (ACE::is_inequal (my_bar_array_2[h][2].f, (h*10)+5.6F))
          {
            this->my_bar_array_success_ = false;
            ACE_ERROR ((LM_ERROR, "ERROR for index <2>: float value != %f, it is %f\n",
                       (h*10)+5.6F, my_bar_array_2[h][2].f));
          }

        if (ACE::is_inequal (my_bar_array_2[h][2].d, (h*10)+7.8))
          {
            this->my_bar_array_success_ = false;
            ACE_ERROR ((LM_ERROR, "ERROR for index <2>: double value != %f, it is %f\n",
                        (h*10)+7.8, my_bar_array_2[h][2].d));
          }
      }
  }

  ::SequenceArray_slice *
  Foo_exec_i::my_sequence_array (void)
  {
    return 0;
  }

  void
  Foo_exec_i::my_sequence_array (
    const ::SequenceArray my_sequence_array)
  {
    if (my_sequence_array[0].length() != 2)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: The first sequence does not have the correct length\n"));
        return;
      }
    if (my_sequence_array[1].length() != 2)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: The second sequence does not have the correct length\n"));
        return;
      }
    this->my_sequence_array_success_ = true;

    if (ACE_OS::strcmp (my_sequence_array[0][0], "Hi") != 0)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: my_sequence_array[0][0] != Hi, it is %C\n",
                    my_sequence_array[0][0].in ()));
        this->my_sequence_array_success_ = false;
      }
    if (ACE_OS::strcmp (my_sequence_array[0][1], "World") != 0)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: my_sequence_array[0][1] != World, it is %C\n",
                    my_sequence_array[0][1].in ()));
        this->my_sequence_array_success_ = false;
      }

    if (ACE_OS::strcmp (my_sequence_array[1][0], "Bye") != 0)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: my_sequence_array[1][0] != Bye, it is %C\n",
                    my_sequence_array[1][0].in ()));
        this->my_sequence_array_success_ = false;
      }
    if (ACE_OS::strcmp (my_sequence_array[1][1], "World") != 0)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: my_sequence_array[1][1] != World, it is %C\n",
                    my_sequence_array[1][1].in ()));
        this->my_sequence_array_success_ = false;
      }
  }

  ::BarStruct
  Foo_exec_i::my_bar_struct_array (void)
  {
    return BarStruct ();
  }

  void
  Foo_exec_i::my_bar_struct_array (
    const ::BarStruct & my_bar_struct_array)
  {
    this->my_bar_struct_array_success_ = true;
    for (int i = 0; i < 3; ++i)
      {
        if (my_bar_struct_array.bar[i].s != i+1)
          {
            this->my_bar_struct_array_success_ = false;
            ACE_ERROR ((LM_ERROR, "ERROR for index <%d>: short value != %d, it is %d\n",
                        i, i+1, my_bar_struct_array.bar[i].s));
          }

        if (my_bar_struct_array.bar[i].l != i+2)
          {
            this->my_bar_struct_array_success_ = false;
            ACE_ERROR ((LM_ERROR, "ERROR for index <%d>: short value != %d, it is %d\n",
                        i, i+2, my_bar_struct_array.bar[i].l));
          }
      }

    if (ACE::is_inequal (my_bar_struct_array.bar[0].f, 3.4F))
      {
        this->my_bar_struct_array_success_ = false;
        ACE_ERROR ((LM_ERROR, "ERROR for index <0>: float value != 3.4, it is %f\n",
                    my_bar_struct_array.bar[0].f));
      }
    if (ACE::is_inequal (my_bar_struct_array.bar[0].d, 5.6))
      {
        this->my_bar_struct_array_success_ = false;
        ACE_ERROR ((LM_ERROR, "ERROR for index <0>: double value != 5.6, it is %f\n",
                    my_bar_struct_array.bar[0].d));
      }

    if (ACE::is_inequal (my_bar_struct_array.bar[1].f, 4.5F))
      {
        this->my_bar_struct_array_success_ = false;
        ACE_ERROR ((LM_ERROR, "ERROR for index <1>: float value != 4.5, it is %f\n",
                    my_bar_struct_array.bar[1].f));
      }
    if (ACE::is_inequal (my_bar_struct_array.bar[1].d, 6.7))
      {
        this->my_bar_struct_array_success_ = false;
        ACE_ERROR ((LM_ERROR, "ERROR for index <1>: double value != 6.7, it is %f\n",
                    my_bar_struct_array.bar[1].d));
      }

    if (ACE::is_inequal (my_bar_struct_array.bar[2].f, 5.6F))
      {
        this->my_bar_struct_array_success_ = false;
        ACE_ERROR ((LM_ERROR, "ERROR for index <2>: float value != 5.6, it is %f\n",
                    my_bar_struct_array.bar[2].f));
      }
    if (ACE::is_inequal (my_bar_struct_array.bar[2].d, 7.8))
      {
        this->my_bar_struct_array_success_ = false;
        ACE_ERROR ((LM_ERROR, "ERROR for index <2>: double value != 7.8, it is %f\n",
                    my_bar_struct_array.bar[2].d));
      }
  }

  ::Baz *
  Foo_exec_i::my_baz_struct (void)
  {
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

    this->my_baz_struct_success_ = true;

    if (ACE_OS::strcmp (my_baz_struct.name.in (),
                        "My Baz Struct") != 0)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: my_baz struct has incorrect name %C\n",
                    my_baz_struct.name.in ()));
        this->my_baz_struct_success_ = false;
      }

    CORBA::Short inc_s = 0;
    CORBA::Long inc_l = 0;
    CORBA::Float inc_f = 0;
    CORBA::Double inc_d = 0;

    for (CORBA::ULong i = 0;
         i < my_baz_struct.my_bar_sequence.length ();
         ++i)
      {
        if (my_baz_struct.my_bar_sequence[i].s != 3 + inc_s)
          {
            this->my_baz_struct_success_ = false;
            ACE_ERROR ((LM_ERROR, "ERROR: short value != %d, it is %d\n",
                        3 + inc_s,
                        my_baz_struct.my_bar_sequence[i].s));
          }

        inc_s += 10;
        if (my_baz_struct.my_bar_sequence[i].l != 4 + inc_l)
          {
            this->my_baz_struct_success_ = false;
            ACE_ERROR ((LM_ERROR, "ERROR: long value != %d, it is %d\n",
                        4 + inc_l,
                        my_baz_struct.my_bar_sequence[i].l));
          }

        inc_l += 10;
        if (ACE::is_inequal (my_baz_struct.my_bar_sequence[i].f, 5.6F + inc_f))
          {
            this->my_baz_struct_success_ = false;
            ACE_ERROR ((LM_ERROR, "ERROR: float value != %f, it is %f\n",
                        5.6F + inc_f,
                        my_baz_struct.my_bar_sequence[i].f));
          }

        inc_f += 10.0F;

        if (ACE::is_inequal (my_baz_struct.my_bar_sequence[i].d, 7.8 + inc_d))
          {
            this->my_baz_struct_success_ = false;
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
  Foo_exec_i::my_data_union (
    const ::Data & /*my_data_union*/)
  {
  }

  ::Data2* Foo_exec_i::my_var_data_union (void)
  {
    return 0;
  }

  void Foo_exec_i::my_var_data_union (
    const ::Data2 & /*my_data_union*/)
  {
  }

  ::string_sequence * Foo_exec_i::my_string_sequence (void)
  {
    return 0;
  }

  void
  Foo_exec_i::my_string_sequence (
    const ::string_sequence & my_string_sequence)
  {
    if (my_string_sequence.length() != 2)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: my_string_sequence does not have the correct length\n"));
        return;
      }

    this->my_string_sequence_success_ = true;

    if (ACE_OS::strcmp (my_string_sequence[0], "Hi") != 0)
      {
        this->my_string_sequence_success_ = false;
        ACE_ERROR ((LM_ERROR, "ERROR: my_string_sequence[0] != Hi, it is %C\n",
                    my_string_sequence[0].in ()));
      }

    if (ACE_OS::strcmp (my_string_sequence[1], "World") != 0)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: my_string_sequence[1] != World, it is %C\n",
                    my_string_sequence[1].in ()));
        this->my_string_sequence_success_ = false;
      }
  }

  ::string_sequence2 *
  Foo_exec_i::my_string_sequence_2 (void)
  {
    return 0;
  }


  void
  Foo_exec_i::my_string_sequence_2 (
    const ::string_sequence2 & my_string_sequence_2)
  {
    if (my_string_sequence_2.length() != 2)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: The main string sequence does not have the correct length\n"));
        return;
      }
    if (my_string_sequence_2[0].length() != 2)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: String sequence I does not have the correct length\n"));
        return;
      }
    if (my_string_sequence_2[1].length() != 2)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: String sequence II does not have the correct length\n"));
        return;
      }

    this->my_string_sequence_2_success_ = true;

    if (ACE_OS::strcmp (my_string_sequence_2[0][0], "Hi") != 0)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: my_string_sequence_2[0][0] != Hi, it is %C\n",
                    my_string_sequence_2[0][0].in ()));
        this->my_string_sequence_2_success_ = false;
      }
    if (ACE_OS::strcmp (my_string_sequence_2[0][1], "World") != 0)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: my_string_sequence_2[0][1] != World, it is %C\n",
                    my_string_sequence_2[0][1].in ()));
        this->my_string_sequence_2_success_ = false;
      }

    if (ACE_OS::strcmp (my_string_sequence_2[1][0], "Bye") != 0)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: my_string_sequence_2[1][0] != Bye, it is %C\n",
                    my_string_sequence_2[1][0].in ()));
        this->my_string_sequence_2_success_ = false;
      }
    if (ACE_OS::strcmp (my_string_sequence_2[1][1], "World") != 0)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: my_string_sequence_2[1][1] != World, it is %C\n",
                    my_string_sequence_2[1][1].in ()));
        this->my_string_sequence_2_success_ = false;
      }

  }

  ::array_sequence *
  Foo_exec_i::my_array_sequence (void)
  {
    return 0;
  }


  void
  Foo_exec_i::my_array_sequence (
    const ::array_sequence & my_array_sequence)
  {
    if (my_array_sequence.length() != 2)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: The main array sequence does not have the correct length\n"));
        return;
      }

    this->my_array_sequence_success_ = true;

    if (ACE_OS::strcmp (my_array_sequence[0][0], "Hi") != 0)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: my_array_sequence[0][0] != Hi, it is %C\n",
                    my_array_sequence[0][0].in ()));
        this->my_array_sequence_success_ = false;
      }
    if (ACE_OS::strcmp (my_array_sequence[0][1], "World") != 0)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: my_array_sequence[0][1] != World, it is %C\n",
                    my_array_sequence[0][1].in ()));
        this->my_array_sequence_success_ = false;
      }

    if (ACE_OS::strcmp (my_array_sequence[1][0], "Bye") != 0)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: my_array_sequence[1][0] != Bye, it is %C\n",
                    my_array_sequence[1][0].in ()));
        this->my_array_sequence_success_ = false;
      }
    if (ACE_OS::strcmp (my_array_sequence[1][1], "World") != 0)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: my_array_sequence[1][1] != World, it is %C\n",
                    my_array_sequence[1][1].in ()));
        this->my_array_sequence_success_ = false;
      }

  }

  char * Foo_exec_i::my_bounded_string (void)
  {
    return 0;
  }

  void
  Foo_exec_i::my_bounded_string (
    const char * my_bounded_string)
  {
    if (ACE_OS::strcmp (my_bounded_string, "Hi") != 0)
      ACE_ERROR ((LM_ERROR, "ERROR: my_bounded_string != Hi, it is %C\n",
                  my_bounded_string));
    else this->my_bounded_string_success_ = true;
  }

  char *Foo_exec_i::my_variable_string (void)
  {
    return 0;
  }

  void
  Foo_exec_i::my_variable_string (
    const char * my_variable_string)
  {
    if (ACE_OS::strcmp (my_variable_string, "Hi") != 0)
      ACE_ERROR ((LM_ERROR, "ERROR: my_variable_string != Hi, it is %C\n",
                  my_variable_string));
    else this->my_variable_string_success_ = true;
  }

  char *Foo_exec_i::my_default_variable_string (void)
  {
    return 0;
  }

  void
  Foo_exec_i::my_default_variable_string (
    const char * my_default_variable_string)
  {
    if (ACE_OS::strcmp (my_default_variable_string, "") != 0)
      ACE_ERROR ((LM_ERROR, "ERROR: my_default_variable_string != empty, it is %C\n",
                  my_default_variable_string));
    else this->my_default_variable_string_success_ = true;
  }

  ::CORBA::WChar *Foo_exec_i::my_bounded_wstring (void)
  {
    return 0;
  }

  void
  Foo_exec_i::my_bounded_wstring (
    const ::CORBA::WChar * /*my_bounded_wstring*/)
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

  void
  Foo_exec_i::my_long_array (
    const ::long_array my_long_array)
  {
    this->my_long_array_success_ =
      check_array<const long_array, CORBA::Long>::check (my_long_array, "my_long_array", 1, 5);
  }

  ::ulong_array_slice *Foo_exec_i::my_ulong_array (void)
  {
    return 0;
  }

  void
  Foo_exec_i::my_ulong_array (
    const ::ulong_array my_ulong_array)
  {
    this->my_ulong_array_success_ =
      check_array<const ulong_array, CORBA::ULong>::check (my_ulong_array, "my_ulong_array", 2, 5);
  }

  ::longlong_array_slice * Foo_exec_i::my_longlong_array (void)
  {
    return 0;
  }

  void
  Foo_exec_i::my_longlong_array (
    const ::longlong_array my_longlong_array)
  {
    this->my_longlong_array_success_ =
      check_array<const longlong_array, CORBA::LongLong>::check (my_longlong_array, "my_longlong_array", 3, 5);
  }


  ::ulonglong_array_slice * Foo_exec_i::my_ulonglong_array (void)
  {
    return 0;
  }

  void
  Foo_exec_i::my_ulonglong_array (
    const ::ulonglong_array my_ulonglong_array)
  {
    this->my_ulonglong_array_success_ =
      check_array<const ulonglong_array, CORBA::ULongLong>::check (my_ulonglong_array, "my_ulonglong_array", 4, 5);
  }

  ::short_array_slice * Foo_exec_i::my_short_array (void)
  {
    return 0;
  }

  void
  Foo_exec_i::my_short_array (
    const ::short_array my_short_array)
  {
    this->my_short_array_success_ =
      check_array<const short_array, CORBA::Short>::check (my_short_array, "my_short_array", 5, 5);
  }

  ::ushort_array_slice * Foo_exec_i::my_ushort_array (void)
  {
    return 0;
  }

  void
  Foo_exec_i::my_ushort_array (
    const ::ushort_array my_ushort_array)
  {
    this->my_ushort_array_success_ =
      check_array<const ushort_array, CORBA::Short>::check (my_ushort_array, "my_ushort_array", 6, 5);
  }

  ::string_array_slice * Foo_exec_i::my_string_array (void)
  {
    return 0;
  }

  void
  Foo_exec_i::my_string_array (
    const ::string_array my_string_array)
  {
    this->my_string_array_success_ = true;

    if (ACE_OS::strcmp (my_string_array[0].in (), "Hello") != 0)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: my_string_array[0] != 'Hello', it is %C\n",
                    my_string_array[0].in ()));
        this->my_string_array_success_ = false;
      }

    if (ACE_OS::strcmp (my_string_array[1], "World") != 0)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: my_string_array[1] != 'World', it is %C\n",
                    my_string_array[1].in ()));
        this->my_string_array_success_ = false;
      }
  }

  ::wstring_array_slice * Foo_exec_i::my_wstring_array (void)
  {
    return 0;
  }

  void
  Foo_exec_i::my_wstring_array (
    const ::wstring_array /*my_wstring_array*/)
  {
  }

  ::char_array_slice * Foo_exec_i::my_char_array (void)
  {
    return 0;
  }

  void
  Foo_exec_i::my_char_array (
    const ::char_array /*my_char_array*/)
  {
    // Not supported since it's not configured in the xsd.
  }

  ::wchar_array_slice * Foo_exec_i::my_wchar_array (void)
  {
    return 0;
  }

  void
  Foo_exec_i::my_wchar_array (
    const ::wchar_array /*my_wchar_array*/)
  {
  }

  ::double_array_slice * Foo_exec_i::my_double_array (void)
  {
    return 0;
  }

  void
  Foo_exec_i::my_double_array (
    const ::double_array my_double_array)
  {
    this->my_double_array_success_ = true;
    if (ACE::is_inequal (my_double_array[0], 0.123))
      {
        ACE_ERROR ((LM_ERROR, "ERROR: my_double_array[0] != 0.123, it is %f\n",
                    my_double_array[0]));
        this->my_double_array_success_ = false;
      }
    if (ACE::is_inequal (my_double_array[1], 4.56))
      {
        ACE_ERROR ((LM_ERROR, "ERROR: my_double_array[1] != 4.56, it is %f\n",
                    my_double_array[1]));
        this->my_double_array_success_ = false;
      }
    if (ACE::is_inequal (my_double_array[2], 78.9))
      {
        ACE_ERROR ((LM_ERROR, "ERROR: my_double_array[2] != 78.9, it is %f\n",
                    my_double_array[2]));
        this->my_double_array_success_ = false;
      }
  }

  ::longdouble_array_slice * Foo_exec_i::my_longdouble_array (void)
  {
    return 0;
  }

  void
  Foo_exec_i::my_longdouble_array (
    const ::longdouble_array /*my_longdouble_array*/)
  {
    // Not supported since a longdouble is defined in the xsd as double.
    // We are then creating a long double initialized with a regular
    // double. This is a very tricky conversion and doesn't work in
    // combination with certain (versions of) compilers.
  }

  ::float_array_slice * Foo_exec_i::my_float_array (void)
  {
    return 0;
  }

  void
  Foo_exec_i::my_float_array (
    const ::float_array my_float_array)
  {
    this->my_float_array_success_ = true;
    if (ACE::is_inequal (my_float_array[0], 0.369F))
      {
        ACE_ERROR ((LM_ERROR, "ERROR: my_float_array[0] != 0.369, it is %f\n",
                    my_float_array[0]));
        this->my_float_array_success_ = false;
      }
    if (ACE::is_inequal (my_float_array[1], 13.68F))
      {
        ACE_ERROR ((LM_ERROR, "ERROR: my_float_array[1] != 13.68, it is %f\n",
                    my_float_array[1]));
        this->my_float_array_success_ = false;
      }
    if (ACE::is_inequal (my_float_array[2], 236.7F))
      {
        ACE_ERROR ((LM_ERROR, "ERROR: my_float_array[2] != 236.7, it is %f\n",
                    my_float_array[2]));
        this->my_float_array_success_ = false;
      }
  }

  ::boolean_array_slice * Foo_exec_i::my_boolean_array (void)
  {
    return 0;
  }

  void
  Foo_exec_i::my_boolean_array (
    const ::boolean_array my_boolean_array)
  {
    this->my_boolean_array_success_ = true;
    if (!my_boolean_array[0])
      {
        ACE_ERROR ((LM_ERROR, "ERROR: my_boolean_array[0] != true, it is false\n"));
        this->my_boolean_array_success_ = false;
      }
    if (my_boolean_array[1])
      {
        ACE_ERROR ((LM_ERROR, "ERROR: my_boolean_array[1] != false, it is true\n"));
        this->my_boolean_array_success_ = false;
      }
  }

  ::octet_array_slice * Foo_exec_i::my_octet_array (void)
  {
    return 0;
  }

  void
  Foo_exec_i::my_octet_array (
    const ::octet_array my_octet_array)
  {
    this->my_octet_array_success_ = true;
    if (my_octet_array[0] != 1)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: my_octet_array[0] != 1, it is %d\n",
                    my_octet_array[0]));
        this->my_octet_array_success_ = false;
      }
    if (my_octet_array[1] != 2)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: my_octet_array[1] != 2, it is %d\n",
                    my_octet_array[1]));
        this->my_octet_array_success_ = false;
      }
  }

  StructModule::StructStruct Foo_exec_i::my_struct_struct (void)
  {
    return StructModule::StructStruct ();
  }

  void
  Foo_exec_i::my_struct_struct (
    const StructModule::StructStruct & my_struct_struct)
  {
    bool error_found = false;

    if (my_struct_struct.type != StructModule::DLT_HARD)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: my_struct_struct.deadline_type != 2, it is %d\n",
                    my_struct_struct.type));
        error_found = true;
      }
    if (my_struct_struct.struct_time.sec != 15)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: my_struct_struct.struct_time.sec != 15, it is %d\n",
                    my_struct_struct.struct_time.sec));
        error_found = true;
      }
    if (my_struct_struct.struct_time.nanosec != 20)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: my_struct_struct.struct_time.nanosec != 20, it is %d\n",
                    my_struct_struct.struct_time.nanosec));
        error_found = true;
      }

    this->my_struct_struct_success_ = !error_found;
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
    check_error (this->my_octet_success_, "octet");
    check_error (this->my_short_success_, "short");
    check_error (this->my_boolean_success_, "boolean");
    check_error (this->my_long_success_, "long");
    check_error (this->my_ulong_success_, "ulong");
    check_error (this->my_longlong_success_, "longlong");
    check_error (this->my_ulonglong_success_, "ulonglong");
    check_error (this->my_float_success_, "float");
    check_error (this->my_double_success_, "double");
    check_error (this->my_default_octet_success_, "default octet");
    check_error (this->my_default_short_success_, "default short");
    check_error (this->my_default_boolean_success_, "default boolean");
    check_error (this->my_default_long_success_, "default long");
    check_error (this->my_default_ulong_success_, "default ulong");
    check_error (this->my_default_longlong_success_, "default longlong");
    check_error (this->my_default_ulonglong_success_, "default ulonglong");
    check_error (this->my_default_float_success_, "default float");
    check_error (this->my_default_double_success_, "default double");
    check_error (this->my_short_sequence_success_, "short sequence");
    check_error (this->my_empty_sequence_success_, "empty sequence");
    check_error (this->my_long_sequence_success_, "long sequence");

    check_error (this->my_long_array_success_, "long array");
    check_error (this->my_ulong_array_success_, "unsigned long array");
    check_error (this->my_longlong_array_success_, "long long array");
    check_error (this->my_ulonglong_array_success_, "unsigned long long array");
    check_error (this->my_short_array_success_, "short array");
    check_error (this->my_ushort_array_success_, "unsigned short array");
    check_error (this->my_string_array_success_, "string array");
    check_error (this->my_double_array_success_, "double array");
    check_error (this->my_float_array_success_, "float array");
    check_error (this->my_boolean_array_success_, "boolean array");
    check_error (this->my_octet_array_success_, "octet array");

    check_error (this->my_float_sequence_success_, "float sequence");
    check_error (this->my_double_sequence_success_, "double sequence");
    check_error (this->my_bar_struct_success_, "bar struct");
    check_error (this->my_default_bar_struct_success_, "default bar struct");
    check_error (this->my_bar_array_success_, "bar array");
    check_error (this->my_bar_array_2_success_, "array in array");
    check_error (this->my_sequence_array_success_, "sequence in array");
    check_error (this->my_bar_struct_array_success_, "bar array in struct");
    check_error (this->my_baz_struct_success_, "baz struct");
    check_error (this->my_string_sequence_success_, "string sequence");
    check_error (this->my_string_sequence_2_success_, "sequence in sequence");
    check_error (this->my_array_sequence_success_, "array in sequence");
    check_error (this->my_bounded_string_success_, "fixed string");
    check_error (this->my_variable_string_success_, "variable string");
    check_error (this->my_default_variable_string_success_, "default variable string");
    check_error (this->supported_short_success_, "supported short");
    check_error (this->my_struct_struct_success_, "nested struct");
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

