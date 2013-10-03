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
#include "ace/OS_NS_Thread.h"
#include "ace/Log_Msg.h"
#include "ace/Object_Manager.h"
#include "ace/Service_Config.h"
#include "ace/Service_Object.h"
#include "ace/Service_Repository.h"
#include "ace/Service_Types.h"
#include "ace/Reactor.h"
#include "ace/Thread_Manager.h"
#include "ace/ARGV.h"

static const u_int VARIETIES = 3;

static u_int error = 0;

class MyRepository : public ACE_Service_Repository
{
public:
  array_type& array ()
  {
    return service_array_;
  }
};

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
test_singleton_cleanup (void *object, void *)
{
  // We can't reliably use ACE_Log_Msg in a cleanup hook.  Yet.
  /* ACE_DEBUG ((LM_DEBUG, "cleanup %d\n", (u_short) param)); */

  delete (Test_Singleton *) object;
}

Test_Singleton *
Test_Singleton::instance (u_short variety)
{
  static Test_Singleton *instances[VARIETIES] = { 0 };

  if (instances[variety] == 0)
    {
      ACE_NEW_RETURN (instances[variety],
                      Test_Singleton (variety),
                      0);
    }

  ACE_Object_Manager::at_exit (instances[variety],
                               test_singleton_cleanup,
                  reinterpret_cast<void *> (static_cast<size_t> (variety)),
                  "Test_Singleton");
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
      ++error;
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Failed init test should have returned 1; ")
                  ACE_TEXT ("returned %d instead\n"),
                  error_count));
    }

  // Try to find the service; it should not be there.
  ACE_Service_Type const *svcp = 0;
  if (-1 != ACE_Service_Repository::instance ()->find (ACE_TEXT ("Refuses_Svc"),
                                                       &svcp))
    {
      ++error;
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
  u_int error0 = error;
  ACE_ARGV new_argv;

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT("Starting testLoadingServiceConfFileAndProcessNo\n")));

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

  ACE_TCHAR pid_file_name [MAXPATHLEN];
#if defined (TEST_DIR)
  ACE_OS::strcpy (pid_file_name, TEST_DIR);
  ACE_OS::strcat (pid_file_name, ACE_DIRECTORY_SEPARATOR_STR);
  ACE_OS::strcat (pid_file_name, ACE_TEXT ("Service_Config_Test.pid"));
#else
  ACE_OS::strcpy (pid_file_name, ACE_TEXT ("Service_Config_Test.pid"));
#endif
  ACE_TCHAR svc_conf_file_name [MAXPATHLEN];
#if defined (TEST_DIR)
  ACE_OS::strcpy (svc_conf_file_name, TEST_DIR);
  ACE_OS::strcat (svc_conf_file_name, ACE_DIRECTORY_SEPARATOR_STR);
  ACE_OS::strcat (svc_conf_file_name, svc_conf);
#else
  ACE_OS::strcpy (svc_conf_file_name, svc_conf);
