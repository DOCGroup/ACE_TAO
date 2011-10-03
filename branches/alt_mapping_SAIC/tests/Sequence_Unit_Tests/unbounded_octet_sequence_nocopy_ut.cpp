/**
 * @file
 *
 * @brief Unit test for unbounded sequences of octet types using the TAO
 * specific no copy specialization
 *
 * $Id$
 *
 * @author Johnny Willemsen
 */
#include "testing_allocation_traits.hpp"
#include "testing_range_checking.hpp"

#include "tao/Unbounded_Octet_Sequence_T.h"

#include "value_sequence_tester.hpp"

#include "test_macros.h"

#include "tao/Basic_Types.h"
#include "tao/CDR.h"

#include "ace/OS_NS_stdio.h"


using namespace TAO_VERSIONED_NAMESPACE_NAME::TAO;

typedef unbounded_value_sequence<CORBA::Octet> tested_sequence;
typedef tested_sequence::element_traits tested_element_traits;
typedef tested_sequence::allocation_traits tested_allocation_traits;
typedef details::range_checking<CORBA::Octet,true> range;

struct Tester
{
  typedef tested_sequence::value_type value_type;

  int test_copy_constructor_from_ulong()
  {
    expected_calls a(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);
    {
      tested_sequence x(16);
      FAIL_RETURN_IF_NOT(a.expect(1), a);
      x.length(8);

      CHECK_EQUAL(CORBA::ULong(16), x.maximum());
      CHECK_EQUAL(CORBA::ULong(8), x.length());
      CHECK_EQUAL(true, x.release());

      tested_sequence y(x);
      FAIL_RETURN_IF_NOT(a.expect(1), a);
      CHECK_EQUAL(CORBA::ULong(16), y.maximum());
      CHECK_EQUAL(CORBA::ULong(8), y.length());
      CHECK_EQUAL(true, y.release());
    }
    FAIL_RETURN_IF_NOT(f.expect(2), f);
    return 0;
  }

  int test_assignment_from_ulong()
  {
    expected_calls a(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);
    {
      tested_sequence x(16);
      x.length(8);
      FAIL_RETURN_IF_NOT(a.expect(1), a);
      CHECK_EQUAL(CORBA::ULong(16), x.maximum());
      CHECK_EQUAL(CORBA::ULong(8), x.length());
      CHECK_EQUAL(true, x.release());

      tested_sequence y;
      FAIL_RETURN_IF_NOT(a.expect(0),  a);

      y = x;
      FAIL_RETURN_IF_NOT(a.expect(1), a);
      // Since above no allocation for y was done then
      // no deallocation needed during assignment.
      FAIL_RETURN_IF_NOT(f.expect(0), f);
      CHECK_EQUAL(CORBA::ULong(16), y.maximum());
      CHECK_EQUAL(CORBA::ULong(8), y.length());
      CHECK_EQUAL(true, y.release());
    }
    FAIL_RETURN_IF_NOT(f.expect(2), f);
    return 0;
  }

  int test_ulong_constructor()
  {
    expected_calls a(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);
    {
      tested_sequence x(16);

      CHECK_EQUAL(CORBA::ULong(16), x.maximum());
      CHECK_EQUAL(CORBA::ULong(0), x.length());
      CHECK_EQUAL(true, x.release());
    }
    FAIL_RETURN_IF_NOT(a.expect(1), a);
    FAIL_RETURN_IF_NOT(f.expect(1), f);
    return 0;
  }

  int test_exception_in_ulong_constructor()
  {
    expected_calls a(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);
    {
      tested_allocation_traits::allocbuf_calls.failure_countdown(1);
      CHECK_THROW(tested_sequence x(16), testing_exception);
      FAIL_RETURN_IF_NOT(a.expect(1), a);
    }
    FAIL_RETURN_IF_NOT(f.expect(0), f);
    return 0;
  }

