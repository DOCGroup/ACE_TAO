// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    RB_Tree_Test.cpp
//
// = DESCRIPTION
//    This is a test to verify and illustrate the use of the
//    <ACE_RB_Tree ACE_RB_Tree_Iterator> and
//    <ACE_RB_Tree_Reverse_Iterator> classes.  Two different key and
//    item types are used in order to demonstrate specialization of
//    the <ACE_Less_Than> comparison function object template: int
//    (for which the native < operator is sufficient), and const char
//    * (for which < operator semantics must be replaced by strcmp
//    semantics).  An RB tree for each of the four possible type
//    parameter permutations over int and const char * is constructed
//    and filled in, and the resulting order is checked via an
//    iterator over each.
//
// = AUTHOR
//    Chris Gill <cdgill@cs.wustl.edu>
//
// ============================================================================

#include "test_config.h" /* Include first to enable ACE_ASSERT. */
#include "ace/RB_Tree.h"
#include "ace/SString.h"

#include "RB_Tree_Test.h"

ACE_RCSID(tests, RB_Tree_Test, "$Id$")

// Type definitions for the four distinct parameterizations of the
// test.

typedef ACE_RB_Tree_Test<int, int, ACE_Less_Than<int>, ACE_Null_Mutex> INT_INT_RB_TREE_TEST;
typedef ACE_RB_Tree_Test<int, const char *, ACE_Less_Than<int>, ACE_Null_Mutex> INT_STR_RB_TREE_TEST;
typedef ACE_RB_Tree_Test<const char *, int, ACE_Less_Than<const char *>, ACE_Null_Mutex> STR_INT_RB_TREE_TEST;
typedef ACE_RB_Tree_Test<const char *, const char *, ACE_Less_Than<const char *>, ACE_Null_Mutex> STR_STR_RB_TREE_TEST;

// Number of entries placed in each tree.
static int RB_TREE_TEST_ENTRIES = 8;

// These arrays of numbers as ints and character strings are used to
// instantiate key and item arrays in the tests.
static const char *number_strings [] =
{
  "10", "20", "30", "40", "50", "60", "70", "80"
};

static int number_integers [] =
{
  10, 20, 30, 40, 50, 60, 70, 80
};

// These arrays of ints are used to shuffle the order of insertion and
// deletaion of keys and items for the various tests.
static int int_int_index [] = {0, 1, 2, 3, 4, 5, 6, 7}; // LR inorder
static int int_str_index [] = {7, 6, 5, 4, 3, 2, 1, 0}; // RL inorder
static int str_int_index [] = {4, 6, 2, 7, 5, 3, 1, 0}; // RL BFS
static int str_str_index [] = {4, 2, 1, 0, 3, 6, 5, 7}; // LR preorder


int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("RB_Tree_Test"));

  // Construct and run four distinctly parameterized tests.  Note that
  // specialization of the ACE_Less_Than template for character
  // strings performs strcmp style string comparisons rather than <
  // operator comparison of the const char * pointers themselves.

  INT_INT_RB_TREE_TEST int_int_test (RB_TREE_TEST_ENTRIES,
                                     number_integers,
                                     number_integers,
                                     int_int_index);
  INT_STR_RB_TREE_TEST int_str_test (RB_TREE_TEST_ENTRIES,
                                     number_integers,
                                     number_strings,
                                     int_str_index);
  STR_INT_RB_TREE_TEST str_int_test (RB_TREE_TEST_ENTRIES,
                                     number_strings,
                                     number_integers,
                                     str_int_index);
  STR_STR_RB_TREE_TEST str_str_test (RB_TREE_TEST_ENTRIES,
                                     number_strings,
                                     number_strings,
                                     str_str_index);
  int_int_test.run_test ();
  int_str_test.run_test ();
  str_int_test.run_test ();
  str_str_test.run_test ();