#endif

  // Process the Service Configurator directives in this test's Making
  // sure we have more than one option with an argument, to capture
  // any errors caused by "reshuffling" of the options.
  if (new_argv.add (argv) == -1
              || new_argv.add (ACE_TEXT ("-d")) == -1
              || new_argv.add (ACE_TEXT ("-k")) == -1
              || new_argv.add (ACE_TEXT ("xxx")) == -1
              || new_argv.add (ACE_TEXT ("-p")) == -1
              || new_argv.add (pid_file_name) == -1
              || new_argv.add (ACE_TEXT ("-f")) == -1
              || new_argv.add (svc_conf_file_name) == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("line %l %p\n"),
                  ACE_TEXT ("new_argv.add")));
      ++error;
    }

  // We need this scope to make sure that the destructor for the
  // <ACE_Service_Config> gets called.
  ACE_Service_Config daemon;

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT("Starting daemon using %s\n"), new_argv.buf ()));

  if (daemon.open (new_argv.argc (), new_argv.argv ()) == -1 &&
      errno != ENOENT)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("line %l %p\n"),
                  ACE_TEXT ("daemon.open")));
      ++error;
    }

  ACE_Time_Value tv (argc > 1 ? ACE_OS::atoi (argv[1]) : 2);

  if (ACE_Reactor::instance()->run_reactor_event_loop (tv) == -1)
    {
      ++error;
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("line %l %p\n"),
                  ACE_TEXT ("run_reactor_event_loop")));
    }

  // Wait for all threads to complete.
  ACE_Thread_Manager::instance ()->wait ();

  if (error == error0)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT("testLoadingServiceConfFileAndProcessNo completed successfully\n")));
  else
    ACE_ERROR ((LM_ERROR, ACE_TEXT("testLoadingServiceConfFileAndProcessNo test failed\n")));
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
  if (new_argv.add (argv) == -1
              || new_argv.add (ACE_TEXT ("-f")) == -1
              || new_argv.add (svc_conf) == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("line %l %p\n"),
                  ACE_TEXT ("new_argv.add")));
      ++error;
    }

  // We need this scope to make sure that the destructor for the
  // <ACE_Service_Config> gets called.
  ACE_Service_Config daemon;

  if (daemon.open (new_argv.argc (), new_argv.argv ()) == -1)
    {
      if (errno == ENOENT)
        ACE_DEBUG ((LM_WARNING,
                    ACE_TEXT ("ACE_Service_Config::open: %p\n"),
                    svc_conf));
      else
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("ACE_Service_Config::open: %p\n"),
                    ACE_TEXT ("error")));
    }

  ACE_Time_Value tv (argc > 1 ? ACE_OS::atoi (argv[1]) : 2);

  if (ACE_Reactor::instance()->run_reactor_event_loop (tv) == -1)
    {
      ++error;
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("line %l %p\n"),
                  ACE_TEXT ("run_reactor_event_loop")));
    }

  // Wait for all threads to complete.
  ACE_Thread_Manager::instance ()->wait ();
}


// Loading and unloading the ACE logger service should not smash singletons.
void
testUnloadingACELoggingStrategy (int, ACE_TCHAR *[])
{
  static const ACE_TCHAR *load_logger =
#if (ACE_USES_CLASSIC_SVC_CONF == 1)
    ACE_TEXT ("dynamic Logger Service_Object * ")
    ACE_TEXT ("  ACE:_make_ACE_Logging_Strategy() \"\"")
#else
    ACE_TEXT ("<dynamic id=\"Logger\" type=\"Service_Object\">")
    ACE_TEXT ("  <initializer init=\"_make_ACE_Logging_Strategy\" ")
    ACE_TEXT ("    path=\"ACE\" params=\"\"/>")
    ACE_TEXT ("</dynamic>")
#endif /* (ACE_USES_CLASSIC_SVC_CONF == 1) */
    ;

  static const ACE_TCHAR *unload_logger =
#if (ACE_USES_CLASSIC_SVC_CONF == 1)
    ACE_TEXT ("remove Logger")
#else
    ACE_TEXT ("<remove id=\"Logger\" />");
#endif /* (ACE_USES_CLASSIC_SVC_CONF == 1) */
    ;

  ACE_Reactor *r1 = ACE_Reactor::instance();

  // Ensure no errors are logged while searching for a valid ACE lib name;
  // these skew the scoreboard results.
  u_long mask = ACE_LOG_MSG->priority_mask (ACE_Log_Msg::PROCESS);
  ACE_LOG_MSG->priority_mask (mask & ~LM_ERROR, ACE_Log_Msg::PROCESS);
  ACE_DEBUG ((LM_DEBUG, "Was %x, now %x\n", mask, mask & ~LM_ERROR));
  int error_count = ACE_Service_Config::process_directive (load_logger);
  ACE_LOG_MSG->priority_mask (mask);
  if (error_count != 0)
    {
      ++error;
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Load ACE Logger should have returned 0; ")
                  ACE_TEXT ("returned %d instead\n"),
                  error_count));
    }
  ACE_Service_Config::process_directive (unload_logger);

  ACE_Reactor *r2 = ACE_Reactor::instance ();
  if (r1 != r2)
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Reactor before %@, after %@\n"), r1, r2));
}


