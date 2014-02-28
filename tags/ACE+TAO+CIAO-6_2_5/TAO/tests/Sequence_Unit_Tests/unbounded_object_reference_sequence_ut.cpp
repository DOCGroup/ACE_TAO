/**
 * @file
 *
 * @brief Unit test for unbounded sequences of object references.
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

#include "tao/Unbounded_Object_Reference_Sequence_T.h"

#include "test_macros.h"


using namespace TAO_VERSIONED_NAMESPACE_NAME::TAO;

struct Tester
{
  typedef unbounded_object_reference_sequence<mock_reference, mock_reference_var> tested_sequence;
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

  int test_ulong_constructor()
  {
    expected_calls a(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);
    expected_calls i(tested_element_traits::default_initializer_calls);
    {
      tested_sequence x(16);

      CHECK_EQUAL(CORBA::ULong(16), x.maximum());
      CHECK_EQUAL(CORBA::ULong(0), x.length());
      CHECK_EQUAL(true, x.release());
    }
    FAIL_RETURN_IF_NOT(a.expect(1), a);
    FAIL_RETURN_IF_NOT(f.expect(1), f);
    // ulong constructor calls allocbuf and thus there must be
    // maximum() default initilized elements.
    FAIL_RETURN_IF_NOT(i.expect(16), i);
    return 0;
  }

  int test_ulong_constructor_throw()
  {
    expected_calls a(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);
    expected_calls i(tested_element_traits::default_initializer_calls);
    {
      tested_allocation_traits::allocbuf_calls.failure_countdown(1);
      CHECK_THROW(tested_sequence x(16), testing_exception);
    }
    FAIL_RETURN_IF_NOT(a.expect(1), a);
    FAIL_RETURN_IF_NOT(f.expect(0), f);
    FAIL_RETURN_IF_NOT(i.expect(0), i);
    return 0;
  }

  int test_buffer_constructor_release_true()
  {
    expected_calls a(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);
    expected_calls i(tested_element_traits::default_initializer_calls);
    {
      CORBA::ULong maximum = 32;
      tested_sequence::value_type * data = tested_sequence::allocbuf(maximum);
      a.reset();
      tested_sequence x(maximum, maximum / 2, data, true);

      CHECK_EQUAL(x.maximum(), maximum);
      CHECK_EQUAL(x.length(), maximum / 2);
      CHECK_EQUAL(x.get_buffer(), data);
      CHECK_EQUAL(x.release(), true);
    }
    FAIL_RETURN_IF_NOT(a.expect(0), a);
    FAIL_RETURN_IF_NOT(f.expect(1), f);
    // allocbuf default initializes maximum elements.
    FAIL_RETURN_IF_NOT(i.expect(32), i);
    return 0;
  }

  int test_buffer_constructor_release_false()
  {
    expected_calls a(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);
    expected_calls i(tested_element_traits::default_initializer_calls);
    {
      CORBA::ULong maximum = 64;
      tested_sequence::value_type * data = tested_sequence::allocbuf(maximum);
      a.reset();
      {
        tested_sequence x(maximum, maximum / 2, data, false);

        CHECK_EQUAL(x.maximum(), maximum);
        CHECK_EQUAL(x.length(), maximum / 2);
        CHECK_EQUAL(x.get_buffer(), data);
        CHECK_EQUAL(x.release(), false);
      }
      FAIL_RETURN_IF_NOT(a.expect(0), a);
      FAIL_RETURN_IF_NOT(f.expect(0), f);
      // allocbuf default initializes maximum elements.
      FAIL_RETURN_IF_NOT(i.expect(64), i);

      tested_sequence::freebuf(data);
    }
    FAIL_RETURN_IF_NOT(a.expect(0), a);
    FAIL_RETURN_IF_NOT(f.expect(1), f);
    FAIL_RETURN_IF_NOT(i.expect(0), i);
    return 0;
  }

  int test_copy_constructor_from_default()
  {
    expected_calls a(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);
    expected_calls i(tested_element_traits::default_initializer_calls);
    {
      tested_sequence x;

      tested_sequence y(x);
    }
    FAIL_RETURN_IF_NOT(a.expect(0), a);
    FAIL_RETURN_IF_NOT(f.expect(0), f);
    FAIL_RETURN_IF_NOT(i.expect(0), i);
    return 0;
  }

  int test_copy_constructor()
  {
    expected_calls a(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);
    expected_calls i(tested_element_traits::default_initializer_calls);
    expected_calls d(mock_reference::duplicate_calls);
    expected_calls r(mock_reference::release_calls);
    CORBA::ULong const l = 16;
    {
      tested_sequence x(l);
      x.length(l);
      FAIL_RETURN_IF_NOT(i.expect(l), i);

      FAIL_RETURN_IF_NOT(a.expect(1), a);
      FAIL_RETURN_IF_NOT(f.expect(0), f);
      CHECK_EQUAL(l, x.length());
      for(CORBA::ULong i = 0; i != l; ++i)
      {
        x[i] = mock_reference::allocate(i);
      }

      d.reset(); r.reset();

      tested_sequence y(x);
      FAIL_RETURN_IF_NOT(a.expect(1), a);
      FAIL_RETURN_IF_NOT(f.expect(0), f);
      CHECK_EQUAL(l, x.length());
      FAIL_RETURN_IF_NOT(d.expect(l), d);
      for(CORBA::ULong i = 0; i != l; ++i)
      {
        CHECK_EQUAL(int(i), y[i]->id());
      }
    }
    FAIL_RETURN_IF_NOT(d.expect(0), d);
    FAIL_RETURN_IF_NOT(r.expect(2*l), r);
    FAIL_RETURN_IF_NOT(a.expect(0), a);
    FAIL_RETURN_IF_NOT(f.expect(2), f);
    FAIL_RETURN_IF_NOT(i.expect(0), i);
    return 0;
  }

  int test_copy_constructor_throw_duplicate()
  {
    expected_calls a(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);
    expected_calls i(tested_element_traits::default_initializer_calls);
    expected_calls d(mock_reference::duplicate_calls);
    expected_calls r(mock_reference::release_calls);
    CORBA::ULong const l = 16;
    {
      tested_sequence x(l);
      x.length(l);

      FAIL_RETURN_IF_NOT(i.expect(l), i);

      FAIL_RETURN_IF_NOT(a.expect(1), a);
      FAIL_RETURN_IF_NOT(f.expect(0), f);
      CHECK_EQUAL(l, x.length());
      for(CORBA::ULong i = 0; i != l; ++i)
      {
        x[i] = mock_reference::allocate(i);
      }

      d.reset(); r.reset();

      mock_reference::duplicate_calls.failure_countdown(8);
      CHECK_THROW(tested_sequence y(x), testing_exception);
      FAIL_RETURN_IF_NOT(a.expect(1), a);
      FAIL_RETURN_IF_NOT(f.expect(1), f);
      FAIL_RETURN_IF_NOT(d.expect(8), d);
      FAIL_RETURN_IF_NOT(r.expect(l), r);
    }
    FAIL_RETURN_IF_NOT(d.expect(0), d);
    FAIL_RETURN_IF_NOT(r.expect(l), r);
    FAIL_RETURN_IF_NOT(a.expect(0), a);
    FAIL_RETURN_IF_NOT(f.expect(1), f);
    FAIL_RETURN_IF_NOT(i.expect(0), i);
    return 0;
  }

  int test_set_length_less_than_maximum()
  {
    expected_calls a(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);
    expected_calls i(tested_element_traits::default_initializer_calls);
    {
      tested_sequence x(16);

      x.length(8);
      CHECK_EQUAL(CORBA::ULong(16), x.maximum());
      CHECK_EQUAL(CORBA::ULong(8), x.length());
      CHECK_EQUAL(true, x.release());

      // allocbuf default initializes 16 elements.
      FAIL_RETURN_IF_NOT(i.expect(16), i);
    }
    FAIL_RETURN_IF_NOT(a.expect(1), a);
    FAIL_RETURN_IF_NOT(f.expect(1), f);
    return 0;
  }

  int test_set_length_more_than_maximum()
  {
    expected_calls a(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);
    expected_calls i(tested_element_traits::default_initializer_calls);
    {
      tested_sequence x(16);
      FAIL_RETURN_IF_NOT(a.expect(1), a);

      x.length(32);
      FAIL_RETURN_IF_NOT(a.expect(1), a);
      FAIL_RETURN_IF_NOT(f.expect(1), f);

      // ulong constructor calls allocbuf and thus there must be
      // maximum() default initilized elements + length() leads to buffer
      // reallocation maximum() gets set to a new value.
      FAIL_RETURN_IF_NOT(i.expect(48), i);

      CHECK_EQUAL(CORBA::ULong(32), x.maximum());
      CHECK_EQUAL(CORBA::ULong(32), x.length());
      CHECK_EQUAL(true, x.release());
    }
    FAIL_RETURN_IF_NOT(f.expect(1), f);
    return 0;
  }

  int test_set_length_copy_elements()
  {
    expected_calls a(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);
    expected_calls i(tested_element_traits::default_initializer_calls);
    {
      tested_sequence x(16);
      FAIL_RETURN_IF_NOT(a.expect(1), a);

      x.length(16);
      CHECK_EQUAL(CORBA::ULong(16), x.length());
      FAIL_RETURN_IF_NOT(a.expect(0), a);
      FAIL_RETURN_IF_NOT(f.expect(0), a);
      FAIL_RETURN_IF_NOT(i.expect(16), i);

      for(int j = 0; j != 16; ++j)
      {
        CHECK_EQUAL(mock_reference::_nil(), x[j]);
        x[j] = mock_reference::allocate(j);
      }

      x.length(32);
      CHECK_EQUAL(CORBA::ULong(32), x.length());
      FAIL_RETURN_IF_NOT(a.expect(1), a);
      FAIL_RETURN_IF_NOT(f.expect(1), f);
      FAIL_RETURN_IF_NOT(i.expect(16), i);

      tested_sequence const & y = x;

      for(CORBA::ULong i = 0; i != 16UL; ++i)
      {
        CHECK(mock_reference::_nil() != y[i]);
        CHECK_EQUAL(int(i), y[i]->id());
      }
      for(CORBA::ULong i = 16; i != 32UL; ++i)
      {
        CHECK_EQUAL(mock_reference::_nil(), y[i]);
      }
    }
    FAIL_RETURN_IF_NOT(f.expect(1), f);
    FAIL_RETURN_IF_NOT(i.expect(0), i);
    return 0;
  }

  int test_set_length_throw()
  {
    expected_calls a(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);
    expected_calls i(tested_element_traits::default_initializer_calls);
    {
      tested_sequence x(16);
      FAIL_RETURN_IF_NOT(a.expect(1), a);
      x.length(16);
      FAIL_RETURN_IF_NOT(i.expect(16), i);

      for(int j = 0; j != 16; ++j)
      {
        CHECK_EQUAL(mock_reference::_nil(), x[j]);
        x[j] = mock_reference::allocate(j);
      }

      tested_allocation_traits::allocbuf_calls.failure_countdown(1);
      CHECK_THROW(x.length(32), testing_exception);
      FAIL_RETURN_IF_NOT(a.expect(1), a);
      FAIL_RETURN_IF_NOT(f.expect(0), f);
      CHECK_EQUAL(CORBA::ULong(16), x.length());
      FAIL_RETURN_IF_NOT(i.expect(0), i);

      tested_sequence const & y = x;

      for(int j = 0; j != 16; ++j)
      {
        CHECK(mock_reference::_nil() != y[j]);
        CHECK_EQUAL(j, y[j]->id());
      }
    }
    FAIL_RETURN_IF_NOT(a.expect(0), a);
    FAIL_RETURN_IF_NOT(f.expect(1), f);
    FAIL_RETURN_IF_NOT(i.expect(0), i);
    return 0;
  }

  value_type * alloc_and_init_buffer()
  {
    value_type * buf = tested_sequence::allocbuf(8);
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

  int test_replace_release_true()
  {
    value_type * buffer = alloc_and_init_buffer();

    expected_calls c(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);
    expected_calls r(tested_element_traits::release_calls);
    {
      tested_sequence a;
      a.replace(8, 4, buffer, true);
      FAIL_RETURN_IF_NOT(c.expect(0), c);
      FAIL_RETURN_IF_NOT(f.expect(0), f);
      FAIL_RETURN_IF_NOT(r.expect(0), r);

      CHECK_EQUAL(CORBA::ULong(8), a.maximum());
      CHECK_EQUAL(CORBA::ULong(4), a.length());
      CHECK_EQUAL(buffer, a.get_buffer());
      CHECK_EQUAL(true, a.release());
      check_values(a);
    }
    FAIL_RETURN_IF_NOT(c.expect(0), c);
    // Since we've given away the ownership the buffer is deallocated by
    // the sequence.
    FAIL_RETURN_IF_NOT(f.expect(1), f);
    FAIL_RETURN_IF_NOT(r.expect(8), r);
    return 0;
  }

  int test_replace_release_false()
  {
    value_type * buffer = alloc_and_init_buffer();

    expected_calls c(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);
    expected_calls r(tested_element_traits::release_calls);
    {
      tested_sequence a;
      a.replace(8, 4, buffer, false);
      FAIL_RETURN_IF_NOT(c.expect(0), c);
      FAIL_RETURN_IF_NOT(f.expect(0), f);
      FAIL_RETURN_IF_NOT(r.expect(0), 0);

      CHECK_EQUAL(CORBA::ULong(8), a.maximum());
      CHECK_EQUAL(CORBA::ULong(4), a.length());
      CHECK_EQUAL(buffer, a.get_buffer());
      CHECK_EQUAL(false, a.release());
      check_values(a);
    }
    FAIL_RETURN_IF_NOT(c.expect(0), c);
    FAIL_RETURN_IF_NOT(f.expect(0), f);
    tested_sequence::freebuf(buffer);
    FAIL_RETURN_IF_NOT(r.expect(8), r);
    return 0;
  }

  int test_replace_release_default()
  {
    value_type * buffer = alloc_and_init_buffer();

    expected_calls c(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);
    expected_calls r(tested_element_traits::release_calls);
    {
      tested_sequence a;
      a.replace(8, 4, buffer);
      FAIL_RETURN_IF_NOT(c.expect(0), c);
      FAIL_RETURN_IF_NOT(f.expect(0), f);
      FAIL_RETURN_IF_NOT(r.expect(0), 0);

      CHECK_EQUAL(CORBA::ULong(8), a.maximum());
      CHECK_EQUAL(CORBA::ULong(4), a.length());
      CHECK_EQUAL(buffer, a.get_buffer());
      CHECK_EQUAL(false, a.release());
      check_values(a);
    }
    FAIL_RETURN_IF_NOT(c.expect(0), c);
    FAIL_RETURN_IF_NOT(f.expect(0), f);
    tested_sequence::freebuf(buffer);
    FAIL_RETURN_IF_NOT(r.expect(8), r);
    return 0;
  }

  int test_all()
  {
    int status = 0;
    status += this->test_default_constructor();
    status += this->test_ulong_constructor();
    status += this->test_ulong_constructor_throw();
    status += this->test_buffer_constructor_release_true();
    status += this->test_buffer_constructor_release_false();
    status += this->test_copy_constructor_from_default();
    status += this->test_copy_constructor();
    status += this->test_copy_constructor_throw_duplicate();
    status += this->test_set_length_less_than_maximum();
    status += this->test_set_length_more_than_maximum();
    status += this->test_set_length_copy_elements();
    status += this->test_set_length_throw();
    status += this->test_replace_release_true();
    status += this->test_replace_release_false();
    status += this->test_replace_release_default();
    return status;
  }

  Tester() {}
};

int ACE_TMAIN(int,ACE_TCHAR*[])
{
  int status = 0;
  try
    {
      {
        Tester tester;
        status += tester.test_all();
      }
    }
  catch (const ::CORBA::Exception &ex)
    {
      ex._tao_print_exception("ERROR : unexpected CORBA exception caugth :");
      ++status;
    }

  return status;
}