// ======= Stress Test contributed by Klaus H. Wolf <hw@cyland.com> ==================

  ACE_RB_Tree<ACE_CString, int, ACE_Less_Than<ACE_CString>, ACE_Null_Mutex> tree;

  tree.bind (ACE_CString ("51"),  1);
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.bind (ACE_CString ("13"),  2);
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.bind (ACE_CString ("36"),  3);
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.bind (ACE_CString ("15"),  4);
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.bind (ACE_CString ("22"),  5);
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.bind (ACE_CString ("25"),  6);
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.bind (ACE_CString ("42"),  7);
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.bind (ACE_CString ("48"),  8);
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.bind (ACE_CString ("03"),  9);
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.bind (ACE_CString ("56"), 10);
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.bind (ACE_CString ("28"), 11);
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.bind (ACE_CString ("55"), 12);
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.bind (ACE_CString ("21"), 13);
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.bind (ACE_CString ("62"), 14);
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.bind (ACE_CString ("18"), 15);
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.bind (ACE_CString ("20"), 16);
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.bind (ACE_CString ("26"), 17);
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.bind (ACE_CString ("29"), 18);
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.bind (ACE_CString ("50"), 19);
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.bind (ACE_CString ("05"), 20);
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.bind (ACE_CString ("59"), 21);
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.bind (ACE_CString ("65"), 22);
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.bind (ACE_CString ("66"), 23);
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.bind (ACE_CString ("45"), 24);
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.bind (ACE_CString ("34"), 25);
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.bind (ACE_CString ("27"), 26);
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.bind (ACE_CString ("40"), 27);
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.bind (ACE_CString ("30"), 28);
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.bind (ACE_CString ("64"), 29);
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.bind (ACE_CString ("11"), 30);
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.bind (ACE_CString ("16"), 31);
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.bind (ACE_CString ("47"), 32);
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.bind (ACE_CString ("10"), 33);
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.bind (ACE_CString ("37"), 34);
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.bind (ACE_CString ("09"), 35);
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.bind (ACE_CString ("54"), 36);
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.bind (ACE_CString ("23"), 37);
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.bind (ACE_CString ("44"), 38);
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.bind (ACE_CString ("19"), 39);
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.bind (ACE_CString ("00"), 40);
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.bind (ACE_CString ("04"), 41);
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.bind (ACE_CString ("63"), 42);
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.bind (ACE_CString ("08"), 43);
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.bind (ACE_CString ("39"), 44);
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.bind (ACE_CString ("31"), 45);
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.bind (ACE_CString ("02"), 46);
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.bind (ACE_CString ("33"), 47);
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.bind (ACE_CString ("60"), 48);
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.bind (ACE_CString ("61"), 49);
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.bind (ACE_CString ("57"), 50);
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.bind (ACE_CString ("43"), 51);
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.bind (ACE_CString ("46"), 52);
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.bind (ACE_CString ("38"), 53);
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.bind (ACE_CString ("01"), 54);
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.bind (ACE_CString ("12"), 55);
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.bind (ACE_CString ("24"), 56);
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.bind (ACE_CString ("52"), 57);
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.bind (ACE_CString ("07"), 58);
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.bind (ACE_CString ("14"), 59);
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.bind (ACE_CString ("06"), 60);
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.bind (ACE_CString ("58"), 61);
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.bind (ACE_CString ("49"), 62);
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.bind (ACE_CString ("17"), 63);
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.bind (ACE_CString ("53"), 64);
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.bind (ACE_CString ("32"), 65);
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.bind (ACE_CString ("35"), 66);
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.bind (ACE_CString ("41"), 67);
  ACE_ASSERT (tree.test_invariant () == 0);

  tree.unbind (ACE_CString ("51"));
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.unbind (ACE_CString ("13"));
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.unbind (ACE_CString ("36"));
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.unbind (ACE_CString ("15"));
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.unbind (ACE_CString ("22"));
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.unbind (ACE_CString ("25"));
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.unbind (ACE_CString ("42"));
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.unbind (ACE_CString ("48"));
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.unbind (ACE_CString ("03"));
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.unbind (ACE_CString ("56"));
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.unbind (ACE_CString ("28"));
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.unbind (ACE_CString ("55"));
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.unbind (ACE_CString ("21"));
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.unbind (ACE_CString ("62"));
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.unbind (ACE_CString ("18"));
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.unbind (ACE_CString ("20"));
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.unbind (ACE_CString ("26"));
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.unbind (ACE_CString ("29"));
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.unbind (ACE_CString ("50"));
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.unbind (ACE_CString ("05"));
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.unbind (ACE_CString ("59"));
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.unbind (ACE_CString ("65"));
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.unbind (ACE_CString ("66"));
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.unbind (ACE_CString ("45"));
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.unbind (ACE_CString ("34"));
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.unbind (ACE_CString ("27"));
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.unbind (ACE_CString ("40"));
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.unbind (ACE_CString ("30"));
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.unbind (ACE_CString ("64"));
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.unbind (ACE_CString ("11"));
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.unbind (ACE_CString ("16"));
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.unbind (ACE_CString ("47"));
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.unbind (ACE_CString ("10"));
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.unbind (ACE_CString ("37"));
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.unbind (ACE_CString ("09"));
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.unbind (ACE_CString ("54"));
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.unbind (ACE_CString ("23"));
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.unbind (ACE_CString ("44"));
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.unbind (ACE_CString ("19"));
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.unbind (ACE_CString ("00"));
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.unbind (ACE_CString ("04"));
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.unbind (ACE_CString ("63"));
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.unbind (ACE_CString ("08"));
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.unbind (ACE_CString ("39"));
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.unbind (ACE_CString ("31"));
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.unbind (ACE_CString ("02"));
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.unbind (ACE_CString ("33"));
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.unbind (ACE_CString ("60"));
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.unbind (ACE_CString ("61"));
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.unbind (ACE_CString ("57"));
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.unbind (ACE_CString ("43"));
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.unbind (ACE_CString ("46"));
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.unbind (ACE_CString ("38"));
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.unbind (ACE_CString ("01"));
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.unbind (ACE_CString ("12"));
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.unbind (ACE_CString ("24"));
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.unbind (ACE_CString ("52"));
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.unbind (ACE_CString ("07"));
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.unbind (ACE_CString ("14"));
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.unbind (ACE_CString ("06"));
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.unbind (ACE_CString ("58"));
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.unbind (ACE_CString ("49"));
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.unbind (ACE_CString ("17"));
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.unbind (ACE_CString ("53"));
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.unbind (ACE_CString ("32"));
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.unbind (ACE_CString ("35"));
  ACE_ASSERT (tree.test_invariant () == 0);
  tree.unbind (ACE_CString ("41"));
  ACE_ASSERT (tree.test_invariant () == 0);

