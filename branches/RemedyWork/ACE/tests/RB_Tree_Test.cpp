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
#include "ace/Null_Mutex.h"

#include "RB_Tree_Test.h"



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
run_main (int, ACE_TCHAR *[])
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

// ======= Stress Test contributed by Klaus H. Wolf <hw@cyland.com> =========

  ACE_RB_Tree<ACE_CString, int, ACE_Less_Than<ACE_CString>, ACE_Null_Mutex> tree;

  tree.bind (ACE_CString ("51"),  1);
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.bind (ACE_CString ("13"),  2);
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.bind (ACE_CString ("36"),  3);
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.bind (ACE_CString ("15"),  4);
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.bind (ACE_CString ("22"),  5);
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.bind (ACE_CString ("25"),  6);
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.bind (ACE_CString ("42"),  7);
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.bind (ACE_CString ("48"),  8);
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.bind (ACE_CString ("03"),  9);
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.bind (ACE_CString ("56"), 10);
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.bind (ACE_CString ("28"), 11);
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.bind (ACE_CString ("55"), 12);
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.bind (ACE_CString ("21"), 13);
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.bind (ACE_CString ("62"), 14);
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.bind (ACE_CString ("18"), 15);
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.bind (ACE_CString ("20"), 16);
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.bind (ACE_CString ("26"), 17);
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.bind (ACE_CString ("29"), 18);
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.bind (ACE_CString ("50"), 19);
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.bind (ACE_CString ("05"), 20);
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.bind (ACE_CString ("59"), 21);
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.bind (ACE_CString ("65"), 22);
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.bind (ACE_CString ("66"), 23);
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.bind (ACE_CString ("45"), 24);
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.bind (ACE_CString ("34"), 25);
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.bind (ACE_CString ("27"), 26);
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.bind (ACE_CString ("40"), 27);
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.bind (ACE_CString ("30"), 28);
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.bind (ACE_CString ("64"), 29);
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.bind (ACE_CString ("11"), 30);
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.bind (ACE_CString ("16"), 31);
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.bind (ACE_CString ("47"), 32);
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.bind (ACE_CString ("10"), 33);
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.bind (ACE_CString ("37"), 34);
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.bind (ACE_CString ("09"), 35);
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.bind (ACE_CString ("54"), 36);
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.bind (ACE_CString ("23"), 37);
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.bind (ACE_CString ("44"), 38);
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.bind (ACE_CString ("19"), 39);
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.bind (ACE_CString ("00"), 40);
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.bind (ACE_CString ("04"), 41);
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.bind (ACE_CString ("63"), 42);
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.bind (ACE_CString ("08"), 43);
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.bind (ACE_CString ("39"), 44);
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.bind (ACE_CString ("31"), 45);
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.bind (ACE_CString ("02"), 46);
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.bind (ACE_CString ("33"), 47);
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.bind (ACE_CString ("60"), 48);
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.bind (ACE_CString ("61"), 49);
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.bind (ACE_CString ("57"), 50);
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.bind (ACE_CString ("43"), 51);
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.bind (ACE_CString ("46"), 52);
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.bind (ACE_CString ("38"), 53);
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.bind (ACE_CString ("01"), 54);
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.bind (ACE_CString ("12"), 55);
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.bind (ACE_CString ("24"), 56);
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.bind (ACE_CString ("52"), 57);
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.bind (ACE_CString ("07"), 58);
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.bind (ACE_CString ("14"), 59);
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.bind (ACE_CString ("06"), 60);
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.bind (ACE_CString ("58"), 61);
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.bind (ACE_CString ("49"), 62);
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.bind (ACE_CString ("17"), 63);
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.bind (ACE_CString ("53"), 64);
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.bind (ACE_CString ("32"), 65);
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.bind (ACE_CString ("35"), 66);
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.bind (ACE_CString ("41"), 67);
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));

  tree.unbind (ACE_CString ("51"));
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.unbind (ACE_CString ("13"));
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.unbind (ACE_CString ("36"));
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.unbind (ACE_CString ("15"));
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.unbind (ACE_CString ("22"));
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.unbind (ACE_CString ("25"));
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.unbind (ACE_CString ("42"));
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.unbind (ACE_CString ("48"));
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.unbind (ACE_CString ("03"));
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.unbind (ACE_CString ("56"));
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.unbind (ACE_CString ("28"));
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.unbind (ACE_CString ("55"));
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.unbind (ACE_CString ("21"));
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.unbind (ACE_CString ("62"));
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.unbind (ACE_CString ("18"));
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.unbind (ACE_CString ("20"));
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.unbind (ACE_CString ("26"));
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.unbind (ACE_CString ("29"));
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.unbind (ACE_CString ("50"));
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.unbind (ACE_CString ("05"));
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.unbind (ACE_CString ("59"));
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.unbind (ACE_CString ("65"));
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.unbind (ACE_CString ("66"));
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.unbind (ACE_CString ("45"));
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.unbind (ACE_CString ("34"));
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.unbind (ACE_CString ("27"));
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.unbind (ACE_CString ("40"));
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.unbind (ACE_CString ("30"));
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.unbind (ACE_CString ("64"));
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.unbind (ACE_CString ("11"));
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.unbind (ACE_CString ("16"));
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.unbind (ACE_CString ("47"));
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.unbind (ACE_CString ("10"));
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.unbind (ACE_CString ("37"));
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.unbind (ACE_CString ("09"));
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.unbind (ACE_CString ("54"));
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.unbind (ACE_CString ("23"));
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.unbind (ACE_CString ("44"));
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.unbind (ACE_CString ("19"));
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.unbind (ACE_CString ("00"));
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.unbind (ACE_CString ("04"));
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.unbind (ACE_CString ("63"));
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.unbind (ACE_CString ("08"));
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.unbind (ACE_CString ("39"));
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.unbind (ACE_CString ("31"));
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.unbind (ACE_CString ("02"));
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.unbind (ACE_CString ("33"));
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.unbind (ACE_CString ("60"));
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.unbind (ACE_CString ("61"));
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.unbind (ACE_CString ("57"));
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.unbind (ACE_CString ("43"));
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.unbind (ACE_CString ("46"));
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.unbind (ACE_CString ("38"));
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.unbind (ACE_CString ("01"));
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.unbind (ACE_CString ("12"));
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.unbind (ACE_CString ("24"));
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.unbind (ACE_CString ("52"));
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.unbind (ACE_CString ("07"));
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.unbind (ACE_CString ("14"));
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.unbind (ACE_CString ("06"));
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.unbind (ACE_CString ("58"));
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.unbind (ACE_CString ("49"));
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.unbind (ACE_CString ("17"));
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.unbind (ACE_CString ("53"));
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.unbind (ACE_CString ("32"));
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.unbind (ACE_CString ("35"));
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));
  tree.unbind (ACE_CString ("41"));
  if (0 != tree.test_invariant ())
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Invariant failure at line %l\n")));

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
  test_partial_iteration();
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
      if (!(k >= 0 && k < entry_count_))
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("Assert failure at line %l\n")));

      // Test the new stable ACE_Hash_Map_Manager_Ex compliant interface.
      if (0 != stable_tree_.bind (key_array_ [k], item_array_ [k]))
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("Stable bind %p\n"),
                    ACE_TEXT ("failure")));

      if (0 != stable_tree_.find (key_array_ [k], item) ||
          item != item_array_ [k])
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("Stable find %p\n"),
                    ACE_TEXT ("failure")));

      // Test the deprecated interface.
      if (0 == deprecated_tree_.insert (key_array_ [k], item_array_ [k]))
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("Deprecated insert %p\n"),
                    ACE_TEXT ("failure")));

      if (0 == deprecated_tree_.find (key_array_ [k]) ||
          *deprecated_tree_.find (key_array_ [k]) != item_array_ [k])
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("Deprecated find %p\n"),
                    ACE_TEXT ("failure")));
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
      if (item != item_array_ [i])
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("Stable fwd iter, pass %d %p\n"),
                    i,
                    ACE_TEXT ("failure")));

      item = (*stable_rev_iter_).item ();
      if (item != item_array_ [entry_count_ - i - 1])
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("Stable rev iter, pass %d %p\n"),
                    i,
                    ACE_TEXT ("failure")));

      item = (*deprecated_fwd_iter_).item ();
      if (item != item_array_ [i])
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("Deprecated fwd iter, pass %d %p\n"),
                    i,
                    ACE_TEXT ("failure")));

      item = (*deprecated_rev_iter_).item ();
      if (item != item_array_ [entry_count_ - i - 1])
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("Deprecated rev iter, pass %d %p\n"),
                    i,
                    ACE_TEXT ("failure")));

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
  if (stable_fwd_iter_.done () != 1)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("Stable fwd iter not done but should be\n")));
  if (stable_rev_iter_.done () != 1)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("Stable fwd iter not done but should be\n")));
  if (deprecated_fwd_iter_.done () != 1)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("Stable fwd iter not done but should be\n")));
  if (deprecated_rev_iter_.done () != 1)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("Stable fwd iter not done but should be\n")));
}

