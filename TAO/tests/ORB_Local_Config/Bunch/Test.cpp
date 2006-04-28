// $Id$

#include "tao/CORBANAME_Parser.h"
#include "tao/CORBALOC_Parser.h"
#include "tao/IIOP_Factory.h"

#include "ace/ARGV.h"
#include "ace/Dynamic_Service.h"

ACE_RCSID (tests, server, "$Id$")


#include "Service_Configuration_Per_ORB.h"

// @brief The "new" interfaces must be compatible with the "old" ones

int
testCompatibility (int , ACE_TCHAR *[])
{
  ACE_TRACE ("testCompatibility");

  // This uses the same default ACE_Service_Repository
  ACE_Service_Gestalt_Test glob;

  // Use the "old" interface
  ACE_ASSERT (0 == ACE_Service_Config::process_directive
                     (ace_svc_desc_TAO_CORBANAME_Parser));
  ACE_ASSERT (0 == ACE_Service_Config::process_directive
                     (ace_svc_desc_TAO_CORBALOC_Parser));

  {
    // This uses the same default ACE_Service_Repository
    ACE_Service_Gestalt_Test one;

    TAO_CORBANAME_Parser* p20 =
      ACE_Dynamic_Service<TAO_CORBANAME_Parser>::instance (&one, "CORBANAME_Parser");
    ACE_ASSERT ((p20 != 0));

    TAO_CORBALOC_Parser* p21 =
      ACE_Dynamic_Service<TAO_CORBALOC_Parser>::instance (&one, "CORBALOC_Parser");
    ACE_ASSERT ((p21 != 0));

    ACE_DEBUG ((LM_DEBUG, "\tglobal.services_count () -> %d\n",
                one.services_count ()));
    ACE_ASSERT (one.services_count () > 2);

    // Exiting this scope should fini all services ...
  }

  TAO_CORBANAME_Parser* p20 =
    ACE_Dynamic_Service<TAO_CORBANAME_Parser>::instance ("CORBANAME_Parser");
  ACE_ASSERT ((p20 == 0));

  TAO_CORBALOC_Parser* p21 =
    ACE_Dynamic_Service<TAO_CORBALOC_Parser>::instance ("CORBALOC_Parser");
  ACE_ASSERT ((p21 == 0));



  p20 = ACE_Dynamic_Service<TAO_CORBANAME_Parser>::instance (&glob, "CORBANAME_Parser");
  ACE_ASSERT ((p20 == 0));

  p21 =ACE_Dynamic_Service<TAO_CORBALOC_Parser>::instance (&glob, "CORBALOC_Parser");
  ACE_ASSERT ((p21 == 0));

  return 0;
}

// @brief Test commandline processing


int
testCommandLineDirectives (int , ACE_TCHAR *[])
{
  ACE_TRACE ("testCommandLineDirectives");

  ACE_ARGV new_argv;
  ACE_ASSERT (new_argv.add (ACE_TEXT ("-f")) != -1
              && new_argv.add (ACE_TEXT ("-S")) != -1
              && new_argv.add (ACE_TEXT ("d1")) != -1
              && new_argv.add (ACE_TEXT ("-S")) != -1
              && new_argv.add (ACE_TEXT ("d2")) != -1);

  ACE_Service_Gestalt_Test g(5);
  ACE_ASSERT (g.parse_args (new_argv.argc (),
                            new_argv.argv ()) != -1
              || errno == ENOENT);

  ACE_DEBUG ((LM_DEBUG, "\tg.command_line_directives_count () -> %d\n",
              g.command_line_directives_count ()));

  ACE_ASSERT (2 == g.command_line_directives_count ());
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

  ACE_Service_Gestalt *global_instance = ACE_Service_Config::instance ();

  ACE_ASSERT (global_instance == ACE_Service_Config::instance ());
  ACE_ASSERT (global_instance != &one);

  {
    ACE_Service_Config_Guard temporary (&one);

    ACE_Service_Gestalt *global_instance2 = ACE_Service_Config::instance ();

    ACE_ASSERT (global_instance != global_instance2);
    ACE_ASSERT (global_instance2 == &one);
  }

  ACE_ASSERT (global_instance == ACE_Service_Config::instance ());
  ACE_ASSERT (global_instance != &one);

  return 0;
}



// @brief the main driver

int
run_main(int argc, ACE_TCHAR *argv[])
{
  testCompatibility (argc, argv);
  testCommandLineDirectives (argc, argv);
  testTSSGestalt(argc, argv);
  return 0;
}




