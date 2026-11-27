#include "ace/ARGV.h"
#include "ace/OS_NS_stdio.h"
#include "orbsvcs/CosNotifyChannelAdminC.h"
#include "tao/TimeBaseC.h"
#include "tao/corba.h"

using namespace CORBA;
namespace CNCA = CosNotifyChannelAdmin;
namespace CN = CosNotification;

constexpr char notify_ior[] = "corbaloc::localhost:9050/NotifyEventChannelFactory";

static void
init_qos_props (CN::QoSProperties& qos, bool topology_persist)
{
  qos.length (13);
  ULong i {};
  qos[i].name = "EventReliability";
  qos[i++].value <<= CN::BestEffort;
  qos[i].name = "ConnectionReliability";
  qos[i++].value <<= topology_persist ? CN::Persistent : CN::BestEffort;
  qos[i].name = "Priority";
  qos[i++].value <<= static_cast<Short> (1234);
  qos[i].name = "Timeout";
  qos[i++].value <<= static_cast<TimeBase::TimeT> (55555); // 5.5555 ms
  qos[i].name = "MaximumBatchSize";
  qos[i++].value <<= static_cast<Long> (200);
  qos[i].name = "PacingInterval";
  qos[i++].value <<= static_cast<TimeBase::TimeT> (42000); // 4.2ms
  qos[i].name = "StopTimeSupported";
  qos[i++].value <<= Any::from_boolean (false);
  qos.length (i);
}

static void
init_admin_props (CN::AdminProperties& admin)
{
  admin.length (13);
  ULong i {};
  admin[i].name = "MaxQueueLength";
  admin[i++].value <<= static_cast<Long> (200);
  admin[i].name = "MaxConsumers";
  admin[i++].value <<= static_cast<Long> (2);
  admin[i].name = "MaxSuppliers";
  admin[i++].value <<= static_cast<Long> (1);
  admin[i].name = "RejectNewEvents";
  admin[i++].value <<= Any::from_boolean (true);
}

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  try
    {
      ACE_ARGV av;
      int argc {};
      ORB_var orb = ORB_init (argc, av.argv ());
      ACE_ASSERT (!is_nil (orb.in ()));

      Object_var const object = orb->string_to_object (notify_ior);
      ACE_ASSERT (!is_nil (object.in ()));
      CNCA::EventChannelFactory_var const ecf = CNCA::EventChannelFactory::_narrow (object.in ());
      ACE_ASSERT (!is_nil (ecf.in ()));

      CN::QoSProperties qos;
      CN::AdminProperties admin;
      init_qos_props (qos, true);
      init_admin_props (admin);

      CNCA::ChannelID ecid {};
      CNCA::EventChannel_var const ec = ecf->create_channel (qos, admin, ecid);
      ACE_OS::printf ("Created channel id=\"%ld\"\n", static_cast<long> (ecid));

      orb->destroy ();
      return 0;
    }
  catch (Exception const& e)
    {
      ACE_OS::fputs (e._name (), stderr);
      ACE_OS::fputs ("\n", stderr);
    }

  return 1;
}