template <class EXT_ID, class INT_ID, class COMPARE_KEYS, class ACE_LOCK> void
ACE_RB_Tree_Test<EXT_ID, INT_ID, COMPARE_KEYS, ACE_LOCK>::test_partial_iteration(void)
{
  ACE_RB_Tree_Node<EXT_ID, INT_ID> *tree_node = 0;

  stable_tree_.find(key_array_ [2], tree_node);
  part_rev_iter_ = ACE_RB_Tree_Reverse_Iterator<EXT_ID, INT_ID, COMPARE_KEYS, ACE_LOCK> (stable_tree_, tree_node);
  for (int i=2; i >= 0 ; --i)
    {
      INT_ID item;

      item = (*part_rev_iter_).item ();
      if (item != item_array_ [i])
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("Partial rev iter, pass %d %p\n"),
                    i,
                    ACE_TEXT ("failure")));
      part_rev_iter_++;
    }

  part_fwd_iter_ = ACE_RB_Tree_Iterator<EXT_ID, INT_ID, COMPARE_KEYS, ACE_LOCK> (key_array_ [5], stable_tree_);
  for (int k = 5; k < entry_count_; ++k)
    {
      INT_ID item;

      item = (*part_fwd_iter_).item ();
      if (item != item_array_ [k])
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("Partial fwd iter, pass %d %p\n"),
                    k,
                    ACE_TEXT ("failure")));
      part_fwd_iter_++;
    }
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
      if (stable_tree_.unbind (key_array_ [i]) != 0)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("Stable tree, failure pass %d %p\n"),
                    i,
                    ACE_TEXT ("unbind")));

      // Test the deprecated interface.
      if (deprecated_tree_.remove (key_array_ [i]) != 1)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("Deprecated tree, failure pass %d %p\n"),
                    i,
                    ACE_TEXT ("remove")));
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
      if (item != item_array_ [i])
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("Stable fwd iter, pass %d %p\n"),
                    i,
                    ACE_TEXT ("failure")));

      item = (*stable_rev_iter_).item ();
      if (item != item_array_ [entry_count_ - i])
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("Stable rev iter, pass %d %p\n"),
                    i,
                    ACE_TEXT ("failure")));

      item = (*deprecated_fwd_iter_).item ();
      if (item != item_array_ [i])
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("Deprecated fwd iter, pass %d %p\n"),
                    i,
                    ACE_TEXT ("failure")));

      item = (*deprecated_rev_iter_).item ();
      if (item != item_array_ [entry_count_ - i])
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("Deprecated rev iter, pass %d %p\n"),
                    i,
                    ACE_TEXT ("failure")));

      // Advance each iterator via postfix increment.
      stable_fwd_iter_++;
      stable_rev_iter_++;
      deprecated_fwd_iter_++;
      deprecated_rev_iter_++;
    }

  // Make sure each item in each tree has been visited a second time.
  if (stable_fwd_iter_.done () != 1)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("Stable fwd iter not done but should be\n")));
  if (stable_rev_iter_.done () != 1)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("Stable rev iter not done but should be\n")));
  if (deprecated_fwd_iter_.done () != 1)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("Deprecated fwd iter not done but should be\n")));
  if (deprecated_rev_iter_.done () != 1)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("Deprecated rev iter not done but should be\n")));
}


