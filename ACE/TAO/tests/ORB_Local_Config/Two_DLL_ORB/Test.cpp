// $Id$

#include "ace/ARGV.h"
#include "ace/Thread_Manager.h"

#include "Service_Configuration_Per_ORB.h"

// Uncomment this line to test the SSLIOP scenario, as described in
//the README.
// #define TEST_WITH_SSLIOP

const ACE_TCHAR*
config_file_name ()
{
  // ASCII (UTF-8) encoded Service Configurator file.
  static const ACE_TCHAR svc_conf[] =
#ifdef TEST_WITH_SSLIOP
    ACE_TEXT ("primary-ssl")
#else
    ACE_TEXT ("primary-csd")
#endif
    ACE_TEXT (ACE_DEFAULT_SVC_CONF_EXT);

  return svc_conf;
}


// @brief Loading a dynamic services in a local repository, which
// initializes its own ORB

int
testLoadingTwoOrbs (int , ACE_TCHAR *argv[])
{
  ACE_ARGV new_argv;

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Looking for conf file %s\n"),
              config_file_name ()));

  // Process the Service Configurator directives in this test's
  if (new_argv.add (argv) == -1
      || new_argv.add (ACE_TEXT ("-f")) == -1
      || new_argv.add (config_file_name ()) == -1)
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
