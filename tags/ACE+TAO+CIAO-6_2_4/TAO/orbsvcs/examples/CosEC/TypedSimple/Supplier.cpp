// $Id$

#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/CosTypedEventChannelAdminC.h"
#include "CountryC.h"



int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{

  try
    {
      // ORB initialization boiler plate...
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      // Obtain the event channel using the Naming Service.
      CORBA::Object_var nam_obj =
        orb->resolve_initial_references ("NameService" );

      CosNaming::NamingContext_var root_context =
        CosNaming::NamingContext::_narrow(nam_obj.in ());

      CosNaming::Name channel_name (1);
      channel_name.length (1);
      channel_name[0].id = CORBA::string_dup ("CountryEventChannel");

      CORBA::Object_var ec_obj =
        root_context->resolve(channel_name);

      // Downcast the object reference to a TypedEventChannel reference
      CosTypedEventChannelAdmin::TypedEventChannel_var typed_event_channel =
        CosTypedEventChannelAdmin::TypedEventChannel::_narrow(ec_obj.in ());

      // Connect to the typed channel
      CosTypedEventChannelAdmin::TypedSupplierAdmin_var typed_supplier_admin =
        typed_event_channel->for_suppliers ();

      CosTypedEventChannelAdmin::TypedProxyPushConsumer_var typed_proxy_push_consumer =
        typed_supplier_admin->obtain_typed_push_consumer (_tc_Country->id());

      typed_proxy_push_consumer->connect_push_supplier (CosEventComm::PushSupplier::_nil());

      // Obtain the interface from the event channel
      CORBA::Object_var typed_consumer =
        typed_proxy_push_consumer->get_typed_consumer();

      // Narrow the interface
      Country_var typed_supplier = Country::_narrow(typed_consumer.in () );

      // Invoke the events...
      for (int i = 0; i != 100; ++i)
        {
          typed_supplier->update_population ("England", i);
        }

      // Disconnect from the EC
      typed_proxy_push_consumer->disconnect_push_consumer ();

      // Destroy the EC....
      typed_event_channel->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("main");
      return 1;
    }
  return 0;
}



