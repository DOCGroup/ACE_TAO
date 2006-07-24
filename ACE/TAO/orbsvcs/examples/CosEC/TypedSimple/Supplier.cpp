// $Id$

#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/CosTypedEventChannelAdminC.h"
#include "CountryC.h"

ACE_RCSID(CosEC_Examples, Supplier, "$Id:")

int
main (int argc, char* argv[])
{

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // ORB initialization boiler plate...
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Obtain the event channel using the Naming Service.
      CORBA::Object_var nam_obj = 
        orb->resolve_initial_references ("NameService" ACE_ENV_ARG_PARAMETER );
      ACE_TRY_CHECK;

      CosNaming::NamingContext_var root_context =
        CosNaming::NamingContext::_narrow(nam_obj.in ()
                                          ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CosNaming::Name channel_name (1);
      channel_name.length (1);
      channel_name[0].id = CORBA::string_dup ("CountryEventChannel");

      CORBA::Object_var ec_obj = 
        root_context->resolve(channel_name);
      ACE_TRY_CHECK;

      // Downcast the object reference to a TypedEventChannel reference
      CosTypedEventChannelAdmin::TypedEventChannel_var typed_event_channel =
        CosTypedEventChannelAdmin::TypedEventChannel::_narrow(ec_obj.in ()
                                                              ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Connect to the typed channel
      CosTypedEventChannelAdmin::TypedSupplierAdmin_var typed_supplier_admin =
        typed_event_channel->for_suppliers (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CosTypedEventChannelAdmin::TypedProxyPushConsumer_var typed_proxy_push_consumer =
        typed_supplier_admin->obtain_typed_push_consumer (_tc_Country->id()
                                                          ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      typed_proxy_push_consumer->connect_push_supplier (CosEventComm::PushSupplier::_nil()
                                                        ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Obtain the interface from the event channel
      CORBA::Object_var typed_consumer = 
        typed_proxy_push_consumer->get_typed_consumer(ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Narrow the interface
      Country_var typed_supplier = Country::_narrow(typed_consumer.in () );
      ACE_TRY_CHECK;

      // Invoke the events...
      for (int i = 0; i != 100; ++i)
        {
          typed_supplier->update_population ("England", i ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      // Disconnect from the EC
      typed_proxy_push_consumer->disconnect_push_consumer (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Destroy the EC....
      typed_event_channel->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "main");
      return 1;
    }
  ACE_ENDTRY;
  return 0;
}



