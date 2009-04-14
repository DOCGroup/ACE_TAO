/**
 *  $Id$
 */

#include "orbsvcs/CosNotifyChannelAdminC.h"
//#include <orbsvcs/CosNotifyCommS.h>
//#include <orbsvcs/CosNotificationS.h>
#include "tao/TimeBaseC.h"

#include "ace/ARGV.h"
#include "ace/OS_NS_stdio.h"

using namespace CORBA;
namespace CNCA = CosNotifyChannelAdmin;
namespace CN = CosNotification;
namespace CNC = CosNotifyComm;

const char* NOTIFY_IOR = "corbaloc::localhost:9050/NotifyEventChannelFactory";

ORB_var orb;
CNCA::EventChannelFactory_var ecf;

namespace {
  void init_qos_props(CN::QoSProperties& qos, bool topology_persist) {
    qos.length(13);
    ULong i = 0;
    qos[i].name = "EventReliability";
    qos[i++].value <<= CN::BestEffort;
    if (topology_persist) {
      qos[i].name = "ConnectionReliability";
      qos[i++].value <<= CN::Persistent;
    }
    else
    {
      qos[i].name = "ConnectionReliability";
      qos[i++].value <<= CN::BestEffort;
    }
    qos[i].name = "Priority";
    qos[i++].value <<= (Short) 1234;
    qos[i].name = "Timeout";
    qos[i++].value <<= (TimeBase::TimeT) 55555; // 5.5555 ms
    qos[i].name = "MaximumBatchSize";
    qos[i++].value <<= (Long) 200;
    qos[i].name = "PacingInterval";
    qos[i++].value <<= (TimeBase::TimeT) 42000; // 4.2ms
    qos[i].name = "StopTimeSupported";
    qos[i++].value <<= Any::from_boolean(0);
    qos.length(i);
  }
  void init_admin_props(CN::AdminProperties& admin) {
    admin.length(13);
    ULong i = 0;
    admin[i].name = "MaxQueueLength";
    admin[i++].value <<= (Long) 200;
    admin[i].name = "MaxConsumers";
    admin[i++].value <<= (Long) 2;
    admin[i].name = "MaxSuppliers";
    admin[i++].value <<= (Long) 1;
    admin[i].name = "RejectNewEvents";
    admin[i++].value <<= Any::from_boolean(1);
  }
  void setUp()
  {
    ACE_ARGV av;
    int ac = 0;
    orb = ORB_init(ac, av.argv());
    ACE_ASSERT(! is_nil(orb.in()));
    Object_var obj = orb->string_to_object(NOTIFY_IOR);
    ACE_ASSERT(! is_nil(obj.in()));
    ecf = CNCA::EventChannelFactory::_narrow(obj.in());
    ACE_ASSERT(! is_nil(ecf.in()));
  }

  void tearDown()
  {
    ecf = CNCA::EventChannelFactory::_nil();
    orb->destroy();
    orb = ORB::_nil();
  }
}


int ACE_TMAIN (int, ACE_TCHAR *[])
{
  try
  {
    setUp();

    CN::QoSProperties qos;
    CN::AdminProperties admin;
    init_qos_props(qos, true);
    init_admin_props(admin);

    CNCA::ChannelID ecid;
    CNCA::EventChannel_var ec = ecf->create_channel(qos, admin, ecid);

    ACE_OS::printf("Created channel id=\"%ld\"\n", static_cast<long>(ecid));

    tearDown();

    return 0;
  }
  catch (Exception& e)
  {
    ACE_OS::fputs(e._name (), stderr);
    ACE_OS::fputs("\n", stderr);
  }
  return 1;
}
