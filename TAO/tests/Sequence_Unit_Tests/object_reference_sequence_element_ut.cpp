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

#include <sstream>
#include <stdexcept>
#include <iostream>

#include <boost/test/unit_test.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

using namespace TAO_VERSIONED_NAMESPACE_NAME::TAO::details;

using namespace boost::unit_test_framework;

struct Tester
{
  typedef object_reference_traits<mock_reference,mock_reference_var,true> tested_element_traits;
  typedef object_reference_sequence_element<tested_element_traits> tested_element;
  typedef mock_reference * reference_type;
  typedef tested_element_traits::object_type_var reference_var;

  void test_assignment_from_var_release_true()
  {
    expected_calls d(tested_element_traits::duplicate_calls);
    expected_calls r(tested_element_traits::release_calls);
    expected_calls md(mock_reference::duplicate_calls);
    expected_calls mr(mock_reference::release_calls);
    {
      reference_type xe = mock_reference::allocate(1);
      tested_element x(xe, true);
      reference_var y(mock_reference::allocate(2));

      BOOST_CHECK_MESSAGE(md.expect(0), md);
      BOOST_CHECK_MESSAGE(mr.expect(0), mr);

      x = y;

      BOOST_CHECK_MESSAGE(d.expect(0), d);
      BOOST_CHECK_MESSAGE(r.expect(1), r);
      BOOST_CHECK_MESSAGE(md.expect(1), md);
      BOOST_CHECK_MESSAGE(mr.expect(2), mr);

      BOOST_CHECK_EQUAL(2, x->id());

      tested_element_traits::release(x);
      BOOST_CHECK_MESSAGE(r.expect(1), r);
      BOOST_CHECK_MESSAGE(mr.expect(1), mr);
    }

    BOOST_CHECK_MESSAGE(d.expect(0), d);
    BOOST_CHECK_MESSAGE(r.expect(0), r);
    BOOST_CHECK_MESSAGE(md.expect(0), md);
    BOOST_CHECK_MESSAGE(mr.expect(1), mr);
  }

  void test_assignment_from_var_release_false()
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

      BOOST_CHECK_MESSAGE(md.expect(0), md);
      BOOST_CHECK_MESSAGE(mr.expect(0), mr);

      x = y;

      BOOST_CHECK_MESSAGE(d.expect(0), d);
      BOOST_CHECK_MESSAGE(r.expect(0), r);
      BOOST_CHECK_MESSAGE(md.expect(1), md);
      BOOST_CHECK_MESSAGE(mr.expect(1), mr);

      BOOST_CHECK_EQUAL(2, x->id());

