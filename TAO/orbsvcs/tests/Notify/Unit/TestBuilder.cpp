// $Id$
#include "TestBuilder.h"

#include <orbsvcs/Notify/Builder.h>
#include <orbsvcs/Notify/Factory.h>
#include <orbsvcs/Notify/Properties.h>
#include <orbsvcs/Notify/EventChannelFactory.h>
// The following includes are only needed for vc6
#include <orbsvcs/Notify/EventChannel.h>
#include <orbsvcs/Notify/ConsumerAdmin.h>
#include <orbsvcs/Notify/SupplierAdmin.h>
#include <orbsvcs/Notify/Admin.h>
#include <orbsvcs/Notify/Proxy.h>
#include <orbsvcs/Notify/ProxyConsumer.h>
#include <orbsvcs/Notify/ProxySupplier.h>

#include <orbsvcs/Notify/Topology_Saver.h>

#include <ace/ARGV.h>

using namespace CORBA;
using namespace PortableServer;
namespace CNCA = CosNotifyChannelAdmin;
namespace CN = CosNotification;
namespace CNC = CosNotifyComm;
namespace CNF = CosNotifyFilter;
using namespace TAO_NOTIFY;

namespace {
  void init_qos_props(CN::QoSProperties& qos, bool topology_persist) {
    qos.length(13);
    ULong i = 0;
    qos[i].name = CORBA::string_dup ("EventReliability");
    qos[i++].value <<= CN::BestEffort;
    if (topology_persist) {
      qos[i].name = CORBA::string_dup ("ConnectionReliability");
      qos[i++].value <<= CN::Persistent;
    }
    else
    {
      qos[i].name = CORBA::string_dup ("ConnectionReliability");
      qos[i++].value <<= CN::BestEffort;
    }
    qos[i].name = CORBA::string_dup ("Priority");
    qos[i++].value <<= (Short) 1234;
    qos[i].name = CORBA::string_dup ("Timeout");
    qos[i++].value <<= (TimeBase::TimeT) 55555;
    qos[i].name = CORBA::string_dup ("MaximumBatchSize");
    qos[i++].value <<= (Long) 200;
    qos[i].name = CORBA::string_dup ("PacingInterval");
    qos[i++].value <<= (TimeBase::TimeT) 420000;
    qos[i].name = CORBA::string_dup ("StopTimeSupported");
    qos[i++].value <<= Any::from_boolean(0);
    qos.length(i);
  }
  void init_admin_props(CN::AdminProperties& admin) {
    admin.length(13);
    ULong i = 0;
    admin[i].name = CORBA::string_dup ("MaxQueueLength");
    admin[i++].value <<= (Long) 200;
    admin[i].name = CORBA::string_dup ("MaxConsumers");
    admin[i++].value <<= (Long) 3;
    admin[i].name = CORBA::string_dup ("MaxSuppliers");
    admin[i++].value <<= (Long) 3;
    admin[i].name = CORBA::string_dup ("RejectNewEvents");
    admin[i++].value <<= Any::from_boolean(1);
  }
}

namespace TestBuilder
{
  class DestroyChecker : public TAO_NS_Destroy_Callback {
  public:
    DestroyChecker() : cnt(0) {
    }
    virtual void release (void) {
      cnt++;
    }
    int cnt;
  };

  class TestTopologyFactory;

  class TestSaver : public TAO_NOTIFY::Topology_Saver {
    TestTopologyFactory& f_;
    bool verbose_;
  public:
    TestSaver(TestTopologyFactory& fact, bool verbose)
      : f_(fact)
      , verbose_ (verbose)
    {
    }
    virtual bool begin_object (CORBA::Long id,
      const ACE_CString &type,
      const NVPList& attrs,
      bool changed
      ACE_ENV_ARG_DECL);

    virtual void end_object (CORBA::Long id,
      const ACE_CString &type ACE_ENV_ARG_DECL);
  };

  class TestTopologyFactory : public TAO_NOTIFY::Topology_Factory {
  public:
    int num_begin;  // @@ todo: public data? Aw, comon guys!
    int num_end;
    bool verbose_;

    TestTopologyFactory(bool verbose = false)
      : num_begin(0)
      , num_end(0)
      , verbose_ (verbose)
    {
    }
    virtual Topology_Saver* create_saver()
    {
      return new TestSaver(*this, this->verbose_);
    }

