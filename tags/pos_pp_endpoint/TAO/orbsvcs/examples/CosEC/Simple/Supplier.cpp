// $Id$

#include "Supplier.h"
#include "orbsvcs/CosEventChannelAdminS.h"

ACE_RCSID(CosEC_Examples, Supplier, "$Id$")

int
main (int argc, char* argv[])
{
  Supplier supplier;

  return supplier.run (argc, argv);
}

// ****************************************************************

Supplier::Supplier (void)
{
}

int
Supplier::run (int argc, char* argv[])
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // ORB initialization boiler plate...
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "", ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (argc <= 1)
        {
          ACE_ERROR ((LM_ERROR,
                      "Usage: Supplier <event_channel_ior>\n"));
          return 1;
        }

      CORBA::Object_var object =
        orb->resolve_initial_references ("RootPOA", ACE_TRY_ENV);
      ACE_TRY_CHECK;
      PortableServer::POA_var poa =
        PortableServer::POA::_narrow (object.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;
      PortableServer::POAManager_var poa_manager =
        poa->the_POAManager (ACE_TRY_ENV);
      ACE_TRY_CHECK;
      poa_manager->activate (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Obtain the event channel, we could use a naming service, a
      // command line argument or resolve_initial_references(), but
      // this is simpler...
      object =
        orb->string_to_object (argv[1], ACE_TRY_ENV);
      ACE_TRY_CHECK;

      CosEventChannelAdmin::EventChannel_var event_channel =
        CosEventChannelAdmin::EventChannel::_narrow (object.in (),
                                                      ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // The canonical protocol to connect to the EC
      CosEventChannelAdmin::SupplierAdmin_var supplier_admin =
        event_channel->for_suppliers (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      CosEventChannelAdmin::ProxyPushConsumer_var consumer =
        supplier_admin->obtain_push_consumer (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      CosEventComm::PushSupplier_var supplier =
        this->_this (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      consumer->connect_push_supplier (supplier.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Push the events...
      ACE_Time_Value sleep_time (0, 10000); // 10 milliseconds

      CORBA::Any event;
      event <<= CORBA::ULong (10);

      for (int i = 0; i != 2000; ++i)
        {
          consumer->push (event, ACE_TRY_ENV);
          ACE_TRY_CHECK;
          ACE_OS::sleep (sleep_time);
        }

      // Disconnect from the EC
      consumer->disconnect_push_consumer (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Destroy the EC....
      event_channel->destroy (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Deactivate this object...
      PortableServer::ObjectId_var id =
        poa->servant_to_id (this, ACE_TRY_ENV);
      ACE_TRY_CHECK;
      poa->deactivate_object (id.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Destroy the POA
      poa->destroy (1, 0, ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Supplier::run");
      return 1;
    }
  ACE_ENDTRY;
  return 0;
}

void
Supplier::disconnect_push_supplier (CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
}

// ****************************************************************

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
