// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Unbounded_Set_Test_Ex.h
//
// = DESCRIPTION
//    This class gets its own header file to work around AIX C++
//    compiler "features" related to template instantiation...  It is
//    only used by Unbounded_Set_Test_Ex.cpp.
//
// = AUTHOR
//      Oliver Kellogg <Oliver.Kellogg@sysde.eads.net>
//
// ============================================================================

#ifndef ACE_TESTS_UNBOUNDED_SET_TEST_EX_H
#define ACE_TESTS_UNBOUNDED_SET_TEST_EX_H

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

struct MyNode
{
  unsigned k;
  MyNode () : k (0) {}
  MyNode (int pk) : k (pk) {}
  MyNode (const MyNode& o) : k (o.k) {}
  bool operator== (const MyNode& o) { return (k == o.k); }
};

#endif /* ACE_TESTS_UNBOUNDED_SET_TEST_EX_H */
