// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    OrdMultiSet_Test.cpp
//
// = DESCRIPTION
//    This is a simple test of the ACE_Ordered_MultiSet and 
//    ACE_Ordered_MultiSet_Iterator class templates, instantiating them with
//    type int.  No command line arguments are needed to run the test.  
//
// = AUTHOR
//    Chris Gill
//
// ============================================================================

// Note, for this test the config.h file *must* come first!
#include "ace/inc_user_config.h"

#include "test_config.h"
#include "ace/Containers.h"

ACE_RCSID(tests, OrdMultiSet_Test, "$Id$")

#if defined(__BORLANDC__) && __BORLANDC__ >= 0x0530
USELIB("..\ace\aced.lib");
//---------------------------------------------------------------------------
#endif /* defined(__BORLANDC__) && __BORLANDC__ >= 0x0530 */

int
main (int, ASYS_TCHAR *[])
{
  int ret = 0;
  int *ptr = 0;

  ACE_START_TEST (ASYS_TEXT ("OrdMultiSet_Test"));

  // make an empty set of int and an iterator
  ACE_Ordered_MultiSet<int> set;
  ACE_Ordered_MultiSet_Iterator<int> iter(set);

  // put in a range of odd ints, without an iterator
  int i;
  for (i = -10; i < 10; ++i)
  {
    set.insert (2 * i + 1);
  }

  // put in an interleaved range of even ints, using an iterator
  for (i = -10; i <= 10; ++i)
  {
    set.insert (2 * i, iter);
  }

  // remove the first and last elements of range
  while (set.remove (-20) == 0);
  while (set.remove (20) == 0);

  // should still have 39 elements in the set
  ACE_ASSERT (set.is_empty () == 0);
  ACE_ASSERT (set.size () == 39);

  // iterate forward through the range we created: should be one of each
  iter.first ();
  for (i = -19; i <= 19; ++i)
  {
    // we should still be in the set
    ACE_ASSERT (iter.done () == 0);

    // make sure the current element is what we expect
    iter.next (ptr);
    ACE_ASSERT (ptr != 0);
    ACE_ASSERT (*ptr == i);
    
    // move to the next element in the set
    iter.advance ();
  }

  // we should have iterated through the entire set
  ACE_ASSERT (iter.done () != 0);

  // iterate backward through the range we created: should be one of each
  iter.last ();
  for (i = 19; i >= -19; --i)
  {
    // we should still be in the set
    ACE_ASSERT (iter.done () == 0);

    // make sure the current element is what we expect
    int *ptr;
    iter.next (ptr);
    ACE_ASSERT (ptr != 0);
    ACE_ASSERT (*ptr == i);
    
    // move to the previous element in the set
    iter.retreat ();
  }

  // we should have iterated through the entire set
  ACE_ASSERT (iter.done () != 0);

  // clear the set, restart the iterator, and make sure 
  // the iterator is out of range at both ends, the set is empty,
  // and a subsequent advance or retreat on an out of range iterator
  // does not cause problems 
  set.reset ();
  ACE_ASSERT (set.is_empty () != 0);
  iter.first ();
  ACE_ASSERT (iter.done () != 0);
  iter.retreat ();
  iter.last ();
  ACE_ASSERT (iter.done () != 0);
  iter.advance ();

  // put in a bunch of ints in various relative positions, 
  // using an iterator for the odds and no iterator for the evens
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
  while (set.remove (204) == 0);
  while (set.remove (202) == 0);
  while (set.remove (203) == 0);

  // put the iterator out of range and make sure it stays 
  // that way for finds on the missing elements
  iter.last ();
  iter.advance ();
  set.find (203, iter);
  ACE_ASSERT (iter.done () != 0);
  set.find (202, iter);
  ACE_ASSERT (iter.done () != 0);
  set.find (204, iter);
  ACE_ASSERT (iter.done () != 0);

  // make sure the other elements can be found
  set.find (205, iter);
  ACE_ASSERT (iter.done () == 0);  
  iter.next (ptr);
  ACE_ASSERT (ptr != 0);  
  ACE_ASSERT (*ptr == 205);  
  set.find (201, iter);
  ACE_ASSERT (iter.done () == 0);  
  iter.next (ptr);
  ACE_ASSERT (ptr != 0);  
  ACE_ASSERT (*ptr == 201);  

  // finally, iterate through the set and make sure its
  // contents are correct (one 201 and five 205s)
  iter.first ();
  ACE_ASSERT (iter.done () == 0);
  iter.next (ptr);
  ACE_ASSERT (ptr != 0);
  ACE_ASSERT (*ptr == 201);  
  iter.advance ();
  for (i = 1; i <= 5; ++i)
  {
    // should be in the set, able to access the element, value should be 205
    ACE_ASSERT (iter.done () == 0);
    iter.next (ptr);
    ACE_ASSERT (ptr != 0);
    ACE_ASSERT (*ptr == 205);  

    // move to the next element in the set
    iter.advance ();
  }

  // should not be anything else in the set
  ACE_ASSERT (iter.done () != 0);

  // remove the rest
  while (set.remove (205) == 0);
  while (set.remove (201) == 0);

  // should have no more elements in the set
  ACE_ASSERT (set.is_empty () != 0);
  ACE_ASSERT (set.size () == 0);
  iter.first ();
  ACE_ASSERT (iter.done () != 0);
  iter.last ();
  ACE_ASSERT (iter.done () != 0);


  ACE_END_TEST;

  return ret;
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Ordered_MultiSet<int>;
template class ACE_Ordered_MultiSet_Iterator<int>;
template class ACE_DNode<int>;

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Ordered_MultiSet<int>
#pragma instantiate ACE_Ordered_MultiSet_Iterator<int>
#pragma instantiate ACE_DNode<int>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
