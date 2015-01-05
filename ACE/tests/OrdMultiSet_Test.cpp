
//=============================================================================
/**
 *  @file    OrdMultiSet_Test.cpp
 *
 *  $Id$
 *
 *  This is a simple test of the <ACE_Ordered_MultiSet> and
 *  <ACE_Ordered_MultiSet_Iterator> class templates, instantiating
 *  them with type int.  No command line arguments are needed to run
 *  the test.
 *
 *
 *  @author Chris Gill <cdgill@cs.wustl.edu>
 */
//=============================================================================


// Note, for this test the config.h file *must* come first!
#include "ace/config-all.h"

#include "test_config.h"
#include "ace/Containers.h"



int
run_main (int, ACE_TCHAR *[])
{
  int ret = 0;
  int *ptr = 0;

  ACE_START_TEST (ACE_TEXT ("OrdMultiSet_Test"));

  // make an empty set of int and an iterator
  ACE_Ordered_MultiSet<int> set;
  ACE_Ordered_MultiSet_Iterator<int> iter(set);

  // Put in a range of odd ints, without an iterator.
  int i;
  for (i = -10; i < 10; ++i)
    set.insert (2 * i + 1);

  // Put in an interleaved range of even ints, using an iterator.
  for (i = -10; i <= 10; ++i)
    set.insert (2 * i, iter);

  // Remove the first and last elements of range.
  while (set.remove (-20) == 0)
    {
      // No action.
    }

  while (set.remove (20) == 0)
    {
      // No action.
    }

  // Should still have 39 elements in the set.
  ACE_TEST_ASSERT (set.is_empty () == 0);
  ACE_TEST_ASSERT (set.size () == 39);

  // Iterate forward through the range we created: should be one of
  // each.
  iter.first ();

  for (i = -19; i <= 19; ++i)
    {
      // we should still be in the set
      ACE_TEST_ASSERT (iter.done () == 0);

      // make sure the current element is what we expect
      iter.next (ptr);
      ACE_TEST_ASSERT (ptr != 0);
      ACE_TEST_ASSERT (*ptr == i);

      // move to the next element in the set
      iter.advance ();
    }

  // We should have iterated through the entire set.
  ACE_TEST_ASSERT (iter.done () != 0);

  // Iterate backward through the range we created: should be one of
  // each.
  iter.last ();

  for (i = 19; i >= -19; --i)
    {
      // We should still be in the set.
      ACE_TEST_ASSERT (iter.done () == 0);

      // Make sure the current element is what we expect.
      int *ptr = 0;
      iter.next (ptr);
      ACE_TEST_ASSERT (ptr != 0);
      ACE_TEST_ASSERT (*ptr == i);

      // Move to the previous element in the set.
      iter.retreat ();
    }

  // We should have iterated through the entire set.
  ACE_TEST_ASSERT (iter.done () != 0);

  // Iterate through the set and use the operator* to get the element
  iter.first ();

  for (i = -19; i <= 19; ++i)
    {
      // we should still be in the set
      ACE_TEST_ASSERT (iter.done () == 0);

      // make sure the current element is what we expect
      int& l = *iter;
      ACE_TEST_ASSERT (l == i);

      // move to the next element in the set
      iter.advance ();
    }

  // We should have iterated through the entire set.
  ACE_TEST_ASSERT (iter.done () != 0);

  // Clear the set, restart the iterator, and make sure the iterator
  // is out of range at both ends, the set is empty, and a subsequent
  // advance or retreat on an out of range iterator does not cause
  // problems
  set.reset ();
  ACE_TEST_ASSERT (set.is_empty () != 0);
  iter.first ();
  ACE_TEST_ASSERT (iter.done () != 0);
  iter.retreat ();
  iter.last ();
  ACE_TEST_ASSERT (iter.done () != 0);
  iter.advance ();

  // Put in a bunch of ints in various relative positions, using an
  // iterator for the odds and no iterator for the evens.
  set.insert (203, iter);
  set.insert (202);
  set.insert (204);
  set.insert (201, iter);
  set.insert (205, iter);

  set.insert (203, iter);
  set.insert (203, iter);

  set.insert (204);
  set.insert (204);
  set.insert (204);
  set.insert (205, iter);
  set.insert (205, iter);
  set.insert (205, iter);
  set.insert (205, iter);
  set.insert (202);

  // remove the middle elements
  while (set.remove (204) == 0)
    {
      // No action.
    }

  while (set.remove (202) == 0)
    {
      // No action.
    }

  while (set.remove (203) == 0)
    {
      // No action.
    }

  // Put the iterator out of range and make sure it stays
  // that way for finds on the missing elements.
  iter.last ();
  iter.advance ();
  set.find (203, iter);
  ACE_TEST_ASSERT (iter.done () != 0);
  set.find (202, iter);
  ACE_TEST_ASSERT (iter.done () != 0);
  set.find (204, iter);
  ACE_TEST_ASSERT (iter.done () != 0);

  // Make sure the other elements can be found.
  set.find (205, iter);
  ACE_TEST_ASSERT (iter.done () == 0);
  iter.next (ptr);
  ACE_TEST_ASSERT (ptr != 0);
  ACE_TEST_ASSERT (*ptr == 205);
  set.find (201, iter);
  ACE_TEST_ASSERT (iter.done () == 0);
  iter.next (ptr);
  ACE_TEST_ASSERT (ptr != 0);
  ACE_TEST_ASSERT (*ptr == 201);

  // Finally, iterate through the set and make sure its contents are
  // correct (one 201 and five 205s).
  iter.first ();
  ACE_TEST_ASSERT (iter.done () == 0);
  iter.next (ptr);
  ACE_TEST_ASSERT (ptr != 0);
  ACE_TEST_ASSERT (*ptr == 201);
  iter.advance ();

  for (i = 1; i <= 5; ++i)
    {
      // Should be in the set, able to access the element, value
      // should be 205
      ACE_TEST_ASSERT (iter.done () == 0);
      iter.next (ptr);
      ACE_TEST_ASSERT (ptr != 0);
      ACE_TEST_ASSERT (*ptr == 205);

      // Move to the next element in the set.
      iter.advance ();
    }

  // Should not be anything else in the set.
  ACE_TEST_ASSERT (iter.done () != 0);

  // remove the rest
  while (set.remove (205) == 0)
    {
      // No action.
    }

  while (set.remove (201) == 0)
    {
      // No action.
    }

  // Should have no more elements in the set.
  ACE_TEST_ASSERT (set.is_empty () != 0);
  ACE_TEST_ASSERT (set.size () == 0);
  iter.first ();
  ACE_TEST_ASSERT (iter.done () != 0);
  iter.last ();
  ACE_TEST_ASSERT (iter.done () != 0);

  ACE_END_TEST;

  return ret;
}

