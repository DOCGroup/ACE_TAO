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
//    ACE_RB_Tree ACE_RB_Tree_Iterator, and
//    ACE_RB_Tree_Reverse_Iterator classes.  Two different key and
//    item types are used in order to demonstrate specialization of
//    the ACE_Less_Than comparison function object template: int (for
//    which the native < operator is sufficient), and const char *
//    (for which < operator semantics must be replaced by strcmp
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

ACE_RCSID(tests, RB_Tree_Test, "$Id$")

#if defined(__BORLANDC__) && __BORLANDC__ >= 0x0530
USELIB("..\ace\aced.lib");
//---------------------------------------------------------------------------
#endif /* defined(__BORLANDC__) && __BORLANDC__ >= 0x0530 */

template <class EXT_ID, class INT_ID, class COMPARE_KEYS, class ACE_LOCK>
class ACE_RB_Tree_Test
{
  // = TITLE
  //   Implements a templatized test class for the RB_Tree ADT and its
  //   iterators.
  //
  // = DESCRIPTION

  //   To run the test class on a particular type instantiation of the
  //   RB_Tree, simply instantiate the test class template with the
  //   same type parameters, and invoke the run_test method.
public:
  // = Traits

  typedef ACE_RB_Tree<EXT_ID, INT_ID, COMPARE_KEYS, ACE_LOCK> 
          TREE;
  typedef ACE_RB_Tree_Iterator<EXT_ID, INT_ID, COMPARE_KEYS, ACE_LOCK> 
          ITERATOR;
  typedef ACE_RB_Tree_Reverse_Iterator<EXT_ID, INT_ID, COMPARE_KEYS, ACE_LOCK> 
          REVERSE_ITERATOR;

  // = Initialization and termination methods.

  ACE_RB_Tree_Test (int entry_count,
                    EXT_ID key_array [],
                    INT_ID item_array [],
                    int order_index []);
  // Constructor.

  ~ACE_RB_Tree_Test (void);
  // Destructor.

  void run_test (void);
  // Run the individual interface and iteration tests in order.

private:

  void test_tree_insertion (void);
  // Tests stable and deprecated insertion interfaces.

  void test_post_insertion_iteration (void);
  // Tests forward and reverse iteration after insertion in both
  // trees.

  void test_tree_deletion (void);
  // Tests stable and deprecated deletion interfaces.

  void test_post_deletion_iteration (void);
  // Tests forward and reverse iteration after deletions in both
  // trees.

  TREE stable_tree_;
  // Tree for testing stable interface.

  ITERATOR stable_fwd_iter_;
  // Forward iterator for tree for testing stable interface.

  REVERSE_ITERATOR stable_rev_iter_;
  // Forward iterator for tree for testing stable interface.

  TREE deprecated_tree_;
  // Tree for testing deprecated interface.

  ITERATOR deprecated_fwd_iter_;
  // Forward iterator for tree for testing deprecated interface.

  REVERSE_ITERATOR deprecated_rev_iter_;
  // Forward iterator for tree for testing deprecated interface.

  int entry_count_;
  // Number of entries in the key, item, and index arrays.

  EXT_ID *key_array_;
  // Array of EXT_IDs (keys) with which to test.

  INT_ID *item_array_;
  // Array of INT_IDs (items) with which to test.

  int *order_index_;
  // Order of indices in the key and item arrays.

};

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
main (int, ASYS_TCHAR *[])
{
  ACE_START_TEST (ASYS_TEXT ("RB_Tree_Test"));

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

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

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

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
