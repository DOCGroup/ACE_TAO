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
//    and ACE_RB_Tree_Iterator classes.  Two different less than comparison 
//    function object types are demonstrated for native key types int (for 
//    which the native < operator is sufficient) and char * (for which the
//    native < operator is not sufficient).  An RB tree for each of the four
//    possible type parameter combinations over int and char * is constructed
//    and filled in, and the resulting order checked via an iterator.
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

class RB_Test_String_Less_Than_Functor : 
  public ACE_Const_Binary_Functor_Base<char *, char *>
{
  // = TITLE
  //    Defines a class template that allows us to invoke a binary less than 
  //    function over two parameterized types without knowing anything about 
  //    the function and operand objects except their types.
  //
  // = DESCRIPTION
  //    This class depends on the definition 
  //    objects of the paramterized types.  A class can invoke such 
  //    an operation without knowing anything about it, or its implementation.
  //
public:

  virtual int execute (char * const & operand1,
	               char * const & operand2)
      {return (ACE_OS::strcmp (operand1, operand2) < 0) ? 1 : 0;}
  // Invokes the function object.

  virtual ACE_Const_Binary_Functor_Base<char *, char *> * clone ()
      {return new RB_Test_String_Less_Than_Functor;}
  // Creates another object of the same type.
};



int
main (int, ASYS_TCHAR *[])
{
  ACE_START_TEST (ASYS_TEXT ("RB_Tree_Test"));

  // Local variables used to index arrays.
  int i, k;

  // Construct four RB_Trees, using the default comparison semantics for those
  // keyed with int, and using a custom comparison function object for those
  // keyed with char *.
  RB_Test_String_Less_Than_Functor strcmp_functor;
  ACE_RB_Tree<int, int> int_int_tree;
  ACE_RB_Tree<int, char *> int_str_tree;
  ACE_RB_Tree<char *, int> str_int_tree (&strcmp_functor);
  ACE_RB_Tree<char *, char *> str_str_tree (&strcmp_functor);

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
  ACE_RB_Tree_Iterator<int, int> int_int_iter (int_int_tree);
  ACE_RB_Tree_Iterator<int, char *> int_str_iter (int_str_tree);
  ACE_RB_Tree_Iterator<char *, int> str_int_iter (str_int_tree);
  ACE_RB_Tree_Iterator<char *, char *> str_str_iter (str_str_tree);

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
template class ACE_Const_Binary_Functor_Base<char *, char *>;
template class RB_Tree<int, int>;
template class RB_Tree_Node<int, int>;
template class RB_Tree_Iterator<int, int>;
template class RB_Tree<int, char *>;
template class RB_Tree_Node<int, char *>;
template class RB_Tree_Iterator<int, char *>;
template class RB_Tree<char *, int>;
template class RB_Tree_Node<char *, int>;
template class RB_Tree_Iterator<char *, int>;
template class RB_Tree<char *, char *>;
template class RB_Tree_Node<char *, char *>;
template class RB_Tree_Iterator<char *, char *>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Const_Binary_Functor_Base<char *, char *>
#pragma instantiate RB_Tree<int, int>
#pragma instantiate RB_Tree_Node<int, int>
#pragma instantiate RB_Tree_Iterator<int, int>
#pragma instantiate RB_Tree<int, char *>
#pragma instantiate RB_Tree_Node<int, char *>
#pragma instantiate RB_Tree_Iterator<int, char *>
#pragma instantiate RB_Tree<char *, int>
#pragma instantiate RB_Tree_Node<char *, int>
#pragma instantiate RB_Tree_Iterator<char *, int>
#pragma instantiate RB_Tree<char *, char *>
#pragma instantiate RB_Tree_Node<char *, char *>
#pragma instantiate RB_Tree_Iterator<char *, char *>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

