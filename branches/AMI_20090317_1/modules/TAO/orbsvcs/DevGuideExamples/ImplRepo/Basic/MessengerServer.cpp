// $Id$

#include "Messenger_i.h"
#include <iostream>
#include <fstream>


PortableServer::POA_ptr
createPersistentPOA(PortableServer::POA_ptr root_poa,
                    const char* poa_name)
{
  CORBA::PolicyList policies;
  policies.length(2);

  policies[0] = root_poa->create_lifespan_policy(PortableServer::PERSISTENT);
  policies[1] = root_poa->create_id_assignment_policy(PortableServer::USER_ID);

  PortableServer::POAManager_var mgr = root_poa->the_POAManager();
  PortableServer::POA_var poa =
    root_poa->create_POA(poa_name, mgr.in(), policies);

  policies[0]->destroy();
  policies[1]->destroy();

  return poa._retn();
}

void writeIORFile(const char* ior) {
  std::ofstream out("messenger.ior");
  out << ior;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try {
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

    CORBA::Object_var obj = orb->resolve_initial_references("RootPOA");
    PortableServer::POA_var root_poa = PortableServer::POA::_narrow(obj.in());

    PortableServer::POAManager_var mgr = root_poa->the_POAManager();

    const char* poa_name = "MessengerService";

    PortableServer::POA_var poa = createPersistentPOA(root_poa.in(), poa_name);

    Messenger_i servant;

    PortableServer::ObjectId_var object_id =
      PortableServer::string_to_ObjectId("object");

    poa->activate_object_with_id(object_id.in(), &servant);

    obj = poa->id_to_reference(object_id.in());
    CORBA::String_var ior = orb->object_to_string(obj.in());

    writeIORFile(ior.in());

    mgr->activate();

    std::cout << "Messenger server ready." << std::endl;

    orb->run();

    std::cout << "Messenger server shutting down." << std::endl;

    root_poa->destroy(1,1);
    orb->destroy();

    return 0;
  } catch(const CORBA::Exception& ex) {
    std::cerr << "Server main() Caught Exception" << ex << std::endl;
  }
  return 1;
}
