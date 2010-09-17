// $Id$

#include "orbsvcs/CosNotifyChannelAdminC.h"
#include "orbsvcs/CosNotifyCommC.h"
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/NotifyExtC.h"
#include "tao/RTCORBA/RTCORBA.h"

#include "StructuredEventConsumer_i.h"
#include "Priorities.h"
#include <iostream>
#include <fstream>
#include "ace/Get_Opt.h"

const ACE_TCHAR *output_file = ACE_TEXT ("MessengerConsumer.ready");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        output_file = get_opts.opt_arg ();
        break;

      case '?':
      default:
      ;
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

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
        CosNotifyChannelAdmin::AND_OP;

      CosNotifyChannelAdmin::ConsumerAdmin_var consumer_admin =
        ec->new_for_consumers(ifgop,
            adminid);

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA");

      PortableServer::POA_var poa =
        PortableServer::POA::_narrow (poa_object.in());

      CORBA::Object_var rtorb_obj = orb->resolve_initial_references ("RTORB");
      RTCORBA::RTORB_var rt_orb = RTCORBA::RTORB::_narrow (rtorb_obj.in ());

      // Create an RT POA with a lane at the given priority.
      CORBA::Policy_var priority_model_policy =
        rt_orb->create_priority_model_policy (RTCORBA::CLIENT_PROPAGATED,
                DEFAULT_PRIORITY);

      RTCORBA::ThreadpoolLanes lanes (2);
      lanes.length (2);

      lanes[0].lane_priority   = LOW_PRIORITY;
      lanes[0].static_threads  = 2;
      lanes[0].dynamic_threads = 0;
      lanes[1].lane_priority   = HIGH_PRIORITY;
      lanes[1].static_threads  = 2;
      lanes[1].dynamic_threads = 0;


      // Create a thread-pool.
      CORBA::ULong stacksize = 0;
      CORBA::Boolean allow_request_buffering = 0;
      CORBA::ULong max_buffered_requests = 0;
      CORBA::ULong max_request_buffer_size = 0;
      CORBA::Boolean allow_borrowing = 0;

      // Create the thread-pool.
      RTCORBA::ThreadpoolId threadpool_id =
        rt_orb->create_threadpool_with_lanes (stacksize,
                lanes,
                allow_borrowing,
                allow_request_buffering,
                max_buffered_requests,
                max_request_buffer_size);

      // Create a thread-pool policy.
      CORBA::Policy_var lanes_policy =
        rt_orb->create_threadpool_policy (threadpool_id);

      CORBA::PolicyList poa_policy_list(2);
      poa_policy_list.length (2);
      poa_policy_list[0] = priority_model_policy;
      poa_policy_list[1] = lanes_policy;

      PortableServer::POAManager_var poa_manager = poa->the_POAManager ();

      PortableServer::POA_var rt_poa = poa->create_POA ("RT POA",
              poa_manager.in (),
              poa_policy_list);

      PortableServer::Servant_var<StructuredEventConsumer_i> servant =
        new StructuredEventConsumer_i(orb.in());

      PortableServer::ObjectId_var objectId =
        rt_poa->activate_object (servant.in());

      CORBA::Object_var consumer_obj =
        rt_poa->id_to_reference (objectId.in ());

      CosNotifyComm::StructuredPushConsumer_var consumer =
        CosNotifyComm::StructuredPushConsumer::_narrow (consumer_obj.in ());

      NotifyExt::ThreadPoolLanesParams tpl_params;

      tpl_params.priority_model = NotifyExt::CLIENT_PROPAGATED;
      tpl_params.server_priority = DEFAULT_PRIORITY;
      tpl_params.stacksize = 0;
      tpl_params.allow_borrowing = 0;
      tpl_params.allow_request_buffering = 0;
      tpl_params.max_buffered_requests = 0;
      tpl_params.max_request_buffer_size = 0;
      tpl_params.lanes.length (2);
      tpl_params.lanes[0].lane_priority   = LOW_PRIORITY;
      tpl_params.lanes[0].static_threads  = 2;
      tpl_params.lanes[0].dynamic_threads = 0;
      tpl_params.lanes[1].lane_priority   = HIGH_PRIORITY;
      tpl_params.lanes[1].static_threads  = 2;
      tpl_params.lanes[1].dynamic_threads = 0;
      CosNotification::QoSProperties qos;
      qos.length(1);
      qos[0].name = CORBA::string_dup (NotifyExt::ThreadPoolLanes);
      qos[0].value <<= tpl_params;

      consumer_admin->set_qos(qos);
      CORBA::Object_var current_obj =
        orb->resolve_initial_references ("RTCurrent");

      RTCORBA::Current_var current =
        RTCORBA::Current::_narrow (current_obj.in ());
      current->the_priority(HIGH_PRIORITY);

      CosNotifyChannelAdmin::ProxyID consumeradmin_proxy_id;

      CosNotifyChannelAdmin::ProxySupplier_var proxy_supplier =
        consumer_admin->obtain_notification_push_supplier(
          CosNotifyChannelAdmin::STRUCTURED_EVENT,
          consumeradmin_proxy_id);

      CosNotifyChannelAdmin::StructuredProxyPushSupplier_var supplier_proxy;
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

      poa_manager->activate();

      // Write a file to let the run_test.pl script know we are ready.
      std::ofstream iorFile( ACE_TEXT_ALWAYS_CHAR(output_file) );
      iorFile << "Ready" << std::endl;
      iorFile.close();

      orb->run();
    }
  catch(const CORBA::Exception& ex)
    {
      std::cerr << "Caught exception: " << ex << std::endl;
        return 1;
    }

  return 0;
}
