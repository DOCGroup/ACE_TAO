// $Id$

#include "Constants.h"
#include "orbsvcs/orbsvcs/Event_Utilities.h"
#include "orbsvcs/orbsvcs/RtecEventCommC.h"
#include "orbsvcs/orbsvcs/RtecEventChannelAdminC.h"

void
send_events (RtecEventChannelAdmin::ProxyPushConsumer_ptr consumer
             ACE_ENV_ARG_DECL)
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
      consumer->push (events ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      events[0].header.type = B_EVENT_TYPE;
      consumer->push (events ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      events[0].header.type = C_EVENT_TYPE;
      consumer->push (events ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
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
main (int argc, char *argv[])
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // Initialize ORB and parse args.
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) == -1)
        return 1;

      // Obtain reference to EC.
      CORBA::Object_var obj =
        orb->resolve_initial_references ("Event_Service" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      RtecEventChannelAdmin::EventChannel_var ec =
        RtecEventChannelAdmin::EventChannel::_narrow (obj.in ()
                                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      if (check_for_nil (ec.in (), "EC") == -1)
        return 1;

      // Obtain reference to SupplierAdmin.
      RtecEventChannelAdmin::SupplierAdmin_var supplier_admin =
        ec->for_suppliers (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Obtain ProxyPushConsumer and connect this supplier.
      RtecEventChannelAdmin::ProxyPushConsumer_var consumer =
        supplier_admin->obtain_push_consumer (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_SupplierQOS_Factory qos;
      qos.insert (SOURCE_ID, A_EVENT_TYPE, 0, 1);
      qos.insert (SOURCE_ID, B_EVENT_TYPE, 0, 1);
      qos.insert (SOURCE_ID, C_EVENT_TYPE, 0, 1);

      consumer->connect_push_supplier
        (RtecEventComm::PushSupplier::_nil (),
         qos.get_SupplierQOS ()
         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Send events to EC.
      send_events (consumer.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Tell EC to shut down.
      ec->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }

  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception in Supplier:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
