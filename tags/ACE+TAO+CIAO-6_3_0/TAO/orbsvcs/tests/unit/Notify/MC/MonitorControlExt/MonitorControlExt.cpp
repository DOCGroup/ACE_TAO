// $Id$

#include "ace/Monitor_Base.h"
#include "ace/Monitor_Point_Registry.h"

#include "tao/ORB.h"
#include "tao/PortableServer/PortableServer.h"

#include "orbsvcs/CosNotifyChannelAdminC.h"

#include "orbsvcs/Notify/Service.h"
#include "orbsvcs/Notify/Factory.h"

#include "orbsvcs/Notify/MonitorControl/MonitorManager.h"
#include "orbsvcs/Notify/MonitorControlExt/NotifyMonitoringExtC.h"

#if defined (TAO_HAS_MONITOR_FRAMEWORK) && (TAO_HAS_MONITOR_FRAMEWORK == 1)

using namespace ACE_VERSIONED_NAMESPACE_NAME::ACE::Monitor_Control;

void
error (const char* msg)
{
#if defined (ACE_NLOGGING)
  ACE_UNUSED_ARG (msg);
#endif /* ACE_NLOGGING */
  ACE_ERROR ((LM_ERROR, "%s\n", msg));
  ACE_OS::exit (1);
}

#endif /* TAO_HAS_MONITOR_FRAMEWORK==1 */