    virtual Topology_Loader* create_loader()
    {
      return 0;
    }

    void reset()
    {
      num_begin = 0;
      num_end = 0;
    }
  };

  inline bool TestSaver::begin_object (CORBA::Long id,
    const ACE_CString& name,
    const NVPList& attrs,
    bool
    ACE_ENV_ARG_DECL)
  {
    if (this->verbose_)
    {
      cout << "begin [" << f_.num_begin + 1 << "] " << id << " " << name << " {" << attrs.size() << "}" << endl;
    }
    ++f_.num_begin;
    return true;
  }
  inline void TestSaver::end_object (CORBA::Long id,
      const ACE_CString& name ACE_ENV_ARG_DECL)
  {
    if (this->verbose_)
    {
      cout << "end   [" << f_.num_end + 1 << "] " << id << " " << name << endl;
    }
    ++f_.num_end;
  }

  ORB_var orb;
  POA_var poa;

  TAO_NS_Properties* props;
  TAO_NS_Factory factory;
  TAO_NS_Builder builder;

  class SetupTeardown {
  public:
    SetupTeardown() {
      setUp();
    }
    ~SetupTeardown() {
      try {
        tearDown();
      } catch (...) {
      }
    }
  private:
    void setUp()
    {
      ACE_ARGV av;
      int ac = 0;
      orb = ORB_init(ac, av.argv());
      ACE_ASSERT(! is_nil(orb.in()));
      Object_var obj = orb->resolve_initial_references("RootPOA");
      ACE_ASSERT(! is_nil(obj.in()));
      poa = POA::_narrow(obj.in());
      ACE_ASSERT(! is_nil(poa.in()));
      POAManager_var mgr = poa->the_POAManager();
      mgr->activate();

      props = TAO_NS_PROPERTIES::instance();
      props->orb(orb.in());
      props->default_poa(poa.in());
      props->factory(&factory);
      props->builder(&builder);
    }
    void tearDown()
    {
      poa->destroy(1,1);
      orb->destroy();
      poa = POA::_nil();
      orb = ORB::_nil();
    }
  };

  void testBuildECF(ACE_ENV_SINGLE_ARG_DECL) {
    SetupTeardown sutd;
    TAO_NS_EventChannelFactory* ecf = builder.build_event_channel_factory(poa.in());
    assertTrue(ecf != 0);
    assertEquals(-1, ecf->id());

    // Test that the object is destroyed.
    DestroyChecker dc;
    ecf->destroy_callback(&dc);
    ecf->deactivate();
    assertEquals(1, dc.cnt);
    delete ecf;
  }

  void testBuildEC(ACE_ENV_SINGLE_ARG_DECL) {
    SetupTeardown sutd;
    TAO_NS_EventChannelFactory* ecf = builder.build_event_channel_factory(poa.in());
    TAO_NS_EventChannel* ec = builder.build_event_channel(ecf->object_poa(), CN::QoSProperties(), CN::AdminProperties(), 42);
    assertTrue(ec != 0);
    assertEquals(-1, ecf->id());
    assertEquals(42, ec->id());

    // Test that the object is destroyed.
    DestroyChecker dc;
    ec->destroy_callback(&dc);
    ec->deactivate();
    assertEquals(1, dc.cnt);
    delete ec;

    ec = builder.build_event_channel(ecf->object_poa(), CN::QoSProperties(), CN::AdminProperties(), 0);
    assertTrue(ec != 0);
    ec->destroy_callback(&dc);
    dc.cnt = 0;
    assertEquals(43, ec->id());

    ec = builder.build_event_channel(ecf->object_poa(), CN::QoSProperties(), CN::AdminProperties(), 10);
    assertTrue(ec != 0);
    ec->destroy_callback(&dc);
    assertEquals(0, dc.cnt);
    assertEquals(10, ec->id());

    ec = builder.build_event_channel(ecf->object_poa(), CN::QoSProperties(), CN::AdminProperties(), 0);
    assertTrue(ec != 0);
    ec->destroy_callback(&dc);
    assertEquals(0, dc.cnt);
    assertEquals(44, ec->id()); // will be 11 if the id generator is broken.

    ecf->deactivate();
    // @@ todo : assertEquals(3, dc.cnt); because destroying the ecf should destroy the ecs
  }

