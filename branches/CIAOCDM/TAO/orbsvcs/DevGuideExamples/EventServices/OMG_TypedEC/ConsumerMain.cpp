// $Id$

// ConsumerMain.cpp
// Main program for a TypedPushConsumer of Messenger objects.


#include "Messenger_i.h"
#include "Consumer_i.h"

#include "orbsvcs/CosTypedEventCommC.h"
#include "orbsvcs/CosTypedEventChannelAdminC.h"
#include "orbsvcs/CosNamingC.h"
#include "tao/AnyTypeCode/TypeCode.h"
#include "ace/OS_NS_stdio.h"
#include <iostream>

const int EVENTS_TILL_SHUTDOWN = 10;

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
  {
    // Initialize the ORB.
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

    // Find the Naming Service.
    CORBA::Object_var obj = orb->resolve_initial_references("NameService");
    CosNaming::NamingContextExt_var root_context = CosNaming::NamingContextExt::_narrow(obj.in());

    // Find the EventChannel.
    obj = root_context->resolve_str("CosEventService");

    // Downcast the object reference to an TypedEventChannel reference.
    CosTypedEventChannelAdmin::TypedEventChannel_var ec =
      CosTypedEventChannelAdmin::TypedEventChannel::_narrow(obj.in());
    if (CORBA::is_nil(ec.in())) {
      std::cerr << "Could not narrow TypedEventChannel." << std::endl;
      return 1;
    }
    std::cout << "Found the TypedEventChannel." << std::endl;

    // Get a ConsumerAdmin object from the EventChannel.
    CosTypedEventChannelAdmin::TypedConsumerAdmin_var consumerAdmin =
      ec->for_consumers();

    // Get a ProxyPushSupplier from the ConsumerAdmin.
    CosEventChannelAdmin::ProxyPushSupplier_var supplier =
      consumerAdmin->obtain_typed_push_supplier(::_tc_Messenger->id());

    // Instantiate an Messenger_i servant.
    PortableServer::Servant_var<Messenger_i> servant =
      new Messenger_i(orb.in(), supplier.in(), EVENTS_TILL_SHUTDOWN);

    // Register it with the RootPOA.
    // Activate the POA here before we connect our consumer.
    obj = orb->resolve_initial_references("RootPOA");
    PortableServer::POA_var poa = PortableServer::POA::_narrow(obj.in());
    PortableServer::POAManager_var poa_manager = poa->the_POAManager();
    poa_manager->activate();

    PortableServer::ObjectId_var oid = poa->activate_object(servant.in());
    CORBA::Object_var messenger_obj = poa->id_to_reference(oid.in());
    PortableServer::Servant_var<Consumer_i> consumer_servant =
      new Consumer_i(orb.in(), messenger_obj.in());
    PortableServer::ObjectId_var cons_oid =
      poa->activate_object(consumer_servant.in());
    CORBA::Object_var consumer_obj = poa->id_to_reference(cons_oid.in());
    CosTypedEventComm::TypedPushConsumer_var consumer =
      CosTypedEventComm::TypedPushConsumer::_narrow(consumer_obj.in());

    // Connect to the ProxyPushSupplier, passing our PushConsumer object
    // reference to it.
    supplier->connect_push_consumer(consumer.in());

    std::cout << "Ready to receive events..." << std::endl;

    CORBA::String_var str = orb->object_to_string (consumer.in ());
    const char* ior_file_name = "Consumer.ior";
    FILE *output_file= ACE_OS::fopen (ACE_TEXT_CHAR_TO_TCHAR(ior_file_name),
                                      ACE_TEXT("w"));
    if (output_file == 0) {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Cannot open output file for writing IOR: %s",
                         ior_file_name),
                        1);
    }
    ACE_OS::fprintf (output_file, "%s", str.in ());
    ACE_OS::fclose (output_file);

    // Enter the ORB event loop.
    orb->run();

    // If we have reached this, we must be shutting down...
    // Disconnect the ProxyPushSupplier.
    orb->destroy();

    std::cout << "Test complete." << std::endl;

    return 0;
  }
  catch(const CORBA::Exception& ex)
  {
    std::cerr << "Caught CORBA::Exception. " << std::endl << ex << std::endl;
  }

  return 1;
}
