// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Service_Config_Test.cpp
//
// = DESCRIPTION
//      This is a simple test to make sure the Service_Configurator is
//      working correctly.
//
// = AUTHOR
//    David Levine
//
// ============================================================================

#include "ace/Object_Manager.h"
#include "ace/Service_Config.h"
#include "test_config.h"

template <int NUMBER>
class Test_Singleton
{
public:
  static Test_Singleton *instance (void);
  static void cleanup (void *, void *);
private:
  static Test_Singleton *instance_;

  Test_Singleton (void) { ACE_DEBUG ((LM_DEBUG, "Test_Singleton %d ctor\n", NUMBER)); }
  ~Test_Singleton (void) { ACE_DEBUG ((LM_DEBUG, "Test_Singleton %d dtor\n", NUMBER)); }

  friend class verbase_friend_declaration_to_avoid_compiler_warning_with_private_ctor;
};

Test_Singleton<1> *Test_Singleton<1>::instance_ = 0;
Test_Singleton<2> *Test_Singleton<2>::instance_ = 0;
Test_Singleton<3> *Test_Singleton<3>::instance_ = 0;

template <int NUMBER>
Test_Singleton<NUMBER> *
Test_Singleton<NUMBER>::instance (void)
{
  if (instance_ == 0)
    {
      ACE_NEW_RETURN (instance_, Test_Singleton, 0);
      ACE_Object_Manager::at_exit (instance_, cleanup, 0);
    }

  return instance_;
}

template <int NUMBER>
void
Test_Singleton<NUMBER>::cleanup (void *object, void *)
{
  delete (Test_Singleton *) object;
}

static void
run_test (int argc, char *argv[])
{
  // We need this scope to make sure that the destructor for the
  // ACE_Service_Config gets called.
  ACE_Service_Config daemon;

  daemon.open (argc, argv);

  ACE_Time_Value tv (argc > 1 ? atoi (argv[1]) : 2);

  ACE_ASSERT (ACE_Reactor::run_event_loop (tv) == 0);
}

int
main (int argc, char *argv[])
{
  ACE_START_TEST ("Service_Config_Test");

  Test_Singleton<1> &one = *Test_Singleton<1>::instance ();
  Test_Singleton<2> &two = *Test_Singleton<2>::instance ();
  Test_Singleton<3> &three = *Test_Singleton<3>::instance ();

  if (&one == 0  ||  &two == 0  || &three == 0)
    ACE_ERROR_RETURN ((LM_ERROR, "instance () allocate failed!\n"), 1);

  run_test (argc, argv);

  ACE_END_TEST;
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class Test_Singleton<1>;
template class Test_Singleton<2>;
template class Test_Singleton<3>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate Test_Singleton<1>
#pragma instantiate Test_Singleton<2>
#pragma instantiate Test_Singleton<3>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

