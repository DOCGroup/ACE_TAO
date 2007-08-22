/**
 *  $Id$
 */

#include "orbsvcs/CosNotificationC.h"
#include "orbsvcs/NotifyExtC.h"
#include "orbsvcs/Notify/Notify_EventChannelFactory_i.h"

// On SunOS 5.8 and MacOS X, the static initialization trick used
// in the CosNotification_Serv library does not work.  Including the
// initializer class here works around the problem.
#if defined (sun) || defined (__APPLE__)
#include "orbsvcs/Notify/CosNotify_Initializer.h"
#endif /* sun || __APPLE__ */

#include "tao/TimeBaseC.h"
#include "tao/corba.h"
#include "tao/PortableServer/PortableServer.h"

#include "ace/OS_NS_string.h"
#include "ace/OS_NS_stdio.h"
#include "ace/Dynamic_Service.h"
#include "ace/ARGV.h"

class TestSupplier
: public POA_CosNotifyComm::StructuredPushSupplier
{
  virtual void disconnect_structured_push_supplier(void)
  {
  }

  virtual void subscription_change( const CosNotification::EventTypeSeq&,
                                    const CosNotification::EventTypeSeq&)
  {
  }
};

class TestConsumer
: public POA_CosNotifyComm::StructuredPushConsumer
{
  virtual void disconnect_structured_push_consumer(void)
  {
  }

  virtual void offer_change( const CosNotification::EventTypeSeq&, const CosNotification::EventTypeSeq&)
  {
  }

  virtual void push_structured_event(const CosNotification::StructuredEvent&)
  {
  }
};

