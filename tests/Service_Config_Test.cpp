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
//    Doug Schmidt
//
// ============================================================================

#include "ace/Object_Manager.h"
#include "ace/Service_Config.h"
#include "test_config.h"

template <int NUMBER>
class mySingleton
{
public:
  static mySingleton *instance ();
  static void cleanup (void *, void *);
private:
  static mySingleton *instance_;

  mySingleton () { ACE_DEBUG ((LM_DEBUG, "mySingleton %d ctor\n", NUMBER)); }
  ~mySingleton () { ACE_DEBUG ((LM_DEBUG, "mySingleton %d dtor\n", NUMBER)); }

  friend class friend_declaration_to_avoid_compiler_warning_with_private_ctor;
};

mySingleton<1> *mySingleton<1>::instance_ = 0;
mySingleton<2> *mySingleton<2>::instance_ = 0;
mySingleton<3> *mySingleton<3>::instance_ = 0;

template <int NUMBER>
mySingleton<NUMBER> *
mySingleton<NUMBER>::instance ()
{
  if (instance_ == 0)
    {
      ACE_NEW_RETURN (instance_, mySingleton, 0);
      ACE_Object_Manager::at_exit (instance_, cleanup, 0);
    }

  return instance_;
}

template <int NUMBER>
void
mySingleton<NUMBER>::cleanup (void *object, void *)
{
  delete (mySingleton *) object;
}

static void
run_test (int argc, char *argv[])
{
  // We need this scope to make sure
  ACE_Service_Config daemon;

  daemon.open (argc, argv);

  ACE_Time_Value tv (argc > 1 ? atoi (argv[1]) : 2);

  ACE_ASSERT (ACE_Reactor::run_event_loop (tv) == 0);
}

int
main (int argc, char *argv[])
{
  ACE_START_TEST ("Service_Config_Test");

  mySingleton<1> &one = *mySingleton<1>::instance ();
  mySingleton<2> &two = *mySingleton<2>::instance ();
  mySingleton<3> &three = *mySingleton<3>::instance ();
  if (&one == 0  ||  &two == 0  || &three == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR, "instance () allocate failed!\n"), 1);
    }

  run_test (argc, argv);

  ACE_END_TEST;
  return 0;
}

#if defined (ACE_TEMPLATES_REQUIRE_SPECIALIZATION)
template class mySingleton<1>;
template class mySingleton<2>;
template class mySingleton<3>;
#endif /* ACE_TEMPLATES_REQUIRE_SPECIALIZATION */