// ======== End Stress Test ===================

  ACE_END_TEST;
  return 0;
}

// Constructor.

template <class EXT_ID, class INT_ID, class COMPARE_KEYS, class ACE_LOCK>
ACE_RB_Tree_Test<EXT_ID, INT_ID, COMPARE_KEYS, ACE_LOCK>::ACE_RB_Tree_Test
  (int entry_count,
   EXT_ID key_array [],
   INT_ID item_array [],
   int order_index [])
  : stable_fwd_iter_ (stable_tree_),
    stable_rev_iter_ (stable_tree_),
    deprecated_fwd_iter_ (deprecated_tree_),
    deprecated_rev_iter_ (deprecated_tree_),
    entry_count_ (entry_count),
    key_array_ (key_array),
    item_array_ (item_array),
    order_index_ (order_index)
{
}

// Destructor.

template <class EXT_ID, class INT_ID, class COMPARE_KEYS, class ACE_LOCK>
ACE_RB_Tree_Test<EXT_ID, INT_ID, COMPARE_KEYS, ACE_LOCK>::~ACE_RB_Tree_Test (void)
{
}

// Run the interface and iteration tests.

template <class EXT_ID, class INT_ID, class COMPARE_KEYS, class ACE_LOCK> void
ACE_RB_Tree_Test<EXT_ID, INT_ID, COMPARE_KEYS, ACE_LOCK>::run_test (void)
{
  // Run the individual portions of the test, in order.

  test_tree_insertion ();
  test_post_insertion_iteration ();
  test_tree_deletion ();
  test_post_deletion_iteration ();
}


// Tests stable and deprecated insertion interfaces.

