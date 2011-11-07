/**
 * @file
 *
 * @brief Unit test for unbounded sequences of forward declared object
 * references.
 *
 * $Id$
 *
 * @author Johnny Willemsen  <jwillemsen@remedy.nl>
 */
#include "testing_object_reference_traits.hpp"
#include "testing_allocation_traits.hpp"
#include "testing_range_checking.hpp"

#include "fwd_mock_reference.hpp"

#include "tao/Unbounded_Object_Reference_Sequence_T.h"

#include "test_macros.h"


using namespace TAO_VERSIONED_NAMESPACE_NAME::TAO;

struct Tester
{
  typedef unbounded_object_reference_sequence<fwd_mock_reference, fwd_mock_reference_var> tested_sequence;
  typedef tested_sequence::value_type value_type;
  typedef tested_sequence::const_value_type const_value_type;

  typedef tested_sequence::element_traits tested_element_traits;
  typedef tested_sequence::allocation_traits tested_allocation_traits;
  typedef TAO::details::range_checking<value_type,true> range;

  int test_default_constructor()
  {
    expected_calls a(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);
    expected_calls i(tested_element_traits::default_initializer_calls);
    {
      tested_sequence x;

      CHECK_EQUAL(CORBA::ULong(0), x.maximum());
      CHECK_EQUAL(CORBA::ULong(0), x.length());
      CHECK_EQUAL(false, x.release());
    }
    FAIL_RETURN_IF_NOT(a.expect(0), a);
    FAIL_RETURN_IF_NOT(f.expect(0), f);
    FAIL_RETURN_IF_NOT(i.expect(0), i);
    return 0;
  }
};

int ACE_TMAIN(int,ACE_TCHAR*[])
{
  int status = 0;
  Tester x;
  status += x.test_default_constructor ();

  return status;
}
