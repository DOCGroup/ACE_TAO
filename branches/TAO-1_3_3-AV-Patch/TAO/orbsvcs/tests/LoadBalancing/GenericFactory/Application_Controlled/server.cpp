#include "LB_server.h"
#include "Basic.h"

ACE_RCSID (Application_Controlled,
           server,
           "$Id$")

int
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      const char *location1 = "MyLocation 1";
      const char *location2 = "MyLocation 2";

      LB_server lb_server (argc, argv);

      if (lb_server.start_orb_and_poa () == -1)
        return 1;

      if (lb_server.create_object_group () == -1)
        return 1;

      Basic *basic_servant1;
      Basic *basic_servant2;

      ACE_NEW_RETURN (basic_servant1,
		      Basic (lb_server.object_group (),
			     lb_server.load_manager (),
			     lb_server.orb (),
			     location1),
		      1);
      PortableServer::ServantBase_var owner_transfer1(basic_servant1);

      ACE_NEW_RETURN (basic_servant2,
		      Basic (lb_server.object_group (),
			     lb_server.load_manager (),
			     lb_server.orb (),
			     location2),
		      1);
      PortableServer::ServantBase_var owner_transfer2(basic_servant2);

      if (lb_server.register_servant (basic_servant1, location1) == -1
          || lb_server.register_servant (basic_servant2, location2) == -1)
        {
          (void) lb_server.destroy ();
          return 1;
        }

      lb_server.orb ()->run (ACE_ENV_SINGLE_ARG_PARAMETER);
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
