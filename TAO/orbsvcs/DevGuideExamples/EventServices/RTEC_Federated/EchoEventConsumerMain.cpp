// $Id$

// EchoEventConsumerMain.cpp
// Main program for a PushConsumer of Echo events.

#include "EchoEventConsumer_i.h"

#include "orbsvcs/RtecEventCommC.h"
#include "orbsvcs/RtecEventChannelAdminC.h"
#include "orbsvcs/Time_Utilities.h"
#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/CosNamingC.h"

#include <iostream>
const RtecEventComm::EventSourceID MY_SOURCE_ID  = ACE_ES_EVENT_SOURCE_ANY + 1;
const RtecEventComm::EventType     MY_EVENT_TYPE = ACE_ES_EVENT_UNDEFINED + 1;

const int EVENT_LIMIT = 10;

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
  {
    // Initialize the ORB.
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

    const ACE_TCHAR *ecname = ACE_TEXT ("EventService");
    for (int i = 0; argv[i] != 0; i++) {
      if (ACE_OS::strcmp(argv[i], ACE_TEXT("-ecname")) == 0) {
        if (argv[i+1] != 0) {
          ecname = argv[i+1];
        } else {
          std::cerr << "Missing Event channel name" << std::endl;
        }
      }
    }

    // Find the Naming Service.
    CORBA::Object_var obj = orb->resolve_initial_references("NameService");
    CosNaming::NamingContextExt_var root_context
      = CosNaming::NamingContextExt::_narrow(obj.in());

    // Find the EchoEventChannel.
    obj = root_context->resolve_str (ACE_TEXT_ALWAYS_CHAR (ecname));

    // Downcast the object reference to an EventChannel reference.
    RtecEventChannelAdmin::EventChannel_var ec =
      RtecEventChannelAdmin::EventChannel::_narrow(obj.in());
    if (CORBA::is_nil(ec.in())) {
      std::cerr << "Could not narrow EchoEventChannel." << std::endl;
      return 1;
    }
    std::cout << "EchoEventConsumerMain.cpp: Found the EchoEventChannel." << std::endl;

    // Obtain a reference to the consumer administration object.
    RtecEventChannelAdmin::ConsumerAdmin_var admin = ec->for_consumers();

    // Obtain a reference to the push supplier proxy.
    RtecEventChannelAdmin::ProxyPushSupplier_var supplier =
      admin->obtain_push_supplier();

    // Get the RootPOA.
    obj = orb->resolve_initial_references("RootPOA");
    PortableServer::POA_var poa = PortableServer::POA::_narrow(obj.in());

    // Instantiate an EchoEventConsumer_i servant and register it
    // with the RootPOA
    EchoEventConsumer_i servant(orb.in(), supplier.in(), EVENT_LIMIT);
    PortableServer::ObjectId_var oid = poa->activate_object(&servant);
    CORBA::Object_var consumer_obj = poa->id_to_reference(oid.in());
    RtecEventComm::PushConsumer_var consumer =
      RtecEventComm::PushConsumer::_narrow(consumer_obj.in());

    // Connect as a consumer.
    ACE_ConsumerQOS_Factory qos;
    qos.start_disjunction_group ();
    qos.insert (MY_SOURCE_ID,   // Source ID
                MY_EVENT_TYPE,  // Event Type
                0);             // handle to the rt_info
    supplier->connect_push_consumer (consumer.in (),
                                     qos.get_ConsumerQOS ());

    // Activate the POA via its POAManager.
    PortableServer::POAManager_var poa_manager = poa->the_POAManager();
    poa_manager->activate();

    std::cout << "EchoEventConsumerMain.cpp: Ready to receive events..." << std::endl;

    // Enter the ORB event loop.
    orb->run();

    // If we have reached this, we must be shutting down...
    // Disconnect the ProxyPushSupplier.
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
