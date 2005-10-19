// $Id$
#include "NodeApplication_Core.h"
#include "ciao/Server_init.h"
#include "ciao/CIAO_common.h"

int
main (int argc, char *argv[])
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      ACE_DEBUG ((LM_DEBUG, "*** Starting NodeApplication\n"));

      // Initialize orb
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc,
                         argv,
                         ""
                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CIAO::Server_init (orb.in ());

      CIAO::NodeApplication_Options nodeapp_options;

      if (nodeapp_options.parse_args (argc, argv) != 0)
        return -1;

      CIAO::NodeApplication_Core nodeapp_core (orb.in (),
                                               nodeapp_options);

      // Run the main event loop for the ORB.
      nodeapp_core.run_orb ();
      ACE_TRY_CHECK;
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
