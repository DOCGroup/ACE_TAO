// $Id$

#include "Supplier.h"
#include "orbsvcs/CosEventChannelAdminS.h"
#include "ace/OS_NS_unistd.h"

ACE_RCSID (CosEC_Examples,
           Supplier,
           "$Id$")

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  Supplier supplier;

  return supplier.run (argc, argv);
}

// ****************************************************************

Supplier::Supplier (void)
{
}

int
Supplier::run (int argc, ACE_TCHAR* argv[])
{
  try
    {
      // ORB initialization boiler plate...
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      if (argc <= 1)
        {
          ACE_ERROR ((LM_ERROR,
                      "Usage: Supplier <event_channel_ior>\n"));
          return 1;
        }

      CORBA::Object_var object =
        orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var poa =
        PortableServer::POA::_narrow (object.in ());
      PortableServer::POAManager_var poa_manager =
        poa->the_POAManager ();
      poa_manager->activate ();

      // Obtain the event channel, we could use a naming service, a
      // command line argument or resolve_initial_references(), but
      // this is simpler...
      object =
        orb->string_to_object (argv[1]);

      CosEventChannelAdmin::EventChannel_var event_channel =
        CosEventChannelAdmin::EventChannel::_narrow (object.in ());

      // The canonical protocol to connect to the EC
      CosEventChannelAdmin::SupplierAdmin_var supplier_admin =
        event_channel->for_suppliers ();

      CosEventChannelAdmin::ProxyPushConsumer_var consumer =
        supplier_admin->obtain_push_consumer ();

      CosEventComm::PushSupplier_var supplier =
        this->_this ();

      consumer->connect_push_supplier (supplier.in ());

      // Push the events...
      ACE_Time_Value sleep_time (0, 10000); // 10 milliseconds

      CORBA::Any event;
      event <<= CORBA::ULong (10);

      for (int i = 0; i != 2000; ++i)
        {
          consumer->push (event);
          ACE_OS::sleep (sleep_time);
        }

      // Disconnect from the EC
      consumer->disconnect_push_consumer ();

      // Destroy the EC....
      event_channel->destroy ();

      // Deactivate this object...
      PortableServer::ObjectId_var id =
        poa->servant_to_id (this);
      poa->deactivate_object (id.in ());

      // Destroy the POA
      poa->destroy (1, 0);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Supplier::run");
      return 1;
    }
  return 0;
}

void
Supplier::disconnect_push_supplier (void)
{
}

