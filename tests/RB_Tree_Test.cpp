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
//    This is a test to verify and illustrate the use of the ACE_RB_Tree
//    and ACE_RB_Tree_Iterator classes.  Two different key and item types are
//    used in order to demonstrate specialization of the ACE_Less_Than
//    comparison function object template: int (for which the native <
//    operator is sufficient), and char * (for which < operator semantics must
//    be replaced by strcmp semantics).  An RB tree for each of the four
//    possible type parameter permutations over int and char * is constructed
//    and filled in, and the resulting order is checked via an iterator over
//    each.
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



// These arrays of numbers as ints and character strings
// are used to instantiate key and item nodes in the tree.
static char *number_strings [] = {"10", "20", "30", "40",
                                  "50", "60", "70", "80"};

static int number_integers [] = {10, 20, 30, 40,
                                 50, 60, 70, 80};

// These arrays of ints are used to shuffle the order of insertion
// of keys and items for the various trees.
static int int_int_index [] = {0, 1, 2, 3, 4, 5, 6, 7}; // LR inorder
static int int_str_index [] = {7, 6, 5, 4, 3, 2, 1, 0}; // RL inorder
static int str_int_index [] = {4, 6, 2, 7, 5, 3, 1, 0}; // RL BFS
static int str_str_index [] = {4, 2, 1, 0, 3, 6, 5, 7}; // LR preorder

// Number of entries placed in each tree.
static int RB_TREE_TEST_ENTRIES = 8;

