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

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Singleton<test_class,ACE_Null_Mutex>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Singleton<test_class,ACE_Null_Mutex>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