  void testTopologySaveBasic(ACE_ENV_SINGLE_ARG_DECL) {
    SetupTeardown sutd;
    TAO_NS_EventChannelFactory* ecf = builder.build_event_channel_factory(poa.in());
    assertTrue(ecf != 0);
    TestTopologyFactory tsf;
    ecf->set_topology_factory(&tsf);

    assertEquals(0, tsf.num_begin);
    assertEquals(0, tsf.num_end);

    // This will cause the ecf to use our tsf to create a saver, and save.
    ecf->child_change(ACE_ENV_SINGLE_ARG_PARAMETER);

    // Since we start persisting topology at the ecf level, there will always be
    // at least a minimal empty topology persisted.
    assertEquals(1, tsf.num_begin);
    assertEquals(1, tsf.num_end);
    tsf.reset();
    assertEquals(0, tsf.num_begin);
    assertEquals(0, tsf.num_end);

    CN::QoSProperties qos;
    CN::AdminProperties admin;
    init_qos_props(qos, false);
    init_admin_props(admin);

    // Creating a channel, should call signal_change, but topology
    // persistence is disabled, so only the ecf should be written again.
    TAO_NS_EventChannel* ec = ecf->create_channel_i(qos, admin);
    assertTrue(ec != 0);
    assertEquals(1, ec->id());
    assertEquals(1, tsf.num_begin); // + begin ecf
    assertEquals(1, tsf.num_end);
    tsf.reset();

    // This time we'll persist the new channel
    init_qos_props(qos, true);
    ec = ecf->create_channel_i(qos, admin);
    assertTrue(ec != 0);
    assertEquals(2, ec->id());
    assertEquals(2, tsf.num_begin);
    assertEquals(2, tsf.num_end);
  }