int
ACE_TMAIN (int argc, ACE_TCHAR* argv[])
{
#if defined (TAO_HAS_MONITOR_FRAMEWORK) && (TAO_HAS_MONITOR_FRAMEWORK == 1)

  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      CORBA::Object_var obj = orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var poa =
        PortableServer::POA::_narrow (obj.in ());

      PortableServer::POAManager_var poa_manager = poa->the_POAManager ();
      poa_manager->activate ();

      TAO_Notify_Service* notify_service =
        TAO_Notify_Service::load_default ();

      if (notify_service == 0)
        {
          error ("Unable to load the Notify Service");
        }

      notify_service->init_service (orb.in ());
      ACE_OS::sleep (1);

      const ACE_CString ecf_name ("MonitoringEventChannelFactory");
      CosNotifyChannelAdmin::EventChannelFactory_var ecf =
         notify_service->create (poa.in (), ecf_name.c_str ());
      NotifyMonitoringExt::EventChannelFactory_var monitor_ec_factory =
        NotifyMonitoringExt::EventChannelFactory::_narrow (ecf.in ());

      if (CORBA::is_nil (monitor_ec_factory.in ()))
        {
         error ("Unable to create the Monitoring Event Channel Factory");
        }

      CosNotification::QoSProperties qos_prop;
      CosNotification::AdminProperties admin_prop;
      CosNotifyChannelAdmin::ChannelID id;
      const ACE_CString ec_name ("test1");

      CosNotifyChannelAdmin::EventChannel_var ec =
        monitor_ec_factory->create_named_channel (qos_prop,
                                                  admin_prop,
                                                  id,
                                                  ec_name.c_str ());
      NotifyMonitoringExt::EventChannel_var mec =
        NotifyMonitoringExt::EventChannel::_narrow (ec.in ());

      if (CORBA::is_nil (mec.in ()))
        {
          error ("Unable to narrow the event channel");
        }

      try
        {
          CosNotifyChannelAdmin::ChannelID fake_id;
          CosNotifyChannelAdmin::EventChannel_var fake =
            monitor_ec_factory->create_named_channel (qos_prop,
                                                      admin_prop,
                                                      fake_id,
                                                      "test1");
          error ("Expected a NotifyMonitoringExt::"
                 "NameAlreadyUsed exception");
        }
      catch (const NotifyMonitoringExt::NameAlreadyUsed&)
        {
          // This is expected.
        }

      Monitor_Point_Registry* instance = Monitor_Point_Registry::instance ();
      ACE_CString stat_name =
        ecf_name
        + "/"
        + ACE_CString (NotifyMonitoringExt::InactiveEventChannelCount);

      Monitor_Base* stat = instance->get (stat_name);

      if (stat == 0)
        {
          error ("Could not find InactiveEventChannelCount statistic");
        }

      stat->update ();
      double count = stat->last_sample ();

      if (!ACE::is_equal (count, 1.0))
        {
          error ("Invalid inactive event channel count");
        }

      stat_name =
        ecf_name
        + "/"
        + ACE_CString (NotifyMonitoringExt::ActiveEventChannelCount);

      stat = instance->get (stat_name);

      if (stat == 0)
        {
          error ("Could not find ActiveEventChannelCount statistic");
        }

      stat->update ();
      count = stat->last_sample ();

      if (!ACE::is_equal (count, 0.0))
        {
          error ("Invalid active event channel count");
        }

      stat_name =
        ecf_name
        + "/"
        + ACE_CString (NotifyMonitoringExt::InactiveEventChannelNames);

      stat = instance->get (stat_name);

      if (stat == 0)
        {
          error ("Could not find InactiveEventChannels statistic");
        }

      stat->update ();
      Monitor_Control_Types::NameList list = stat->get_list ();

      if (list.size () != 1)
        {
          error ("Invalid inactive event channel list");
        }

      ACE_CString full_ec_name (ecf_name + "/" + ec_name);

      if (list[0] != full_ec_name)
        {
          error ("Wrong event channel name");
        }

      CosNotifyChannelAdmin::AdminID aid;
      CosNotifyChannelAdmin::SupplierAdmin_var admin =
        mec->named_new_for_suppliers (CosNotifyChannelAdmin::AND_OP,
                                      aid,
                                      "TestSupplierAdmin");

      try
        {
          admin =
            mec->named_new_for_suppliers (CosNotifyChannelAdmin::AND_OP,
                                          aid,
                                          "TestSupplierAdmin");

          error ("Expected a SupplierAdmin "
                 "NotifyMonitoringExt::NameAlreadyUsed exception");
        }
      catch (const NotifyMonitoringExt::NameAlreadyUsed&)
        {
          // This is expected.
        };

      // We should be able to create another one with the same name.
      admin->destroy ();
      admin =
        mec->named_new_for_suppliers (CosNotifyChannelAdmin::AND_OP,
                                      aid,
                                      "TestSupplierAdmin");

      NotifyMonitoringExt::SupplierAdmin_var madmin =
        NotifyMonitoringExt::SupplierAdmin::_narrow (admin.in ());

      if (CORBA::is_nil (madmin.in ()))
        {
          error ("Could not narrow the supplier admin");
        }

      CosNotifyChannelAdmin::ProxyID pid;
      CosNotifyChannelAdmin::ProxyConsumer_var conproxy =
        madmin->obtain_named_notification_push_consumer (
          CosNotifyChannelAdmin::STRUCTURED_EVENT,
          pid,
          "supplier");

      try
        {
          CosNotifyChannelAdmin::ProxyConsumer_var fake =
            madmin->obtain_named_notification_push_consumer
            (CosNotifyChannelAdmin::STRUCTURED_EVENT, pid, "supplier");

          error ("Expected a ProxyConsumer "
                 "NotifyMonitoringExt::NameAlreadyUsed exception");
        }
      catch (const NotifyMonitoringExt::NameAlreadyUsed&)
        {
          // This is expected.
        }

      stat_name =
        ecf_name
        + "/"
        + ec_name
        + "/"
        + ACE_CString (NotifyMonitoringExt::EventChannelSupplierCount);

      stat = instance->get (stat_name);

      if (stat == 0)
        {
          error ("Could not find the event channel suppliers statistic");
        }

      stat->update ();
      count = stat->last_sample ();

      if (!ACE::is_equal (count, 1.0))
        {
          error ("Invalid supplier count");
        }

      CosNotifyChannelAdmin::StructuredProxyPushConsumer_var push_conproxy =
        CosNotifyChannelAdmin::StructuredProxyPushConsumer::_narrow (
          conproxy.in ());
      ACE_ASSERT (!CORBA::is_nil (push_conproxy.in ()));
      push_conproxy->disconnect_structured_push_consumer ();

      try
        {
          CosNotifyChannelAdmin::ProxyConsumer_var fake =
            madmin->obtain_named_notification_push_consumer (
              CosNotifyChannelAdmin::STRUCTURED_EVENT,
              pid,
              "supplier");
        }
      catch (const NotifyMonitoringExt::NameAlreadyUsed&)
        {
          error ("Unexpected ProxyConsumer "
                 "NotifyMonitoringExt::NameAlreadyUsed exception");
        }

      CosNotifyChannelAdmin::ConsumerAdmin_var cadmin =
        mec->named_new_for_consumers (CosNotifyChannelAdmin::AND_OP,
                                      aid,
                                      "TestConsumerAdmin");

      try
        {
          cadmin =
            mec->named_new_for_consumers (CosNotifyChannelAdmin::AND_OP,
                                          aid,
                                          "TestConsumerAdmin");

          error ("Expected a ConsumerAdmin "
                 "NotifyMonitoringExt::NameAlreadyUsed exception");
        }
      catch (const NotifyMonitoringExt::NameAlreadyUsed&)
        {
          // This is expected.
        };

      // We should be able to create another one with the same name
      cadmin->destroy ();
      cadmin =
        mec->named_new_for_consumers (CosNotifyChannelAdmin::AND_OP,
                                      aid,
                                      "TestConsumerAdmin");

      NotifyMonitoringExt::ConsumerAdmin_var mcadmin =
        NotifyMonitoringExt::ConsumerAdmin::_narrow (cadmin.in ());

      if (CORBA::is_nil (mcadmin.in ()))
        {
          error ("Could not narrow the consumer admin");
        }

      CosNotifyChannelAdmin::ProxySupplier_var supproxy =
        mcadmin->obtain_named_notification_push_supplier (
          CosNotifyChannelAdmin::STRUCTURED_EVENT,
          pid,
          "consumer");

      try
        {
          CosNotifyChannelAdmin::ProxySupplier_var fake =
            mcadmin->obtain_named_notification_push_supplier (
              CosNotifyChannelAdmin::STRUCTURED_EVENT,
              pid,
              "consumer");

          error ("Expected a ProxySupplier "
                 "NotifyMonitoringExt::NameAlreadyUsed exception");
        }
      catch (const NotifyMonitoringExt::NameAlreadyUsed&)
        {
          // This is expected.
        }

      stat_name =
        ecf_name
        + "/"
        + ec_name
        + "/"
        + ACE_CString (NotifyMonitoringExt::EventChannelConsumerCount);

      stat = instance->get (stat_name);

      if (stat == 0)
        {
          error ("Could not find the event channel consumers statistic");
        }

      stat->update ();
      count = stat->last_sample ();

      if (!ACE::is_equal (count, 1.0))
        {
          error ("Invalid consumer count");
        }

      CosNotifyChannelAdmin::StructuredProxyPushSupplier_var push_supproxy =
        CosNotifyChannelAdmin::StructuredProxyPushSupplier::_narrow (
          supproxy.in());
      ACE_ASSERT (!CORBA::is_nil (push_supproxy.in ()));
      push_supproxy->disconnect_structured_push_supplier ();

      try
        {
          CosNotifyChannelAdmin::ProxySupplier_var fake =
            mcadmin->obtain_named_notification_push_supplier (
              CosNotifyChannelAdmin::STRUCTURED_EVENT,
              pid,
              "consumer");
        }
      catch (const NotifyMonitoringExt::NameAlreadyUsed&)
        {
          error ("Unexpected ProxySupplier "
                 "NotifyMonitoringExt::NameAlreadyUsed exception");
        }

      TAO_MonitorManager::shutdown ();
      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("MonitorControlExt: ");
    }
  catch (...)
    {
      error ("Caught an unexpected exception type");
    }
#else /* ACE_HAS_MONITOR_FRAMEWORK==1 */
  ACE_UNUSED_ARG (argc);
  ACE_UNUSED_ARG (argv);
#endif /* TAO_HAS_MONITOR_FRAMEWORK==1 */

  return 0;
}

