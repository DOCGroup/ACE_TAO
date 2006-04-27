// $Id$

#include "ace/ARGV.h"
#include "ace/Thread_Manager.h"

#include "Service_Configuration_Per_ORB.h"

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

