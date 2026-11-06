#include "orbsvcs/CosNotificationC.h"
#include "orbsvcs/NotifyExtC.h"
#include "orbsvcs/Notify/Notify_EventChannelFactory_i.h"

// On MacOS X, the static initialization trick used
// in the CosNotification_Serv library does not work.  Including the
// initializer class here works around the problem.
#if defined (__APPLE__)
#include "orbsvcs/Notify/CosNotify_Initializer.h"
#endif /* __APPLE__ */

#include "tao/TimeBaseC.h"
#include "tao/corba.h"
#include "tao/PortableServer/PortableServer.h"

#include "ace/OS_NS_string.h"
#include "ace/OS_NS_stdio.h"
#include <memory>

class TestSupplier : public POA_CosNotifyComm::StructuredPushSupplier
{
public:
  TestSupplier () = default;
  TestSupplier (TestSupplier const&) = delete;
  TestSupplier (TestSupplier&&) = delete;
  TestSupplier& operator= (TestSupplier const&) = delete;
  TestSupplier& operator= (TestSupplier&&) = delete;

private:
  void disconnect_structured_push_supplier () override
  {
  }

  void subscription_change (CosNotification::EventTypeSeq const&,
                            CosNotification::EventTypeSeq const&) override
  {
  }
};

class TestConsumer : public POA_CosNotifyComm::StructuredPushConsumer
{
public:
  TestConsumer () = default;
  TestConsumer (TestConsumer const&) = delete;
  TestConsumer (TestConsumer&&) = delete;
  TestConsumer& operator= (TestConsumer const&) = delete;
  TestConsumer& operator= (TestConsumer&&) = delete;

private:
  void disconnect_structured_push_consumer () override
  {
  }

  void offer_change (CosNotification::EventTypeSeq const&,
                     CosNotification::EventTypeSeq const&) override
  {
  }

  void push_structured_event (CosNotification::StructuredEvent const&) override
  {
  }
};

struct Pass_Options
{
  bool pass1 {};
  bool pass2 {};
};

