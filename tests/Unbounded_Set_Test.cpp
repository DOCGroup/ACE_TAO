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
//      This test shall evolve to illustrate the use of ACE_Unbounded_Set.
//      For the moment it only tests some insertion and removal, and acts
//      as a regression test for Bugzilla bug 1460.
//      No command line arguments are needed to run the test.
//
// = AUTHOR
//    Rudolf Weber <rfweber@tesionmail.de>,
//    ace/tests integration <Oliver.Kellogg@sysde.eads.net>
//
// ============================================================================

#include "test_config.h"
#include <ace/Unbounded_Set.h>

ACE_RCSID(tests, Unbounded_Set_Test, "$Id$")

struct MyNode
{
  int k;
  MyNode () : k (0) {}
  MyNode (int pk) : k (pk) {}
  MyNode (const MyNode& o) : k (o.k) {}
  bool operator== (const MyNode& o) { return (k == o.k); }
};

int main()
{
  int r;
  MyNode node (1);
  ACE_Unbounded_Set<MyNode> ubs;

  ACE_ASSERT (ubs.size () == 0);

  // Insert a node. Immediately remove it.
  r = ubs.insert (node);
  ACE_ASSERT (r == 0);
  ACE_ASSERT (ubs.size () == 1);
  r = ubs.remove (node);
  ACE_ASSERT (r == 0);
  ACE_ASSERT (ubs.size () == 0);

  // Insert a node. Remove it within an iterator.
  int n_iterations = 0;
  r = ubs.insert (node);
  ACE_Unbounded_Set<MyNode>::ITERATOR end = ubs.end ();
  for (ACE_Unbounded_Set<MyNode>::ITERATOR i = ubs.begin (); i != end; i++)
    {
      ACE_ASSERT (*i == node);
      r = ubs.remove (*i);
      n_iterations++;
    }
  ACE_ASSERT (n_iterations == 1);
  ACE_ASSERT (r == 0);
  ACE_ASSERT (ubs.size () == 0);

  return 0;
}

