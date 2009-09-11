// $Id$

// Main program for a TypedPushSupplier of Messenger objects.

#include "orbsvcs/CosTypedEventCommC.h"
#include "orbsvcs/CosTypedEventChannelAdminC.h"
#include "orbsvcs/CosNamingC.h"
#include "tao/AnyTypeCode/TypeCode.h"

#include <iostream>
#include "MessengerC.h"

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

    // Downcast the object reference to a TypedEventChannel reference.
    CosTypedEventChannelAdmin::TypedEventChannel_var ec =
      CosTypedEventChannelAdmin::TypedEventChannel::_narrow(obj.in ());
    if (CORBA::is_nil(ec.in())) {
      std::cerr << "Could not resolve TypedEventChannel." << std::endl;
      return 1;
    }

    // Get a SupplierAdmin object from the EventChannel.
    CosTypedEventChannelAdmin::TypedSupplierAdmin_var supplierAdmin =
      ec->for_suppliers();

    // Get a ProxyPushConsumer from the SupplierAdmin.
    CosTypedEventChannelAdmin::TypedProxyPushConsumer_var consumer =
      supplierAdmin->obtain_typed_push_consumer(::_tc_Messenger->id());

    // Connect to the ProxyPushConsumer as a PushSupplier
    // (passing a nil PushSupplier object reference to it because
    // we don't care to be notified about disconnects).
    consumer->connect_push_supplier(CosEventComm::PushSupplier::_nil());

    // Obtain the interface from the event channel
    CORBA::Object_var messenger_obj = consumer->get_typed_consumer();

    // Narrow the interface
    Messenger_var messenger = Messenger::_narrow(messenger_obj.in () );

    // Create an event (just a string in this case).
    const CORBA::String_var eventData = CORBA::string_dup("Hello, world.");

    // Send one event per second. (approx)
    while (1) {
      messenger->send_message("King Lizard",
                              "Proclamations",
                              eventData.in());

      ACE_Time_Value event_delay(0, 1000 * EVENT_DELAY_MS);
      orb->run(event_delay);
    }

  }
  catch(const CORBA::Exception& ex)
  {
    std::cerr << "Caught CORBA::Exception. " << std::endl << ex << std::endl;
  }

  return 1;
}
