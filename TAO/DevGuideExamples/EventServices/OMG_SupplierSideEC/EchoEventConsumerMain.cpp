// $Id$

// EchoEventConsumerMain.cpp
// Main program for a PushConsumer of Echo events.

#include "EchoEventConsumer_i.h"

#include "orbsvcs/CosEventCommC.h"
#include "orbsvcs/CosEventChannelAdminC.h"
#include "orbsvcs/CosNamingC.h"

#include <iostream>
const int EVENT_LIMIT = 10;

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
  {
    // Initialize the ORB.
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

    // Find the Naming Service.
     CORBA::Object_var obj = orb->resolve_initial_references("NameService");
    CosNaming::NamingContextExt_var root_context = CosNaming::NamingContextExt::_narrow(obj.in());

    // Find the EchoEventChannel.
    obj = root_context->resolve_str("CosEventService");

    // Downcast the object reference to an EventChannel reference.
    CosEventChannelAdmin::EventChannel_var echoEC =
      CosEventChannelAdmin::EventChannel::_narrow(obj.in());
    if (CORBA::is_nil(echoEC.in())) {
      std::cerr << "Could not narrow EchoEventChannel." << std::endl;
      return 1;
    }
    std::cout << "Found the EchoEventChannel." << std::endl;

    // Get a ConsumerAdmin object from the EventChannel.
    CosEventChannelAdmin::ConsumerAdmin_var consumerAdmin =
      echoEC->for_consumers();

    // Get a ProxyPushSupplier from the ConsumerAdmin.
    CosEventChannelAdmin::ProxyPushSupplier_var supplier =
      consumerAdmin->obtain_push_supplier();

    // Instantiate an EchoEventConsumer_i servant.
    EchoEventConsumer_i servant(orb.in(), supplier.in(), EVENT_LIMIT);

    // Register it with the RootPOA.
    obj = orb->resolve_initial_references("RootPOA");
    PortableServer::POA_var poa = PortableServer::POA::_narrow(obj.in());
    PortableServer::ObjectId_var oid = poa->activate_object(&servant);
    CORBA::Object_var consumer_obj = poa->id_to_reference(oid.in());
    CosEventComm::PushConsumer_var consumer =
      CosEventComm::PushConsumer::_narrow(consumer_obj.in());

    // Connect to the ProxyPushSupplier, passing our PushConsumer object
    // reference to it.
    supplier->connect_push_consumer(consumer.in());

    // Activate the POA via its POAManager.
    PortableServer::POAManager_var poa_manager = poa->the_POAManager();
    poa_manager->activate();

    std::cout << "Ready to receive events..." << std::endl;

    // Enter the ORB event loop.
    orb->run();

    // If we have reached this, we must be shutting down...
    orb->destroy();

    std::cout << "Test completed." << std::endl;

    return 0;
  }
  catch(const CORBA::Exception& exc)
  {
    std::cerr << "Caught CORBA::Exception" << std::endl << exc << std::endl;
  }

  return 1;
}
