// $Id$
#include "orbsvcs/CosNotifyChannelAdminC.h"
#include "orbsvcs/CosNotifyCommC.h"
#include "orbsvcs/CosNamingC.h"

#include "StructuredEventConsumer_i.h"
#include <iostream>

#include "tao/ORB_Core.h"
#include "ace/Get_Opt.h"

static CORBA::ORB_ptr orb;
static CosNotifyChannelAdmin::StructuredProxyPushSupplier_ptr supplier_proxy;
static int pass;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "p:");
  int c;

  while ((c = get_opts ()) != -1)    switch (c)
      {
      case 'p':
        pass = ACE_OS::atoi(get_opts.opt_arg ());
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
  // Indicates successful parsing of the command line
  return 0;
}


int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      orb = CORBA::ORB_init(argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var naming_obj =
        orb->resolve_initial_references ("NameService");

      CosNaming::NamingContext_var naming_context =
        CosNaming::NamingContext::_narrow(naming_obj.in());

      CosNaming::Name name;
      name.length (1);
      name[0].id = CORBA::string_dup("MyEventChannel");
      CORBA::Object_var ecObj = naming_context->resolve(name);

      CosNotifyChannelAdmin::EventChannel_var ec =
        CosNotifyChannelAdmin::EventChannel::_narrow(ecObj.in());

      CosNotifyChannelAdmin::AdminID adminid;
      CosNotifyChannelAdmin::InterFilterGroupOperator ifgop =
        CosNotifyChannelAdmin::OR_OP;

      CosNotifyChannelAdmin::ConsumerAdmin_var consumer_admin =
        ec->new_for_consumers(ifgop,
                              adminid);

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA");

      PortableServer::POA_var poa =
        PortableServer::POA::_narrow (poa_object.in());

      StructuredEventConsumer_i  servant (orb);

      PortableServer::ObjectId_var objectId = poa->activate_object (&servant);

      CORBA::Object_var consumer_obj = poa->id_to_reference (objectId.in ());

      CosNotifyComm::StructuredPushConsumer_var consumer =
        CosNotifyComm::StructuredPushConsumer::_narrow (consumer_obj.in ());

      CosNotifyChannelAdmin::ProxyID consumeradmin_proxy_id;

      CosNotifyChannelAdmin::ProxySupplier_var proxy_supplier =
        consumer_admin->obtain_notification_push_supplier(
                                  CosNotifyChannelAdmin::STRUCTURED_EVENT,
                                  consumeradmin_proxy_id);

      supplier_proxy = CosNotifyChannelAdmin::StructuredProxyPushSupplier::
        _narrow(proxy_supplier.in());

      supplier_proxy->connect_structured_push_consumer(consumer.in());




      CosNotification::EventTypeSeq added (1);
      CosNotification::EventTypeSeq removed (1);
      added.length (1);
      removed.length (1);

      added[0].domain_name = CORBA::string_dup ("OCI_TAO");
      added[0].type_name = CORBA::string_dup ("examples");

      removed[0].domain_name = CORBA::string_dup ("*");
      removed[0].type_name = CORBA::string_dup ("*");

      supplier_proxy->subscription_change(added, removed);





      PortableServer::POAManager_var poa_manager = poa->the_POAManager();

      poa_manager->activate();

      ACE_Time_Value run_time(12);
      orb->run(&run_time);

      if ( servant.nummsgs() > 0 )
      {
        std::cout << "Success pass " << pass << ": events received " << std::endl;
      }
      else
      {
        std::cout << "ERROR pass " << pass << ": no events received " << std::endl;
      }

  }
  catch(const CORBA::Exception& ex)
    {
          std::cerr << "Caught exception: " << ex << std::endl;
      return 1;
    }

  return 0;
}


// VxWorks shutdown hook
int consumerStop()
{
printf("in consumerStop\n");
   try
   {
      supplier_proxy->disconnect_structured_push_supplier();
   }

   catch(const CORBA::Exception& ex)
   {
      std::cerr << "Caught exception: " << ex << std::endl;
      return 1;
   }

   orb->shutdown(0);
   return 0;
}

