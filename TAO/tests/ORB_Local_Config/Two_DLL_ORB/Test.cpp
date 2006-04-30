// $Id$

/// It's a test - we need ACE_ASSERT
#ifdef ACE_NDEBUG
#  undef ACE_NDEBUG
#endif

#include "ace/ARGV.h"
#include "ace/Thread_Manager.h"

#include "Service_Configuration_Per_ORB.h"

const ACE_TCHAR*
file_Service_Config_ORB_Test ()
{
  // ASCII (UTF-8) encoded Service Configurator file.
  static const ACE_TCHAR svc_conf[] =
    ACE_TEXT ("Service_Config_ORB_Test")
    ACE_TEXT (ACE_DEFAULT_SVC_CONF_EXT);

  return svc_conf;
}

// @brief Loading a dynamic services in a local repository, which
// initializes its own ORB

int
run_main (int , ACE_TCHAR *argv[])
{
  ACE_ARGV new_argv;

  ACE_DEBUG ((LM_DEBUG, "Looking for conf file %s\n", file_Service_Config_ORB_Test ()));

  // Process the Service Configurator directives in this test's
  ACE_ASSERT (new_argv.add (argv) != -1
              && new_argv.add (ACE_TEXT ("-f")) != -1
              && new_argv.add (file_Service_Config_ORB_Test ()) != -1);

  ACE_ASSERT (ACE_Service_Config::instance() ->open (new_argv.argc (),
                                                     new_argv.argv ()) != -1 || errno == ENOENT);


  // Since the loaded services start their own threads, wait until all of them
  // are done to avoid pulling the rug under their feet.
  ACE_Thread_Manager::instance ()->wait ();
  return 0;
}

