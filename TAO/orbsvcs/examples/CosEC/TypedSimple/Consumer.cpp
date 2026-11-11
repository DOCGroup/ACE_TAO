#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/CosTypedEventChannelAdminC.h"
#include "Country_i.h"
#include "ace/OS_NS_stdio.h"
#include "ace/Get_Opt.h"
#include <memory>

int
parse_args (int argc, ACE_TCHAR *argv[], ACE_TCHAR const *&ior_output_file)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("o:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage:  %s -o <ior_output_file>\n"),
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv);

      ACE_TCHAR const *ior_output_file = ACE_TEXT ("Consumer.ior");
      if (parse_args (argc, argv, ior_output_file) != 0)
        return 1;

      CORBA::Object_var const poa_obj =
        orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var const poa =
        PortableServer::POA::_narrow (poa_obj.in ());
      PortableServer::POAManager_var const poa_manager =
        poa->the_POAManager ();
      poa_manager->activate ();

      // Obtain the event channel using the Naming Service.
      CORBA::Object_var const nam_obj =
        orb->resolve_initial_references ("NameService");

      CosNaming::NamingContext_var const root_context =
        CosNaming::NamingContext::_narrow (nam_obj.in ());

      CosNaming::Name channel_name (1);
      channel_name.length (1);
      channel_name[0].id = CORBA::string_dup ("CountryEventChannel");

      CORBA::Object_var const ec_obj =
        root_context->resolve (channel_name);

      // Downcast the object reference to a TypedEventChannel reference
      CosTypedEventChannelAdmin::TypedEventChannel_var const typed_event_channel =
        CosTypedEventChannelAdmin::TypedEventChannel::_narrow (ec_obj.in ());

      // Initialise the Country Impl
      Country_i country (orb.in ());
      Country_var const typed_consumer = country._this ();

      // Connect to the typed channel
      CosTypedEventChannelAdmin::TypedConsumerAdmin_var const typed_consumer_admin =
        typed_event_channel->for_consumers ();

      CosEventChannelAdmin::ProxyPushSupplier_var const proxy_push_supplier =
        typed_consumer_admin->obtain_typed_push_supplier (_tc_Country->id());

      proxy_push_supplier->connect_push_consumer (typed_consumer.in ());

      CORBA::String_var const str =
        orb->object_to_string (typed_consumer.in ());

      {
        std::unique_ptr<FILE, int (*) (FILE *)> const output_file (
          ACE_OS::fopen (ior_output_file, ACE_TEXT ("w")), ACE_OS::fclose);
        if (!output_file)
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Cannot open output file for writing IOR: %s"),
                             ior_output_file),
                            1);

        ACE_OS::fprintf (output_file.get (), "%s", str.in ());
      }

      // Wait for events.
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Waiting on orb->run for events...\n")));
      orb->run ();

      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("...ORB shutdown\n")));

      // Destroy the POA
      poa->destroy (1, 0);

      // Destroy the ORB
      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception (ACE_TEXT ("main"));
      return 1;
    }
  return 0;
}
