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

#include "test_config.h"
#include "ace/Object_Manager.h"
#include "ace/Service_Config.h"

#define VARIETIES 3

u_int error = 0;

// This should be a template class, with singleton instantiations.
// But to avoid having to deal with compilers that want template
// declarations in separate files, it's just a plain class.  The
// instance argument differentiates the "singleton" instances.
// It also demonstrates the use of the param arg to the cleanup ()
// function.
class Test_Singleton
{
public:
  static Test_Singleton *instance (u_short variety);
  static void cleanup (void *, void *);
private:
  u_short variety_;
  static u_short current_;

  Test_Singleton (u_short variety);
  ~Test_Singleton (void);

  friend class misspelled_verbase_friend_declaration_to_avoid_compiler_warning_with_private_ctor;
};

u_short Test_Singleton::current_ = 0;

Test_Singleton *
Test_Singleton::instance (u_short variety)
{
  static Test_Singleton *instances[VARIETIES] = { 0 };

  if (instances[variety] == 0)
    {
      ACE_NEW_RETURN (instances[variety], Test_Singleton (variety), 0);
    }

  ACE_Object_Manager::at_exit (instances[variety], cleanup, (void *) variety);

  return instances[variety];
}

void
Test_Singleton::cleanup (void *object, void *param)
{
  // We can't reliably use ACE_Log_Msg in a cleanup hook.  Yet.
  ACE_UNUSED_ARG (param);
  /* ACE_DEBUG ((LM_DEBUG, "cleanup %d\n", (u_short) param)); */

  delete (Test_Singleton *) object;
}

Test_Singleton::Test_Singleton (u_short variety)
  : variety_ (variety)
{
  if (variety_ != current_++)
    {
      ACE_DEBUG ((LM_ERROR, "ERROR: instance %u created out of order!\n",
                 variety_));
      ++error;
    }
}

// We can't reliably use ACE_Log_Msg in a destructor that is called by
// ACE_Object_Manager.  Yet.
Test_Singleton::~Test_Singleton (void)
{
  /* ACE_DEBUG ((LM_DEBUG, "Test_Singleton %u dtor\n", variety_)); */

  if (variety_ != --current_)
    {
      ACE_OS::fprintf (stderr, "ERROR: instance %u destroyed out of order!\n",
                       variety_);
      /* ACE_DEBUG ((LM_ERROR, "ERROR: instance %u destroyed out of order!\n",
                 variety_)); */
      ++error;
    }
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

  for (u_int i = 0; i < VARIETIES; ++i)
    {
      Test_Singleton &s = *Test_Singleton::instance (i);

      if (&s == 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR, "instance () allocate failed!\n"), 1);
        }
    }

  run_test (argc, argv);

  ACE_END_TEST;
  return error == 0 ? 0 : 1;
}
