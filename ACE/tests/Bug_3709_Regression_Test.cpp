/**
 * @file Bug_3709_Regression_Test.cpp
 *
 * Reproduces the problems reported in bug 3709
 *   http://bugzilla.dre.vanderbilt.edu/show_bug.cgi?id=3709
 */

#include "test_config.h"
#include "ace/ACE.h"
#include "ace/OS_NS_stdio.h"
#include <vector>

using namespace std;

// MSVC_71_OR_OLDER
#if defined(_MSC_VER) && _MSC_VER < 1400
#define BROKEN_TEMPLATE_TEMPLATE
#endif

// clang version 2.9 crashes when trying to compile the test
// http://llvm.org/bugs/show_bug.cgi?id=9643
#ifdef __clang__
#define BROKEN_TEMPLATE_TEMPLATE
#endif

#ifdef __SUNPRO_CC
#define BROKEN_TEMPLATE_TEMPLATE
#endif

// HP aC++ 03.x fails this
#if defined(__HP_aCC) && (__HP_aCC < 40000)
#define BROKEN_TEMPLATE_TEMPLATE
#endif

#ifndef BROKEN_TEMPLATE_TEMPLATE
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

#ifndef BROKEN_TEMPLATE_TEMPLATE
  vector<int> v = initializer<vector>(5);
  v.clear ();
#endif

  ACE_END_TEST;

  return 0;
}