// @brief The size of a repository is unlimited and can be exceeded
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

  // Ensure enough room for one in a own, the repository can extend
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

  if (-1 == one.find (ACE_TEXT ("Test_Object_2_More"), 0, 0))
    {
      ++error;
      ACE_ERROR ((LM_ERROR, ACE_TEXT("Expected to have registered the second service\n")));
    }

  ACE_Service_Repository_Iterator sri (*one.current_service_repository (), 0);

  size_t index = 0;
  for (const ACE_Service_Type *sr;
       sri.next (sr) != 0;
       sri.advance ())
    {
      if (index == 0 && ACE_OS::strcmp (sr->name(), ACE_TEXT ("Test_Object_1_More")) != 0)
        {
          ++error;
          ACE_ERROR ((LM_ERROR, ACE_TEXT("Service 1 is wrong\n")));
        }
      if (index == 1 && ACE_OS::strcmp (sr->name(), ACE_TEXT ("Test_Object_2_More")) != 0)
        {
          ++error;
          ACE_ERROR ((LM_ERROR, ACE_TEXT("Service 2 is wrong\n")));
        }
      ++index;
    }

  // Test close
  one.current_service_repository ()->close();

  if (one.current_service_repository ()->current_size () != 0)
    {
      ++error;
      ACE_ERROR ((LM_ERROR, ACE_TEXT("Size of repository should be 0\n")));
    }

  if (error == error0)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT("Limits test completed successfully\n")));
  else
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT("Limits test failed\n")));
}

void
testrepository (int, ACE_TCHAR *[])
{
  MyRepository repository;
  ACE_DLL handle;
  ACE_Service_Type s0 (ACE_TEXT ("0"), 0, handle, false);
  ACE_Service_Type s1 (ACE_TEXT ("1"), 0, handle, false);
  ACE_Service_Type s2 (ACE_TEXT ("2"), 0, handle, false);
  ACE_Service_Type s3 (ACE_TEXT ("3"), 0, handle, false);
  ACE_Service_Type* result = 0;
  repository.insert (&s0);
  if (repository.current_size () != 1)
    {
      ++error;
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Repository was wrong size %d\n"),
                  repository.current_size ()));
    }
  repository.insert (&s1);
  if (repository.current_size () != 2)
    {
      ++error;
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Repository was wrong size %d\n"),
                  repository.current_size ()));
    }
  repository.insert (&s2);
  if (repository.current_size () != 3)
    {
      ++error;
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Repository was wrong size %d\n"),
                  repository.current_size ()));
    }
  if (repository.remove (ACE_TEXT ("1"), &result) != 0)
    {
      ++error;
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Remove failed\n")));
    }
  if (repository.current_size () != 3)
    {
      ++error;
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Repository was wrong size %d\n"),
                  repository.current_size ()));
    }
  if (repository.array ()[1] != 0)
    {
      ++error;
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Element 1 not zero\n"),
                  repository.current_size ()));
    }
  repository.insert (&s3);
  if (repository.current_size () != 4)
    {
      ++error;
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Repository was wrong size %d\n"),
                  repository.current_size ()));
    }
  repository.remove (ACE_TEXT ("0"), &result);
  if (repository.remove (ACE_TEXT ("1"), &result) != -1)
    {
      ++error;
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Double remove didn't return -1\n")));
    }
  repository.remove (ACE_TEXT ("2"), &result);
  repository.remove (ACE_TEXT ("3"), &result);
  if (repository.current_size () != 4)
    {
      ++error;
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Repository was wrong size %d\n"),
                  repository.current_size ()));
    }
  repository.close ();
  if (repository.current_size () != 0)
    {
      ++error;
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Repository was wrong size %d\n"),
                  repository.current_size ()));
    }
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

// This test verifies that services loaded by a normal ACE startup can be
// located from a thread spawned outside of ACE's control.
//
// To do this, we need a native thread entry and, thus, it needs special care
// for each platform type. Feel free to add more platforms as needed here and
// in main() where the test is called.
#if defined (ACE_HAS_WTHREADS) || defined (ACE_HAS_PTHREADS)
#  if defined (ACE_HAS_WTHREADS)
extern "C" unsigned int __stdcall
#  else
extern "C" ACE_THR_FUNC_RETURN
#  endif
nonacethreadentry (void *args)
{
  ACE_Log_Msg::inherit_hook (0, *(ACE_OS_Log_Msg_Attributes *)args);

  if (ACE_Service_Config::instance()->find(ACE_TEXT("Test_Object_1_Thr")) != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("In thr %t cannot find Test_Object_1_Thr ")
                  ACE_TEXT ("via ACE_Service_Config\n")));
      ++error;
    }
  else
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("In thr %t, located Test_Object_1_Thr ")
                ACE_TEXT ("via ACE_Service_Config\n")));

  if (0 != ACE_Service_Repository::instance()->find
      (ACE_TEXT("Test_Object_1_Thr")))
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("In thr %t cannot find Test_Object_1_Thr ")
                  ACE_TEXT ("via ACE_Service_Repository\n")));
      ++error;
    }
  else
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("In thr %t, located Test_Object_1_Thr ")
                ACE_TEXT ("via ACE_Service_Repository\n")));

  return 0;
}

