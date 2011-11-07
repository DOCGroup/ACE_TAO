// $Id$

#include "Messenger_i.h"
#include "Factory.h"

#include <iostream>
#include <fstream>
#include <fstream>

const char* server_ior_file = "server.ior";


void write_ior(const char* ior) {
  std::ofstream out(server_ior_file);
  out << ior;
}

int ACE_TMAIN (int ac, ACE_TCHAR* av[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init(ac, av);

      NodeFactory::register_new_factory(* orb.in());
      BoxedValueFactory::register_new_factory(* orb.in());
      BaseValueFactory::register_new_factory(* orb.in());
      TValueFactory::register_new_factory(* orb.in());
      ConfigValueFactory::register_new_factory(* orb.in());

      CORBA::Object_var obj = orb->resolve_initial_references("RootPOA");
      PortableServer::POA_var poa = PortableServer::POA::_narrow(obj.in());

      PortableServer::POAManager_var poaman = poa->the_POAManager();

      PortableServer::Servant_var<Messenger_i> svt = new Messenger_i;

      PortableServer::ObjectId_var id = poa->activate_object(svt.in());
      obj = poa->id_to_reference(id.in());
      CORBA::String_var ior = orb->object_to_string(obj.in());
      write_ior(ior.in());

      std::cout << "Starting server." << std::endl;

      poaman->activate();

      orb->run();

      poa->destroy(true, true);
      orb->destroy();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
