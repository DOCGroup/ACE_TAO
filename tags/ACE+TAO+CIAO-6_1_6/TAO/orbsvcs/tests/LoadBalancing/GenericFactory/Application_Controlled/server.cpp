// $Id$

#include "LB_server.h"
#include "Basic.h"

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      const char *location1 = "MyLocation 1";
      const char *location2 = "MyLocation 2";
      const char *location3 = "MyLocation 3";
      const char *location4 = "MyLocation 4";
      const char *location5 = "MyLocation 5";
      const char *location6 = "MyLocation 6";

      LB_server lb_server (argc, argv);

      if (lb_server.start_orb_and_poa () == -1)
        return 1;

      if (lb_server.create_object_group () == -1)
        return 1;

      Basic *basic_servant1;
      Basic *basic_servant2;
      Basic *basic_servant3;
      Basic *basic_servant4;
      Basic *basic_servant5;
      Basic *basic_servant6;

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

      ACE_NEW_RETURN (basic_servant3,
                      Basic (lb_server.object_group (),
                             lb_server.load_manager (),
                             lb_server.orb (),
                             location3),
                      1);
      PortableServer::ServantBase_var owner_transfer3(basic_servant3);

      ACE_NEW_RETURN (basic_servant4,
                      Basic (lb_server.object_group (),
                             lb_server.load_manager (),
                             lb_server.orb (),
                             location4),
                      1);
      PortableServer::ServantBase_var owner_transfer4(basic_servant4);

      ACE_NEW_RETURN (basic_servant5,
                      Basic (lb_server.object_group (),
                             lb_server.load_manager (),
                             lb_server.orb (),
                             location5),
                      1);
      PortableServer::ServantBase_var owner_transfer5(basic_servant5);

      ACE_NEW_RETURN (basic_servant6,
                      Basic (lb_server.object_group (),
                             lb_server.load_manager (),
                             lb_server.orb (),
                             location6),
                      1);
      PortableServer::ServantBase_var owner_transfer6(basic_servant6);

      if (lb_server.register_servant (basic_servant1, location1) == -1
          || lb_server.register_servant (basic_servant2, location2) == -1
          || lb_server.register_servant (basic_servant3, location3) == -1
          || lb_server.register_servant (basic_servant4, location4) == -1
          || lb_server.register_servant (basic_servant5, location5) == -1
          || lb_server.register_servant (basic_servant6, location6) == -1)
        {
          (void) lb_server.destroy ();
          return 1;
        }

      lb_server.orb ()->run ();

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - event loop finished\n"));

      if (lb_server.destroy () == -1)
        return 1;

    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("lb_server exception");
      return 1;
    }

  return 0;
}
