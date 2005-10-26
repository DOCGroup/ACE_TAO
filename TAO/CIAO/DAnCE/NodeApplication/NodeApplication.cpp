// $Id$
#include "NodeApplication_Core.h"

int
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      ACE_DEBUG ((LM_DEBUG, "*** Starting NodeApplication\n"));

      CIAO::NodeApplication_Options nodeapp_options;

      if (nodeapp_options.parse_args (argc, argv) != 0)
        return -1;

      CIAO::NodeApplication_Core nodeapp_core (nodeapp_options);

      if (nodeapp_core.startup (argc, argv) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Unable to start up the NodeApplication_Core\n"), -1);

      // Run the main event loop for the ORB.
      nodeapp_core.run_orb ();
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "NodeApp::main\t\n");
      return 1;
    }
  ACE_ENDTRY;

  ACE_DEBUG ((LM_DEBUG, "This NodeApplication is quitting now!\n"));
  return 0;
}
