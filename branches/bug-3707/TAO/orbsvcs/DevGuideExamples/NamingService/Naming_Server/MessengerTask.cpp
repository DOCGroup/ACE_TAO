// $Id$

#include "MessengerTask.h"
#include "Messenger_i.h"
#include "orbsvcs/CosNamingC.h"
#include <iostream>

MessengerTask::MessengerTask()
{
  // cast away constness to make Sun CC family of compilers happy.
  char* argv[] = {const_cast<char *>("Messenger"), 0 };
  int argc = 1;
  orb_ = CORBA::ORB_init(argc, argv, "ServerORB");
}

void MessengerTask::end()
{
  orb_->shutdown(0);
  this->wait();
}

int MessengerTask::svc()
{

  try {
    // Get reference to Root POA
    CORBA::Object_var obj = orb_->resolve_initial_references("RootPOA");
    PortableServer::POA_var poa = PortableServer::POA::_narrow(obj.in());

    // Activate POA Manager
    PortableServer::POAManager_var mgr = poa->the_POAManager();
    mgr->activate();

    // Find the Naming Service
    obj = orb_->resolve_initial_references("NameService");
    CosNaming::NamingContext_var root =
      CosNaming::NamingContext::_narrow(obj.in());

    if (CORBA::is_nil(root.in())) {
      std::cerr << "Nil Naming Context reference" << std::endl;
      return 1;
    }
    // Bind the example Naming Context, if necessary
    CosNaming::Name name;
    name.length(1);
    name[0].id = CORBA::string_dup("example");
    try {
      root->resolve(name);
    }
    catch(const CosNaming::NamingContext::NotFound&) {
      root->bind_new_context(name);
    }

    // Bind the Messenger object
    name.length(2);
    name[1].id = CORBA::string_dup("Messenger");

    // Create an object
    PortableServer::Servant_var<Messenger_i> servant = new Messenger_i;
    PortableServer::ObjectId_var oid = poa->activate_object(servant.in());
    obj = poa->id_to_reference(oid.in());
    root->rebind(name, obj.in());

    std::cout << "Messenger object bound in Naming Service" << std::endl;

    // Normally we run the orb and the orb is shutdown by
    // calling MessengerTask::end(). To simplify the coordination
    // between the main thread and this Messenger thread, specify
    // the time period to let the Messenger thread finish by itself.
    // Accept requests
    ACE_Time_Value tv(1);
    orb_->run(tv);
    orb_->destroy();

    return 0;
  }
  catch(const CORBA::Exception& ex) {
    std::cerr << "CORBA exception: " << ex << std::endl;
  }

  return -1;
}


