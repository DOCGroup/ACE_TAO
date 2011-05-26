#ifndef guard_value_sequence_tester_hpp
#define guard_value_sequence_tester_hpp
/**
 * @file
 *
 * @brief Helper class to implement tests for *_value_sequence
 *
 * $Id$
 *
 * @author Carlos O'Ryan
 */
#include "tao/Basic_Types.h"

#include "test_macros.h"
#include <tao/SystemException.h>

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template<class tested_sequence,
    class tested_allocation_traits>
struct value_sequence_tester
{
  typedef typename tested_sequence::value_type value_type;
  typedef typename tested_sequence::const_value_type const_value_type;

  int test_default_constructor()
  {
    expected_calls a(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);
    {
      tested_sequence x;

      CHECK_EQUAL(
          CORBA::ULong(tested_allocation_traits::default_maximum()),
          x.maximum());
      CHECK_EQUAL(CORBA::ULong(0), x.length());
    }
    FAIL_RETURN_IF_NOT(a.expect(0), a);
    // Nothing was allocated then there is nothing to free.
    FAIL_RETURN_IF_NOT(f.expect(0), f);
    return 0;
  }


  int test_copy_constructor_from_default()
  {
    expected_calls a(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);
    {
      tested_sequence x;
      FAIL_RETURN_IF_NOT(a.expect(0), a);
      CHECK_EQUAL(
          CORBA::ULong(tested_allocation_traits::default_maximum()),
          x.maximum());
      CHECK_EQUAL(CORBA::ULong(0), x.length());

      tested_sequence y(x);
      // Default constructed sequence doesn't have elements,
      // thus there is nothing to allocate/copy in copy constructor.
      FAIL_RETURN_IF_NOT(a.expect(0), a);
      CHECK_EQUAL(x.maximum(), y.maximum());
      CHECK_EQUAL(x.length(), y.length());
      CHECK_EQUAL(x.release(), y.release());
    }
    // Nothing was allocated then there is nothing to free.
    FAIL_RETURN_IF_NOT(f.expect(0), f);
    return 0;
  }

  int test_index_accessor()
  {
    try
      {
        tested_sequence x;
        x.length(8);
        // Set x[4] to any value just for suppressing valgrind complains.
        x[4] = 1;

        tested_sequence const & y = x;
        const_value_type & z = y[4];
        CHECK_EQUAL(z, y[4]);
      }
    catch (const ::CORBA::BAD_PARAM &)
      {
        ACE_ERROR ((LM_ERROR, "Error: test_index_accessor: BAD_PARAM exception caught\n"));
        return 1;
      }
    return 0;
  }

  int test_index_modifier()
  {
    tested_sequence x;
    x.length(8);

    tested_sequence const & y = x;
    const_value_type & z = y[4];
    x[4] = 4;
    CHECK_EQUAL(4, x[4]);
    CHECK_EQUAL(4, y[4]);
    CHECK_EQUAL(4, z);
    return 0;
  }

  int test_index_checking()
  {
    tested_sequence x;
    x.length(8);

    tested_sequence const & y = x;
    int z = 0;

    CHECK_THROW(z = y[32], std::range_error);
    CHECK_THROW(x[32] = z, std::range_error);
    return 0;
  }

  int test_copy_constructor_values()
  {
    tested_sequence a;
    a.length(16);
    for(CORBA::ULong i = 0; i != 16; ++i) a[i] = i*i;

    tested_sequence b(a);
    CHECK_EQUAL(a.length(), b.length());
    for(CORBA::ULong i = 0; i != a.length(); ++i)
    {
      FAIL_RETURN_IF_NOT(a[i] == b[i],
          "Mismatched elements at index " << i);
    }
    return 0;
  }