  void testTopologyPersist(ACE_ENV_SINGLE_ARG_DECL)
  {
    SetupTeardown sutd;
    TAO_NS_EventChannelFactory* ecf = builder.build_event_channel_factory(poa.in());

    TestTopologyFactory tsf;
    ecf->set_topology_factory(&tsf);

    // This will cause the ecf to use our tsf to create a saver, and save.
    ecf->child_change(ACE_ENV_SINGLE_ARG_PARAMETER);
    int expected = 2; // 1 ecf, +1 reconnect reg

    assertEquals(expected, tsf.num_begin);
    assertEquals(tsf.num_begin, tsf.num_end);
    tsf.reset();

    CN::QoSProperties qos;
    CN::AdminProperties admin;
    init_qos_props(qos, true);
    init_admin_props(admin);

    // Creating a channel, should call signal_change
    TAO_NS_EventChannel* ec = ecf->create_channel_i(qos, admin);
    expected += 1; // 1 channel
    assertTrue(ec != 0);
    assertEquals(1, ec->id());
    assertEquals(expected, tsf.num_begin);
    assertEquals(tsf.num_begin, tsf.num_end);
    tsf.reset();

    TAO_NS_ConsumerAdmin* ca = ec->new_for_consumers_i(CNCA::OR_OP);
    expected += 3; // 1 consumeradmin, 1 subs section, 1 subscription (*/%ALL)
    assertTrue(ca != 0);
    assertEquals(2, ca->id());
    assertEquals(expected, tsf.num_begin);
    assertEquals(tsf.num_begin, tsf.num_end);
    tsf.reset();

    TAO_NS_SupplierAdmin* sa = ec->new_for_suppliers_i(CNCA::OR_OP);
    expected += 3; // 1 supplieradmin, 1 subs section, 1 subscription (*/%ALL)
    assertTrue(sa != 0);
    assertEquals(3, sa->id());
    assertEquals(expected, tsf.num_begin);
    assertEquals(tsf.num_begin, tsf.num_end);
    tsf.reset();

    // consumer #1
    TAO_NS_ProxySupplier* ps = ca->obtain_notification_push_supplier_i(CNCA::STRUCTURED_EVENT);
    expected += 1; // 1 proxy
    assertTrue(ps != 0);
    assertEquals(4, ps->id());
    assertEquals(expected, tsf.num_begin);
    assertEquals(tsf.num_begin, tsf.num_end);
    tsf.reset();

    // consumer #2
    ps = ca->obtain_notification_push_supplier_i(CNCA::SEQUENCE_EVENT);
    expected += 1; // 1 proxy
    assertTrue(ps != 0);
    assertEquals(5, ps->id());
    assertEquals(expected, tsf.num_begin);
    assertEquals(tsf.num_begin, tsf.num_end);
    tsf.reset();

    // consumer #3
    ps = ca->obtain_notification_push_supplier_i(CNCA::ANY_EVENT);
    expected += 1; // 1 proxy
    assertTrue(ps != 0);
    assertEquals(6, ps->id());
    assertEquals(expected, tsf.num_begin);
    assertEquals(tsf.num_begin, tsf.num_end);
    tsf.reset();

    // supplier #1
    TAO_NS_ProxyConsumer* pc = sa->obtain_notification_push_consumer_i(CNCA::STRUCTURED_EVENT);
    expected += 1; // 1 proxy
    assertTrue(pc != 0);
    assertEquals(7, pc->id());
    assertEquals(expected, tsf.num_begin);
    assertEquals(tsf.num_begin, tsf.num_end);
    tsf.reset();

    // supplier #2
    pc = sa->obtain_notification_push_consumer_i(CNCA::SEQUENCE_EVENT);
    expected += 1; // 1 proxy
    assertTrue(pc != 0);
    assertEquals(8, pc->id());
    assertEquals(expected, tsf.num_begin);
    assertEquals(tsf.num_begin, tsf.num_end);
    tsf.reset();

    // supplier #3
    pc = sa->obtain_notification_push_consumer_i(CNCA::ANY_EVENT);
    expected += 1; // 1 proxy
    assertTrue(pc != 0);
    assertEquals(9, pc->id());
    assertEquals(expected, tsf.num_begin);
    assertEquals(tsf.num_begin, tsf.num_end);
    tsf.reset();

    // Setting the ConnectionReliability = false should not only trigger
    // another save, but should cause the last push consumer to no longer
    // be persisted
    // On one hand if we save it, we're saving something
    // that's not suposed to be saved.  On the other hand
    // if we don't save it, we're leaving the old version
    // out there with the old qos (on updatable as opposed
    // to replacable persisnt stores.

    init_qos_props(qos, false);
    pc->set_qos(qos);
    int nothing = 0;
    // a change of a non-persistent object does not
    // trigger a save
    assertEquals(nothing, tsf.num_begin);

    ec->child_change (ACE_ENV_SINGLE_ARG_PARAMETER);
    expected -= 1; // 1 proxy
    assertEquals(expected, tsf.num_begin);
    assertEquals(tsf.num_begin, tsf.num_end);
    tsf.reset();

    ps->set_qos(qos);
    assertEquals(nothing, tsf.num_begin);

    ec->child_change (ACE_ENV_SINGLE_ARG_PARAMETER);
    expected -= 1; // 1 proxy
    assertEquals(expected, tsf.num_begin);
    assertEquals(tsf.num_begin, tsf.num_end);
    tsf.reset();

    //SCOPE
    {
      ACE_TRY_NEW_ENV
      {
        ca->set_qos(qos ACE_ENV_ARG_PARAMETER);
        failIt("Expected UnsupportedQoS, because children exist.");
      }
      ACE_CATCH (CN::UnsupportedQoS, ex)
      {
        // this is good
      }
      ACE_CATCHALL;
      {
        failIt("unknown exception received from set_qos.");
      }
      ACE_ENDTRY;
    } // endscope

    ecf->deactivate(ACE_ENV_ARG_PARAMETER);
  }

