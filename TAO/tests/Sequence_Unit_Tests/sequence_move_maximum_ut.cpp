/**
 * @file
 *
 * @brief Test that moving sequences preserves their maximum value.
 */
#include "mock_array.hpp"
#include "mock_reference.hpp"

#include "tao/Bounded_Array_Sequence_T.h"
#include "tao/Bounded_Basic_String_Sequence_T.h"
#include "tao/Bounded_BD_String_Sequence_T.h"
#include "tao/Bounded_Object_Reference_Sequence_T.h"
#include "tao/Bounded_Value_Sequence_T.h"
#include "tao/Unbounded_Array_Sequence_T.h"
#include "tao/Unbounded_Basic_String_Sequence_T.h"
#include "tao/Unbounded_BD_String_Sequence_T.h"
#include "tao/Unbounded_Object_Reference_Sequence_T.h"
#include "tao/Unbounded_Value_Sequence_T.h"
#include "tao/Valuetype/Bounded_Valuetype_Sequence_T.h"
#include "tao/Valuetype/Unbounded_Valuetype_Sequence_T.h"
#include "tao/Valuetype/ValueBase.h"

#include "test_macros.h"

#include <utility>

CORBA::ULong constexpr TEST_MAXIMUM = 8;
using namespace TAO_VERSIONED_NAMESPACE_NAME::TAO;

template<typename Sequence>
int test_move_maximum (CORBA::ULong expected_maximum)
{
  Sequence source {};
  CHECK_EQUAL (expected_maximum, source.maximum ());
  CORBA::ULong const default_maximum = source.maximum ();

  source.length (1);
  CORBA::ULong const allocated_maximum = source.maximum ();
  CHECK_EQUAL (true, source.release ());

  Sequence moved (std::move (source));
  CHECK_EQUAL (allocated_maximum, moved.maximum ());
  CHECK_EQUAL (default_maximum, source.maximum ());

  Sequence assigned {};
  assigned.length (1);
  CHECK_EQUAL (true, assigned.release ());
  assigned = std::move (moved);
  CHECK_EQUAL (allocated_maximum, assigned.maximum ());
  CHECK_EQUAL (default_maximum, moved.maximum ());
  return 0;
}

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  using bounded_array = bounded_array_sequence<
    my_array, my_array_slice, my_array_tag, TEST_MAXIMUM>;
  using unbounded_array = unbounded_array_sequence<
    my_array, my_array_slice, my_array_tag>;
  using bounded_string = bounded_basic_string_sequence<char, TEST_MAXIMUM>;
  using unbounded_string = unbounded_basic_string_sequence<char>;
  using bounded_bd_string = bounded_bd_string_sequence<
    char, TEST_MAXIMUM, TEST_MAXIMUM>;
  using unbounded_bd_string = unbounded_bd_string_sequence<char, TEST_MAXIMUM>;
  using bounded_reference = bounded_object_reference_sequence<
    mock_reference, mock_reference_var, TEST_MAXIMUM>;
  using unbounded_reference = unbounded_object_reference_sequence<
    mock_reference, mock_reference_var>;
  using bounded_value = bounded_value_sequence<int, TEST_MAXIMUM>;
  using unbounded_value = unbounded_value_sequence<int>;
  using bounded_octet = bounded_value_sequence<CORBA::Octet, TEST_MAXIMUM>;
  using unbounded_octet = unbounded_value_sequence<CORBA::Octet>;
  using bounded_valuetype = bounded_valuetype_sequence<
    CORBA::ValueBase, CORBA::ValueBase_var, TEST_MAXIMUM>;
  using unbounded_valuetype = unbounded_valuetype_sequence<
    CORBA::ValueBase, CORBA::ValueBase_var>;

  int status {};
  status += test_move_maximum<bounded_array> (TEST_MAXIMUM);
  status += test_move_maximum<unbounded_array> (0);
  status += test_move_maximum<bounded_string> (TEST_MAXIMUM);
  status += test_move_maximum<unbounded_string> (0);
  status += test_move_maximum<bounded_bd_string> (TEST_MAXIMUM);
  status += test_move_maximum<unbounded_bd_string> (0);
  status += test_move_maximum<bounded_reference> (TEST_MAXIMUM);
  status += test_move_maximum<unbounded_reference> (0);
  status += test_move_maximum<bounded_value> (TEST_MAXIMUM);
  status += test_move_maximum<unbounded_value> (0);
  status += test_move_maximum<bounded_octet> (TEST_MAXIMUM);
  status += test_move_maximum<unbounded_octet> (0);
  status += test_move_maximum<bounded_valuetype> (TEST_MAXIMUM);
  status += test_move_maximum<unbounded_valuetype> (0);
#if defined(ACE_HAS_WCHAR) && !defined(TAO_LACKS_WCHAR_CXX_STDLIB)
  using bounded_wstring = bounded_basic_string_sequence<CORBA::WChar, TEST_MAXIMUM>;
  using unbounded_wstring = unbounded_basic_string_sequence<CORBA::WChar>;
  status += test_move_maximum<bounded_wstring> (TEST_MAXIMUM);
  status += test_move_maximum<unbounded_wstring> (0);
#endif
  return status;
}