  int test_set_length_less_than_maximum()
  {
    expected_calls a(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);
    {
      tested_sequence x(16);

      x.length(8);
      CHECK_EQUAL(CORBA::ULong(16), x.maximum());
      CHECK_EQUAL(CORBA::ULong(8), x.length());
      CHECK_EQUAL(true, x.release());
    }
    FAIL_RETURN_IF_NOT(a.expect(1), a);
    FAIL_RETURN_IF_NOT(f.expect(1), f);
    return 0;
  }

  int test_set_length_more_than_maximum()
  {
    expected_calls a(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);
    {
      tested_sequence x(16);
      FAIL_RETURN_IF_NOT(a.expect(1), a);

      x.length(32);
      FAIL_RETURN_IF_NOT(a.expect(1), a);
      FAIL_RETURN_IF_NOT(f.expect(1), f);

      CHECK_EQUAL(CORBA::ULong(32), x.maximum());
      CHECK_EQUAL(CORBA::ULong(32), x.length());
      CHECK_EQUAL(true, x.release());
    }
    FAIL_RETURN_IF_NOT(f.expect(1), f);
    return 0;
  }

  int test_exception_in_set_length()
  {
    expected_calls f(tested_allocation_traits::freebuf_calls);
    {
      tested_sequence x;

      expected_calls a(tested_allocation_traits::allocbuf_calls);
      tested_allocation_traits::allocbuf_calls.failure_countdown(1);
      CHECK_THROW(x.length(8), testing_exception);
      FAIL_RETURN_IF_NOT(a.expect(1), a);
    }
    // length() above tried to allocate a buffer but it didn't reach
    // new[] and sequence was not changed, thus no need to deallocate.
    FAIL_RETURN_IF_NOT(f.expect(0), f);
    return 0;
  }

  value_type * alloc_and_init_buffer()
  {
    value_type * buf = tested_sequence::allocbuf(8);
    buf[0] = 1; buf[1] = 4; buf[2] = 9; buf[3] = 16;

    return buf;
  }

  ACE_Message_Block * alloc_and_init_mb()
  {
    char buf[9];
    ACE_OS::sprintf (buf, "%s", "testing ");
    size_t n = (ACE_OS::strlen (buf) + 1) * sizeof (char);
    ACE_Message_Block * mb = new ACE_Message_Block (n);
    mb->copy ((char *) buf, n);

    return mb;
  }

  int test_buffer_constructor_default()
  {
    value_type * buffer = alloc_and_init_buffer();

    expected_calls a(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);
    {
      tested_sequence a(8, 4, buffer);
      CHECK_EQUAL(CORBA::ULong(8), a.maximum());
      CHECK_EQUAL(CORBA::ULong(4), a.length());
      CHECK_EQUAL(buffer, a.get_buffer());
      CHECK_EQUAL(CORBA::Octet( 1), a[0]);
      CHECK_EQUAL(CORBA::Octet( 4), a[1]);
      CHECK_EQUAL(CORBA::Octet( 9), a[2]);
      CHECK_EQUAL(CORBA::Octet(16), a[3]);
      CHECK_EQUAL(false, a.release());
    }
    FAIL_RETURN_IF_NOT(a.expect(0), a);
    FAIL_RETURN_IF_NOT(f.expect(0), f);
    tested_sequence::freebuf(buffer);
    return 0;
  }

  int test_buffer_constructor_false()
  {
    value_type * buffer = alloc_and_init_buffer();
    expected_calls a(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);
    {
      tested_sequence a(8, 4, buffer, false);
      CHECK_EQUAL(CORBA::ULong(8), a.maximum());
      CHECK_EQUAL(CORBA::ULong(4), a.length());
      CHECK_EQUAL(buffer, a.get_buffer());
      CHECK_EQUAL(CORBA::Octet( 1), a[0]);
      CHECK_EQUAL(CORBA::Octet( 4), a[1]);
      CHECK_EQUAL(CORBA::Octet( 9), a[2]);
      CHECK_EQUAL(CORBA::Octet(16), a[3]);
      CHECK_EQUAL(false, a.release());
    }
    FAIL_RETURN_IF_NOT(a.expect(0), a);
    FAIL_RETURN_IF_NOT(f.expect(0), f);
    tested_sequence::freebuf(buffer);
    return 0;
  }

