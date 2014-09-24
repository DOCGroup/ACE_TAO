// $Id$

#include "LB_server.h"
#include "Basic.h"
#include "Simple.h"

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      const char *location1 = "MyLocation 1";

      LB_server lb_server (argc, argv);

      if (lb_server.start_orb_and_poa () == -1)
        return 1;

      if (lb_server.create_basic_object_group () == -1)
        return 1;

      if (lb_server.create_simple_object_group () == -1)
        return 1;

      Basic *basic_servant1;
      Basic *basic_servant2;

      Simple *simple_servant1;
      Simple *simple_servant2;


      ACE_NEW_RETURN (basic_servant1,
                      Basic (lb_server.get_basic_object_group (),
                             lb_server.load_manager (),
                             lb_server.orb (),
                             1,
                             location1),
                      1);
      PortableServer::ServantBase_var basic_owner_transfer1(basic_servant1);

      ACE_NEW_RETURN (simple_servant1,
                      Simple (lb_server.get_simple_object_group (),
                              lb_server.load_manager (),
                              lb_server.orb (),
                              1,
                              location1),
                      1);
      PortableServer::ServantBase_var simple_owner_transfer1(simple_servant1);

      if (lb_server.register_basic_servant (basic_servant1, location1) == -1)
        {
          (void) lb_server.destroy();
          return 1;
        }

      if (lb_server.register_simple_servant (simple_servant1, location1) == -1)
        {
          (void) lb_server.destroy();
          return 1;
        }

      if (lb_server.remove_basic_member() == -1)
        {
          return 1;
        }

      if (lb_server.remove_simple_member() == -1)
        {
          return 1;
        }

      ACE_NEW_RETURN (basic_servant2,
                      Basic (lb_server.get_basic_object_group (),
                             lb_server.load_manager (),
                             lb_server.orb (),
                             2,
                             location1),
                      1);
      PortableServer::ServantBase_var basic_owner_transfer2(basic_servant2);

      ACE_NEW_RETURN (simple_servant2,
                      Simple (lb_server.get_simple_object_group (),
                              lb_server.load_manager (),
                              lb_server.orb (),
                              2,
                              location1),
                      1);
      PortableServer::ServantBase_var simple_owner_transfer2(simple_servant2);

      if (lb_server.register_basic_servant (basic_servant2, location1) == -1)
        {
          (void) lb_server.destroy();
          return 1;
        }

      if (lb_server.register_simple_servant (simple_servant2, location1) == -1)
        {
          (void) lb_server.destroy();
          return 1;
        }

      if (lb_server.remove_basic_member() == -1)
        {
          return 1;
        }

      if (lb_server.remove_simple_member() == -1)
        {
          return 1;
        }

      //lb_server.orb ()->shutdown (0);

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
