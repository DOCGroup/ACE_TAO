// $Id$

#include "Control.h"
#include "Counting_Supplier.h"

#include "ace/OS_NS_unistd.h"

#include "orbsvcs/Time_Utilities.h"
#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/Event/EC_Event_Channel.h"
#include "orbsvcs/Event/EC_Default_Factory.h"

ACE_RCSID (EC_Tests,
           Control,
           "$Id$")

const int event_type = 20;
const int event_source = 10;

int
main (int argc, char* argv[])
{
  TAO_EC_Default_Factory::init_svcs ();

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // ORB initialization boiler plate...
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var object =
        orb->resolve_initial_references ("RootPOA" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      PortableServer::POA_var poa =
        PortableServer::POA::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      PortableServer::POAManager_var poa_manager =
        poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // ****************************************************************

      TAO_EC_Event_Channel_Attributes attributes (poa.in (),
                                                  poa.in ());
      attributes.consumer_reconnect = 1;
      attributes.supplier_reconnect = 1;

      TAO_EC_Event_Channel ec_impl (attributes);
      ec_impl.activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      RtecEventChannelAdmin::EventChannel_var event_channel =
        ec_impl._this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;


      // ****************************************************************

      // Obtain the consumer admin..
      RtecEventChannelAdmin::ConsumerAdmin_var consumer_admin =
        event_channel->for_consumers (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Obtain the supplier admin..
      RtecEventChannelAdmin::SupplierAdmin_var supplier_admin =
        event_channel->for_suppliers (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // ****************************************************************

      EC_Counting_Supplier supplier0;

      supplier0.connect (supplier_admin.in (),
                         event_source,
                         event_type,
                         event_source,
                         event_type
                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // ****************************************************************

      // Create a consumer, intialize its RT_Info structures, and
      // connnect to the event channel....
      Consumer consumer0 ("Consumer/0", 100);

      ACE_ConsumerQOS_Factory consumer_qos0;
      consumer_qos0.start_disjunction_group ();
      consumer_qos0.insert (event_source, event_type, 0);

      consumer0.connect (consumer_admin.in (),
                         consumer_qos0.get_ConsumerQOS ()
                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Create a consumer, intialize its RT_Info structures, and
      // connnect to the event channel....
      Consumer consumer1 ("Consumer/1", 200);

      consumer1.connect (consumer_admin.in (),
                         consumer_qos0.get_ConsumerQOS ()
                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // ****************************************************************

      EC_Counting_Supplier_Task task0 (&supplier0);

      task0.activate ();

      // ****************************************************************

      ACE_Time_Value tv (10, 0);
      ACE_OS::sleep (tv);

      task0.stop ();

      ACE_Thread_Manager::instance ()->wait ();

      // ****************************************************************

      // Cleanup..

      // The consumers should be disconnected already, but make sure
      // that they did...
      //consumer1.disconnect (ACE_ENV_SINGLE_ARG_PARAMETER);
      //ACE_TRY_CHECK;
      //consumer0.disconnect (ACE_ENV_SINGLE_ARG_PARAMETER);
      //ACE_TRY_CHECK;

      // ****************************************************************

      supplier0.disconnect (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // ****************************************************************

      event_channel->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // ****************************************************************

      poa->destroy (1, 1 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // ****************************************************************

      orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // ****************************************************************

      ACE_DEBUG ((LM_DEBUG,
                  "Task 0 pushed %d events\n", task0.push_count ()));
      ACE_DEBUG ((LM_DEBUG,
                  "Supplier 0 pushed %d events\n", supplier0.event_count));

      consumer0.dump_results (100, 5);
      consumer1.dump_results (200, 5);
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Service");
      return 1;
    }
  ACE_ENDTRY;
  return 0;
}

// ****************************************************************

Consumer::Consumer (const char* name,
                    int shutdown_count)
  : EC_Counting_Consumer (name),
    shutdown_count_ (shutdown_count)
{
}

void
Consumer::push (const RtecEventComm::EventSet& events
                ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (events.length () == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "%s (%P|%t) no events\n", this->name_));
      return;
    }

  {
    ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);
    this->event_count++;

    if (this->event_count != this->shutdown_count_)
      return;
  }

  this->deactivate (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}