void
testNonACEThread ()
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Beginning non-ACE thread lookup test\n")));

  static const ACE_TCHAR *svc_desc =
#if (ACE_USES_CLASSIC_SVC_CONF == 1)
    ACE_TEXT ("dynamic Test_Object_1_Thr Service_Object * ")
    ACE_TEXT ("  Service_Config_DLL:_make_Service_Config_DLL() \"Test_Object_1_Thr\"")
#else
    ACE_TEXT ("<dynamic id=\"Test_Object_1_Thr\" type=\"Service_Object\">")
    ACE_TEXT ("  <initializer init=\"_make_Service_Config_DLL\" path=\"Service_Config_DLL\" params=\"Test_Object_1_Thr\"/>")
    ACE_TEXT ("</dynamic>")
#endif /* (ACE_USES_CLASSIC_SVC_CONF == 1) */
    ;

  static const ACE_TCHAR *svc_remove =
#if (ACE_USES_CLASSIC_SVC_CONF == 1)
    ACE_TEXT ("remove Test_Object_1_Thr")
#else
    ACE_TEXT ("<remove id=\"Test_Object_1_Thr\"/>")
    ACE_TEXT ("</remove>")
#endif /* (ACE_USES_CLASSIC_SVC_CONF == 1) */
    ;

  if (-1 == ACE_Service_Config::process_directive (svc_desc))
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("%p\n"),
                  ACE_TEXT ("Error loading service")));
      ++error;
      return;
    }

  // Allow the spawned thread to contribute to the logging output.
  ACE_OS_Log_Msg_Attributes log_msg_attrs;
  ACE_Log_Msg::init_hook (log_msg_attrs);

  u_int errors_before = error;

#if defined (ACE_HAS_WTHREADS) && !defined (ACE_HAS_WINCE)
  HANDLE thr_h = (HANDLE)_beginthreadex (0,
                                         0,
                                         &nonacethreadentry,
                                         &log_msg_attrs,
                                         0,
                                         0);
  if (thr_h == 0)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("_beginthreadex")));
      ++error;
    }
  else
    {
      WaitForSingleObject (thr_h, INFINITE);
      CloseHandle (thr_h);
    }
#elif defined (ACE_HAS_PTHREADS)
  pthread_t thr_id;
  int status = pthread_create (&thr_id, 0, nonacethreadentry, &log_msg_attrs);
  if (status != 0)
    {
      errno = status;
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("pthread_create")));
      ++error;
    }
  else
    {
      pthread_join (thr_id, 0);
    }
#endif

  if (error != errors_before)  // The test failed; see if we can still see it
    {
      if (0 != ACE_Service_Config::instance()->find (ACE_TEXT ("Test_Object_1_Thr")))
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("Main thr %t cannot find Test_Object_1_Thr\n")));
          ++error;
        }
      else
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("Main thr %t DOES find Test_Object_1_Thr\n")));
    }

  if (-1 == ACE_Service_Config::process_directive (svc_remove))
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("%p\n"),
                  ACE_TEXT ("Error removing service")));
      ++error;
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Non-ACE thread lookup test completed\n")));
    }
}
#endif /* ACE_HAS_WTHREADS || ACE_HAS_PTHREADS */

int
run_main (int argc, ACE_TCHAR *argv[])
{
  ACE_START_TEST (ACE_TEXT ("Service_Config_Test"));

  testOrderlyInstantiation (argc, argv);
  testFailedServiceInit (argc, argv);
  testLoadingServiceConfFile (argc, argv);
  testLoadingServiceConfFileAndProcessNo (argc, argv);
  testUnloadingACELoggingStrategy (argc, argv);
  testLimits (argc, argv);
  testrepository (argc, argv);
#if defined (ACE_HAS_WTHREADS) || defined (ACE_HAS_PTHREADS)
  unsigned int n_threads = 64;
#if defined (ACE_DEFAULT_THREAD_KEYS)
  n_threads = 2 * ACE_DEFAULT_THREAD_KEYS;
#endif
  // Test with a large amount of threads to determine whether
  // TSS works correctly with non ACE threads
  for (unsigned int i = 0 ; i < n_threads; i++)
    {
      testNonACEThread();
    }
#endif

  ACE_END_TEST;
  return error;
}
