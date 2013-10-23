// $Id$
#include "dll.h"

int test_variable = 0;

int
test_function ()
{
  test_variable = RETVAL;
  return RETVAL;
}

int
test_class::method ()
{
  return RETVAL;
}

test_class *
get_dll_singleton ()
{
  return TEST_SINGLETON::instance ();
}

ACE_SINGLETON_TEMPLATE_INSTANTIATE(ACE_Singleton, test_class, ACE_Null_Mutex);
