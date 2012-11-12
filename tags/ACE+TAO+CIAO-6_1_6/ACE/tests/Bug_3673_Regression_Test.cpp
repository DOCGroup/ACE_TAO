/**
 * @file Bug_3673_Regression_Test.cpp
 *
 * $Id$
 *
 * Reproduces the problems reported in bug 3673
 *   http://deuce.doc.wustl.edu/bugzilla/show_bug.cgi?id=3673
 */

#include "test_config.h"
#include "ace/ACE.h"
#include "ace/OS_NS_stdio.h"

static bool construct_alpha = false;
static bool destruct_alpha = false;
static bool construct_beta = false;
static bool destruct_beta = false;

template <typename T>
struct Alpha
{
  Alpha() { ACE_DEBUG ((LM_DEBUG, ACE_TEXT("construct alpha\n"))); construct_alpha = true;}
  ~Alpha() { ACE_DEBUG ((LM_DEBUG, ACE_TEXT("destruct alpha\n"))); destruct_alpha = true;}
};

struct Beta
{
  Beta() { ACE_DEBUG ((LM_DEBUG, ACE_TEXT("construct beta\n"))); construct_beta = true;}
  ~Beta() { ACE_DEBUG ((LM_DEBUG, ACE_TEXT("destruct bepha\n"))); destruct_beta = true;}
};

struct Test
{
  Alpha<int> a;
  Beta b;

  Test() { ACE_DEBUG ((LM_DEBUG, ACE_TEXT("throw oops\n"))); throw "oops"; }
};

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Bug_3673_Regression_Test"));
  int result = 0;

  bool caught_excep = false;
  try
  {
    Test test;
  }
  catch (...)
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Caught exception!\n")));
    caught_excep = true;
  }

  if (!caught_excep)
  {
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Not caught exception\n")));
    ++result;
  }
  if (!construct_alpha)
  {
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Constructor alpha not called\n")));
    ++result;
  }
  if (!construct_beta)
  {
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Constructor beta not called\n")));
    ++result;
  }
  if (!destruct_alpha)
  {
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Destructor alpha not called\n")));
    ++result;
  }
  if (!destruct_beta)
  {
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Destructor beta not called\n")));
    ++result;
  }

  ACE_END_TEST;

  return result;
}
