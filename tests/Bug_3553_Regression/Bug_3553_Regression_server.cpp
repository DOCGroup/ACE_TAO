// $Id$

#include "Hello.h"
#include "ace/Get_Opt.h"
#include "tao/IORTable/IORTable.h"

int cache_size = 512;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("c:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'c':
        cache_size = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-c <transport_cache_size> "
                           "\n",
                           argv [0]),
                          -1);
      }
  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args(argc, argv) != 0)
          return 1;

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil RootPOA\n"),
                          1);

      Hello *hello_impl = 0;
      ACE_NEW_RETURN (hello_impl,
                      Hello (orb.in ()),
                      1);

      PortableServer::ObjectId_var id =
        root_poa->activate_object (hello_impl);

      CORBA::Object_var object = root_poa->id_to_reference (id.in ());

      Test::Hello_var hello = Test::Hello::_narrow (object.in ());

      CORBA::Object_var table_obj = orb->resolve_initial_references("IORTable");
      IORTable::Table_var table = IORTable::Table::_narrow(table_obj.in());
      for (int i = 1; i <= cache_size; ++i)
        {
          CORBA::String_var ior_string = orb->object_to_string (object.in());
          ACE_DEBUG((LM_DEBUG, ACE_TEXT("Registering object %d with IOR string: %C\n"),
            i, ior_string.in ()));
          char identifier[256];
          ACE_OS::sprintf (identifier, "TransportCacheTest%d", i);
          table->bind(identifier, ior_string.in());
        }

      PortableServer::POAManager_var poa_manager = root_poa->the_POAManager ();
      poa_manager->activate ();

      orb->run ();

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - event loop finished\n"));

      root_poa->destroy (1, 1);

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
