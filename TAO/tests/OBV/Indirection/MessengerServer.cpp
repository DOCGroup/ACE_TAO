#include "Messenger_i.h"
#include "Factory.h"

#include <fstream>
#include <iostream>

constexpr char server_ior_file[] = "server.ior";

bool
write_ior (char const *ior)
{
  std::ofstream output_file (server_ior_file);
  if (!output_file)
  {
    return false;
  }

  output_file << ior;
  output_file.close ();
  return output_file.good ();
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

      NodeFactory::register_new_factory (*orb.in ());
      BoxedValueFactory::register_new_factory (*orb.in ());
      BaseValueFactory::register_new_factory (*orb.in ());
      TValueFactory::register_new_factory (*orb.in ());
      ConfigValueFactory::register_new_factory (*orb.in ());

      CORBA::Object_var const poa_object =
        orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var const poa =
        PortableServer::POA::_narrow (poa_object.in ());
      if (CORBA::is_nil (poa.in ()))
        {
          std::cerr << "Unable to narrow RootPOA" << std::endl;
          return 1;
        }

      PortableServer::POAManager_var const poaman = poa->the_POAManager ();

      PortableServer::Servant_var<Messenger_i> const svt (new Messenger_i);

      PortableServer::ObjectId_var const id = poa->activate_object (svt.in ());
      CORBA::Object_var const obj = poa->id_to_reference (id.in ());
      CORBA::String_var const ior = orb->object_to_string (obj.in ());
      if (!write_ior (ior.in ()))
        {
          std::cerr << "Unable to write server IOR to " << server_ior_file
                    << std::endl;
          return 1;
        }

      std::cout << "Starting server." << std::endl;

      poaman->activate ();

      orb->run ();

      poa->destroy (true, true);
      orb->destroy ();
    }
  catch (CORBA::Exception const& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