int ACE_TMAIN (int argc, ACE_TCHAR* argv[])
{
  int retval {1};

  Pass_Options pass_options {};
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);
      ACE_ASSERT (!CORBA::is_nil (orb.in ()));

      if (argc > 2 && ACE_OS::strcmp (argv[1], ACE_TEXT ("-pass")) == 0)
        {
          int const pass = argv[2][0] - '0';
          switch (pass)
            {
            case 1:
              pass_options.pass1 = true;
              break;
            case 2:
              pass_options.pass2 = true;
              break;
            case 3:
              pass_options.pass1 = true;
              pass_options.pass2 = true;
              break;
            default:
              ACE_OS::fprintf (stderr, "Illegal -pass command line option.  Expecting 1, 2, or 3\n");
              return -1;
            }
        }

      if (!pass_options.pass1 && !pass_options.pass2)
        {
          std::unique_ptr<FILE, int (*) (FILE *)> load_file (
            ACE_OS::fopen ("loadtest.xml", "r"), ACE_OS::fclose);
          if (load_file)
            pass_options.pass2 = true;
          else
            pass_options.pass1 = true;
        }

      CORBA::Object_var const poa_object = orb->resolve_initial_references ("RootPOA");
      ACE_ASSERT (!CORBA::is_nil (poa_object.in ()));
      PortableServer::POA_var poa = PortableServer::POA::_narrow (poa_object.in ());
      ACE_ASSERT (!CORBA::is_nil (poa.in ()));
      PortableServer::POAManager_var const manager = poa->the_POAManager ();
      manager->activate ();

      CORBA::PolicyList policies (1);
      policies.length (1);

      policies[0] = poa->create_lifespan_policy (PortableServer::PERSISTENT);

      PortableServer::POA_var const persistent_poa = poa->create_POA (
        "PersistentPOA", manager.in (), policies);

      policies[0]->destroy ();

      if (pass_options.pass1)
        {
          CosNotifyChannelAdmin::EventChannelFactory_var const cosecf =
            TAO_Notify_EventChannelFactory_i::create (persistent_poa.in ());

          NotifyExt::EventChannelFactory_var const ecf =
            NotifyExt::EventChannelFactory::_narrow (cosecf.in ());

          if (CORBA::is_nil (ecf.in ()))
            return -1;

          CosNotification::QoSProperties qosprops (7);
          CORBA::ULong index {};

          qosprops.length (7);
          qosprops[index].name = CORBA::string_dup (CosNotification::EventReliability);
          qosprops[index++].value <<= CosNotification::Persistent;
          qosprops[index].name = CORBA::string_dup (CosNotification::ConnectionReliability);
          qosprops[index++].value <<= CosNotification::Persistent; // Required, or we won't persist much.
          qosprops[index].name = CORBA::string_dup (CosNotification::Priority);
          qosprops[index++].value <<= CosNotification::HighestPriority;
          qosprops[index].name = CORBA::string_dup (CosNotification::Timeout);
          qosprops[index++].value <<= static_cast<TimeBase::TimeT> (42000); // 4.2 ms
          qosprops[index].name = CORBA::string_dup (CosNotification::StopTimeSupported);
          qosprops[index++].value <<= CORBA::Any::from_boolean (true);
          qosprops[index].name = CORBA::string_dup (CosNotification::MaximumBatchSize);
          qosprops[index++].value <<= static_cast<CORBA::Long> (555);
          qosprops[index].name = CORBA::string_dup (CosNotification::PacingInterval);
          qosprops[index++].value <<= static_cast<TimeBase::TimeT> (34300); // 3.4 ms
          qosprops.length (index);

          CosNotification::AdminProperties adminprops (4);
          adminprops.length (4);
          index = 0;
          adminprops[index].name = CORBA::string_dup (CosNotification::MaxQueueLength);
          adminprops[index++].value <<= static_cast<CORBA::Long> (1234);
          adminprops[index].name = CORBA::string_dup (CosNotification::MaxConsumers);
          adminprops[index++].value <<= static_cast<CORBA::Long> (3);
          adminprops[index].name = CORBA::string_dup (CosNotification::MaxSuppliers);
          adminprops[index++].value <<= static_cast<CORBA::Long> (3);
          adminprops[index].name = CORBA::string_dup (CosNotification::RejectNewEvents);
          adminprops[index++].value <<= CORBA::Any::from_boolean (true);
          adminprops.length (index);

          CosNotifyChannelAdmin::ChannelID ecid {};
          CosNotifyChannelAdmin::EventChannel_var ec =
            ecf->create_channel (qosprops, adminprops, ecid);

          CosNotifyChannelAdmin::AdminID consumer_admin_id {};
          CosNotifyChannelAdmin::ConsumerAdmin_var ca =
            ec->new_for_consumers (CosNotifyChannelAdmin::OR_OP, consumer_admin_id);

          CosNotifyChannelAdmin::AdminID supplier_admin_id {};
          CosNotifyChannelAdmin::SupplierAdmin_var sa =
            ec->new_for_suppliers (CosNotifyChannelAdmin::OR_OP, supplier_admin_id);

          CosNotifyChannelAdmin::ProxyID proxy_id {};
          CosNotifyChannelAdmin::ProxySupplier_var ps =
            ca->obtain_notification_push_supplier (CosNotifyChannelAdmin::STRUCTURED_EVENT, proxy_id);

          CosNotifyChannelAdmin::StructuredProxyPushSupplier_var strps =
            CosNotifyChannelAdmin::StructuredProxyPushSupplier::_narrow (ps.in ());

          ps = ca->obtain_notification_push_supplier (CosNotifyChannelAdmin::SEQUENCE_EVENT, proxy_id);
          CosNotifyChannelAdmin::SequenceProxyPushSupplier_var seqps =
            CosNotifyChannelAdmin::SequenceProxyPushSupplier::_narrow (ps.in ());

          ps = ca->obtain_notification_push_supplier (CosNotifyChannelAdmin::ANY_EVENT, proxy_id);
          CosNotifyChannelAdmin::ProxyPushSupplier_var anyps =
            CosNotifyChannelAdmin::ProxyPushSupplier::_narrow (ps.in ());

          CosNotifyChannelAdmin::ProxyConsumer_var pc =
            sa->obtain_notification_push_consumer (CosNotifyChannelAdmin::STRUCTURED_EVENT, proxy_id);
          CosNotifyChannelAdmin::StructuredProxyPushConsumer_var strpc =
            CosNotifyChannelAdmin::StructuredProxyPushConsumer::_narrow (pc.in ());

          pc = sa->obtain_notification_push_consumer (CosNotifyChannelAdmin::SEQUENCE_EVENT, proxy_id);
          CosNotifyChannelAdmin::SequenceProxyPushConsumer_var seqpc =
            CosNotifyChannelAdmin::SequenceProxyPushConsumer::_narrow (pc.in ());

          pc = sa->obtain_notification_push_consumer (CosNotifyChannelAdmin::ANY_EVENT, proxy_id);
          CosNotifyChannelAdmin::ProxyPushConsumer_var anypc =
            CosNotifyChannelAdmin::ProxyPushConsumer::_narrow (pc.in ());

          CosNotifyFilter::FilterFactory_var const filter_factory = ec->default_filter_factory ();
          CosNotifyFilter::Filter_var const filter1 = filter_factory->create_filter ("EXTENDED_TCL");
          ACE_ASSERT (!CORBA::is_nil (filter1.in ()));
          CosNotifyFilter::Filter_var const filter2 = filter_factory->create_filter ("EXTENDED_TCL");
          ACE_ASSERT (!CORBA::is_nil (filter2.in ()));

          CosNotifyFilter::ConstraintExpSeq constraint_list (1);
          constraint_list.length (1);
          constraint_list[0].event_types.length (0);
          constraint_list[0].constraint_expr = CORBA::string_dup ("Number == 100");

          filter1->add_constraints (constraint_list);
          filter2->add_constraints (constraint_list);

          ca->add_filter (filter1.in ());
          sa->add_filter (filter2.in ());
          strps->add_filter (filter1.in ());
          seqps->add_filter (filter2.in ());
          anyps->add_filter (filter1.in ());
          strpc->add_filter (filter2.in ());
          seqpc->add_filter (filter1.in ());
          anypc->add_filter (filter1.in ());
          anypc->add_filter (filter2.in ());

          CosNotification::EventTypeSeq added (1);
          CosNotification::EventTypeSeq removed;
          added.length (1);
          added[0].domain_name = CORBA::string_dup ("nightly_builds");
          added[0].type_name = CORBA::string_dup ("*");
          ca->subscription_change (added, removed);

          // Connect a PushConsumer and PushSupplier.
          TestSupplier test_supplier_svt;
          PortableServer::ObjectId_var const supplier_oid = persistent_poa->activate_object (&test_supplier_svt);
          CORBA::Object_var const supplier_object = persistent_poa->id_to_reference (supplier_oid.in ());
          CosNotifyComm::StructuredPushSupplier_var const push_supplier =
            CosNotifyComm::StructuredPushSupplier::_narrow (supplier_object.in ());

          TestConsumer test_consumer_svt;
          PortableServer::ObjectId_var const consumer_oid = persistent_poa->activate_object (&test_consumer_svt);
          CORBA::Object_var const consumer_object = persistent_poa->id_to_reference (consumer_oid.in ());
          CosNotifyComm::StructuredPushConsumer_var const push_consumer =
            CosNotifyComm::StructuredPushConsumer::_narrow (consumer_object.in ());
          strpc->connect_structured_push_supplier (push_supplier.in ());
          strps->connect_structured_push_consumer (push_consumer.in ());
          strps->suspend_connection ();

          persistent_poa->deactivate_object (supplier_oid.in ());
          persistent_poa->deactivate_object (consumer_oid.in ());
          ecf->destroy ();

          // TODO: make this not hardcoded.
          ACE_OS::rename ("abc.xml", "loadtest.xml");
        }

      if (pass_options.pass2)
        {
          // Create a new factory, which should load itself from loadtest.xml.
          CosNotifyChannelAdmin::EventChannelFactory_var const cosecf =
            TAO_Notify_EventChannelFactory_i::create (persistent_poa.in ());
          NotifyExt::EventChannelFactory_var const ecf =
            NotifyExt::EventChannelFactory::_narrow (cosecf.in ());
          if (CORBA::is_nil (ecf.in ()))
            return -1;

          // Force a change, which should write out a new abc.xml.
          ecf->save_topology ();
          ecf->destroy ();
        }

      poa->destroy (true, true);
      orb->destroy ();
      poa = PortableServer::POA::_nil ();
      orb = CORBA::ORB::_nil ();
      retval = 0;
    }
  catch (CORBA::Exception const& ex)
    {
      ex._tao_print_exception ("Error: Unexpected exception caught in main. ");
      retval = -1;
    }
  catch (...)
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Error : Unknown exception caught in main.")));
      retval = -2;
    }
  return retval;
}
