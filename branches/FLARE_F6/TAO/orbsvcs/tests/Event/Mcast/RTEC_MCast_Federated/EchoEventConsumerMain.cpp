// $Id$

// EchoEventConsumerMain.cpp
// Main program for a PushConsumer of Echo events.

#include "EchoEventConsumer_i.h"

#include <orbsvcs/RtecEventCommC.h>
#include <orbsvcs/RtecEventChannelAdminC.h>
#include <orbsvcs/Time_Utilities.h>
#include <orbsvcs/Event_Utilities.h>
#include <orbsvcs/CosNamingC.h>

const RtecEventComm::EventSourceID MY_SOURCE_ID  = ACE_ES_EVENT_SOURCE_ANY + 1;
const RtecEventComm::EventType     MY_EVENT_TYPE = ACE_ES_EVENT_UNDEFINED + 1;

const int EVENT_LIMIT = 20;

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
  {
    // Initialize the ORB.
    CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

    const ACE_TCHAR *ecname = ACE_TEXT ("EventService");
    for (int i = 0; argv[i] != 0; i++) {
      if (ACE_OS::strcmp (argv[i], ACE_TEXT ("-ecname")) == 0) {
        if (argv[i+1] != 0) {
          ecname = argv[i+1];
        } else {
    ACE_ERROR ((LM_ERROR,
          "Missing Event channel name\n"));
        }
      }
    }

    // Find the Naming Service.
    CORBA::Object_var obj = orb->resolve_initial_references("NameService");
    CosNaming::NamingContextExt_var root_context = CosNaming::NamingContextExt::_narrow(obj.in());

    // Find the EchoEventChannel.
    obj = root_context->resolve_str (ACE_TEXT_ALWAYS_CHAR (ecname));

    // Downcast the object reference to an EventChannel reference.
    RtecEventChannelAdmin::EventChannel_var ec =
      RtecEventChannelAdmin::EventChannel::_narrow(obj.in());
    if (CORBA::is_nil(ec.in())) {
      ACE_ERROR_RETURN ((LM_ERROR,
       "Could not narrow EchoEventChannel.\n"),
      1);
    }
    ACE_DEBUG ((LM_DEBUG,
    "EchoEventConsumerMain.cpp: Found the EchoEventChannel.\n"));

    // Obtain a reference to the consumer administration object.
    RtecEventChannelAdmin::ConsumerAdmin_var admin = ec->for_consumers();

    // Obtain a reference to the push supplier proxy.
    RtecEventChannelAdmin::ProxyPushSupplier_var supplier =
      admin->obtain_push_supplier();

    // Get the RootPOA.
    obj = orb->resolve_initial_references("RootPOA");
    PortableServer::POA_var poa = PortableServer::POA::_narrow(obj.in());

    // Instantiate an EchoEventConsumer_i servant.
    EchoEventConsumer_i servant(orb.in(), EVENT_LIMIT);

    // Register it with the RootPOA.
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

    ACE_DEBUG ((LM_DEBUG,
    "EchoEventConsumerMain.cpp: Ready to receive events...\n"));

    // Enter the ORB event loop.
    orb->run();

    // If we have reached this, we must be shutting down...
    // Disconnect the ProxyPushSupplier.
    //supplier->disconnect_push_supplier();
    //supplier = RtecEventChannelAdmin::ProxyPushSupplier::_nil();
    //admin = RtecEventChannelAdmin::ConsumerAdmin::_nil();

    orb->destroy();

    return 0;

  }
  catch (CORBA::Exception& exc)
  {
    ACE_ERROR ((LM_ERROR,
    "Caught CORBA::Exception\n%s (%s)\n",
    ACE_TEXT_CHAR_TO_TCHAR (exc._name ()),
    ACE_TEXT_CHAR_TO_TCHAR (exc._rep_id ()) ));
  }

  return 1;
}
