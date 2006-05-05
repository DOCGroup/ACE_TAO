// $Id$

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
testLoadingTwoOrbs (int , ACE_TCHAR *argv[])
{
  ACE_ARGV new_argv;

  ACE_DEBUG ((LM_DEBUG, "Looking for conf file %s\n", file_Service_Config_ORB_Test ()));

  // Process the Service Configurator directives in this test's
    if (new_argv.add (argv) == -1
      || new_argv.add (ACE_TEXT ("-f")) == -1
      || new_argv.add (file_Service_Config_ORB_Test ()) == -1)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT("ARGV initialization failed\n")));
      return -1;
    }

  if (ACE_Service_Config::instance() ->open (new_argv.argc (),
                                             new_argv.argv ()) == -1)
    {
      if (errno == ENOENT)
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("No service config file found\n")));
      else
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("Service Config open failed: %m\n")));

      return -1;
    }

  // Since the loaded services start their own threads, wait until all of them
  // are done to avoid pulling the rug under their feet.
  ACE_Thread_Manager::instance ()->wait ();
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  return testLoadingTwoOrbs (argc, argv);
}