int
main (int, ASYS_TCHAR *[])
{
  ACE_START_TEST (ASYS_TEXT ("RB_Tree_Test"));

  // Local variables used to index arrays.
  int i, k;

  // Construct four RB_Trees.  Specialization of the ACE_Less_Than template
  // for character strings performs strcmp style string comparisons rather
  // than < operator comparison of the pointers themselves.
  ACE_RB_Tree<int, int, ACE_Less_Than<int>, ACE_Null_Mutex> int_int_tree;
  ACE_RB_Tree<int, char *, ACE_Less_Than<int>, ACE_Null_Mutex> int_str_tree;
  ACE_RB_Tree<char *, int, ACE_Less_Than<char *>, ACE_Null_Mutex> str_int_tree;
  ACE_RB_Tree<char *, char *, ACE_Less_Than<char *>, ACE_Null_Mutex> str_str_tree;

  // Fill in each tree with the key and item from the appropriate arrays,
  // using the shuffle indexes to create different insertion orders.
  for (i = 0; i < RB_TREE_TEST_ENTRIES; ++i)
    {
      k = int_int_index [i];
      ACE_ASSERT ((k >= 0) && (k < RB_TREE_TEST_ENTRIES));
      int_int_tree.insert (number_integers [k], number_integers [k]);
      ACE_ASSERT ((int_int_tree.find (number_integers [k]) != 0) &&
                  (*int_int_tree.find (number_integers [k]) ==
                   number_integers [k]));

      k = int_str_index [i];
      ACE_ASSERT ((k >= 0) && (k < RB_TREE_TEST_ENTRIES));
      int_str_tree.insert (number_integers [k], number_strings [k]);
      ACE_ASSERT ((int_str_tree.find (number_integers [k]) != 0) &&
                  (*int_str_tree.find (number_integers [k]) ==
                   number_strings [k]));

      k = str_int_index [i];
      ACE_ASSERT ((k >= 0) && (k < RB_TREE_TEST_ENTRIES));
      str_int_tree.insert (number_strings [k], number_integers [k]);
      ACE_ASSERT ((str_int_tree.find (number_strings [k]) != 0) &&
                  (*str_int_tree.find (number_strings [k]) ==
                   number_integers [k]));

      k = str_str_index [i];
      ACE_ASSERT ((k >= 0) && (k < RB_TREE_TEST_ENTRIES));
      str_str_tree.insert (number_strings [k], number_strings [k]);
      ACE_ASSERT ((str_str_tree.find (number_strings [k]) != 0) &&
                  (*str_str_tree.find (number_strings [k]) ==
                   number_strings [k]));
    }

  // Construct an iterator for each of the trees.
  ACE_RB_Tree_Iterator<int, int, ACE_Less_Than<int>, ACE_Null_Mutex> int_int_iter (int_int_tree);
  ACE_RB_Tree_Iterator<int, char *, ACE_Less_Than<int>, ACE_Null_Mutex> int_str_iter (int_str_tree);
  ACE_RB_Tree_Iterator<char *, int, ACE_Less_Than<char *>, ACE_Null_Mutex> str_int_iter (str_int_tree);
  ACE_RB_Tree_Iterator<char *, char *, ACE_Less_Than<char *>, ACE_Null_Mutex> str_str_iter (str_str_tree);

  // Iterate over each of the trees, making sure their entries
  // are in the same relative order (i.e., the integers and strings
  // represent the same values at each respective position in the tree).
  for (i = 0; i < RB_TREE_TEST_ENTRIES; ++i)
    {
      char **str_item;
      int *int_item;

      int_item = int_int_iter.item ();
      ACE_ASSERT (int_item != 0);
      ACE_ASSERT (*int_item == number_integers [i]);

      int_item = str_int_iter.item ();
      ACE_ASSERT (int_item != 0);
      ACE_ASSERT (*int_item == number_integers [i]);

      str_item = int_str_iter.item ();
      ACE_ASSERT (str_item != 0);
      ACE_ASSERT (*str_item == number_strings [i]);

      str_item = str_str_iter.item ();
      ACE_ASSERT (str_item != 0);
      ACE_ASSERT (*str_item == number_strings [i]);

      // Advance each iterator.
      int_int_iter.next ();
      int_str_iter.next ();
      str_int_iter.next ();
      str_str_iter.next ();
    }

  // Make sure each item in each tree has been visited
  ACE_ASSERT (int_int_iter.is_done () == 1);
  ACE_ASSERT (int_str_iter.is_done () == 1);
  ACE_ASSERT (str_int_iter.is_done () == 1);
  ACE_ASSERT (str_str_iter.is_done () == 1);

  // Remove the even numbered entries from each of the trees.
  for (i = 0; i < RB_TREE_TEST_ENTRIES; i += 2)
    {
      ACE_ASSERT (int_int_tree.remove (number_integers [i]) == 1);
      ACE_ASSERT (int_str_tree.remove (number_integers [i]) == 1);
      ACE_ASSERT (str_int_tree.remove (number_strings [i]) == 1);
      ACE_ASSERT (str_str_tree.remove (number_strings [i]) == 1);
    }

  // Reset iterators
  int_int_iter.first ();
  int_str_iter.first ();
  str_int_iter.first ();
  str_str_iter.first ();

  // Iterate over each of the trees, making sure their entries are
  // still in the same relative order (i.e., the integers and strings
  // represent the same values at each respective position in the tree).
  for (i = 1; i < RB_TREE_TEST_ENTRIES; i += 2)
    {
      char **str_item;
      int *int_item;

      int_item = int_int_iter.item ();
      ACE_ASSERT (int_item != 0);
      ACE_ASSERT (*int_item == number_integers [i]);

      int_item = str_int_iter.item ();
      ACE_ASSERT (int_item != 0);
      ACE_ASSERT (*int_item == number_integers [i]);

      str_item = int_str_iter.item ();
      ACE_ASSERT (str_item != 0);
      ACE_ASSERT (*str_item == number_strings [i]);

      str_item = str_str_iter.item ();
      ACE_ASSERT (str_item != 0);
      ACE_ASSERT (*str_item == number_strings [i]);

      // Advance each iterator.
      int_int_iter.next ();
      int_str_iter.next ();
      str_int_iter.next ();
      str_str_iter.next ();
    }

  // Make sure each item in each tree has been visited a second time.
  ACE_ASSERT (int_int_iter.is_done () == 1);
  ACE_ASSERT (int_str_iter.is_done () == 1);
  ACE_ASSERT (str_int_iter.is_done () == 1);
  ACE_ASSERT (str_str_iter.is_done () == 1);

  ACE_END_TEST;
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_RB_Tree<int, int, ACE_Less_Than<int>, ACE_Null_Mutex>;
template class ACE_RB_Tree_Node<int, int>;
template class ACE_RB_Tree_Iterator_Base<int, int, ACE_Less_Than<int>, ACE_Null_Mutex>;
template class ACE_RB_Tree_Iterator<int, int, ACE_Less_Than<int>, ACE_Null_Mutex>;
template class ACE_RB_Tree<int, char *, ACE_Less_Than<int>, ACE_Null_Mutex>;
template class ACE_RB_Tree_Node<int, char *>;
template class ACE_RB_Tree_Iterator_Base<int, char *, ACE_Less_Than<int>, ACE_Null_Mutex>;
template class ACE_RB_Tree_Iterator<int, char *, ACE_Less_Than<int>, ACE_Null_Mutex>;
template class ACE_RB_Tree<char *, int, ACE_Less_Than<char *>, ACE_Null_Mutex>;
template class ACE_RB_Tree_Node<char *, int>;
template class ACE_RB_Tree_Iterator_Base<char *, int, ACE_Less_Than<char *>, ACE_Null_Mutex>;
template class ACE_RB_Tree_Iterator<char *, int, ACE_Less_Than<char *>, ACE_Null_Mutex>;
template class ACE_RB_Tree<char *, char *, ACE_Less_Than<char *>, ACE_Null_Mutex>;
template class ACE_RB_Tree_Node<char *, char *>;
template class ACE_RB_Tree_Iterator_Base<char *, char *, ACE_Less_Than<char *>, ACE_Null_Mutex>;
template class ACE_RB_Tree_Iterator<char *, char *, ACE_Less_Than<char *>, ACE_Null_Mutex>;
template class ACE_Less_Than<int>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_RB_Tree<int, int, ACE_Less_Than<int>, ACE_Null_Mutex>
#pragma instantiate ACE_RB_Tree_Node<int, int>
#pragma instantiate ACE_RB_Tree_Iterator_Base<int, int, ACE_Less_Than<int>, ACE_Null_Mutex>
#pragma instantiate ACE_RB_Tree_Iterator<int, int, ACE_Less_Than<int>, ACE_Null_Mutex>
#pragma instantiate ACE_RB_Tree<int, char *, ACE_Less_Than<int>, ACE_Null_Mutex>
#pragma instantiate ACE_RB_Tree_Node<int, char *>
#pragma instantiate ACE_RB_Tree_Iterator_Base<int, char *, ACE_Less_Than<int>, ACE_Null_Mutex>
#pragma instantiate ACE_RB_Tree_Iterator<int, char *, ACE_Less_Than<int>, ACE_Null_Mutex>
#pragma instantiate ACE_RB_Tree<char *, int, ACE_Less_Than<char *>, ACE_Null_Mutex>
#pragma instantiate ACE_RB_Tree_Node<char *, int>
#pragma instantiate ACE_RB_Tree_Iterator_Base<char *, int, ACE_Less_Than<char *>, ACE_Null_Mutex>
#pragma instantiate ACE_RB_Tree_Iterator<char *, int, ACE_Less_Than<char *>, ACE_Null_Mutex>
#pragma instantiate ACE_RB_Tree<char *, char *, ACE_Less_Than<char *>, ACE_Null_Mutex>
#pragma instantiate ACE_RB_Tree_Node<char *, char *>
#pragma instantiate ACE_RB_Tree_Iterator_Base<char *, char *, ACE_Less_Than<char *>, ACE_Null_Mutex>
#pragma instantiate ACE_RB_Tree_Iterator<char *, char *, ACE_Less_Than<char *>, ACE_Null_Mutex>
#pragma instantiate ACE_Less_Than<int>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
