// $Id$

#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/CosTypedEventChannelAdminC.h"
#include "Country_i.h"
#include "ace/OS_NS_stdio.h"

ACE_RCSID (CosEC_Examples, 
           Consumer, 
           "$Id$")

int
main (int argc, char* argv[])
{

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // ORB initialization...
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var poa_obj =
        orb->resolve_initial_references ("RootPOA" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      PortableServer::POA_var poa =
        PortableServer::POA::_narrow (poa_obj.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      PortableServer::POAManager_var poa_manager =
        poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
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

      // Initialise the Country Impl
      Country_i country (orb.in ());
      Country_var typed_consumer = country._this();

      // Connect to the typed channel
      CosTypedEventChannelAdmin::TypedConsumerAdmin_var typed_consumer_admin =
        typed_event_channel->for_consumers (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CosEventChannelAdmin::ProxyPushSupplier_var proxy_push_supplier =
        typed_consumer_admin->obtain_typed_push_supplier (_tc_Country->id()
                                                          ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      proxy_push_supplier->connect_push_consumer (typed_consumer.in () );

      CORBA::String_var str =
         orb->object_to_string (typed_consumer.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      const char* ior_file_name = "Consumer.ior";
      FILE *output_file=
	ACE_OS::fopen (ACE_TEXT_CHAR_TO_TCHAR(ior_file_name),
		       ACE_LIB_TEXT("w"));
      if (output_file == 0)
	ACE_ERROR_RETURN ((LM_ERROR,
			   "Cannot open output file for writing IOR: %s",
			   ior_file_name),
			  1);
      ACE_OS::fprintf (output_file, "%s", str.in ());
      ACE_OS::fclose (output_file);

      // Wait for events.
      ACE_DEBUG ((LM_DEBUG, "Waiting on orb->run for events...\n"));
      orb->run ();

      ACE_DEBUG ((LM_DEBUG, "...ORB shutdown\n"));

      // Destroy the POA
      poa->destroy (1, 0 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Destroy the ORB
      orb->destroy ();
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

