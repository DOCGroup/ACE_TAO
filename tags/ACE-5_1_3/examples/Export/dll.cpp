// $Id$
#include "dll.h"

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
