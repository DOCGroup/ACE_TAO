/**
 * @file
 *
 * @brief Unit test for bounded sequences of object references.
 *
 * $Id$
 *
 * @author Carlos O'Ryan
 */
#include "testing_object_reference_traits.hpp"
#include "tao/Object_Reference_Traits_T.h"
#include "testing_allocation_traits.hpp"
#include "testing_range_checking.hpp"

#include "mock_reference.hpp"

#include "ace/OS_NS_string.h"
#include "tao/Bounded_Object_Reference_Sequence_T.h"
#include "tao/Bounded_Value_Sequence_T.h"
#include "tao/Bounded_Basic_String_Sequence_T.h"
#include "tao/Bounded_BD_String_Sequence_T.h"
#include "tao/Bounded_Array_Sequence_T.h"
#include "tao/Bounded_Sequence_CDR_T.h"
#include "tao/CDR.h"

#include "test_macros.h"


using namespace TAO_VERSIONED_NAMESPACE_NAME::TAO;

CORBA::ULong const TMAX = 64;

typedef bounded_object_reference_sequence<mock_reference, mock_reference_var,TMAX> tested_sequence;

CORBA::Boolean operator<< (TAO_OutputCDR &strm, const tested_sequence &sequence)
{
  return TAO::marshal_sequence(strm, sequence);
}

CORBA::Boolean operator>> (TAO_InputCDR &strm, tested_sequence &sequence)
{
  return TAO::demarshal_sequence(strm, sequence);
}

struct Tester
{
  typedef tested_sequence::value_type value_type;
  typedef tested_sequence::const_value_type const_value_type;

  typedef tested_sequence::element_traits tested_element_traits;
  typedef tested_sequence::allocation_traits tested_allocation_traits;
  typedef TAO::details::range_checking<value_type,true> range;

  value_type * alloc_and_init_buffer()
  {
    value_type * buf = tested_sequence::allocbuf();
    buf[0] = mock_reference::allocate(1);
    buf[1] = mock_reference::allocate(4);
    buf[2] = mock_reference::allocate(9);
    buf[3] = mock_reference::allocate(16);

    return buf;
  }

  int check_values(tested_sequence const & a)
  {
    CHECK_EQUAL( 1, a[0]->id());
    CHECK_EQUAL( 4, a[1]->id());
    CHECK_EQUAL( 9, a[2]->id());
    CHECK_EQUAL(16, a[3]->id());
    return 0;
  }

  int test_stream()
  {
    value_type * buffer = alloc_and_init_buffer();

    expected_calls s(mock_reference::marshal_calls);
    {
      tested_sequence a;
      a.replace(4, buffer, false);

      CHECK_EQUAL(CORBA::ULong(64), a.maximum());
      CHECK_EQUAL(CORBA::ULong(4), a.length());
      CHECK_EQUAL(buffer, a.get_buffer());
      CHECK_EQUAL(false, a.release());
      check_values(a);

      TAO_OutputCDR stream;
      stream << a;
      FAIL_RETURN_IF_NOT(s.expect(4), s);
    }
    tested_sequence::freebuf(buffer);
    return 0;
  }
};

int ACE_TMAIN(int,ACE_TCHAR*[])
{
  int status = 0;

  try
    {
      Tester mytester;

      status += mytester.test_stream();
    }
  catch (const ::CORBA::Exception &ex)
    {
      ex._tao_print_exception("ERROR : unexpected CORBA exception caugth :");
      ++status;
    }

  return status;
}

