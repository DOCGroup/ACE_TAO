// $Id$

/**
 * This program checks if the compiler doesn't have a certain bug
 * that we encountered when testing C++11 features
 */

#include "test_config.h"

#if defined (ACE_HAS_CPP11)

#include <memory>

class foo_impl;
struct foo_impl_deleter
{
  void operator()(foo_impl *p);
};

class Foo
{
private:
  std::unique_ptr<foo_impl, foo_impl_deleter> impl_;
  std::shared_ptr<foo_impl> impl2 {nullptr, foo_impl_deleter ()};
} impl_;

class foo_impl {};

void foo_impl_deleter::operator()(foo_impl *p)
{
  delete p;
}

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT("Compiler_Features_35_Test"));

  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("C++11 support ok\n")));

  ACE_END_TEST;

  return 0;
}

#else
int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT("Compiler_Features_35_Test"));

  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("No C++11 support enabled\n")));

  ACE_END_TEST;
  return 0;
}

#endif
