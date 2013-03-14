// $Id$

// EchoEventSupplierMain.cpp
// Main program for a PushSupplier of Echo events.

#include "EchoEventSupplier_i.h"

#include "orbsvcs/RtecEventCommC.h"
#include "orbsvcs/RtecEventChannelAdminC.h"
#include "orbsvcs/Time_Utilities.h"
#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/CosNamingC.h"

#include <iostream>
const RtecEventComm::EventSourceID MY_SOURCE_ID  = ACE_ES_EVENT_SOURCE_ANY + 1;
const RtecEventComm::EventType     MY_EVENT_TYPE = ACE_ES_EVENT_UNDEFINED + 1;

const int EVENT_DELAY_MS = 10;

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
  {
    // Initialize the ORB.
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

    // Find the Naming Service.
    CORBA::Object_var obj = orb->resolve_initial_references("NameService");
    CosNaming::NamingContextExt_var root_context = CosNaming::NamingContextExt::_narrow(obj.in());

    // Get the Event Channel using Naming Services
    obj = root_context->resolve_str("EventService");

    // Downcast the object reference to an EventChannel reference.
    RtecEventChannelAdmin::EventChannel_var ec =
      RtecEventChannelAdmin::EventChannel::_narrow(obj.in());
    if (CORBA::is_nil(ec.in())) {
      std::cerr << "Could not resolve EchoEventChannel." << std::endl;
      return 1;
    }

    // Get a SupplierAdmin object from the EventChannel.
    RtecEventChannelAdmin::SupplierAdmin_var admin = ec->for_suppliers();

    // Get a ProxyPushConsumer from the SupplierAdmin.
    RtecEventChannelAdmin::ProxyPushConsumer_var consumer =
                                        admin->obtain_push_consumer();

    // Get the RootPOA.
    obj = orb->resolve_initial_references("RootPOA");
    PortableServer::POA_var poa = PortableServer::POA::_narrow(obj.in());

    // Instantiate an EchoEventSupplier_i servant.
    PortableServer::Servant_var<EchoEventSupplier_i> servant =
      new EchoEventSupplier_i(orb.in());

    // Register it with the RootPOA.
    PortableServer::ObjectId_var oid = poa->activate_object(servant.in());
    CORBA::Object_var supplier_obj = poa->id_to_reference(oid.in());
    RtecEventComm::PushSupplier_var supplier =
      RtecEventComm::PushSupplier::_narrow(supplier_obj.in());

    // Publish the events the supplier provides.
    ACE_SupplierQOS_Factory qos;
    qos.insert (MY_SOURCE_ID,      // Supplier's unique id
                MY_EVENT_TYPE,     // Event type
                0,                 // handle to the rt_info structure
                1);                // number of calls

    // Connect as a supplier of the published events.
    consumer->connect_push_supplier (supplier.in (),
                                     qos.get_SupplierQOS ());

    // Activate the POA via its POAManager.
    PortableServer::POAManager_var poa_manager = poa->the_POAManager();
    poa_manager->activate();

    // Create an event (just a string in this case).
    const CORBA::String_var eventData = CORBA::string_dup("Hello, world.");

    // Create an event set for one event
    RtecEventComm::EventSet events (1);
    events.length (1);

    // Initialize event header.
    events[0].header.source = MY_SOURCE_ID;
    events[0].header.type = MY_EVENT_TYPE;

    // Initialize data fields in event.
    events[0].data.any_value <<= eventData;

    std::cout << "Supplier starting sending of events" << std::endl;

    while (1) {
      consumer->push (events);
      ACE_Time_Value tv(0, 1000 * EVENT_DELAY_MS);
      orb->run(tv);
    }

    orb->destroy();

    return 0;
  }
  catch(const CORBA::Exception& ex)
  {
    std::cerr << "Supplier::main() Caught CORBA::Exception" << std::endl << ex << std::endl;
  }

  return 1;
}
