// $Id$
#include "TestQoS.h"

#include <orbsvcs/CosNotifyChannelAdminS.h>
#include <orbsvcs/CosNotifyCommS.h>
#include <orbsvcs/CosNotificationS.h>
#include <tao/TimeBaseC.h>

#include <ace/ARGV.h>

using namespace CORBA;
using namespace PortableServer;
namespace CNCA = CosNotifyChannelAdmin;
namespace CN = CosNotification;
namespace CNC = CosNotifyComm;

namespace
{
  void destroy_all_channels(CNCA::EventChannelFactory_ptr ecf)
  {
    CNCA::ChannelIDSeq_var channel_ids = ecf->get_all_channels();
    for (ULong i = 0; i < channel_ids->length(); ++i)
    {
      CNCA::ChannelID id = channel_ids[i];
      CNCA::EventChannel_var ec = ecf->get_event_channel(id);
      ec->destroy();
    }
  }
}

namespace TestQoS
{
  const char* NOTIFY_IOR = "corbaloc::localhost:9050/NotifyEventChannelFactory";

  ORB_var orb;
  POA_var poa;
  CNCA::EventChannelFactory_var ecf;

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
      obj = orb->string_to_object(NOTIFY_IOR);
      ACE_ASSERT(! is_nil(obj.in()));
      ecf = CNCA::EventChannelFactory::_narrow(obj.in());
      ACE_ASSERT(! is_nil(ecf.in()));
      destroy_all_channels(ecf.in());
    }

    void tearDown()
    {
      destroy_all_channels(ecf.in());
      ecf = CNCA::EventChannelFactory::_nil();
      poa->destroy(1,1);
      orb->destroy();
      poa = POA::_nil();
      orb = ORB::_nil();
    }
  };

  void testQoSChannel(ACE_ENV_SINGLE_ARG_DECL) {
    SetupTeardown sutd;

    // Create a channel, check that it's qos contains the expected defaults
    CNCA::ChannelID ecid;
    CNCA::EventChannel_var ec = ecf->create_channel(CN::QoSProperties(), CN::AdminProperties(), ecid);
    assertTrue( ! is_nil(ec.in()));

    ACE_CString name;
    ULong i = 0;

    CN::QoSProperties_var qos = ec->get_qos();
    assertEquals(1, qos->length());
    name = qos[i].name.in();
    assertTrue(name == "ThreadPool");
    // Don't really care about the value of ThreadPool

    TimeBase::UtcT defutc;
    defutc.time = 12345;
    defutc.inacclo = 10;
    defutc.inacchi = 50;
    defutc.tdf = 100;

    // The TAO implementation accepts all settings without error
    CN::QoSProperties qos2(13);
    qos2.length(13);
    i = 0;
    qos2[i].name = CORBA::string_dup ("EventReliability");
    qos2[i++].value <<= CN::BestEffort;
    qos2[i].name = CORBA::string_dup ("ConnectionReliability");
    qos2[i++].value <<= CN::Persistent;
    qos2[i].name = CORBA::string_dup ("Priority");
    qos2[i++].value <<= (Short) 1234;
    //qos2[i].name = CORBA::string_dup ("StartTime");
    //qos2[i++].value <<= defutc;
    //qos2[i].name = CORBA::string_dup ("StopTime");
    //qos2[i++].value <<= defutc;
    qos2[i].name = CORBA::string_dup ("Timeout");
    qos2[i++].value <<= (TimeBase::TimeT) 55555;
    qos2[i].name = CORBA::string_dup ("OrderPolicy");
    qos2[i++].value <<= CN::FifoOrder;
    qos2[i].name = CORBA::string_dup ("DiscardPolicy");
    qos2[i++].value <<= CN::PriorityOrder;
    qos2[i].name = CORBA::string_dup ("MaximumBatchSize");
    qos2[i++].value <<= (Long) 200;
    qos2[i].name = CORBA::string_dup ("PacingInterval");
    qos2[i++].value <<= (TimeBase::TimeT) 420000;
    //qos2[i].name = CORBA::string_dup ("StartTimeSupported");
    //qos2[i++].value <<= 0;
    qos2[i].name = CORBA::string_dup ("StopTimeSupported");
    qos2[i++].value <<= Any::from_boolean(0);
    qos2[i].name = CORBA::string_dup ("MaxEventsPerConsumer");
    qos2[i++].value <<= (Long) 150;

    qos2.length(i);
    ec->set_qos(qos2 ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;

    Short sv = 0;
    Long lv = 0;
    Boolean bv = 0;

    // Get channel qos settings, and ensure they match what was added.
    qos = ec->get_qos();
    ULong len = qos->length();
    assertEquals(i + 1, len); // The number we added, plus the original ThreadPool setting.

    i = 0;
    TimeBase::TimeT t;

    for (i =0; i < len; ++i)
    {
      name = qos[i].name.in();

      if (name == "ThreadPool")
      {
        // that's nice.
      }
      else if (name == "EventReliability")
      {
        assertTrue((qos[i].value >>= sv) != 0);
        assertEquals(CN::BestEffort, sv);
      }
      else if (name == "ConnectionReliability")
      {
        assertTrue((qos[i].value >>= sv) != 0);
        assertEquals(CN::Persistent, sv);
      }
      else if (name == "Priority")
      {
        assertTrue((qos[i].value >>= sv) != 0);
        assertEquals(1234, sv);
      }
      else if (name == "Timeout")
      {
        assertTrue((qos[i].value >>= t) != 0);
        assertEquals(55555, t);
      }
      else if (name == "OrderPolicy")
      {
        assertTrue((qos[i].value >>= sv) != 0);
        assertEquals(CN::FifoOrder, sv);
      }
      else if (name == "DiscardPolicy")
      {
        assertTrue((qos[i].value >>= sv) != 0);
        assertEquals(CN::PriorityOrder, sv);
      }
      else if (name == "MaximumBatchSize")
      {
        assertTrue((qos[i].value >>= lv) != 0);
        assertEquals(200, lv);
      }
      else if (name == "PacingInterval")
      {
        assertTrue((qos[i].value >>= t) != 0);
        assertEquals(420000, t);
      }
      else if (name == "StopTimeSupported")
      {
        assertTrue((qos[i].value >>= Any::to_boolean(bv)) != 0);
        assertTrue(bv == 0);
      }
      else if (name == "MaxEventsPerConsumer")
      {
        assertTrue((qos[i].value >>= lv) != 0);
        assertEquals(150, lv);
      }
      else
      {
        cout << "Unknown property name: " << name.c_str () << endl;
        assertTrue(0);
      }
    }
    qos2.length(1);
    i = 0;
    // Test that each unsupported option throws
    //scope
    {
      ACE_TRY_NEW_ENV
      {
        qos2[i].name = CORBA::string_dup ("StartTime");
        qos2[i].value <<= defutc;
        ec->set_qos(qos2 ACE_ENV_ARG_PARAMETER);
        ACE_CHECK;
        failIt("Expected UnsupportedQoS exception.");
      }
      ACE_CATCH (CN::UnsupportedQoS, ex)
      {
        //ok;
      }
      ACE_CATCHANY;
      {
        failIt("set_qos threw wrong exception.");
      }
      ACE_ENDTRY;
    } //scope

    //scope
    {
      ACE_TRY_NEW_ENV
      {
        qos2[i].name = CORBA::string_dup ("StopTime");
        qos2[i].value <<= defutc;
        ec->set_qos(qos2 ACE_ENV_ARG_PARAMETER);
        ACE_CHECK;
        failIt("Expected UnsupportedQoS exception.");
      }
      ACE_CATCH (CN::UnsupportedQoS, ex)
      {
        //ok;
      }
      ACE_CATCHANY;
      {
        failIt("set_qos threw wrong exception.");
      }
      ACE_ENDTRY;
    } //scope

    //scope
    {
      ACE_TRY_NEW_ENV
      {
        qos2[i].name = CORBA::string_dup ("StartTimeSupported");
        qos2[i].value <<= Any::from_boolean(0);
        ec->set_qos(qos2 ACE_ENV_ARG_PARAMETER);
        ACE_CHECK;
        failIt("Expected UnsupportedQoS exception.");
      }
      ACE_CATCH (CN::UnsupportedQoS, ex)
      {
        //ok;
      }
      ACE_CATCHANY;
      {
        failIt("set_qos threw wrong exception.");
      }
      ACE_ENDTRY;
    } //scope

    // Test that common incorrect use of boolean throws
    //scope
    {
      ACE_TRY_NEW_ENV
      {
        qos2[i].name = CORBA::string_dup ("StopTimeSupported");
        qos2[i].value <<= 0; // test no from_boolean()
        ec->set_qos(qos2 ACE_ENV_ARG_PARAMETER);
        ACE_CHECK;
        failIt("Expected UnsupportedQoS exception.");
      }
      ACE_CATCH (CN::UnsupportedQoS, ex)
      {
        //ok;
      }
      ACE_CATCHANY;
      {
        failIt("set_qos threw wrong exception.");
      }
      ACE_ENDTRY;
    } //scope

    // ditto for using an int as a Short
    //scope
    {
      ACE_TRY_NEW_ENV
      {
        qos2[i].name = CORBA::string_dup ("Priority");
        qos2[i].value <<= 0; // test no  <<= (Short) 0;
        ec->set_qos(qos2 ACE_ENV_ARG_PARAMETER);
        ACE_CHECK;
        failIt("Expected UnsupportedQoS exception.");
      }
      ACE_CATCH (CN::UnsupportedQoS, ex)
      {
        //ok;
      }
      ACE_CATCHANY;
      {
        failIt("set_qos threw wrong exception.");
      }
      ACE_ENDTRY;
    } //scope
  }
} // namespace