  int test_assignment_from_default()
  {
    expected_calls a(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);
    {
      tested_sequence x;
      FAIL_RETURN_IF_NOT(a.expect(0), a);
      CHECK_EQUAL(
          CORBA::ULong(tested_allocation_traits::default_maximum()),
          x.maximum());
      CHECK_EQUAL(CORBA::ULong(0), x.length());

      tested_sequence y;
      FAIL_RETURN_IF_NOT(a.expect(0), a);

      y = x;
      // Default constructed sequence doesn't have elements,
      // thus there is nothing to allocate/copy in operator=.
      FAIL_RETURN_IF_NOT(a.expect(0), a);
      FAIL_RETURN_IF_NOT(f.expect(0), f);
      CHECK_EQUAL(x.maximum(), y.maximum());
      CHECK_EQUAL(x.length(), y.length());
      CHECK_EQUAL(x.release(), y.release());
    }
    // Nothing was allocated then there is nothing to free.
    FAIL_RETURN_IF_NOT(f.expect(0), f);
    return 0;
  }

  int test_assignment_values()
  {
    tested_sequence a;
    a.length(16);
    for(CORBA::ULong i = 0; i != 16; ++i) a[i] = i*i;

    tested_sequence b;
    b = a;
    CHECK_EQUAL(a.maximum(), b.maximum());
    CHECK_EQUAL(a.length(),  b.length());
    CHECK_EQUAL(a.release(), b.release());
    for(CORBA::ULong i = 0; i != a.length(); ++i)
    {
      FAIL_RETURN_IF_NOT(a[i] == b[i],
          "Mismatched elements at index " << i);
    }
    return 0;
  }

  int test_exception_in_copy_constructor()
  {
    expected_calls f(tested_allocation_traits::freebuf_calls);
    {
      tested_sequence x; x.length(8);
      f.reset();

      expected_calls a(tested_allocation_traits::allocbuf_calls);
      tested_allocation_traits::allocbuf_calls.failure_countdown(1);
      CHECK_THROW(tested_sequence y(x), testing_exception);
      FAIL_RETURN_IF_NOT(a.expect(1), a);
    }
    FAIL_RETURN_IF_NOT(f.expect(1), f);
    return 0;
  }

  int test_exception_in_assignment()
  {
    expected_calls f(tested_allocation_traits::freebuf_calls);
    {
      tested_sequence x; x.length(2);

      tested_sequence y; y.length(3);

      expected_calls a(tested_allocation_traits::allocbuf_calls);
      f.reset();
      tested_allocation_traits::allocbuf_calls.failure_countdown(1);
      CHECK_THROW(y = x, testing_exception);

      FAIL_RETURN_IF_NOT(a.expect(1), a);
      FAIL_RETURN_IF_NOT(f.expect(0), f);

      CHECK_EQUAL(CORBA::ULong(3), y.length());
    }
    FAIL_RETURN_IF_NOT(f.expect(2), f);
    return 0;
  }

  int test_get_buffer_const()
  {
    tested_sequence a; a.length(4);
    tested_sequence const & b = a;

    value_type const * buffer = b.get_buffer();
    a[0] = 1; a[1] = 4; a[2] = 9; a[3] = 16;

    CHECK_EQUAL(1,  buffer[0]);
    CHECK_EQUAL(4,  buffer[1]);
    CHECK_EQUAL(9,  buffer[2]);
    CHECK_EQUAL(16, buffer[3]);
    return 0;
  }

  int test_all()
  {
    int status = 0;

    status +=this->test_default_constructor();
    status +=this->test_copy_constructor_from_default();
    status +=this->test_index_accessor();
    status +=this->test_index_modifier();
    status +=this->test_index_checking();
    status +=this->test_copy_constructor_values();
    status +=this->test_assignment_from_default();
    status +=this->test_assignment_values();
    status +=this->test_exception_in_copy_constructor();
    status +=this->test_exception_in_assignment();
    status +=this->test_get_buffer_const();
    return status;
  }
};

TAO_END_VERSIONED_NAMESPACE_DECL
#endif // guard_value_sequence_tester_hpp