template <class EXT_ID, class INT_ID, class COMPARE_KEYS, class ACE_LOCK> void
ACE_RB_Tree_Test<EXT_ID, INT_ID, COMPARE_KEYS, ACE_LOCK>::test_tree_insertion (void)
{
  // Fill in each tree with the key and item from the appropriate
  // arrays, using the shuffle index to create the appropriate
  // insertion orders.  Then, make sure the inserted item can be found
  // using the insertion key.

  for (int i = 0; i < entry_count_; ++i)
    {
      INT_ID item;
      int k = order_index_ [i];
      ACE_ASSERT (k >= 0 && k < entry_count_);

      // Test the new stable ACE_Hash_Map_Manager_Ex compliant interface.
      ACE_ASSERT (stable_tree_.bind (key_array_ [k],
                                     item_array_ [k]) == 0);
      ACE_ASSERT (stable_tree_.find (key_array_ [k], item) == 0
                  && item == item_array_ [k]);

      // Test the deprecated interface.
      ACE_ASSERT (deprecated_tree_.insert (key_array_ [k],
                                           item_array_ [k]) != 0);
      ACE_ASSERT (deprecated_tree_.find (key_array_ [k]) != 0
                  && *deprecated_tree_.find (key_array_ [k]) ==
                     item_array_ [k]);
    }

}

// Tests forward and reverse iteration after insertion in both trees.

template <class EXT_ID, class INT_ID, class COMPARE_KEYS, class ACE_LOCK> void
ACE_RB_Tree_Test<EXT_ID, INT_ID, COMPARE_KEYS, ACE_LOCK>::test_post_insertion_iteration (void)
{
  // Reset iterators.

  stable_fwd_iter_ = stable_tree_.begin ();
  stable_rev_iter_ = stable_tree_.rbegin ();
  deprecated_fwd_iter_ = deprecated_tree_.begin ();
  deprecated_rev_iter_ = deprecated_tree_.rbegin ();

  // Iterate over each of the trees, making sure their entries are in
  // the same relative order (i.e., the integers and strings represent
  // the same values at each respective position in the tree).
  for (int i = 0; i < entry_count_; ++i)
    {
      INT_ID item;

      item = (*stable_fwd_iter_).item ();
      ACE_ASSERT (item == item_array_ [i]);

      item = (*stable_rev_iter_).item ();
      ACE_ASSERT (item == item_array_ [entry_count_ - i - 1]);

      item = (*deprecated_fwd_iter_).item ();
      ACE_ASSERT (item == item_array_ [i]);

      item = (*deprecated_rev_iter_).item ();
      ACE_ASSERT (item == item_array_ [entry_count_ - i - 1]);

      // Advance each iterator.
      ++stable_fwd_iter_;
      ++stable_rev_iter_;
      ++deprecated_fwd_iter_;
      ++deprecated_rev_iter_;
    }

  // Advance each iterator again - should be a no-op.
  ++stable_fwd_iter_;
  ++stable_rev_iter_;
  ++deprecated_fwd_iter_;
  ++deprecated_rev_iter_;


  // Make sure each item in each tree has been visited
  ACE_ASSERT (stable_fwd_iter_.done () == 1);
  ACE_ASSERT (stable_rev_iter_.done () == 1);
  ACE_ASSERT (deprecated_fwd_iter_.done () == 1);
  ACE_ASSERT (deprecated_rev_iter_.done () == 1);
}

// Tests stable and deprecated deletion interfaces.

template <class EXT_ID, class INT_ID, class COMPARE_KEYS, class ACE_LOCK> void
ACE_RB_Tree_Test<EXT_ID, INT_ID, COMPARE_KEYS, ACE_LOCK>::test_tree_deletion (void)
{
  // Remove the even numbered entries from each of the trees.

  for (int i = 0; i < entry_count_; i += 2)
    {
      // Test the new stable ACE_Hash_Map_Manager_Ex compliant
      // interface.
      ACE_ASSERT (stable_tree_.unbind (key_array_ [i]) == 0);

      // Test the deprecated interface.
      ACE_ASSERT (deprecated_tree_.remove (key_array_ [i]) == 1);
    }
}

// Tests forward and reverse iteration after deletions in both trees.