  // Test that at least one object (CA) supports subscription topo correctly
  // and test a few basic subscription functions
  void testSubscriptionTopology()
  {
    SetupTeardown sutd;
    TAO_NS_EventChannelFactory* ecf = builder.build_event_channel_factory(poa.in());
    TestTopologyFactory tsf(false);

    CN::QoSProperties qos;
    CN::AdminProperties admin;
    init_qos_props(qos, true);
    init_admin_props(admin);

    ecf->set_topology_factory(0);
    TAO_NS_EventChannel* ec = ecf->create_channel_i(qos, admin);
    TAO_NS_ConsumerAdmin* ca = ec->new_for_consumers_i(CNCA::OR_OP);

    ecf->set_topology_factory(&tsf);
    assertEquals(0, tsf.num_begin);
    assertEquals(tsf.num_begin, tsf.num_end);

    CN::EventTypeSeq added;
    CN::EventTypeSeq removed;

    ca->subscription_change(added, removed);
    // Should force the topo save of ecf, ec, ca, recreg
    assertEquals(4, tsf.num_begin);
    // todo : This exposes what's probably a bug, because we didn't add or remove anything.
    // The default subscriptions should have been included.
    assertEquals(tsf.num_begin, tsf.num_end);
    tsf.reset();
    ecf->set_topology_factory(0);

    // Now that the basic have been established ensure that it works for each of the other
    CNCA::ProxyID id;
    CNCA::ProxySupplier_var ps = ca->obtain_notification_push_supplier(CNCA::STRUCTURED_EVENT, id);
    CNCA::StructuredProxyPushSupplier_var structps = CNCA::StructuredProxyPushSupplier::_narrow(ps.in());
    assertTrue(! is_nil(structps.in()));
    ps = ca->obtain_notification_push_supplier(CNCA::SEQUENCE_EVENT, id);
    CNCA::SequenceProxyPushSupplier_var seqps = CNCA::SequenceProxyPushSupplier::_narrow(ps.in());
    assertTrue(! is_nil(seqps.in()));
    ps = ca->obtain_notification_push_supplier(CNCA::ANY_EVENT, id);
    CNCA::ProxyPushSupplier_var anyps = CNCA::ProxyPushSupplier::_narrow(ps.in());
    assertTrue(! is_nil(anyps.in()));
    TAO_NS_SupplierAdmin* sa = ec->new_for_suppliers_i(CNCA::OR_OP);
    CNCA::ProxyConsumer_var pc = sa->obtain_notification_push_consumer(CNCA::STRUCTURED_EVENT, id);
    CNCA::StructuredProxyPushConsumer_var structpc = CNCA::StructuredProxyPushConsumer::_narrow(pc.in());
    assertTrue(! is_nil(structpc.in()));
    pc = sa->obtain_notification_push_consumer(CNCA::SEQUENCE_EVENT, id);
    CNCA::SequenceProxyPushConsumer_var seqpc = CNCA::SequenceProxyPushConsumer::_narrow(pc.in());
    assertTrue(! is_nil(seqpc.in()));
    pc = sa->obtain_notification_push_consumer(CNCA::ANY_EVENT, id);
    CNCA::ProxyPushConsumer_var anypc = CNCA::ProxyPushConsumer::_narrow(pc.in());
    assertTrue(! is_nil(anypc.in()));

    // Call each one once to set the initial subscriptions so that the topology
    // is consistent
    added.length(1);
    added[0].domain_name = string_dup("a");
    ca->subscription_change(added, removed);
    structps->subscription_change(added, removed);
    seqps->subscription_change(added, removed);
    anyps->subscription_change(added, removed);
    sa->offer_change(added, removed);
    structpc->offer_change(added, removed);
    seqpc->offer_change(added, removed);
    anypc->offer_change(added, removed);

    ecf->set_topology_factory(&tsf);
    assertEquals(0, tsf.num_begin);
    assertEquals(tsf.num_begin, tsf.num_end);

    int expected = 27;
    ca->subscription_change(added, removed);
    assertEquals(expected, tsf.num_begin);
    assertEquals(tsf.num_begin, tsf.num_end);
    tsf.reset();

    structps->subscription_change(added, removed);
    assertEquals(expected, tsf.num_begin);
    assertEquals(tsf.num_begin, tsf.num_end);
    tsf.reset();

    seqps->subscription_change(added, removed);
    assertEquals(expected, tsf.num_begin);
    assertEquals(tsf.num_begin, tsf.num_end);
    tsf.reset();

    anyps->subscription_change(added, removed);
    assertEquals(expected, tsf.num_begin);
    assertEquals(tsf.num_begin, tsf.num_end);
    tsf.reset();

    sa->offer_change(added, removed);
    assertEquals(expected, tsf.num_begin);
    assertEquals(tsf.num_begin, tsf.num_end);
    tsf.reset();

    structpc->offer_change(added, removed);
    assertEquals(expected, tsf.num_begin);
    assertEquals(tsf.num_begin, tsf.num_end);
    tsf.reset();

    seqpc->offer_change(added, removed);
    assertEquals(expected, tsf.num_begin);
    assertEquals(tsf.num_begin, tsf.num_end);
    tsf.reset();

    anypc->offer_change(added, removed);
    assertEquals(expected, tsf.num_begin);
    assertEquals(tsf.num_begin, tsf.num_end);
    tsf.reset();

    ecf->deactivate(ACE_ENV_ARG_PARAMETER);
  }
}
