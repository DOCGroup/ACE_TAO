/* -*- C++ -*- */
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
//    Header file for a test to verify and illustrate the use of the
//    ACE_RB_Tree ACE_RB_Tree_Iterator, and
//    ACE_RB_Tree_Reverse_Iterator classes.
//    iterator over each.
//
// = AUTHOR
//    Chris Gill <cdgill@cs.wustl.edu>
//
// ============================================================================

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
