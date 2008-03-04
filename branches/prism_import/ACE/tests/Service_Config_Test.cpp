// -*- C++ -*-

//=============================================================================
/**
 *  @file    Service_Config_Test.cpp
 *
 *  $Id$
 *
 *  This is a simple test to make sure the ACE Service Configurator
 *  framework is working correctly.
 *
 *  @author David Levine <levine@cs.wustl.edu>
 *  @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================

#include "test_config.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_errno.h"
#include "ace/Log_Msg.h"
#include "ace/Object_Manager.h"
#include "ace/Service_Config.h"
#include "ace/Service_Object.h"
#include "ace/Service_Repository.h"
#include "ace/Service_Types.h"
#include "ace/Reactor.h"
#include "ace/Thread_Manager.h"
#include "ace/ARGV.h"

ACE_RCSID (tests,
           Service_Config_Test,
           "$Id$")

static const u_int VARIETIES = 3;

static u_int error = 0;

/**
 * @class Test_Singleton
 *
 * @brief Test the Singleton
 *
 * This should be a template class, with singleton instantiations.
 * But to avoid having to deal with compilers that want template
 * declarations in separate files, it's just a plain class.  The
 * instance argument differentiates the "singleton" instances.  It
 * also demonstrates the use of the param arg to the cleanup ()
 * function.
 */
class Test_Singleton
{
public:
  static Test_Singleton *instance (u_short variety);
  ~Test_Singleton (void);

private:
  u_short variety_;
  static u_short current_;

  Test_Singleton (u_short variety);

  friend class misspelled_verbase_friend_declaration_to_avoid_compiler_warning_with_private_ctor;
};

u_short Test_Singleton::current_ = 0;

extern "C" void
test_singleton_cleanup (void *object, void *param)
{
  // We can't reliably use ACE_Log_Msg in a cleanup hook.  Yet.
  ACE_UNUSED_ARG (param);
  /* ACE_DEBUG ((LM_DEBUG, "cleanup %d\n", (u_short) param)); */

  delete (Test_Singleton *) object;
}

Test_Singleton *
Test_Singleton::instance (u_short variety)
{
  static Test_Singleton *instances[VARIETIES] = { 0 };

  if (instances[variety] == 0)
    ACE_NEW_RETURN (instances[variety],
                    Test_Singleton (variety),
                    0);

  ACE_Object_Manager::at_exit (instances[variety],
                               test_singleton_cleanup,
                               reinterpret_cast<void *> (static_cast<size_t> (variety)));
  return instances[variety];
}

Test_Singleton::Test_Singleton (u_short variety)
  : variety_ (variety)
{
  if (variety_ != current_++)
    {
      ACE_DEBUG ((LM_ERROR,
                  ACE_TEXT ("ERROR: instance %u created out of order!\n"),
                 variety_));
      ++error;
    }
}

// We can't reliably use ACE_Log_Msg in a destructor that is called by
// ACE_Object_Manager.  Yet.

Test_Singleton::~Test_Singleton (void)
{
  /* ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Test_Singleton %u dtor\n"), variety_)); */

  if (variety_ != --current_)
    {
      ACE_OS::fprintf (stderr,
                       ACE_TEXT ("ERROR: instance %u destroyed out of order!\n"),
                       variety_);
      /* ACE_DEBUG ((LM_ERROR, ACE_TEXT ("ERROR: instance %u destroyed out of order!\n"),
                 variety_)); */
      ++error;
    }
}

void
testFailedServiceInit (int, ACE_TCHAR *[])
{
  static const ACE_TCHAR *refuse_svc =
#if (ACE_USES_CLASSIC_SVC_CONF == 1)
    ACE_TEXT ("dynamic Refuses_Svc Service_Object * ")
    ACE_TEXT ("  Service_Config_DLL:_make_Refuses_Init() \"\"")
#else
    ACE_TEXT ("<dynamic id=\"Refuses_Svc\" type=\"Service_Object\">")
    ACE_TEXT ("  <initializer init=\"_make_Refuses_Init\" path=\"Service_Config_DLL\" params=\"\"/>")
    ACE_TEXT ("</dynamic>")
#endif /* (ACE_USES_CLASSIC_SVC_CONF == 1) */
    ;

  int error_count = 0;
  if ((error_count = ACE_Service_Config::process_directive (refuse_svc)) != 1)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Failed init test should have returned 1; ")
                  ACE_TEXT ("returned %d instead\n"),
                  error_count));
    }

  // Try to find the service; it should not be there.
  ACE_Service_Type const *svcp;
  if (-1 != ACE_Service_Repository::instance ()->find (ACE_TEXT ("Refuses_Svc"),
                                                       &svcp))
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Found service repo entry for Refuses_Svc\n")));
      ACE_Service_Type_Impl const *svc_impl = svcp->type ();
      ACE_TCHAR msg[1024];
      ACE_TCHAR *msgp = msg;
      if (svc_impl->info (&msgp, sizeof (msg) / sizeof (ACE_TCHAR)) > 0)
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("Refuses_Svc said: %s\n"), msg));
    }
  else
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("Repo reports no Refuses_Svc; correct.\n")));
}


