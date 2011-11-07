/**
 * @file
 *
 * @brief Unit test for object_reference_sequence_element, this is the
 *        type returned by operator[] from a string sequence.
 *
 * $Id$
 *
 * @author Carlos O'Ryan
 */
#include "testing_object_reference_traits.hpp"
#include "mock_reference.hpp"

#include "tao/Object_Reference_Sequence_Element_T.h"
#include "tao/SystemException.h"

#include "test_macros.h"

using namespace TAO_VERSIONED_NAMESPACE_NAME::TAO::details;

  typedef object_reference_traits<mock_reference,mock_reference_var,true> tested_element_traits;
  typedef object_reference_sequence_element<tested_element_traits> tested_element;
  typedef mock_reference * reference_type;
  typedef tested_element_traits::object_type_var reference_var;

  int test_assignment_from_var_release_true()
  {
    expected_calls d(tested_element_traits::duplicate_calls);
    expected_calls r(tested_element_traits::release_calls);
    expected_calls md(mock_reference::duplicate_calls);
    expected_calls mr(mock_reference::release_calls);
    {
      reference_type xe = mock_reference::allocate(1);
      tested_element x(xe, true);
      reference_var y(mock_reference::allocate(2));

      FAIL_RETURN_IF_NOT(md.expect(0), md);
      FAIL_RETURN_IF_NOT(mr.expect(0), mr);

      x = y;

      FAIL_RETURN_IF_NOT(d.expect(0), d);
      FAIL_RETURN_IF_NOT(r.expect(1), r);
      FAIL_RETURN_IF_NOT(md.expect(1), md);
      FAIL_RETURN_IF_NOT(mr.expect(2), mr);

      CHECK_EQUAL(2, x->id());

      tested_element_traits::release(x);
      FAIL_RETURN_IF_NOT(r.expect(1), r);
      FAIL_RETURN_IF_NOT(mr.expect(1), mr);
    }

    FAIL_RETURN_IF_NOT(d.expect(0),d );
    FAIL_RETURN_IF_NOT(r.expect(0),r );
    FAIL_RETURN_IF_NOT(md.expect(0), md);
    FAIL_RETURN_IF_NOT(mr.expect(1), mr);

    return 0;
  }

  int test_assignment_from_var_release_false()
  {
    expected_calls d(tested_element_traits::duplicate_calls);
    expected_calls r(tested_element_traits::release_calls);
    expected_calls md(mock_reference::duplicate_calls);
    expected_calls mr(mock_reference::release_calls);
    {
      reference_type xe = mock_reference::allocate(1);
      reference_type oldxe = xe;
      tested_element x(xe, false);
      reference_var y(mock_reference::allocate(2));

      FAIL_RETURN_IF_NOT(md.expect(0), md);
      FAIL_RETURN_IF_NOT(mr.expect(0), mr);

      x = y;

      FAIL_RETURN_IF_NOT(d.expect(0), d );
      FAIL_RETURN_IF_NOT(r.expect(0), r);
      FAIL_RETURN_IF_NOT(md.expect(1), md);
      FAIL_RETURN_IF_NOT(mr.expect(1), mr);

      CHECK_EQUAL(2, x->id());

      tested_element_traits::release(x);
      tested_element_traits::release(oldxe);
      FAIL_RETURN_IF_NOT(r.expect(2), r);
      FAIL_RETURN_IF_NOT(mr.expect(2), mr);
    }

    FAIL_RETURN_IF_NOT(d.expect(0),d );
    FAIL_RETURN_IF_NOT(r.expect(0), r);
    FAIL_RETURN_IF_NOT(md.expect(0), md);
    FAIL_RETURN_IF_NOT(mr.expect(1), mr);
    return 0;
  }

  int test_assignment_from_var_throw()
  {
    expected_calls d(tested_element_traits::duplicate_calls);
    expected_calls r(tested_element_traits::release_calls);
    expected_calls md(mock_reference::duplicate_calls);
    expected_calls mr(mock_reference::release_calls);
    {
      reference_type xe = mock_reference::allocate(1);
      tested_element x(xe, true);
      reference_var y(mock_reference::allocate(2));

      FAIL_RETURN_IF_NOT(md.expect(0), md);
      FAIL_RETURN_IF_NOT(mr.expect(0), mr);

      mock_reference::duplicate_calls.failure_countdown(1);

      CHECK_THROW(x = y, testing_exception);

      FAIL_RETURN_IF_NOT(d.expect(0), d);
      FAIL_RETURN_IF_NOT(r.expect(0), r);
      FAIL_RETURN_IF_NOT(md.expect(1), md);
      FAIL_RETURN_IF_NOT(mr.expect(0), mr);

      CHECK_EQUAL(1, x->id());
      CHECK_EQUAL(2, y->id());

      tested_element_traits::release(x);
      FAIL_RETURN_IF_NOT(r.expect(1), r);
      FAIL_RETURN_IF_NOT(mr.expect(1), mr);
    }

    FAIL_RETURN_IF_NOT(d.expect(0), d);
    FAIL_RETURN_IF_NOT(r.expect(0), r);
    FAIL_RETURN_IF_NOT(md.expect(0), md);
    FAIL_RETURN_IF_NOT(mr.expect(1), mr);
    return 0;
  }

  int test_assignment_from_element_release_true()
  {
    expected_calls d(tested_element_traits::duplicate_calls);
    expected_calls r(tested_element_traits::release_calls);
    expected_calls md(mock_reference::duplicate_calls);
    expected_calls mr(mock_reference::release_calls);
    {
      reference_type xe = mock_reference::allocate(1);
      tested_element x(xe, true);

      reference_type ye = mock_reference::allocate(2);
      tested_element y(ye, true);

      FAIL_RETURN_IF_NOT(d.expect(0), d);
      FAIL_RETURN_IF_NOT(r.expect(0) ,r);

      x = y;

      FAIL_RETURN_IF_NOT(d.expect(1), d);
      FAIL_RETURN_IF_NOT(r.expect(1), r);
      FAIL_RETURN_IF_NOT(md.expect(1), md);
      FAIL_RETURN_IF_NOT(mr.expect(2), mr);

      CHECK_EQUAL(2, x->id());

      tested_element_traits::release(xe);
      tested_element_traits::release(ye);
      FAIL_RETURN_IF_NOT(r.expect(2),r );
      FAIL_RETURN_IF_NOT(mr.expect(2), mr);
    }

    FAIL_RETURN_IF_NOT(d.expect(0), d);
    FAIL_RETURN_IF_NOT(r.expect(0), r);
    FAIL_RETURN_IF_NOT(md.expect(0), md);
    FAIL_RETURN_IF_NOT(mr.expect(0), mr);
    return 0;
  }

  int test_assignment_from_element_release_false()
  {
    expected_calls d(tested_element_traits::duplicate_calls);
    expected_calls r(tested_element_traits::release_calls);
    expected_calls md(mock_reference::duplicate_calls);
    expected_calls mr(mock_reference::release_calls);
    {
      reference_type xe = mock_reference::allocate(1);
      reference_type oldxe = xe;
      tested_element x(xe, false);

      reference_type ye = mock_reference::allocate(2);
      tested_element y(ye, true);

      FAIL_RETURN_IF_NOT(d.expect(0), d);
      FAIL_RETURN_IF_NOT(r.expect(0), r);

      x = y;

      FAIL_RETURN_IF_NOT(d.expect(1), d);
      FAIL_RETURN_IF_NOT(r.expect(0), r);
      FAIL_RETURN_IF_NOT(md.expect(1), md);
      FAIL_RETURN_IF_NOT(mr.expect(1), mr);

      CHECK_EQUAL(2, x->id());

      tested_element_traits::release(xe);
      tested_element_traits::release(oldxe);
      tested_element_traits::release(ye);
      FAIL_RETURN_IF_NOT(r.expect(3), r);
      FAIL_RETURN_IF_NOT(mr.expect(3), mr);
    }

    FAIL_RETURN_IF_NOT(d.expect(0), d);
    FAIL_RETURN_IF_NOT(r.expect(0), r);
    FAIL_RETURN_IF_NOT(md.expect(0), md);
    FAIL_RETURN_IF_NOT(mr.expect(0), mr);
    return 0;
  }

  int test_assignment_from_reference_release_true()
  {
    expected_calls d(tested_element_traits::duplicate_calls);
    expected_calls r(tested_element_traits::release_calls);
    expected_calls md(mock_reference::duplicate_calls);
    expected_calls mr(mock_reference::release_calls);
    {
      reference_type xe = mock_reference::allocate(1);
      tested_element x(xe, true);

      reference_type y = mock_reference::allocate(2);

      FAIL_RETURN_IF_NOT(d.expect(0), d);
      FAIL_RETURN_IF_NOT(r.expect(0), r);

      x = y;

      FAIL_RETURN_IF_NOT(d.expect(0), d);
      FAIL_RETURN_IF_NOT(r.expect(1), r);
      FAIL_RETURN_IF_NOT(md.expect(0), md);
      FAIL_RETURN_IF_NOT(mr.expect(2), mr);

      CHECK_EQUAL(2, x->id());

      tested_element_traits::release(xe);
      FAIL_RETURN_IF_NOT(r.expect(1), r);
      FAIL_RETURN_IF_NOT(mr.expect(1), mr);
    }

    FAIL_RETURN_IF_NOT(d.expect(0), d);
    FAIL_RETURN_IF_NOT(r.expect(0), r);
    FAIL_RETURN_IF_NOT(md.expect(0), md);
    FAIL_RETURN_IF_NOT(mr.expect(0), mr);
    return 0;
  }

  int test_assignment_from_reference_release_false()
  {
    expected_calls d(tested_element_traits::duplicate_calls);
    expected_calls r(tested_element_traits::release_calls);
    expected_calls md(mock_reference::duplicate_calls);
    expected_calls mr(mock_reference::release_calls);
    {
      reference_type xe = mock_reference::allocate(1);
      reference_type oldxe = xe;
      tested_element x(xe, false);

      reference_type y = mock_reference::allocate(2);

      FAIL_RETURN_IF_NOT(d.expect(0), d);
      FAIL_RETURN_IF_NOT(r.expect(0), r);

      x = y;

      FAIL_RETURN_IF_NOT(d.expect(0), d);
      FAIL_RETURN_IF_NOT(r.expect(0), r);
      FAIL_RETURN_IF_NOT(md.expect(0), md);
      FAIL_RETURN_IF_NOT(mr.expect(1), mr);

      CHECK_EQUAL(2, x->id());

      tested_element_traits::release(xe);
      tested_element_traits::release(oldxe);
      FAIL_RETURN_IF_NOT(r.expect(2), 2);
      FAIL_RETURN_IF_NOT(mr.expect(2), mr);
    }

    FAIL_RETURN_IF_NOT(d.expect(0), d);
    FAIL_RETURN_IF_NOT(r.expect(0), r);
    FAIL_RETURN_IF_NOT(md.expect(0), md);
    FAIL_RETURN_IF_NOT(mr.expect(0), mr);
    return 0;
  }

int ACE_TMAIN(int,ACE_TCHAR*[])
{
  int status = 0;
  try
    {
      status += test_assignment_from_var_release_true ();
      status += test_assignment_from_var_release_false ();
      status += test_assignment_from_var_throw ();
      status += test_assignment_from_element_release_true ();
      status += test_assignment_from_element_release_false ();
      status += test_assignment_from_reference_release_true ();
      status += test_assignment_from_reference_release_false ();
    }
  catch (const ::CORBA::Exception &ex)
    {
      ex._tao_print_exception("ERROR : unexpected CORBA exception caugth :");
      ++status;
    }

  return status;
}

