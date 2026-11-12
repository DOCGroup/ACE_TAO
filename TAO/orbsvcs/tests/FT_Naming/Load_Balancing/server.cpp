#include "LB_server.h"
#include "Basic.h"
#include <array>
#include <cstddef>

static constexpr std::array<char const *, 6> locations {
  "MyLocation 1", "MyLocation 2", "MyLocation 3",
  "MyLocation 4", "MyLocation 5", "MyLocation 6"
};

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      LB_server lb_server (argc, argv);

      if (lb_server.start_orb_and_poa () != 0)
        return 1;

      if (lb_server.create_object_group () != 0)
        return 1;

      CosNaming::Name name (1);
      name.length (1);
      name[0].id = CORBA::string_dup ("basic_name");
      try
        {
          lb_server.name_svc ()->rebind (name, lb_server.object_group ());
        }
      catch (CORBA::Exception const &ex)
        {
          ex._tao_print_exception ("Unable to bind object group in name service.\n");
          return 1;
        }

      std::array<PortableServer::Servant_var<Basic>, locations.size ()> servants {};
      for (size_t i {}; i < locations.size (); ++i)
        {
          Basic *servant {};
          ACE_NEW_RETURN (servant,
                          Basic (lb_server.object_group (),
                                 lb_server.naming_manager (),
                                 lb_server.orb (),
                                 locations[i]),
                          1);
          servants[i] = servant;
          if (lb_server.register_servant (servants[i].in (), locations[i]) != 0)
            {
              (void) lb_server.destroy ();
              return 1;
            }
        }

      lb_server.orb ()->run ();

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - event loop finished\n"));

      if (lb_server.destroy () != 0)
        return 1;
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("lb_server exception");
      return 1;
    }

  return 0;
}
