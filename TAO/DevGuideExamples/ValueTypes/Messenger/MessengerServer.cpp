// $Id$

#include "_pch.h"

#include "Messenger_i.h"
#include "Message_i.h"

#include <iostream>
#include <fstream>
#include <fstream>

const char* server_ior_file = "server.ior";


void write_ior(const char* ior) {
  std::ofstream out(server_ior_file);
  out << ior;
}

int ACE_TMAIN (int ac, ACE_TCHAR* av[]) {

  CORBA::ORB_var orb = CORBA::ORB_init(ac, av);

  MessageFactory::register_new_factory(* orb.in());

  CORBA::Object_var obj = orb->resolve_initial_references("RootPOA");
  PortableServer::POA_var poa = PortableServer::POA::_narrow(obj.in());

  PortableServer::POAManager_var poaman = poa->the_POAManager();

  Messenger_i svt;

  PortableServer::ObjectId_var id = poa->activate_object(&svt);
  obj = poa->id_to_reference(id.in());
  CORBA::String_var ior = orb->object_to_string(obj.in());
  write_ior(ior.in());

  std::cout << "Starting server." << std::endl;

  poaman->activate();

  orb->run();

  poa->destroy(1, 1);
  orb->destroy();

  return 0;
}
