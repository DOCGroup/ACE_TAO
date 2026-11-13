// The following is required to be able to access
// ace_svc_desc_TAO_*_Parser, below
#include "tao/CORBALOC_Parser.h"
#include "tao/CORBANAME_Parser.h"

#include "ace/ARGV.h"
#include "ace/Dynamic_Service.h"
#include "ace/Intrusive_Auto_Ptr.h"
#include "ace/Service_Config.h"
#include "Service_Configuration_Per_ORB.h"

int
testCompatibility (int, ACE_TCHAR *[])
{
  ACE_TRACE ("testCompatibility");

  ACE_Intrusive_Auto_Ptr<ACE_Service_Gestalt_Test> const isolated {
    new ACE_Service_Gestalt_Test ()
  };

  // Use the "old" interface
  if (ACE_Service_Config::process_directive (ace_svc_desc_TAO_CORBANAME_Parser) != 0)
    ACE_ERROR_RETURN ((LM_DEBUG,
                       ACE_TEXT ("Failed to process %s\n"),
                       ace_svc_desc_TAO_CORBANAME_Parser),
                      -1);

  if (ACE_Service_Config::process_directive (ace_svc_desc_TAO_CORBALOC_Parser) != 0)
    ACE_ERROR_RETURN ((LM_DEBUG,
                       ACE_TEXT ("Failed to process %s\n"),
                       ace_svc_desc_TAO_CORBALOC_Parser),
                      -1);

  {
    ACE_Intrusive_Auto_Ptr<ACE_Service_Gestalt_Test> const one {
      new ACE_Service_Gestalt_Test ()
    };

    ACE_TCHAR const * const corbaname = ACE_TEXT ("CORBANAME_Parser");
    ACE_Service_Object * const corbaname_service =
      ACE_Dynamic_Service<ACE_Service_Object>::instance (one.get (), corbaname);
    if (corbaname_service == nullptr)
      ACE_ERROR_RETURN ((LM_DEBUG,
                         ACE_TEXT ("Expected %s in the global repository\n"),
                         corbaname),
                        -1);

    ACE_TCHAR const * const corbaloc = ACE_TEXT ("CORBALOC_Parser");
    ACE_Service_Object * const corbaloc_service =
      ACE_Dynamic_Service<ACE_Service_Object>::instance (one.get (), corbaloc);
    if (corbaloc_service == nullptr)
      ACE_ERROR_RETURN ((LM_DEBUG,
                         ACE_TEXT ("Expected %s in the global repository\n"),
                         corbaloc),
                        -1);
  }

  ACE_TCHAR const * const corbaname = ACE_TEXT ("CORBANAME_Parser");
  ACE_Service_Object * const p20 =
    ACE_Dynamic_Service<ACE_Service_Object>::instance (isolated.get (), corbaname, true);
  if (p20 != nullptr)
    ACE_ERROR_RETURN ((LM_DEBUG,
                       ACE_TEXT ("Expected %s to be absent from the isolated gestalt\n"),
                       corbaname),
                      -1);

  ACE_TCHAR const * const corbaloc = ACE_TEXT ("CORBALOC_Parser");
  ACE_Service_Object * const p21 =
    ACE_Dynamic_Service<ACE_Service_Object>::instance (isolated.get (), corbaloc, true);
  if (p21 != nullptr)
    ACE_ERROR_RETURN ((LM_DEBUG,
                       ACE_TEXT ("Expected %s to be absent from the isolated gestalt\n"),
                       corbaloc),
                      -1);

  return 0;
}

/// @brief Test command line processing

int
testCommandLineDirectives (int, ACE_TCHAR *[])
{
  ACE_TRACE ("testCommandLineDirectives");

  ACE_ARGV new_argv;
  if (new_argv.add (ACE_TEXT ("-f")) == -1
      || new_argv.add (ACE_TEXT ("-S")) == -1
      || new_argv.add (ACE_TEXT ("d1")) == -1
      || new_argv.add (ACE_TEXT ("-S")) == -1
      || new_argv.add (ACE_TEXT ("d2")) == -1)
    {
      ACE_ERROR_RETURN ((LM_DEBUG, ACE_TEXT ("Unable to create an argv\n")), -1);
    }

  ACE_Service_Gestalt_Test g {5};
  if (g.parse_args (new_argv.argc (),
                    new_argv.argv ()) == -1
      && errno != ENOENT)
    {
      ACE_ERROR_RETURN ((LM_DEBUG, ACE_TEXT ("Failed to parse the argv\n")), -1);
    }

  ACE_DEBUG ((LM_DEBUG, "\tg.command_line_directives_count () -> %d\n",
              g.command_line_directives_count ()));

  if (g.command_line_directives_count () != 2)
    ACE_ERROR_RETURN ((LM_DEBUG,
                       ACE_TEXT ("Expected %d, but found %d command line directives\n"),
                       2,
                       g.command_line_directives_count ()),
                      -1);

  return 0;
}

/// @brief Test the helper components used to implement the temporary
/// substitution of the repository currently used as "global" for the
/// sake of registering static services, which are dependent on a dynamic
/// service

int
testTSSGestalt (int, ACE_TCHAR *[])
{
  ACE_TRACE ("testTSSGestalt");

  ACE_Intrusive_Auto_Ptr<ACE_Service_Gestalt_Test> one {
    new ACE_Service_Gestalt_Test (10)
  };

  ACE_Intrusive_Auto_Ptr<ACE_Service_Gestalt> const global_instance {
    ACE_Service_Config::instance ()
  };

  if (global_instance == one.get ())
    ACE_ERROR_RETURN ((LM_DEBUG, ACE_TEXT ("Invalid global gestalt\n")), -1);

  {
    // Make one be the ACE_Service_Config::instance () ...
    ACE_Service_Config_Guard const temporary (one.get ());

    ACE_Intrusive_Auto_Ptr<ACE_Service_Gestalt> const global_instance2 {
      ACE_Service_Config::instance ()
    };

    if (global_instance == global_instance2)
      ACE_ERROR_RETURN ((LM_DEBUG, ACE_TEXT ("Expected to see a different global from before\n")), -1);

    if (global_instance2 != one.get ())
      ACE_ERROR_RETURN ((LM_DEBUG, ACE_TEXT ("Expected one to be the global gestalt instance\n")), -1);

    // The guard is dead! Long live the global gestalt that was previously global!
  }

  if (global_instance != ACE_Service_Config::instance ())
    ACE_ERROR_RETURN ((LM_DEBUG, ACE_TEXT ("Expected the original global gestalt\n")), -1);

  if (global_instance == one.get ())
    ACE_ERROR_RETURN ((LM_DEBUG, ACE_TEXT ("Found the wrong instance is still being global\n")), -1);

  return 0;
}

/// @brief The main driver.

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  int result {};
  result |= testCompatibility (argc, argv);
  result |= testCommandLineDirectives (argc, argv);
  result |= testTSSGestalt (argc, argv);
  return result;
}