  int test_buffer_constructor_true()
  {
    value_type * buffer = alloc_and_init_buffer();
    expected_calls a(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);
    {
      tested_sequence a(8, 4, buffer, true);
      CHECK_EQUAL(CORBA::ULong(8), a.maximum());
      CHECK_EQUAL(CORBA::ULong(4), a.length());
      CHECK_EQUAL(buffer, a.get_buffer());
      CHECK_EQUAL(CORBA::Octet( 1), a[0]);
      CHECK_EQUAL(CORBA::Octet( 4), a[1]);
      CHECK_EQUAL(CORBA::Octet( 9), a[2]);
      CHECK_EQUAL(CORBA::Octet(16), a[3]);
      CHECK_EQUAL(true, a.release());
    }
    FAIL_RETURN_IF_NOT(a.expect(0), a);
    FAIL_RETURN_IF_NOT(f.expect(1), f);
    return 0;
  }

  int test_replace_default()
  {
    value_type * buffer = alloc_and_init_buffer();

    expected_calls c(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);
    {
      tested_sequence a;
      a.replace(8, 4, buffer);
      FAIL_RETURN_IF_NOT(c.expect(0), c);
      // Default constructed sequence doesn't allocate a buffer.
      FAIL_RETURN_IF_NOT(f.expect(0), f);

      CHECK_EQUAL(CORBA::ULong(8), a.maximum());
      CHECK_EQUAL(CORBA::ULong(4), a.length());
      CHECK_EQUAL(buffer, a.get_buffer());
      CHECK_EQUAL(CORBA::Octet( 1), a[0]);
      CHECK_EQUAL(CORBA::Octet( 4), a[1]);
      CHECK_EQUAL(CORBA::Octet( 9), a[2]);
      CHECK_EQUAL(CORBA::Octet(16), a[3]);
      CHECK_EQUAL(false, a.release());
    }
    FAIL_RETURN_IF_NOT(c.expect(0), c);
    FAIL_RETURN_IF_NOT(f.expect(0), f);
    tested_sequence::freebuf(buffer);
    return 0;
  }

  int test_replace_false()
  {
    value_type * buffer = alloc_and_init_buffer();
    expected_calls c(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);

    {
      tested_sequence a;
      a.replace(8, 4, buffer, false);
      FAIL_RETURN_IF_NOT(c.expect(0), c);
      // Default constructed sequence doesn't allocate a buffer.
      FAIL_RETURN_IF_NOT(f.expect(0), f);

      CHECK_EQUAL(CORBA::ULong(8), a.maximum());
      CHECK_EQUAL(CORBA::ULong(4), a.length());
      CHECK_EQUAL(buffer, a.get_buffer());
      CHECK_EQUAL(CORBA::Octet( 1), a[0]);
      CHECK_EQUAL(CORBA::Octet( 4), a[1]);
      CHECK_EQUAL(CORBA::Octet( 9), a[2]);
      CHECK_EQUAL(CORBA::Octet(16), a[3]);
      CHECK_EQUAL(false, a.release());
    }
    FAIL_RETURN_IF_NOT(c.expect(0), c);
    FAIL_RETURN_IF_NOT(f.expect(0), f);
    tested_sequence::freebuf(buffer);
    return 0;
  }

  int test_replace_true()
  {
    value_type * buffer = alloc_and_init_buffer();
    expected_calls c(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);

    {
      tested_sequence a;
      a.replace(8, 4, buffer, true);
      FAIL_RETURN_IF_NOT(c.expect(0), c);
      // Default constructed sequence doesn't allocate a buffer.
      FAIL_RETURN_IF_NOT(f.expect(0), f);

      CHECK_EQUAL(CORBA::ULong(8), a.maximum());
      CHECK_EQUAL(CORBA::ULong(4), a.length());
      CHECK_EQUAL(buffer, a.get_buffer());
      CHECK_EQUAL(CORBA::Octet( 1), a[0]);
      CHECK_EQUAL(CORBA::Octet( 4), a[1]);
      CHECK_EQUAL(CORBA::Octet( 9), a[2]);
      CHECK_EQUAL(CORBA::Octet(16), a[3]);
      CHECK_EQUAL(true, a.release());
    }
    FAIL_RETURN_IF_NOT(c.expect(0), c);
    FAIL_RETURN_IF_NOT(f.expect(1), f);
    return 0;
  }