void
testLoadingServiceConfFileAndProcessNo (int argc, ACE_TCHAR *argv[])
{
  ACE_ARGV new_argv;

#if defined (ACE_USES_WCHAR)
  // When using full Unicode support, use the version of the Service
  // Configurator file appropriate to the platform.
  // For example, Windows Unicode uses UTF-16.
  //
  //          iconv(1) found on Linux and Solaris, for example, can
  //          be used to convert between encodings.
  //
  //          Byte ordering is also an issue, so we should be
  //          generating this file on-the-fly from the UTF-8 encoded
  //          file by using functions like iconv(1) or iconv(3).
#  if defined (ACE_WIN32)
  const ACE_TCHAR svc_conf[] =
    ACE_TEXT ("Service_Config_Test.UTF-16")
    ACE_TEXT (ACE_DEFAULT_SVC_CONF_EXT);
#  else
  const ACE_TCHAR svc_conf[] =
    ACE_TEXT ("Service_Config_Test.WCHAR_T")
    ACE_TEXT (ACE_DEFAULT_SVC_CONF_EXT);
#  endif /* ACE_WIN32 */
#else
    // ASCII (UTF-8) encoded Service Configurator file.
  const ACE_TCHAR svc_conf[] =
    ACE_TEXT ("Service_Config_Test")
    ACE_TEXT (ACE_DEFAULT_SVC_CONF_EXT);
#endif  /* ACE_USES_WCHAR */

  // Process the Service Configurator directives in this test's Making
  // sure we have more than one option with an argument, to capture
  // any errors caused by "reshuffling" of the options.
  ACE_ASSERT (new_argv.add (argv) != -1
              && new_argv.add (ACE_TEXT ("-d")) != -1
              && new_argv.add (ACE_TEXT ("-k")) != -1
              && new_argv.add (ACE_TEXT ("xxx")) != -1
              && new_argv.add (ACE_TEXT ("-p")) != -1
              && new_argv.add (ACE_TEXT ("Service_Config_Test.pid")) != -1
              && new_argv.add (ACE_TEXT ("-f")) != -1
              && new_argv.add (svc_conf) != -1);

  // We need this scope to make sure that the destructor for the
  // <ACE_Service_Config> gets called.
  ACE_Service_Config daemon;

  ACE_ASSERT (daemon.open (new_argv.argc (),
                           new_argv.argv ()) != -1 || errno == ENOENT);

  ACE_Time_Value tv (argc > 1 ? ACE_OS::atoi (argv[1]) : 2);

  ACE_ASSERT (ACE_Reactor::instance()->run_reactor_event_loop (tv) == 0);

  // Wait for all threads to complete.
  ACE_Thread_Manager::instance ()->wait ();
}


void
testLoadingServiceConfFile (int argc, ACE_TCHAR *argv[])
{
  ACE_ARGV new_argv;

#if defined (ACE_USES_WCHAR)
  // When using full Unicode support, use the version of the Service
  // Configurator file appropriate to the platform.
  // For example, Windows Unicode uses UTF-16.
  //
  //          iconv(1) found on Linux and Solaris, for example, can
  //          be used to convert between encodings.
  //
  //          Byte ordering is also an issue, so we should be
  //          generating this file on-the-fly from the UTF-8 encoded
  //          file by using functions like iconv(1) or iconv(3).
#  if defined (ACE_WIN32)
  const ACE_TCHAR svc_conf[] =
    ACE_TEXT ("Service_Config_Test.UTF-16")
    ACE_TEXT (ACE_DEFAULT_SVC_CONF_EXT);
#  else
  const ACE_TCHAR svc_conf[] =
    ACE_TEXT ("Service_Config_Test.WCHAR_T")
    ACE_TEXT (ACE_DEFAULT_SVC_CONF_EXT);
#  endif /* ACE_WIN32 */
#else
    // ASCII (UTF-8) encoded Service Configurator file.
  const ACE_TCHAR svc_conf[] =
    ACE_TEXT ("Service_Config_Test")
    ACE_TEXT (ACE_DEFAULT_SVC_CONF_EXT);
#endif  /* ACE_USES_WCHAR */

  // Process the Service Configurator directives in this test's
  ACE_ASSERT (new_argv.add (argv) != -1
              && new_argv.add (ACE_TEXT ("-f")) != -1
              && new_argv.add (svc_conf) != -1);

  // We need this scope to make sure that the destructor for the
  // <ACE_Service_Config> gets called.
  ACE_Service_Config daemon;

  ACE_ASSERT (daemon.open (new_argv.argc (),
                           new_argv.argv ()) != -1 || errno == ENOENT);

  ACE_Time_Value tv (argc > 1 ? ACE_OS::atoi (argv[1]) : 2);

  ACE_ASSERT (ACE_Reactor::instance()->run_reactor_event_loop (tv) == 0);

  // Wait for all threads to complete.
  ACE_Thread_Manager::instance ()->wait ();
}


