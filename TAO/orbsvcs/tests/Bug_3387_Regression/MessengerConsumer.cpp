#include "orbsvcs/CosNotifyChannelAdminC.h"
#include "orbsvcs/CosNotifyCommC.h"
#include "orbsvcs/CosNamingC.h"

#include "StructuredEventConsumer_i.h"
#include <cstdio>
#include <iostream>

#include "tao/ORB_Core.h"
#include "ace/Get_Opt.h"

struct Consumer_Options
{
  int pass {};
};

struct Consumer_State
{
  Consumer_State () = default;
  Consumer_State (Consumer_State const &) = delete;
  Consumer_State (Consumer_State &&) = delete;
  Consumer_State &operator= (Consumer_State const &) = delete;
  Consumer_State &operator= (Consumer_State &&) = delete;

  CORBA::ORB_var orb;
  CosNotifyChannelAdmin::StructuredProxyPushSupplier_var supplier_proxy;
  int pass {};
};

static Consumer_State consumer_state;

int
parse_args (int argc, ACE_TCHAR *argv[], Consumer_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("p:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'p':
        options.pass = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-p <pass> "
                           "\n",
                           argv [0]),
                          -1);
      }
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  Consumer_Options options;
  try
    {
      consumer_state.orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv, options) != 0)
        return 1;
      consumer_state.pass = options.pass;

      CORBA::Object_var const naming_obj =
        consumer_state.orb->resolve_initial_references ("NameService");

      CosNaming::NamingContext_var const naming_context =
        CosNaming::NamingContext::_narrow (naming_obj.in ());

      CosNaming::Name name;
      name.length (1);
      name[0].id = CORBA::string_dup ("MyEventChannel");
      CORBA::Object_var const event_channel_object = naming_context->resolve (name);

      CosNotifyChannelAdmin::EventChannel_var const event_channel =
        CosNotifyChannelAdmin::EventChannel::_narrow (event_channel_object.in ());

      CosNotifyChannelAdmin::AdminID adminid;
      CosNotifyChannelAdmin::InterFilterGroupOperator const ifgop =
        CosNotifyChannelAdmin::OR_OP;

      CosNotifyChannelAdmin::ConsumerAdmin_var const consumer_admin =
        event_channel->new_for_consumers (ifgop, adminid);

      CORBA::Object_var const poa_object =
        consumer_state.orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var const poa =
        PortableServer::POA::_narrow (poa_object.in ());

      StructuredEventConsumer_i servant (consumer_state.orb.in ());

      PortableServer::ObjectId_var const object_id = poa->activate_object (&servant);

      CORBA::Object_var const consumer_object = poa->id_to_reference (object_id.in ());

      CosNotifyComm::StructuredPushConsumer_var const consumer =
        CosNotifyComm::StructuredPushConsumer::_narrow (consumer_object.in ());

      CosNotifyChannelAdmin::ProxyID consumeradmin_proxy_id;

      CosNotifyChannelAdmin::ProxySupplier_var const proxy_supplier =
        consumer_admin->obtain_notification_push_supplier (
          CosNotifyChannelAdmin::STRUCTURED_EVENT, consumeradmin_proxy_id);

      consumer_state.supplier_proxy = CosNotifyChannelAdmin::StructuredProxyPushSupplier::_narrow (
        proxy_supplier.in ());

      consumer_state.supplier_proxy->connect_structured_push_consumer (consumer.in ());

      CosNotification::EventTypeSeq added (1);
      CosNotification::EventTypeSeq removed (1);
      added.length (1);
      removed.length (1);

      added[0].domain_name = CORBA::string_dup ("OCI_TAO");
      added[0].type_name = CORBA::string_dup ("examples");

      removed[0].domain_name = CORBA::string_dup ("*");
      removed[0].type_name = CORBA::string_dup ("*");

      consumer_state.supplier_proxy->subscription_change (added, removed);

      PortableServer::POAManager_var const poa_manager = poa->the_POAManager ();
      poa_manager->activate ();

      ACE_Time_Value run_time (12);
      consumer_state.orb->run (&run_time);

      if (servant.nummsgs () > 0)
        std::cout << "Success pass " << consumer_state.pass << ": events received " << std::endl;
      else
        std::cout << "ERROR pass " << consumer_state.pass << ": no events received " << std::endl;
    }
  catch (CORBA::Exception const &ex)
    {
      std::cerr << "Caught exception: " << ex << std::endl;
      return 1;
    }

  return 0;
}

int
consumerStop ()
{
  std::printf ("in consumerStop\n");
  try
    {
      consumer_state.supplier_proxy->disconnect_structured_push_supplier ();
    }

  catch (CORBA::Exception const &ex)
    {
      std::cerr << "Caught exception: " << ex << std::endl;
      return 1;
    }

  consumer_state.orb->shutdown (false);
  return 0;
}

