#include "LB_server.h"
#include "Factory.h"
#include "Simple.h"

ACE_RCSID (Infrastructure_Controlled,
           server,
           "$Id$");

int
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      const char *loc1 = "MyLocation 1";
      const char *loc2 = "MyLocation 2";

      LB_server lb_server (argc, argv);

      if (lb_server.start_orb_and_poa () == -1)
        return 1;

      if (lb_server.create_object_group (loc1, loc2) == -1)
        return 1;

      lb_server.run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - event loop finished\n"));

      if (lb_server.destroy () == -1)
        return 1;

    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "lb_server exception");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
