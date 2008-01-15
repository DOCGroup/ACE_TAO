// $Id$
#include "orbsvcs/Notify/Service.h"
#include "orbsvcs/Notify/Factory.h"
#include "orbsvcs/CosNotifyChannelAdminC.h"
#include "orbsvcs/Notify/MonitorControl/Statistic.h"
#include "orbsvcs/Notify/MonitorControl/MonitorManager.h"
#include "orbsvcs/Notify/MonitorControl/Statistic_Registry.h"
#include "orbsvcs/Notify/MonitorControlExt/NotifyMonitoringExtC.h"
#include "orbsvcs/Notify/MonitorControlExt/NotifyMonitoringExtC.h"
#include "tao/ORB.h"
#include "tao/PortableServer/PortableServer.h"

void
error(const char* msg)
{
  ACE_ERROR ((LM_ERROR, "%s\n", msg));
  ACE_OS::exit(1);
}

int
ACE_TMAIN (int argc, ACE_TCHAR* argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);
      CORBA::Object_var obj = orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var poa = PortableServer::POA::_narrow (obj.in ());
      PortableServer::POAManager_var poa_manager = poa->the_POAManager ();
      poa_manager->activate();

      TAO_Notify_Service* notify_service = TAO_Notify_Service::load_default ();
      if (notify_service == 0)
        error ("Unable to load the Notify Service");

      notify_service->init_service (orb.in ());
      ACE_OS::sleep (1);

      const ACE_CString ecf_name ("MonitoringEventChannelFactory");
      CosNotifyChannelAdmin::EventChannelFactory_var ecf =
         notify_service->create (poa.in (), ecf_name.c_str ());
      NotifyMonitoringExt::EventChannelFactory_var monitor_ec_factory =
        NotifyMonitoringExt::EventChannelFactory::_narrow (ecf.in ());
      if (CORBA::is_nil (monitor_ec_factory.in ()))
        error ("Unable to create the Monitoring Event Channel Factory");

      CosNotification::QoSProperties qos_prop;
      CosNotification::AdminProperties admin_prop;
      CosNotifyChannelAdmin::ChannelID id;
      const ACE_CString ec_name ("test1");
      CosNotifyChannelAdmin::EventChannel_var ec =
        monitor_ec_factory->create_named_channel (qos_prop, admin_prop,
                                                  id, ec_name.c_str ());

      try
        {
          CosNotifyChannelAdmin::ChannelID fake_id;
          CosNotifyChannelAdmin::EventChannel_var fake =
            monitor_ec_factory->create_named_channel (qos_prop, admin_prop,
                                                      fake_id, "test1");
          error("Expected a NotifyMonitoringExt::NameAlreadyUsed exception");
        }
      catch (const NotifyMonitoringExt::NameAlreadyUsed&)
        {
          // This is expected
        }

      TAO_Statistic_Registry* instance = TAO_Statistic_Registry::instance ();
      ACE_CString stat_name = ecf_name + "/" +
                              ACE_CString (NotifyMonitoringExt::InactiveEventChannelCount);
      TAO_Statistic* stat = instance->get (stat_name);
      if (stat == 0)
        error("Could not find InactiveEventChannelCount statistic");

      stat->calculate ();
      double count = stat->last_sample ();
      if (count != 1)
        error("Invalid inactive event channel count");

      stat_name = ecf_name + "/" +
                  ACE_CString (NotifyMonitoringExt::ActiveEventChannelCount);
      stat = instance->get (stat_name);
      if (stat == 0)
        error("Could not find ActiveEventChannelCount statistic");

      stat->calculate ();
      count = stat->last_sample ();
      if (count != 0)
        error("Invalid active event channel count");

      stat_name = ecf_name + "/" +
                  ACE_CString (NotifyMonitoringExt::InactiveEventChannelNames);
      stat = instance->get (stat_name);
      if (stat == 0)
        error("Could not find InactiveEventChannels statistic");

      stat->calculate ();
      TAO_Statistic::List list = stat->get_list ();
      if (list.size () != 1)
        error("Invalid inactive event channel list");

      ACE_CString full_ec_name (ecf_name + "/" + ec_name);
      if (list[0] != full_ec_name)
        error("Wrong event channel name");

      CosNotifyChannelAdmin::AdminID aid;
      CosNotifyChannelAdmin::SupplierAdmin_var admin =
        ec->new_for_suppliers (CosNotifyChannelAdmin::AND_OP, aid);

      NotifyMonitoringExt::SupplierAdmin_var madmin =
        NotifyMonitoringExt::SupplierAdmin::_narrow (admin.in ());
      if (CORBA::is_nil (madmin.in ()))
        error("Could not narrow the supplier admin");

      CosNotifyChannelAdmin::ProxyID pid;
      CosNotifyChannelAdmin::ProxyConsumer_var conproxy =
        madmin->obtain_named_notification_push_consumer (
          CosNotifyChannelAdmin::STRUCTURED_EVENT, pid, "supplier");

      try
        {
          CosNotifyChannelAdmin::ProxyConsumer_var fake =
            madmin->obtain_named_notification_push_consumer
            (CosNotifyChannelAdmin::STRUCTURED_EVENT, pid, "supplier");
          error("Expected a ProxyConsumer "
                "NotifyMonitoringExt::NameAlreadyUsed exception");
        }
      catch (const NotifyMonitoringExt::NameAlreadyUsed&)
        {
          // This is expected
        }

      stat_name = ecf_name + "/" + ec_name + "/" +
                  ACE_CString (NotifyMonitoringExt::EventChannelSupplierCount);
      stat = instance->get (stat_name);
      if (stat == 0)
        error("Could not find the event channel suppliers statistic");

      stat->calculate ();
      count = stat->last_sample ();
      if (count != 1)
        error("Invalid supplier count");

      CosNotifyChannelAdmin::StructuredProxyPushConsumer_var push_conproxy
        = CosNotifyChannelAdmin::StructuredProxyPushConsumer::_narrow
        (conproxy.in());
      ACE_ASSERT (!CORBA::is_nil (push_conproxy.in()));
      push_conproxy->disconnect_structured_push_consumer();

      try
        {
          CosNotifyChannelAdmin::ProxyConsumer_var fake =
            madmin->obtain_named_notification_push_consumer
            (CosNotifyChannelAdmin::STRUCTURED_EVENT, pid, "supplier");
        }
      catch (const NotifyMonitoringExt::NameAlreadyUsed&)
        {
          error("Unexpected ProxyConsumer "
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
      error("Caught an unexpected exception type");
    }

  return 0;
}
