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



struct MyNode
{
  unsigned k;
  MyNode () : k (0) {}
  MyNode (int pk) : k (pk) {}
  MyNode (const MyNode& o) : k (o.k) {}
  bool operator== (const MyNode& o) const { return (k == o.k); }
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
  int retval = 0;
  unsigned k;
  MyNode node (1);

  ACE_START_TEST (ACE_TEXT ("Unbounded_Set_Test"));

  ACE_Unbounded_Set<MyNode> ubs;
  if (ubs.size () != 0)
    {
      ACE_ERROR ((LM_ERROR, "Error: ubs.size () != 0\n"));
      retval = -1;
    }
  if (!ubs.is_empty ())
    {
      ACE_ERROR ((LM_ERROR, "Error: !ubs.is_empty ()\n"));
      retval = -1;
    }

  // Insert a value. Immediately remove it.
  r = ubs.insert (node);
  if (r != 0)
    {
      ACE_ERROR ((LM_ERROR, "Error: r != 0\n"));
      retval = -1;
    }
  if (ubs.size () != 1)
    {
      ACE_ERROR ((LM_ERROR, "Error: ubs.size () != 1\n"));
      retval = -1;
    }
  r = ubs.remove (node);
  if (r != 0)
    {
      ACE_ERROR ((LM_ERROR, "Error: r != 0\n"));
      retval = -1;
    }
  if (ubs.size () != 0)
    {
      ACE_ERROR ((LM_ERROR, "Error: ubs.size () != 0\n"));
      retval = -1;
    }

  // Insert several different values.
  for (node.k = 1; node.k <= 5; node.k++)
    {
      r = ubs.insert (node);
      if (r != 0)
        {
          ACE_ERROR ((LM_ERROR, "Error: r != 0\n"));
          retval = -1;
        }
      if (ubs.size () != node.k)
        {
          ACE_ERROR ((LM_ERROR, "Error: ubs.size () != node.k\n"));
          retval = -1;
        }
    }

  // Test assigment of sets.
  // To do that, we also test some of the iterator methods.
  typedef ACE_Unbounded_Set<MyNode> MySet;
  MySet ubs2 = ubs;              // Test a typedef of a set.
  if (ubs2.size() != ubs.size())
    {
      ACE_ERROR ((LM_ERROR, "Error: ubs2.size() != ubs.size()\n"));
      retval = -1;
    }
  {
    MySet::ITERATOR it1 (ubs);
    MySet::iterator it2 (ubs2);
    for (k = 1; k <= 5; k++)
      {
        if (it1.done ())
          {
            ACE_ERROR ((LM_ERROR, "Error: it1.done ()\n"));
            retval = -1;
          }
        if (it2.done ())
          {
            ACE_ERROR ((LM_ERROR, "Error: it2.done ()\n"));
            retval = -1;
          }
        MyNode n1 = *it1;
        MyNode n2 = *it2;
        if (!(n1 == n2))
          {
            ACE_ERROR ((LM_ERROR, "Error: !(n1 == n2)\n"));
            retval = -1;
          }
        it1.advance ();
        it2.advance ();
      }
    if (!it1.done ())
      {
        ACE_ERROR ((LM_ERROR, "Error: !it1.done ()\n"));
        retval = -1;
      }
    if (!it2.done ())
      {
        ACE_ERROR ((LM_ERROR, "Error: !it2.done ()\n"));
        retval = -1;
      }
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
    if (ubs2.size () + deleted != ubs.size())
      {
        ACE_ERROR ((LM_ERROR, "Error: ubs2.size () + deleted != ubs.size()\n"));
        retval = -1;
      }

    MyNode node2 (2);
    if (ubs2.find (node2) != 0)
      {
        ACE_ERROR ((LM_ERROR, "Error: ubs2.find (node2) != 0\n"));
        retval = -1;
      }

    MyNode node3 (3);
    if (ubs2.find (node3) == 0)
      {
        ACE_ERROR ((LM_ERROR, "Error: ubs2.find (node3) == 0\n"));
        retval = -1;
      }

    ubs2.insert (node3);
  }

  size_t s = count_const_set (ubs);
  if (s != ubs.size ())
    {
      ACE_ERROR ((LM_ERROR, "Error: s != ubs.size ()\n"));
      retval = -1;
    }

  ACE_Unbounded_Set<MyNode> ubs_insert;
  MyNode node4 (4);
  if (ubs_insert.insert (node4) != 0)
    {
      ACE_ERROR ((LM_ERROR, "Error: insert node4 failed\n"));
      retval = -1;
    }
  if (ubs_insert.insert (node4) != 1)
    {
      ACE_ERROR ((LM_ERROR, "Error: insert node4 didn't return 1\n"));
      retval = -1;
    }

  ACE_END_TEST;
  return retval;
}

