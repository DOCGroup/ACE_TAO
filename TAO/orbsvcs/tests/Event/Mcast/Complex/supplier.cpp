// $Id$

#include "Constants.h"
#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/RtecEventCommC.h"
#include "orbsvcs/RtecEventChannelAdminC.h"
#include "ace/Log_Msg.h"

void
send_events (RtecEventChannelAdmin::ProxyPushConsumer_ptr consumer)
{
  // Events we'll send.
  RtecEventComm::EventSet events (1);
  events.length (1);
  // Events travelling through gateways must have a ttl count of at
  // least 1!
  events[0].header.ttl = 1;
  events[0].header.source = SOURCE_ID;

  for (int i = 0; i < 100; ++i)
    {
      // Send 1 event of each type.
      events[0].header.type = A_EVENT_TYPE;
      consumer->push (events);

      events[0].header.type = B_EVENT_TYPE;
      consumer->push (events);

      events[0].header.type = C_EVENT_TYPE;
      consumer->push (events);
    }
}

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
parse_args (int /*argc*/, char ** /*argv*/)
{
  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      // Initialize ORB and parse args.
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) == -1)
        return 1;

      // Obtain reference to EC.
      CORBA::Object_var obj =
        orb->resolve_initial_references ("Event_Service");
      RtecEventChannelAdmin::EventChannel_var ec =
        RtecEventChannelAdmin::EventChannel::_narrow (obj.in ());
      if (check_for_nil (ec.in (), "EC") == -1)
        return 1;

      // Obtain reference to SupplierAdmin.
      RtecEventChannelAdmin::SupplierAdmin_var supplier_admin =
        ec->for_suppliers ();

      // Obtain ProxyPushConsumer and connect this supplier.
      RtecEventChannelAdmin::ProxyPushConsumer_var consumer =
        supplier_admin->obtain_push_consumer ();

      ACE_SupplierQOS_Factory qos;
      qos.insert (SOURCE_ID, A_EVENT_TYPE, 0, 1);
      qos.insert (SOURCE_ID, B_EVENT_TYPE, 0, 1);
      qos.insert (SOURCE_ID, C_EVENT_TYPE, 0, 1);

      consumer->connect_push_supplier
        (RtecEventComm::PushSupplier::_nil (),
         qos.get_SupplierQOS ());

      // Send events to EC.
      send_events (consumer.in ());

      // Tell EC to shut down.
      ec->destroy ();
    }

  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception in Supplier:");
      return 1;
    }

  return 0;
}
