// $Id$

// Main program for a PushSupplier of Echo events.

#include "orbsvcs/CosEventCommC.h"
#include "orbsvcs/CosEventChannelAdminC.h"
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/CosEvent/CEC_EventChannel.h"
#include "orbsvcs/CosEvent/CEC_Default_Factory.h"

#include <iostream>
const int EVENT_DELAY_MS = 10;

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
  {
    // Initialize the CEC Factory so we can customize the CEC
    TAO_CEC_Default_Factory::init_svcs ();

    // Initialize the ORB.
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

    // Find the Naming Service.
    CORBA::Object_var obj = orb->resolve_initial_references("NameService");
    CosNaming::NamingContextExt_var root_context = CosNaming::NamingContextExt::_narrow(obj.in());

    // Get the root POA
    CORBA::Object_var poa_object = orb->resolve_initial_references("RootPOA");
    if (CORBA::is_nil (poa_object.in ()))
      ACE_ERROR_RETURN ((LM_ERROR,
      " (%P|%t) Unable to initialize the POA.\n"),
      -1);
    PortableServer::POA_var root_poa =
      PortableServer::POA::_narrow (poa_object.in ());
    PortableServer::POAManager_var poa_manager = root_poa->the_POAManager ();
    poa_manager->activate ();

    // Create and activate the event channel servant
    CosEventChannelAdmin::EventChannel_var echoEC;

    TAO_CEC_EventChannel_Attributes attr(root_poa.in(), root_poa.in());
    TAO_CEC_EventChannel* ec = new TAO_CEC_EventChannel(attr);
    ec->activate();
    PortableServer::ObjectId_var oid = root_poa->activate_object(ec);
    CORBA::Object_var ec_obj = root_poa->id_to_reference(oid.in());
    echoEC = CosEventChannelAdmin::EventChannel::_narrow(ec_obj.in());

    // Bind the EventChannel.
    CosNaming::Name_var name = root_context->to_name("CosEventService");
    root_context->rebind(name.in(), echoEC.in());

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

    while (1) {
      // Insert the event data into an any.
      CORBA::Any any;
      any <<= eventData;

      // Now push the event to the consumer
      consumer->push(any);

      ACE_Time_Value tv(0, 1000 * EVENT_DELAY_MS);
      orb->run(tv);
    }
    orb->destroy();

    return 0;
  }
  catch(const CORBA::Exception& exc)
  {
    std::cerr << "Caught unknown CORBA::Exception exception" << std::endl << exc << std::endl;
  }

  return 1;
}