template <class EXT_ID, class INT_ID, class COMPARE_KEYS, class ACE_LOCK> void
ACE_RB_Tree_Test<EXT_ID, INT_ID, COMPARE_KEYS, ACE_LOCK>::test_post_deletion_iteration (void)
{
  // Reset iterators

  stable_fwd_iter_ = stable_tree_.begin ();
  stable_rev_iter_ = stable_tree_.rbegin ();
  deprecated_fwd_iter_ = deprecated_tree_.begin ();
  deprecated_rev_iter_ = deprecated_tree_.rbegin ();

  // Iterate over each of the trees, making sure their entries are
  // still in the same relative order (i.e., the integers and strings
  // represent the same values at each respective position in the
  // tree).
  for (int i = 1; i < entry_count_; i += 2)
    {
      INT_ID item;

      item = (*stable_fwd_iter_).item ();
      ACE_ASSERT (item == item_array_ [i]);

      item = (*stable_rev_iter_).item ();
      ACE_ASSERT (item == item_array_ [entry_count_ - i]);

      item = (*deprecated_fwd_iter_).item ();
      ACE_ASSERT (item == item_array_ [i]);

      item = (*deprecated_rev_iter_).item ();
      ACE_ASSERT (item == item_array_ [entry_count_ - i]);

      // Advance each iterator via postfix increment.
      stable_fwd_iter_++;
      stable_rev_iter_++;
      deprecated_fwd_iter_++;
      deprecated_rev_iter_++;
    }

  // Make sure each item in each tree has been visited a second time.
  ACE_ASSERT (stable_fwd_iter_.done () == 1);
  ACE_ASSERT (stable_rev_iter_.done () == 1);
  ACE_ASSERT (deprecated_fwd_iter_.done () == 1);
  ACE_ASSERT (deprecated_rev_iter_.done () == 1);
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_RB_Tree<ACE_CString, int, ACE_Less_Than<ACE_CString>, ACE_Null_Mutex>;
template class ACE_RB_Tree_Node<ACE_CString, int>;
template class ACE_RB_Tree_Iterator_Base<ACE_CString, int, ACE_Less_Than<ACE_CString>, ACE_Null_Mutex>;
template class ACE_RB_Tree_Iterator<ACE_CString, int, ACE_Less_Than<ACE_CString>, ACE_Null_Mutex>;
template class ACE_RB_Tree_Reverse_Iterator<ACE_CString, int, ACE_Less_Than<ACE_CString>, ACE_Null_Mutex>;
template class ACE_RB_Tree_Test<int, int, ACE_Less_Than<int>, ACE_Null_Mutex>;
template class ACE_RB_Tree<int, int, ACE_Less_Than<int>, ACE_Null_Mutex>;
template class ACE_RB_Tree_Node<int, int>;
template class ACE_RB_Tree_Iterator_Base<int, int, ACE_Less_Than<int>, ACE_Null_Mutex>;
template class ACE_RB_Tree_Iterator<int, int, ACE_Less_Than<int>, ACE_Null_Mutex>;
template class ACE_RB_Tree_Reverse_Iterator<int, int, ACE_Less_Than<int>, ACE_Null_Mutex>;
template class ACE_RB_Tree_Test<int, const char *, ACE_Less_Than<int>, ACE_Null_Mutex>;
template class ACE_RB_Tree<int, const char *, ACE_Less_Than<int>, ACE_Null_Mutex>;
template class ACE_RB_Tree_Node<int, const char *>;
template class ACE_RB_Tree_Iterator_Base<int, const char *, ACE_Less_Than<int>, ACE_Null_Mutex>;
template class ACE_RB_Tree_Iterator<int, const char *, ACE_Less_Than<int>, ACE_Null_Mutex>;
template class ACE_RB_Tree_Reverse_Iterator<int, const char *, ACE_Less_Than<int>, ACE_Null_Mutex>;
template class ACE_RB_Tree_Test<const char *, int, ACE_Less_Than<const char *>, ACE_Null_Mutex>;
template class ACE_RB_Tree<const char *, int, ACE_Less_Than<const char *>, ACE_Null_Mutex>;
template class ACE_RB_Tree_Node<const char *, int>;
template class ACE_RB_Tree_Iterator_Base<const char *, int, ACE_Less_Than<const char *>, ACE_Null_Mutex>;
template class ACE_RB_Tree_Iterator<const char *, int, ACE_Less_Than<const char *>, ACE_Null_Mutex>;
template class ACE_RB_Tree_Reverse_Iterator<const char *, int, ACE_Less_Than<const char *>, ACE_Null_Mutex>;
template class ACE_RB_Tree_Test<const char *, const char *, ACE_Less_Than<const char *>, ACE_Null_Mutex>;
template class ACE_RB_Tree<const char *, const char *, ACE_Less_Than<const char *>, ACE_Null_Mutex>;
template class ACE_RB_Tree_Node<const char *, const char *>;
template class ACE_RB_Tree_Iterator_Base<const char *, const char *, ACE_Less_Than<const char *>, ACE_Null_Mutex>;
template class ACE_RB_Tree_Iterator<const char *, const char *, ACE_Less_Than<const char *>, ACE_Null_Mutex>;
template class ACE_RB_Tree_Reverse_Iterator<const char *, const char *, ACE_Less_Than<const char *>, ACE_Null_Mutex>;
template class ACE_Less_Than<int>;
template class ACE_Less_Than<ACE_CString>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_RB_Tree<ACE_CString, int, ACE_Less_Than<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_RB_Tree_Node<ACE_CString, int>
#pragma instantiate ACE_RB_Tree_Iterator_Base<ACE_CString, int, ACE_Less_Than<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_RB_Tree_Iterator<ACE_CString, int, ACE_Less_Than<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_RB_Tree_Reverse_Iterator<ACE_CString, int, ACE_Less_Than<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_RB_Tree_Test<int, int, ACE_Less_Than<int>, ACE_Null_Mutex>
#pragma instantiate ACE_RB_Tree<int, int, ACE_Less_Than<int>, ACE_Null_Mutex>
#pragma instantiate ACE_RB_Tree_Node<int, int>
#pragma instantiate ACE_RB_Tree_Iterator_Base<int, int, ACE_Less_Than<int>, ACE_Null_Mutex>
#pragma instantiate ACE_RB_Tree_Iterator<int, int, ACE_Less_Than<int>, ACE_Null_Mutex>
#pragma instantiate ACE_RB_Tree_Reverse_Iterator<int, int, ACE_Less_Than<int>, ACE_Null_Mutex>
#pragma instantiate ACE_RB_Tree_Test<int, const char *, ACE_Less_Than<int>, ACE_Null_Mutex>
#pragma instantiate ACE_RB_Tree<int, const char *, ACE_Less_Than<int>, ACE_Null_Mutex>
#pragma instantiate ACE_RB_Tree_Node<int, const char *>
#pragma instantiate ACE_RB_Tree_Iterator_Base<int, const char *, ACE_Less_Than<int>, ACE_Null_Mutex>
#pragma instantiate ACE_RB_Tree_Iterator<int, const char *, ACE_Less_Than<int>, ACE_Null_Mutex>
#pragma instantiate ACE_RB_Tree_Reverse_Iterator<int, const char *, ACE_Less_Than<int>, ACE_Null_Mutex>
#pragma instantiate ACE_RB_Tree_Test<const char *, int, ACE_Less_Than<const char *>, ACE_Null_Mutex>
#pragma instantiate ACE_RB_Tree<const char *, int, ACE_Less_Than<const char *>, ACE_Null_Mutex>
#pragma instantiate ACE_RB_Tree_Node<const char *, int>
#pragma instantiate ACE_RB_Tree_Iterator_Base<const char *, int, ACE_Less_Than<const char *>, ACE_Null_Mutex>
#pragma instantiate ACE_RB_Tree_Iterator<const char *, int, ACE_Less_Than<const char *>, ACE_Null_Mutex>
#pragma instantiate ACE_RB_Tree_Reverse_Iterator<const char *, int, ACE_Less_Than<const char *>, ACE_Null_Mutex>
#pragma instantiate ACE_RB_Tree_Test<const char *, const char *, ACE_Less_Than<const char *>, ACE_Null_Mutex>
#pragma instantiate ACE_RB_Tree<const char *, const char *, ACE_Less_Than<const char *>, ACE_Null_Mutex>
#pragma instantiate ACE_RB_Tree_Node<const char *, const char *>
#pragma instantiate ACE_RB_Tree_Iterator_Base<const char *, const char *, ACE_Less_Than<const char *>, ACE_Null_Mutex>
#pragma instantiate ACE_RB_Tree_Iterator<const char *, const char *, ACE_Less_Than<const char *>, ACE_Null_Mutex>
#pragma instantiate ACE_RB_Tree_Reverse_Iterator<const char *, const char *, ACE_Less_Than<const char *>, ACE_Null_Mutex>
#pragma instantiate ACE_Less_Than<int>
#pragma instantiate ACE_Less_Than<ACE_CString>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
