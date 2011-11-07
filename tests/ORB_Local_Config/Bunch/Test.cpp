// $Id$

// The following is required to be able to access
// ace_svc_desc_TAO_*_Parser, below
#include "tao/CORBALOC_Parser.h"
#include "tao/CORBANAME_Parser.h"

#include "ace/ARGV.h"
#include "ace/Service_Config.h"
#include "ace/Intrusive_Auto_Ptr.h"
#include "ace/Dynamic_Service.h"
#include "Service_Configuration_Per_ORB.h"

int
testCompatibility (int , ACE_TCHAR *[])
{
  ACE_TRACE ("testCompatibility");

  // This uses the same default ACE_Service_Repository
  ACE_Intrusive_Auto_Ptr<ACE_Service_Gestalt_Test> glob (new ACE_Service_Gestalt_Test());

  // Use the "old" interface
  if (0 != ACE_Service_Config::process_directive
      (ace_svc_desc_TAO_CORBANAME_Parser))
    ACE_ERROR_RETURN ((LM_DEBUG, ACE_TEXT("Failed to process %s\n"), ace_svc_desc_TAO_CORBANAME_Parser), -1);

  if(0 != ACE_Service_Config::process_directive
     (ace_svc_desc_TAO_CORBALOC_Parser))
    ACE_ERROR_RETURN ((LM_DEBUG, ACE_TEXT("Failed to process %s\n"), ace_svc_desc_TAO_CORBALOC_Parser), -1);

  const ACE_TCHAR * svcname = 0;

  {
    // This uses the same default ACE_Service_Repository
    ACE_Intrusive_Auto_Ptr<ACE_Service_Gestalt_Test> one (new ACE_Service_Gestalt_Test());

    svcname = ACE_TEXT ("CORBANAME_Parser");

    ACE_Service_Object* p20 = ACE_Dynamic_Service<ACE_Service_Object>::instance (one.get (), svcname);
    if (p20 == 0)
      ACE_ERROR_RETURN ((LM_DEBUG,
                         ACE_TEXT("Expected %s locally, in one\n"), svcname), -1);

    svcname = ACE_TEXT ("CORBALOC_Parser");

    ACE_Service_Object* p21 =  ACE_Dynamic_Service<ACE_Service_Object>::instance (one.get (), svcname);
    if ((p21 == 0))
      ACE_ERROR_RETURN ((LM_DEBUG,
                         ACE_TEXT("Expected %s locally, in one\n"), svcname), -1);

    // Exiting this scope should fini all services in the glob ...
  }

  svcname = ACE_TEXT ("CORBANAME_Parser");

  ACE_Service_Object* p20 =  ACE_Dynamic_Service<ACE_Service_Object>::instance (glob.get (), svcname);
  if ((p20 != 0))
    ACE_ERROR_RETURN ((LM_DEBUG,
                       ACE_TEXT("Expected %s globally, too\n"), svcname), -1);

  svcname = ACE_TEXT ("CORBALOC_Parser");

  ACE_Service_Object* p21 = ACE_Dynamic_Service<ACE_Service_Object>::instance (glob.get (), svcname);
  if ((p21 != 0))
    ACE_ERROR_RETURN ((LM_DEBUG,
                       ACE_TEXT("Expected %s globally, too\n"), svcname), -1);

  return 0;
}

// @brief Test commandline processing

int
testCommandLineDirectives (int , ACE_TCHAR *[])
{
  ACE_TRACE ("testCommandLineDirectives");

  ACE_ARGV new_argv;
  if (new_argv.add (ACE_TEXT ("-f")) == -1
      || new_argv.add (ACE_TEXT ("-S")) == -1
      || new_argv.add (ACE_TEXT ("d1")) == -1
      || new_argv.add (ACE_TEXT ("-S")) == -1
      || new_argv.add (ACE_TEXT ("d2")) == -1)
    {
      ACE_ERROR_RETURN ((LM_DEBUG, ACE_TEXT("Unable to create an argv\n")), -1);
    }

  ACE_Service_Gestalt_Test g(5);
  if (g.parse_args (new_argv.argc (),
                    new_argv.argv ()) == -1
      && errno != ENOENT)
    {
      ACE_ERROR_RETURN ((LM_DEBUG, ACE_TEXT("Failed to parse the argv\n")), -1);
    }

  ACE_DEBUG ((LM_DEBUG, "\tg.command_line_directives_count () -> %d\n",
              g.command_line_directives_count ()));

  if (2 != g.command_line_directives_count ())
    ACE_ERROR_RETURN ((LM_DEBUG,
                       ACE_TEXT("Expected %d, but found %d command line directives\n"),
                       2,
                       g.command_line_directives_count ()),
                      -1);

  return 0;
}

// @brief Test the helper components used to implement the temporary
// substitution of the repository currently used as "global" for the
// sake of registering static services, which are dependent on a dynamic
// service

int
testTSSGestalt (int , ACE_TCHAR *[])
{
  ACE_TRACE ("testTSSGestalt");

  ACE_Service_Gestalt_Test one (10);  // Localized ones go here

  ACE_Intrusive_Auto_Ptr<ACE_Service_Gestalt> global_instance = ACE_Service_Config::instance ();

  // Sanity check
  if (global_instance == &one)
      ACE_ERROR_RETURN ((LM_DEBUG, ACE_TEXT("Invalid global gestalt\n")), -1);

  {
    // Make one be the ACE_Service_Config::instance () ...
    ACE_Service_Config_Guard temporary (&one);

    ACE_Intrusive_Auto_Ptr<ACE_Service_Gestalt> global_instance2 = ACE_Service_Config::instance ();

    if (global_instance == global_instance2)
      ACE_ERROR_RETURN ((LM_DEBUG, ACE_TEXT("Expected to see a different global from before\n")), -1);

    if (global_instance2 != &one)
      ACE_ERROR_RETURN ((LM_DEBUG, ACE_TEXT("Expected one to be the global gestalt instance\n")), -1);

    // The guard is dead! Long live the global gestalt that was previously global!
  }

  if (global_instance != ACE_Service_Config::instance ())
      ACE_ERROR_RETURN ((LM_DEBUG, ACE_TEXT("Expected the original global gestalt\n")), -1);

  if (global_instance == &one)
      ACE_ERROR_RETURN ((LM_DEBUG, ACE_TEXT("Found the wrong instance is still being global\n")), -1);

  return 0;
}

// @brief the main driver

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  return
    testCompatibility (argc, argv)
    && testCommandLineDirectives (argc, argv)
    && testTSSGestalt(argc, argv);
}
