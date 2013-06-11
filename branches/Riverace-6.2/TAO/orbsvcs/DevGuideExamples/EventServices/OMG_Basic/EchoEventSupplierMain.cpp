// $Id$

// Main program for a PushSupplier of Echo events.

#include "orbsvcs/CosEventCommC.h"
#include "orbsvcs/CosEventChannelAdminC.h"
#include "orbsvcs/CosNamingC.h"

#include <iostream>

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

    // Find the EventChannel.
    obj = root_context->resolve_str("CosEventService");

    // Downcast the object reference to an EventChannel reference.
    CosEventChannelAdmin::EventChannel_var echoEC =
      CosEventChannelAdmin::EventChannel::_narrow(obj.in());
    if (CORBA::is_nil(echoEC.in())) {
      std::cerr << "Could not resolve EchoEventChannel." << std::endl;
      return 1;
    }

    // Get a SupplierAdmin object from the EventChannel.
    CosEventChannelAdmin::SupplierAdmin_var supplierAdmin =
      echoEC->for_suppliers();

    // Get a ProxyPushConsumer from the SupplierAdmin.
    CosEventChannelAdmin::ProxyPushConsumer_var consumer =
      supplierAdmin->obtain_push_consumer();

    // Connect to the ProxyPushConsumer as a PushSupplier
    // (passing a nil PushSupplier object reference to it because
    // we don't care to be notified about disconnects).
    consumer->connect_push_supplier(CosEventComm::PushSupplier::_nil());

    // Create an event (just a string in this case).
    const CORBA::String_var eventData = CORBA::string_dup("Hello, world.");

    // Send one event per second. (approx)
    while (1) {
      // Insert the event data into an any.
      CORBA::Any any;
      any <<= eventData;

      // Now push the event to the consumer
      consumer->push(any);

      ACE_Time_Value event_delay(0, 1000 * EVENT_DELAY_MS);
      orb->run(event_delay);
    }

    return 0;
  }
  catch(const CORBA::Exception& ex)
  {
    std::cerr << "Supplier Caught CORBA::Exception. " << ex << std::endl;
  }

  return 1;
}
