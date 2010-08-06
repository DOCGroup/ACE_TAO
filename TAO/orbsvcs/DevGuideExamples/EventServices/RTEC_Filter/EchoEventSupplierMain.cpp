// $Id$

// EchoEventSupplierMain.cpp
// Main program for a PushSupplier of Echo events.

#include "EchoEventSupplier_i.h"

#include "orbsvcs/RtecEventCommC.h"// for Event Communication interfaces
#include "orbsvcs/RtecEventChannelAdminC.h"
#include "orbsvcs/Time_Utilities.h"
#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/CosNamingC.h"// for Naming Service interfaces

#include <iostream>
const RtecEventComm::EventSourceID MY_SOURCE_ID_START = ACE_ES_EVENT_SOURCE_ANY + 1;
const RtecEventComm::EventSourceID MY_SOURCE_ID_END   = ACE_ES_EVENT_SOURCE_ANY + 3;
const RtecEventComm::EventType MY_EVENT_START = ACE_ES_EVENT_UNDEFINED + 1;
const RtecEventComm::EventType MY_EVENT_END   = ACE_ES_EVENT_UNDEFINED + 4;

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

    // Instantiate an EchoEventConsumer_i servant.
    EchoEventSupplier_i servant(orb.in());

    // Register it with the RootPOA.
    PortableServer::ObjectId_var oid = poa->activate_object(&servant);
    CORBA::Object_var supplier_obj = poa->id_to_reference(oid.in());
    RtecEventComm::PushSupplier_var supplier =
      RtecEventComm::PushSupplier::_narrow(supplier_obj.in());

    // Publish the events the supplier provides.
    ACE_SupplierQOS_Factory qos;
    RtecEventComm::EventSourceID supplier_id = MY_SOURCE_ID_START;
    for (; supplier_id <= MY_SOURCE_ID_END; ++supplier_id) {
      RtecEventComm::EventType event_type = MY_EVENT_START;
      for (; event_type <= MY_EVENT_END; ++event_type)
      {
        qos.insert (supplier_id, event_type,
                    0,         // handle to the rt_info structure
                    1);        // number of calls
      }
    }

    // Connect as a supplier of the published events.
    consumer->connect_push_supplier (supplier.in (), qos.get_SupplierQOS ());

    // Create an event (just a string in this case).
    const CORBA::String_var eventData = CORBA::string_dup("Hello, world.");

    // Create an event set for one event
    RtecEventComm::EventSet event (1);
    event.length (1);

    // Initialize event header.
    event[0].header.source = MY_SOURCE_ID_START;
    event[0].header.type = MY_EVENT_START;

    // Initialize data fields in event.
    event[0].data.any_value <<= eventData;

    std::cout << "Supplier starting sending events.\n";
    while (1) {

      event[0].header.type++;
      if (event[0].header.type > MY_EVENT_END) {
        event[0].header.type = MY_EVENT_START;
      }
      event[0].header.source++;
      if (event[0].header.source > MY_SOURCE_ID_END) {
        event[0].header.source = MY_SOURCE_ID_START;
      }
      consumer->push (event);

      ACE_Time_Value tv(0, EVENT_DELAY_MS * 1000);
      orb->run(tv);
    }

    orb->destroy();

    return 0;
  }
  catch(const CORBA::Exception& exc)
  {
    std::cerr << "Caught CORBA::Exception" << std::endl << exc << std::endl;
  }

  return 1;
}
