// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Unbounded_Set_Test.cpp
//
// = DESCRIPTION
//      This test illustrates the use of ACE_Unbounded_Set.
//      In addition, it acts as a regression test for Bugzilla bug 1460.
//      No command line arguments are needed to run the test.
//
// = AUTHOR
//    Rudolf Weber <rfweber@tesionmail.de>,
//    ace/tests integration <Oliver.Kellogg@sysde.eads.net>
//
// ============================================================================

#include "test_config.h"
#include <ace/Unbounded_Set.h>
#include <ace/Auto_Ptr.h>
#include <ace/SString.h>

ACE_RCSID(tests, Unbounded_Set_Test, "$Id$")

struct MyNode
{
  unsigned k;
  MyNode () : k (0) {}
  MyNode (int pk) : k (pk) {}
  MyNode (const MyNode& o) : k (o.k) {}
  bool operator== (const MyNode& o) { return (k == o.k); }
};

size_t count_const_set (const ACE_Unbounded_Set<MyNode>& cubs)
{
  size_t number_of_elements = 0;
  for (ACE_Unbounded_Set<MyNode>::const_iterator ci (cubs); !ci.done(); ci++)
    number_of_elements++;
  return number_of_elements;
}

int
run_main (int, ACE_TCHAR *[])
{
  int r;
  unsigned k;
  MyNode node (1);

  ACE_START_TEST (ACE_TEXT ("Unbounded_Set_Test"));

  ACE_Unbounded_Set<MyNode> ubs;
  ACE_ASSERT (ubs.size () == 0);

  // Insert a value. Immediately remove it.
  r = ubs.insert (node);
  ACE_ASSERT (r == 0);
  ACE_ASSERT (ubs.size () == 1);
  r = ubs.remove (node);
  ACE_ASSERT (r == 0);
  ACE_ASSERT (ubs.size () == 0);

  // Insert several different values.
  for (node.k = 1; node.k <= 5; node.k++)
    {
      r = ubs.insert (node);
      ACE_ASSERT (r == 0);
      ACE_ASSERT (ubs.size () == node.k);
    }

  // Test assigment of sets.
  // To do that, we also test some of the iterator methods.
  typedef ACE_Unbounded_Set<MyNode> MySet;
  MySet ubs2 = ubs;              // Test a typedef of a set.
  ACE_ASSERT (ubs2.size() == ubs.size());
  {
    MySet::ITERATOR it1 (ubs);
    MySet::iterator it2 (ubs2);
    for (k = 1; k <= 5; k++)
      {
        ACE_ASSERT (! it1.done ());
        ACE_ASSERT (! it2.done ());
        MyNode n1 = *it1;
        MyNode n2 = *it2;
        ACE_ASSERT (n1 == n2);
        it1.advance ();
        it2.advance ();
      }
    ACE_ASSERT (it1.done ());
    ACE_ASSERT (it2.done ());
    // Verify that a set may be emptied while an iterator on the set is
    // in-scope but inactive:
    ubs.reset ();
    // Restore original set from ubs2
    ubs = ubs2;
  }

  // Selective deletion of elements and element retrieval.
  {
    MySet::iterator it (ubs2);
    int deleted = 0;
    while (! it.done ())
      {
        MyNode n = *it;
        it.advance ();  /* Being friendly here: Move the iterator on
                           so that element removal does not interfere
                           with the current iterator position.
                           The less friendly case, removal under the
                           current iterator position, is below.  */
        if (n.k % 2 == 1)
          {
            r = ubs2.remove (n);
            deleted++;
          }
      }
    ACE_ASSERT (ubs2.size () + deleted == ubs.size());

    MyNode node2 (2);
    ACE_ASSERT (ubs2.find (node2) == 0);

    MyNode node3 (3);
    ACE_ASSERT (ubs2.find (node3) != 0);

    ubs2.insert (node3);
  }

  size_t s = count_const_set (ubs);
  ACE_ASSERT (s == ubs.size ());

  // Test deletion under the cursor.
  // This is the regression test for Bugzilla bug 1460.
  {
    MySet::iterator end = ubs2.end ();
    for (MySet::iterator i = ubs2.begin (); i != end; i++)
      {
        r = ubs2.remove (*i);
        ACE_ASSERT (r == 0);
      }
    ACE_ASSERT (ubs2.size () == 0);
  }
  ACE_ASSERT (ubs2.is_empty ());

  ACE_END_TEST;
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Node<MyNode>;
template class ACE_Unbounded_Set<MyNode>;
template class ACE_Unbounded_Set_Iterator<MyNode>;
template class ACE_Unbounded_Set_Const_Iterator<MyNode>;
template class ACE_Auto_Basic_Ptr<ACE_CString>;
template class auto_ptr<ACE_CString>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Node<MyNode>
#pragma instantiate ACE_Unbounded_Set<MyNode>
#pragma instantiate ACE_Unbounded_Set_Iterator<MyNode>
#pragma instantiate ACE_Unbounded_Set_Const_Iterator<MyNode>
#pragma instantiate ACE_Auto_Basic_Ptr<ACE_CString>
#pragma instantiate auto_ptr<ACE_CString>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

