// $Id$

#include "Constants.h"
#include "orbsvcs/Event/EC_Lifetime_Utils_T.h"
#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/RtecEventChannelAdminC.h"
#include "orbsvcs/RtecEventCommS.h"

class EC_Consumer:
  public POA_RtecEventComm::PushConsumer
{
public:

  /// Constructor.
  EC_Consumer (CORBA::ORB_var orb,
               RtecEventChannelAdmin::EventChannel_var ec);

  /// PushConsumer methods.
  //@{
  /// Logs each event.  Initiates shutdown after receiving 100 events.
  virtual void push (const RtecEventComm::EventSet &events);
  /// No-op.
  virtual void disconnect_push_consumer (void);
  //@}

private:

  /// Helper - destroys EC, shutdowns the ORB and prints number of
  /// events received.
  void disconnect (void);

  /// Number of events pushed to us by EC.
  size_t n_events_;

  /// Cache these pointers for cleanup.
  CORBA::ORB_var orb_;
  RtecEventChannelAdmin::EventChannel_var ec_;
};

EC_Consumer::EC_Consumer (CORBA::ORB_var orb,
                          RtecEventChannelAdmin::EventChannel_var ec)
  : n_events_ (0),
    orb_ (orb),
    ec_ (ec)
{
}

void
EC_Consumer::push (const RtecEventComm::EventSet &events)
{
  for (CORBA::ULong i = 0; i < events.length (); ++i)
    {
      ++this->n_events_;
      ACE_DEBUG ((LM_DEBUG, " Received event\n"));
    }

  if (this->n_events_ >= 100)
    this->disconnect ();
}

void
EC_Consumer::disconnect_push_consumer (void)
{
}

void
EC_Consumer::disconnect (void)
{
  if (this->n_events_ == 100)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "SUCCESS: consumer received 100 events, as expected\n"));
    }

  this->ec_->destroy ();

  this->orb_->shutdown (0);
}

////////////////////////////////////////////////////////////
int
check_for_nil (CORBA::Object_ptr obj, const char *message)
{
  if (CORBA::is_nil (obj))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "ERROR: Object reference <%s> is nil\n",
                       message),
                      -1);
  else
    return 0;
}

int
parse_args (int /* argc */, ACE_TCHAR ** /* argv */)
{
  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      // Initialize ORB and POA, POA Manager, parse args.
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) == -1)
        return 1;

      CORBA::Object_var obj =
        orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var poa =
        PortableServer::POA::_narrow (obj.in ());
      if (check_for_nil (poa.in (), "POA") == -1)
        return 1;

      PortableServer::POAManager_var manager =
        poa->the_POAManager ();

      // Obtain reference to EC.
      obj = orb->resolve_initial_references ("Event_Service");
      RtecEventChannelAdmin::EventChannel_var ec =
        RtecEventChannelAdmin::EventChannel::_narrow (obj.in ());
      if (check_for_nil (ec.in (), "EC") == -1)
        return 1;

      // Create the consumer and register it with POA.
      PortableServer::Servant_var<EC_Consumer> consumer_impl =
        new EC_Consumer (orb, ec);

      if (!consumer_impl.in ())
        return 1;

      RtecEventComm::PushConsumer_var consumer;
      TAO_EC_Object_Deactivator consumer_deactivator;
      activate (consumer,
                poa.in (),
                consumer_impl.in (),
                consumer_deactivator);
      consumer_deactivator.disallow_deactivation ();

      // Obtain reference to ConsumerAdmin.
      RtecEventChannelAdmin::ConsumerAdmin_var consumer_admin =
        ec->for_consumers ();

      // Obtain ProxyPushSupplier and connect this consumer.
      RtecEventChannelAdmin::ProxyPushSupplier_var supplier =
        consumer_admin->obtain_push_supplier ();

      ACE_ConsumerQOS_Factory qos;
      qos.start_disjunction_group (1);
      qos.insert_type (ACE_ES_EVENT_ANY, 0);
      supplier->connect_push_consumer (consumer.in (),
                                       qos.get_ConsumerQOS ());

      // Allow processing of CORBA requests.
      manager->activate ();

      // Receive events from EC.
      orb->run ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception in Consumer:");
      return 1;
    }

  return 0;
}
