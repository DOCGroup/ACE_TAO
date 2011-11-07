/**
 * @file Bug_3709_Regression_Test.cpp
 *
 * $Id$
 *
 * Reproduces the problems reported in bug 3709
 *   http://deuce.doc.wustl.edu/bugzilla/show_bug.cgi?id=3709
 */

#include "test_config.h"
#include "ace/ACE.h"
#include "ace/OS_NS_stdio.h"
#include <vector>

using namespace std;

#if defined(_MSC_VER) && _MSC_VER < 1400
#define MSVC_71_OR_OLDER
#endif

// clang version 2.9 crashes when trying to compile the test
// http://llvm.org/bugs/show_bug.cgi?id=9643
#ifdef __clang__
#define MSVC_71_OR_OLDER
#endif

#ifndef MSVC_71_OR_OLDER
template<template<typename U, typename = std::allocator<U> > class container, typename DT>
container<DT> initializer(const DT &d)
{
  container<DT> t ;
  t.insert(t.end(), d);
  return t;
}
#endif

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Bug_3709_Regression_Test"));

#ifndef MSVC_71_OR_OLDER
  vector<int> v = initializer<vector>(5);
  v.clear ();
#endif

  ACE_END_TEST;

  return 0;
}
