// $Id$

#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/CosTypedEventChannelAdminC.h"
#include "Country_i.h"
#include "ace/OS_NS_stdio.h"

ACE_RCSID (CosEC_Examples,
           Consumer,
           "$Id$")

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{

  try
    {
      // ORB initialization...
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var poa_obj =
        orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var poa =
        PortableServer::POA::_narrow (poa_obj.in ());
      PortableServer::POAManager_var poa_manager =
        poa->the_POAManager ();
      poa_manager->activate ();

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

      // Initialise the Country Impl
      Country_i country (orb.in ());
      Country_var typed_consumer = country._this();

      // Connect to the typed channel
      CosTypedEventChannelAdmin::TypedConsumerAdmin_var typed_consumer_admin =
        typed_event_channel->for_consumers ();

      CosEventChannelAdmin::ProxyPushSupplier_var proxy_push_supplier =
        typed_consumer_admin->obtain_typed_push_supplier (_tc_Country->id());

      proxy_push_supplier->connect_push_consumer (typed_consumer.in () );

      CORBA::String_var str =
         orb->object_to_string (typed_consumer.in ());

      const ACE_TCHAR* ior_file_name = ACE_TEXT("Consumer.ior");
      FILE *output_file= ACE_OS::fopen (ior_file_name,
                                        ACE_TEXT("w"));
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
      poa->destroy (1, 0);

      // Destroy the ORB
      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("main");
      return 1;
    }
  return 0;
}
