// $Id$
#include "TestBasic.h"

#include <orbsvcs/CosNotifyChannelAdminC.h>
#include <orbsvcs/CosNotifyCommS.h>
#include <orbsvcs/CosNotificationS.h>

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

namespace TestBasic
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

  void testChannelCreate(ACE_ENV_SINGLE_ARG_DECL)
  {
    SetupTeardown sutd;

    // Should be not channels when starting
    CNCA::ChannelIDSeq_var channel_ids = ecf->get_all_channels(ACE_ENV_SINGLE_ARG_PARAMETER);
    assertTrue(channel_ids->length() == 0);

    // get_event_channel(non_existent_id) should throw ChannelNotFound
    ACE_TRY_NEW_ENV
    {
      CNCA::EventChannel_var ec = ecf->get_event_channel(0 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      failIt("Expected ChannelNotFound exception.");
    }
    ACE_CATCH(CNCA::ChannelNotFound, ex)
    {
    }
    ACE_ENDTRY;
    ACE_CHECK;

    // ChannelID should match the one in the lst
    CNCA::ChannelID ecid;
    CNCA::EventChannel_var ec = ecf->create_channel(CN::QoSProperties(), CN::AdminProperties(), ecid);
    assertTrue(! is_nil(ec.in()));
    CNCA::EventChannelFactory_var ecf2 = ec->MyFactory();
    assertTrue(! is_nil(ecf2.in()));
    // Note : Not guaranteed in general, but expected to work in this case
    assertTrue(ecf->_is_equivalent(ecf2.in()));

    channel_ids = ecf->get_all_channels(ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;
    assertTrue(channel_ids->length() == 1);
    assertTrue(channel_ids[0] == ecid);

    // Channels should be retrievable by id
    CNCA::EventChannel_var ec2 = ecf->get_event_channel(ecid ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;
    assertTrue(! is_nil(ec2.in()));
    ecf2 = ec2->MyFactory(ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;
    assertTrue(! is_nil(ecf2.in()));
    // Note : Not guaranteed in general, but expected to work in this case
    assertTrue(ecf->_is_equivalent(ecf2.in()));
    assertTrue(ec->_is_equivalent(ec2.in()));
  }

  void testAdminCreate() {

    SetupTeardown sutd;

    CNCA::ChannelID ecid;
    CNCA::EventChannel_var ec = ecf->create_channel(CN::QoSProperties(), CN::AdminProperties(), ecid);

    // The default SA and CA should exist, but they do not get returned with the others
    CNCA::AdminIDSeq_var admins = ec->get_all_consumeradmins(ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;
    assertTrue(admins->length() == 0);
    admins = ec->get_all_supplieradmins(ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;
    assertTrue(admins->length() == 0);

    ACE_TRY_NEW_ENV
    {
      CNCA::ConsumerAdmin_var sa = ec->get_consumeradmin(1 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      failIt("Expected AdminNotFound exception.");
    }
    ACE_CATCH(CNCA::AdminNotFound, ex)
    {
    }
    ACE_ENDTRY;
    ACE_CHECK;

    ACE_TRY_NEW_ENV
    {
      CNCA::SupplierAdmin_var sa = ec->get_supplieradmin(1 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      failIt("Expected AdminNotFound exception.");
    }
    ACE_CATCH(CNCA::AdminNotFound, ex)
    {
    }
    ACE_ENDTRY;
    ACE_CHECK;

    // get_consumeradmin(0) is synonymous with default_consumer_admin()
    CNCA::ConsumerAdmin_var ca = ec->default_consumer_admin(ACE_ENV_SINGLE_ARG_PARAMETER);
    CNCA::ConsumerAdmin_var ca2 = ec->get_consumeradmin(0 ACE_ENV_ARG_PARAMETER);
    assertTrue(! is_nil(ca.in()));
    assertTrue(! is_nil(ca2.in()));
    // Note : Not guaranteed in general, but expected to work in this case
    assertTrue(ca->_is_equivalent(ca2.in()));

    CNCA::SupplierAdmin_var sa = ec->get_supplieradmin(0 ACE_ENV_ARG_PARAMETER);
    CNCA::SupplierAdmin_var sa2 = ec->default_supplier_admin(ACE_ENV_SINGLE_ARG_PARAMETER);
    assertTrue(! is_nil(sa.in()));
    assertTrue(! is_nil(sa2.in()));
    // Note : Not guaranteed in general, but expected to work in this case
    assertTrue(sa->_is_equivalent(sa2.in()));

    // The default SA and CA should exist, but they do not get returned with the others
    admins = ec->get_all_consumeradmins(ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;
    assertTrue(admins->length() == 0);
    admins = ec->get_all_supplieradmins(ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;
    assertTrue(admins->length() == 0);
  }

}
