#include "LB_server.h"
#include "Basic.h"

#include <array>
#include <string>

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  LB_server lb_server (argc, argv);

  try
    {
      std::array<std::string, 6> const locations {{
        "MyLocation 1",
        "MyLocation 2",
        "MyLocation 3",
        "MyLocation 4",
        "MyLocation 5",
        "MyLocation 6"
      }};

      if (lb_server.start_orb_and_poa () == -1)
        {
          (void) lb_server.destroy ();
          return 1;
        }

      if (lb_server.create_object_group () == -1)
        {
          (void) lb_server.destroy ();
          return 1;
        }

      // ServantBase_var takes ownership; these raw pointers are borrowed aliases.
      Basic *basic_servant1 {};
      Basic *basic_servant2 {};
      Basic *basic_servant3 {};
      Basic *basic_servant4 {};
      Basic *basic_servant5 {};
      Basic *basic_servant6 {};

      ACE_NEW_RETURN (basic_servant1,
                      Basic (lb_server.object_group (),
                             lb_server.load_manager (),
                             lb_server.orb (),
                             locations[0]),
                      1);
      PortableServer::ServantBase_var owner_transfer1 (basic_servant1);

      ACE_NEW_RETURN (basic_servant2,
                      Basic (lb_server.object_group (),
                             lb_server.load_manager (),
                             lb_server.orb (),
                             locations[1]),
                      1);
      PortableServer::ServantBase_var owner_transfer2 (basic_servant2);

      ACE_NEW_RETURN (basic_servant3,
                      Basic (lb_server.object_group (),
                             lb_server.load_manager (),
                             lb_server.orb (),
                             locations[2]),
                      1);
      PortableServer::ServantBase_var owner_transfer3 (basic_servant3);

      ACE_NEW_RETURN (basic_servant4,
                      Basic (lb_server.object_group (),
                             lb_server.load_manager (),
                             lb_server.orb (),
                             locations[3]),
                      1);
      PortableServer::ServantBase_var owner_transfer4 (basic_servant4);

      ACE_NEW_RETURN (basic_servant5,
                      Basic (lb_server.object_group (),
                             lb_server.load_manager (),
                             lb_server.orb (),
                             locations[4]),
                      1);
      PortableServer::ServantBase_var owner_transfer5 (basic_servant5);

      ACE_NEW_RETURN (basic_servant6,
                      Basic (lb_server.object_group (),
                             lb_server.load_manager (),
                             lb_server.orb (),
                             locations[5]),
                      1);
      PortableServer::ServantBase_var owner_transfer6 (basic_servant6);

      std::array<Basic *, 6> const servants {{
        basic_servant1,
        basic_servant2,
        basic_servant3,
        basic_servant4,
        basic_servant5,
        basic_servant6
      }};
      for (std::size_t i {}; i != servants.size (); ++i)
        {
          if (lb_server.register_servant (servants[i], locations[i]) == -1)
            {
              (void) lb_server.destroy ();
              return 1;
            }
        }

      lb_server.orb ()->run ();

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - event loop finished\n"));

      if (lb_server.destroy () == -1)
        return 1;
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("lb_server exception");
      (void) lb_server.destroy ();
      return 1;
    }

  return 0;
}
