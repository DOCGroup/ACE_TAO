//
// $Id$
//

#include "Stock_Consumer.h"
#include <orbsvcs/CosNamingC.h>

int main (int argc, char* argv[])
{
  try {
    // First initialize the ORB, that will remove some arguments...
    CORBA::ORB_var orb =
      CORBA::ORB_init (argc, argv,
                       "" /* the ORB name, it can be anything! */);
    CORBA::Object_var poa_object =
      orb->resolve_initial_references ("RootPOA");
    PortableServer::POA_var poa =
      PortableServer::POA::_narrow (poa_object.in ());
    PortableServer::POAManager_var poa_manager =
      poa->the_POAManager ();
    poa_manager->activate ();

    CORBA::Object_var naming_context_object =
      orb->resolve_initial_references ("NameService");
    CosNaming::NamingContext_var naming_context =
      CosNaming::NamingContext::_narrow (naming_context_object.in ());

    CosNaming::Name name (1);
    name.length (1);
    name[0].id = CORBA::string_dup ("CosEventService");

    CORBA::Object_var ec_object =
      naming_context->resolve (name);

    // Now downcast the object reference to the appropriate type
    CosEventChannelAdmin::EventChannel_var ec =
      CosEventChannelAdmin::EventChannel::_narrow (ec_object.in ());

    Stock_Consumer stock_consumer_i;
    stock_consumer_i.connect (ec.in ());

    orb->run ();

    stock_consumer_i.disconnect ();

    poa->destroy (1, 1);
    orb->destroy ();
  }
  catch (CORBA::Exception &) {
    cerr << "CORBA exception raised!" << endl;
  }
  return 0;
}