  int test_get_buffer_default()
  {
    value_type * buffer = alloc_and_init_buffer();
    tested_sequence a(8, 4, buffer, true);
    CHECK_EQUAL(a.get_buffer(), buffer);
    return 0;
  }

  int test_get_buffer_false()
  {
    value_type * buffer = alloc_and_init_buffer();
    tested_sequence a(8, 4, buffer, true);
    CHECK_EQUAL(a.get_buffer(), buffer);
    return 0;
  }

  int test_get_buffer_true_with_release_false()
  {
    value_type * buffer = alloc_and_init_buffer();
    tested_sequence a(8, 4, buffer, false);
    CHECK(0 == a.get_buffer(true));
    tested_sequence::freebuf(buffer);
    return 0;
  }

  int test_get_buffer_true_with_release_true()
  {
    value_type * buffer = alloc_and_init_buffer();
    expected_calls c(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);
    {
      tested_sequence a(8, 4, buffer, true);
      CHECK_EQUAL(buffer, a.get_buffer(true));

      tested_sequence const & b = a;
      CHECK_EQUAL(0UL, b.maximum());
      CHECK_EQUAL(0UL, b.length());
      CHECK(0 != b.get_buffer());
      CHECK_EQUAL(true, b.release());

      FAIL_RETURN_IF_NOT(c.expect(1), c);

      CHECK(buffer != b.get_buffer());
    }
    FAIL_RETURN_IF_NOT(c.expect(0), c);
    FAIL_RETURN_IF_NOT(f.expect(1), f);
    tested_sequence::freebuf(buffer);
    return 0;
  }

  int test_no_copy_octet()
  {
#if (TAO_NO_COPY_OCTET_SEQUENCES == 1)
    ACE_Message_Block * mb = alloc_and_init_mb();
    tested_sequence a (8, mb);
    CHECK_EQUAL(CORBA::Octet( 't'), a[0]);
    CHECK_EQUAL(CORBA::Octet( 'g'), a[6]);

    char upperbuf[256];
    ACE_OS::sprintf (upperbuf, "%s", "THIS IS A TEST");
    size_t n = (ACE_OS::strlen (upperbuf) + 1) * sizeof (char);
    ACE_Message_Block * upper_mb = 0;
    ACE_NEW_RETURN (upper_mb,
        ACE_Message_Block (n), 1);
    // Copy buf into the Message_Block and update the wr_ptr ().
    upper_mb->copy ((char *) upperbuf, n);
    a.replace (n, upper_mb);
    CHECK_EQUAL(CORBA::Octet( 'T'), a[0]);
    CHECK_EQUAL(CORBA::Octet( 'S'), a[6]);
    delete upper_mb;
    delete mb;
#endif
    return 0;
  }

  int test_all()
  {
    int status = 0;
    status += this->test_no_copy_octet();
    status += this->test_ulong_constructor();
    status += this->test_copy_constructor_from_ulong();
    status += this->test_assignment_from_ulong();
    status += this->test_exception_in_ulong_constructor();
    status += this->test_set_length_less_than_maximum();
    status += this->test_set_length_more_than_maximum();
    status += this->test_exception_in_set_length();
    status += this->test_buffer_constructor_default();
    status += this->test_buffer_constructor_false();
    status += this->test_buffer_constructor_true();
    status += this->test_replace_default();
    status += this->test_replace_false();
    status += this->test_replace_true();
    status += this->test_get_buffer_false();
    status += this->test_get_buffer_true_with_release_false();
    status += this->test_get_buffer_true_with_release_true();
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

      {
        typedef value_sequence_tester<tested_sequence,tested_allocation_traits> common;
        common tester;
        status += tester.test_all ();
      }
    }
  catch (const ::CORBA::Exception &ex)
    {
      ex._tao_print_exception("ERROR : unexpected CORBA exception caugth :");
      ++status;
    }

  return status;
}