// @brief The size of a repository is pre-determined and can not be exceeded
void
testLimits (int , ACE_TCHAR *[])
{
  static const ACE_TCHAR *svc_desc1 =
#if (ACE_USES_CLASSIC_SVC_CONF == 1)
    ACE_TEXT ("dynamic Test_Object_1_More Service_Object * ")
    ACE_TEXT ("  Service_Config_DLL:_make_Service_Config_DLL() \"Test_Object_1_More\"")
#else
    ACE_TEXT ("<dynamic id=\"Test_Object_1_More\" type=\"Service_Object\">")
    ACE_TEXT ("  <initializer init=\"_make_Service_Config_DLL\" path=\"Service_Config_DLL\" params=\"Test_Object_1_More\"/>")
    ACE_TEXT ("</dynamic>")
#endif /* (ACE_USES_CLASSIC_SVC_CONF == 1) */
    ;

  static const ACE_TCHAR *svc_desc2 =
#if (ACE_USES_CLASSIC_SVC_CONF == 1)
    ACE_TEXT ("dynamic Test_Object_2_More Service_Object * ")
    ACE_TEXT ("  Service_Config_DLL:_make_Service_Config_DLL() \"Test_Object_2_More\"")
#else
    ACE_TEXT ("<dynamic id=\"Test_Object_2_More\" type=\"Service_Object\">")
    ACE_TEXT ("  <initializer init=\"_make_Service_Config_DLL\" path=\"Service_Config_DLL\" params=\"Test_Object_2_More\"/>")
    ACE_TEXT ("</dynamic>")
#endif /* (ACE_USES_CLASSIC_SVC_CONF == 1) */
    ;


  u_int error0 = error;

  // Ensure enough room for one in a own
  ACE_Service_Gestalt one (1, true);

  // Add two.
  // We cant simply rely on the fact that insertion fails, because it
  // is typical to have no easy way of getting detailed error
  // information from a parser.
  one.process_directive (svc_desc1);
  one.process_directive (svc_desc2);

  if (-1 == one.find (ACE_TEXT ("Test_Object_1_More"), 0, 0))
    {
      ++error;
      ACE_ERROR ((LM_ERROR, ACE_TEXT("Expected to have registered the first service\n")));
    }

  if (-1 != one.find (ACE_TEXT ("Test_Object_2_More"), 0, 0))
    {
      ++error;
      ACE_ERROR ((LM_ERROR, ACE_TEXT("Being able to add more than 1 service was not expected\n")));
    }

  if (error == error0)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT("Limits test completed successfully\n")));
  else
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT("Limits test failed\n")));
}


// @brief ??
void
testOrderlyInstantiation (int , ACE_TCHAR *[])
{
  for (u_int i = 0; i < VARIETIES; ++i)
    {
      Test_Singleton *s = Test_Singleton::instance (i);

      if (s == 0)
        {
          ++error;
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("instance () allocate failed!\n")));
        }
    }
}

int
run_main (int argc, ACE_TCHAR *argv[])
{
  ACE_START_TEST (ACE_TEXT ("Service_Config_Test"));

  testOrderlyInstantiation (argc, argv);
  testFailedServiceInit (argc, argv);
  testLoadingServiceConfFile (argc, argv);
  testLoadingServiceConfFileAndProcessNo (argc, argv);
  testLimits (argc, argv);

  ACE_END_TEST;
  return error == 0 ? 0 : 1;
}