int main(int ac, char **av)
{
  int retval = 1;

  bool pass1 = false;
  bool pass2 = false;
  try
  {
    CORBA::ORB_var orb;
    PortableServer::POA_var poa;

    orb = CORBA::ORB_init(ac, av, "");
    ACE_ASSERT(! CORBA::is_nil (orb.in ()));

    if (ac > 2 && ACE_OS::strcmp (av[1], "-pass") == 0)
    {
      int pn = av[2][0] - '0';
      switch (pn)
      {
      case 1:
        pass1 = true;
        pass2 = false;
        break;
      case 2:
        pass1 = false;
        pass2 = true;
        break;
      case 3:
        pass1 = true;
        pass2 = true;
        break;
      default:
        ACE_OS::fprintf (stderr, "Illegal -pass command line option.  Expecting 1, 2, or 3\n");
        return -1;
      }
    }
    if (! pass1 && !pass2)
    {
      FILE *f;
      f = ACE_OS::fopen ("loadtest.xml", "r");
      if (f != 0)
      {
        ACE_OS::fclose (f);
        pass1 = false;
        pass2 = true;
      }
      else
      {
        pass1 = true;
        pass2 = false;
      }
    }
    CORBA::Object_var obj =
      orb->resolve_initial_references("RootPOA");
    ACE_ASSERT(! CORBA::is_nil (obj.in ()));
    poa = PortableServer::POA::_narrow(obj.in ());
    ACE_ASSERT(! CORBA::is_nil (poa.in ()));
    PortableServer::POAManager_var mgr = poa->the_POAManager();
    mgr->activate();

    CORBA::PolicyList policies (1);
    policies.length (1);

    policies[0] =
      poa->create_lifespan_policy (PortableServer::PERSISTENT);

    PortableServer::POA_var persistentPOA = poa->create_POA (
                                                    "PersistentPOA",
                                                    mgr.in (),
                                                    policies);

    policies[0]->destroy ();

    if (pass1)
    {
      CosNotifyChannelAdmin::EventChannelFactory_var cosecf =
        TAO_Notify_EventChannelFactory_i::create(persistentPOA.in ());

      NotifyExt::EventChannelFactory_var ecf =
        NotifyExt::EventChannelFactory::_narrow (cosecf.in ());

      if (CORBA::is_nil (ecf.in ()))
      {
        return -1;
      }

      CosNotification::QoSProperties qosprops(7);
      CORBA::ULong i = 0;

      qosprops.length(7);
      qosprops[i].name = CORBA::string_dup(CosNotification::EventReliability);
      qosprops[i++].value <<= CosNotification::Persistent;
      qosprops[i].name = CORBA::string_dup(CosNotification::ConnectionReliability);
      qosprops[i++].value <<= CosNotification::Persistent; // Required, or we won't persist much
      qosprops[i].name = CORBA::string_dup(CosNotification::Priority);
      qosprops[i++].value <<= CosNotification::HighestPriority;
      qosprops[i].name = CORBA::string_dup(CosNotification::Timeout);
      qosprops[i++].value <<= (TimeBase::TimeT) 42000; // 4.2 ms
      qosprops[i].name = CORBA::string_dup(CosNotification::StopTimeSupported);
      qosprops[i++].value <<= CORBA::Any::from_boolean(1);
      qosprops[i].name = CORBA::string_dup(CosNotification::MaximumBatchSize);
      qosprops[i++].value <<= (CORBA::Long) 555;
      qosprops[i].name = CORBA::string_dup(CosNotification::PacingInterval);
      qosprops[i++].value <<= (TimeBase::TimeT) 34300; // 3.4 ms
      qosprops.length(i);

      CosNotification::AdminProperties adminprops(4);
      adminprops.length(4);
      i = 0;
      adminprops[i].name = CORBA::string_dup(CosNotification::MaxQueueLength);
      adminprops[i++].value <<= (CORBA::Long) 1234;
      adminprops[i].name = CORBA::string_dup(CosNotification::MaxConsumers);
      adminprops[i++].value <<= (CORBA::Long) 3;
      adminprops[i].name = CORBA::string_dup(CosNotification::MaxSuppliers);
      adminprops[i++].value <<= (CORBA::Long) 3;
      adminprops[i].name = CORBA::string_dup(CosNotification::RejectNewEvents);
      adminprops[i++].value <<= CORBA::Any::from_boolean(1);
      adminprops.length (i);

      CosNotifyChannelAdmin::ChannelID ecid;
      ::CosNotifyChannelAdmin::EventChannel_var ec =
        ecf->create_channel(qosprops, adminprops, ecid);

      CosNotifyChannelAdmin::AdminID consumer_admin_id;
      CosNotifyChannelAdmin::ConsumerAdmin_var ca =
        ec->new_for_consumers(CosNotifyChannelAdmin::OR_OP,
          consumer_admin_id);

      CosNotifyChannelAdmin::AdminID supplier_admin_id;
      CosNotifyChannelAdmin::SupplierAdmin_var sa =
        ec->new_for_suppliers(CosNotifyChannelAdmin::OR_OP,
          supplier_admin_id);

      CosNotifyChannelAdmin::ProxyID proxy_id;
      CosNotifyChannelAdmin::ProxySupplier_var ps =
        ca->obtain_notification_push_supplier(
          CosNotifyChannelAdmin::STRUCTURED_EVENT,
          proxy_id);

      CosNotifyChannelAdmin::StructuredProxyPushSupplier_var strps =
        CosNotifyChannelAdmin::StructuredProxyPushSupplier::_narrow(ps.in());

      ps = ca->obtain_notification_push_supplier(
          CosNotifyChannelAdmin::SEQUENCE_EVENT,
          proxy_id);

      CosNotifyChannelAdmin::SequenceProxyPushSupplier_var seqps =
        CosNotifyChannelAdmin::SequenceProxyPushSupplier::_narrow(ps.in());

      ps = ca->obtain_notification_push_supplier(
          CosNotifyChannelAdmin::ANY_EVENT,
          proxy_id);

      CosNotifyChannelAdmin::ProxyPushSupplier_var anyps =
        CosNotifyChannelAdmin::ProxyPushSupplier::_narrow(ps.in());

      CosNotifyChannelAdmin::ProxyConsumer_var pc =
        sa->obtain_notification_push_consumer(
          CosNotifyChannelAdmin::STRUCTURED_EVENT,
          proxy_id);

      CosNotifyChannelAdmin::StructuredProxyPushConsumer_var strpc = CosNotifyChannelAdmin::StructuredProxyPushConsumer::_narrow(pc.in());

      pc = sa->obtain_notification_push_consumer(
          CosNotifyChannelAdmin::SEQUENCE_EVENT,
          proxy_id);

      CosNotifyChannelAdmin::SequenceProxyPushConsumer_var seqpc = CosNotifyChannelAdmin::SequenceProxyPushConsumer::_narrow(pc.in());

      pc =
        sa->obtain_notification_push_consumer(
          CosNotifyChannelAdmin::ANY_EVENT,
          proxy_id);

      CosNotifyChannelAdmin::ProxyPushConsumer_var anypc = CosNotifyChannelAdmin::ProxyPushConsumer::_narrow(pc.in());

      CosNotifyFilter::FilterFactory_var ff =
        ec->default_filter_factory ();

      CosNotifyFilter::Filter_var filter1 =
        ff->create_filter("EXTENDED_TCL");

      ACE_ASSERT(! CORBA::is_nil (filter1.in ()));

      CosNotifyFilter::Filter_var filter2 =
        ff->create_filter("EXTENDED_TCL");

      ACE_ASSERT(! CORBA::is_nil (filter2.in ()));

      CosNotifyFilter::ConstraintExpSeq constraint_list(1);
      constraint_list.length(1);
      constraint_list[0].event_types.length(0);
      constraint_list[0].constraint_expr = CORBA::string_dup("Number == 100");

      filter1->add_constraints(constraint_list);

      filter2->add_constraints(constraint_list);

      ca->add_filter (filter1.in());

      sa->add_filter (filter2.in());

      strps->add_filter (filter1.in());

      seqps->add_filter (filter2.in());

      anyps->add_filter (filter1.in());

      strpc->add_filter (filter2.in());

      seqpc->add_filter (filter1.in());

      anypc->add_filter (filter1.in());
      anypc->add_filter (filter2.in());

      CosNotification::EventTypeSeq added1(1), removed1(0);
      added1.length(1);
      added1[0].domain_name = CORBA::string_dup("nightly_builds");
      added1[0].type_name = CORBA::string_dup("*");
      ca->subscription_change(added1, removed1);

      // Connect a PushConsumer and PushSupplier
      TestSupplier test_supplier_svt;
      PortableServer::ObjectId_var oid1 = persistentPOA->activate_object (&test_supplier_svt);
      CORBA::Object_var obj1 = persistentPOA->id_to_reference (oid1.in ());
      CosNotifyComm::StructuredPushSupplier_var push_sup = CosNotifyComm::StructuredPushSupplier::_narrow (obj1.in ());

      TestConsumer test_consumer_svt;
      PortableServer::ObjectId_var oid2 = persistentPOA->activate_object (&test_consumer_svt);
      CORBA::Object_var obj2 = persistentPOA->id_to_reference (oid2.in ());
      CosNotifyComm::StructuredPushConsumer_var push_cons = CosNotifyComm::StructuredPushConsumer::_narrow (obj2.in ());
      strpc->connect_structured_push_supplier(push_sup.in());
      strps->connect_structured_push_consumer(push_cons.in());

      strps->suspend_connection();

      persistentPOA->deactivate_object (oid1.in ());

      persistentPOA->deactivate_object (oid2.in ());

      ecf->destroy();
      ////////////////////////////////
      // TODO make this not hardcoded
      ACE_OS::rename ("abc.xml", "loadtest.xml");

    } // end of pass 1

    if (pass2)
    {

      // Create a new ecf, which should load itself from loadtest.xml
      CosNotifyChannelAdmin::EventChannelFactory_var
        cosecf = TAO_Notify_EventChannelFactory_i::create(persistentPOA.in ());

      NotifyExt::EventChannelFactory_var
        ecf = NotifyExt::EventChannelFactory::_narrow (cosecf.in ());

      if (CORBA::is_nil (ecf.in ()))
      {
        return -1;
      }

      // Force a change, which should write out a new abc.xml.
      ecf->save_topology ();

      ecf->destroy();
    }

    poa->destroy (1, 1);
    orb->destroy ();
    poa = PortableServer::POA::_nil ();
    orb = CORBA::ORB::_nil ();
    retval = 0;
  }
  catch (const CORBA::Exception& ex)
  {
    ex._tao_print_exception ("Error: Unexpected exception caught in main. ");
    retval = -1;
  }
  catch (...)
  {
    ACE_DEBUG ((LM_DEBUG,
      ACE_TEXT ("Error : Unknown exception caught in main.") ));
    retval = -2;
  }
  return retval;
}