      tested_element_traits::release(x);
      tested_element_traits::release(oldxe);
      BOOST_CHECK_MESSAGE(r.expect(2), r);
      BOOST_CHECK_MESSAGE(mr.expect(2), mr);
    }

    BOOST_CHECK_MESSAGE(d.expect(0), d);
    BOOST_CHECK_MESSAGE(r.expect(0), r);
    BOOST_CHECK_MESSAGE(md.expect(0), md);
    BOOST_CHECK_MESSAGE(mr.expect(1), mr);
  }

  void test_assignment_from_var_throw()
  {
    expected_calls d(tested_element_traits::duplicate_calls);
    expected_calls r(tested_element_traits::release_calls);
    expected_calls md(mock_reference::duplicate_calls);
    expected_calls mr(mock_reference::release_calls);
    {
      reference_type xe = mock_reference::allocate(1);
      tested_element x(xe, true);
      reference_var y(mock_reference::allocate(2));

      BOOST_CHECK_MESSAGE(md.expect(0), md);
      BOOST_CHECK_MESSAGE(mr.expect(0), mr);

      mock_reference::duplicate_calls.failure_countdown(1);

      BOOST_CHECK_THROW(x = y, testing_exception);

      BOOST_CHECK_MESSAGE(d.expect(0), d);
      BOOST_CHECK_MESSAGE(r.expect(0), r);
      BOOST_CHECK_MESSAGE(md.expect(1), md);
      BOOST_CHECK_MESSAGE(mr.expect(0), mr);

      BOOST_CHECK_EQUAL(1, x->id());
      BOOST_CHECK_EQUAL(2, y->id());

      tested_element_traits::release(x);
      BOOST_CHECK_MESSAGE(r.expect(1), r);
      BOOST_CHECK_MESSAGE(mr.expect(1), mr);
    }

    BOOST_CHECK_MESSAGE(d.expect(0), d);
    BOOST_CHECK_MESSAGE(r.expect(0), r);
    BOOST_CHECK_MESSAGE(md.expect(0), md);
    BOOST_CHECK_MESSAGE(mr.expect(1), mr);
  }

  void test_assignment_from_element_release_true()
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

      BOOST_CHECK_MESSAGE(d.expect(0), d);
      BOOST_CHECK_MESSAGE(r.expect(0), r);

      x = y;

      BOOST_CHECK_MESSAGE(d.expect(1), d);
      BOOST_CHECK_MESSAGE(r.expect(1), r);
      BOOST_CHECK_MESSAGE(md.expect(1), md);
      BOOST_CHECK_MESSAGE(mr.expect(2), mr);

      BOOST_CHECK_EQUAL(2, x->id());

      tested_element_traits::release(xe);
      tested_element_traits::release(ye);
      BOOST_CHECK_MESSAGE(r.expect(2), r);
      BOOST_CHECK_MESSAGE(mr.expect(2), mr);
    }

    BOOST_CHECK_MESSAGE(d.expect(0), d);
    BOOST_CHECK_MESSAGE(r.expect(0), r);
    BOOST_CHECK_MESSAGE(md.expect(0), md);
    BOOST_CHECK_MESSAGE(mr.expect(0), mr);
  }

  void test_assignment_from_element_release_false()
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

      BOOST_CHECK_MESSAGE(d.expect(0), d);
      BOOST_CHECK_MESSAGE(r.expect(0), r);

      x = y;

      BOOST_CHECK_MESSAGE(d.expect(1), d);
      BOOST_CHECK_MESSAGE(r.expect(0), r);
      BOOST_CHECK_MESSAGE(md.expect(1), md);
      BOOST_CHECK_MESSAGE(mr.expect(1), mr);

      BOOST_CHECK_EQUAL(2, x->id());

      tested_element_traits::release(xe);
      tested_element_traits::release(oldxe);
      tested_element_traits::release(ye);
      BOOST_CHECK_MESSAGE(r.expect(3), r);
      BOOST_CHECK_MESSAGE(mr.expect(3), mr);
    }

    BOOST_CHECK_MESSAGE(d.expect(0), d);
    BOOST_CHECK_MESSAGE(r.expect(0), r);
    BOOST_CHECK_MESSAGE(md.expect(0), md);
    BOOST_CHECK_MESSAGE(mr.expect(0), mr);
  }

  void test_assignment_from_reference_release_true()
  {
    expected_calls d(tested_element_traits::duplicate_calls);
    expected_calls r(tested_element_traits::release_calls);
    expected_calls md(mock_reference::duplicate_calls);
    expected_calls mr(mock_reference::release_calls);
    {
      reference_type xe = mock_reference::allocate(1);
      tested_element x(xe, true);

      reference_type y = mock_reference::allocate(2);

      BOOST_CHECK_MESSAGE(d.expect(0), d);
      BOOST_CHECK_MESSAGE(r.expect(0), r);

      x = y;

      BOOST_CHECK_MESSAGE(d.expect(0), d);
      BOOST_CHECK_MESSAGE(r.expect(1), r);
      BOOST_CHECK_MESSAGE(md.expect(0), md);
      BOOST_CHECK_MESSAGE(mr.expect(2), mr);

      BOOST_CHECK_EQUAL(2, x->id());

      tested_element_traits::release(xe);
      BOOST_CHECK_MESSAGE(r.expect(1), r);
      BOOST_CHECK_MESSAGE(mr.expect(1), mr);
    }

    BOOST_CHECK_MESSAGE(d.expect(0), d);
    BOOST_CHECK_MESSAGE(r.expect(0), r);
    BOOST_CHECK_MESSAGE(md.expect(0), md);
    BOOST_CHECK_MESSAGE(mr.expect(0), mr);
  }

  void test_assignment_from_reference_release_false()
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

      BOOST_CHECK_MESSAGE(d.expect(0), d);
      BOOST_CHECK_MESSAGE(r.expect(0), r);

      x = y;

      BOOST_CHECK_MESSAGE(d.expect(0), d);
      BOOST_CHECK_MESSAGE(r.expect(0), r);
      BOOST_CHECK_MESSAGE(md.expect(0), md);
      BOOST_CHECK_MESSAGE(mr.expect(1), mr);

      BOOST_CHECK_EQUAL(2, x->id());

      tested_element_traits::release(xe);
      tested_element_traits::release(oldxe);
      BOOST_CHECK_MESSAGE(r.expect(2), r);
      BOOST_CHECK_MESSAGE(mr.expect(2), mr);
    }

    BOOST_CHECK_MESSAGE(d.expect(0), d);
    BOOST_CHECK_MESSAGE(r.expect(0), r);
    BOOST_CHECK_MESSAGE(md.expect(0), md);
    BOOST_CHECK_MESSAGE(mr.expect(0), mr);
  }

  void add_all(test_suite * ts)
  {
    boost::shared_ptr<Tester> shared_this(self_);

    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_assignment_from_var_release_true,
                shared_this));
    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_assignment_from_var_release_false,
                shared_this));
    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_assignment_from_var_throw,
                shared_this));
    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_assignment_from_element_release_true,
                shared_this));
    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_assignment_from_element_release_false,
                shared_this));
    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_assignment_from_reference_release_true,
                shared_this));
    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_assignment_from_reference_release_false,
                shared_this));
  }

  static boost::shared_ptr<Tester> allocate()
  {
    boost::shared_ptr<Tester> ptr(new Tester);
    ptr->self_ = ptr;

    return ptr;
  }

private:
  Tester() {}

  boost::weak_ptr<Tester> self_;
};

ACE_Proper_Export_Flag test_suite *
init_unit_test_suite(int, char*[])
{
  test_suite * ts =
      BOOST_TEST_SUITE("object reference sequence element unit test");

  boost::shared_ptr<Tester > mock_reference_tester(
      Tester::allocate());
  mock_reference_tester->add_all(ts);

  return ts;
}

