// $Id$

#include "Supplier.h"
#include "orbsvcs/RtecEventChannelAdminC.h"
#include "orbsvcs/Event_Service_Constants.h"

ACE_RCSID(EC_Examples, Supplier, "$Id$");

extern "C"
int
createSupplier (int argc, char* argv[])
{
  Supplier supplier;
  return supplier.run (argc, argv);
}

Supplier::Supplier (void)
{
}

int
Supplier::run (int argc, char* argv[])
{

//  int register_with_object_manager = 0;
//  if (TAO_Singleton_Manager::instance ()->init (
//         register_with_object_manager) == -1)
//     ACE_ERROR_RETURN ((LM_ERROR,
//                        "Supplier::init -- ORB pre-initialization "
//                        "failed.\n"),
//                       -1);  // No exceptions available yet, so return
//                             // an error status.
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // Initialize the ORB.
      CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                            argv,
                                            "Consumer"
                                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var object =
        orb->resolve_initial_references ("RootPOA" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      PortableServer::POA_var poa =
        PortableServer::POA::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      PortableServer::POAManager_var poa_manager =
        poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      TAO_Naming_Client naming_client;

      // Initialization of the naming service.
      if (naming_client.init (orb.in ()) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) Unable to initialize "
                           "the TAO_Naming_Client. \n"),
                          -1);

      CosNaming::Name ec_name (1);
      ec_name.length (1);
      ec_name[0].id = CORBA::string_dup ("EventChannel");

      CORBA::Object_var ec_obj =
       naming_client->resolve (ec_name
                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      RtecEventChannelAdmin::EventChannel_var event_channel =
        RtecEventChannelAdmin::EventChannel::_narrow (ec_obj.in ()
                                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // The canonical protocol to connect to the EC
      RtecEventChannelAdmin::SupplierAdmin_var supplier_admin =
        event_channel->for_suppliers (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      RtecEventChannelAdmin::ProxyPushConsumer_var consumer =
        supplier_admin->obtain_push_consumer (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      RtecEventComm::PushSupplier_var supplier =
        this->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Simple publication, but usually the helper classes in
      // $TAO_ROOT/orbsvcs/Event_Utils.h are a better way to do this.
      RtecEventChannelAdmin::SupplierQOS qos;
      qos.publications.length (1);
      RtecEventComm::EventHeader& h0 =
        qos.publications[0].event.header;
      h0.type   = ACE_ES_EVENT_UNDEFINED; // first free event type
      h0.source = 1;                      // first free event source

      consumer->connect_push_supplier (supplier.in (), qos
                                       ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Push the events...
      ACE_Time_Value sleep_time (0, 10000); // 10 milliseconds

      RtecEventComm::EventSet event (1);
      event.length (1);
      event[0].header.type   = ACE_ES_EVENT_UNDEFINED;
      event[0].header.source = 1;
      event[0].header.ttl    = 1;

      for (int i = 0; i != 2000; ++i)
        {
          consumer->push (event ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
          ACE_OS::sleep (sleep_time);
        }
      ACE_OS::sleep (3);

      naming_client->unbind (ec_name ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Disconnect from the EC
      consumer->disconnect_push_consumer (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      ACE_DEBUG ((LM_DEBUG, "Push Consumer disconnected\n"));

      // Destroy the EC....
      event_channel->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      ACE_DEBUG ((LM_DEBUG, "EventChannel destroyed\n"));

      // Deactivate this object...
//       PortableServer::ObjectId_var id =
//         poa->servant_to_id (this ACE_ENV_ARG_PARAMETER);
//       ACE_TRY_CHECK;

//       poa->deactivate_object (id.in () ACE_ENV_ARG_PARAMETER);
//       ACE_TRY_CHECK;

//       // Destroy the POA
//       poa->destroy (1, 0 ACE_ENV_ARG_PARAMETER);
//       ACE_TRY_CHECK;

}
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Supplier::run");
      return 1;
    }
  ACE_ENDTRY;
//   if (TAO_Singleton_Manager::instance ()->fini () == -1)
//     ACE_ERROR_RETURN ((LM_ERROR,
//                        "Supplier::fini -- ORB pre-termination failed."),
//                       -1);
   ACE_DEBUG ((LM_DEBUG, "Supplier exiting...\n"));
   return 0;
}

void
Supplier::disconnect_push_supplier (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
