// $Id$

#include "Counting_Consumer.h"
#include "Counting_Supplier.h"

#include "orbsvcs/Time_Utilities.h"
#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/Event/EC_Event_Channel.h"
#include "orbsvcs/Event/EC_Default_Factory.h"

ACE_RCSID(EC_Tests, Timeout, "$Id$")

// ****************************************************************

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

      EC_Counting_Supplier supplier;

      supplier.activate (consumer_admin.in (),
                         50 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      supplier.connect (supplier_admin.in (),
                        0, 20,
                        0, 20
                        ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // ****************************************************************

      EC_Counting_Consumer interval_consumer ("Consumer/interval");
      // Create a consumer, intialize its RT_Info structures, and
      // connnect to the event channel....


      {
        // Let's say that the execution time for event 2 is 1
        // milliseconds...
        ACE_Time_Value tv (0, 100000);
        TimeBase::TimeT time;
        ORBSVCS_Time::Time_Value_to_TimeT (time, tv);

        ACE_ConsumerQOS_Factory consumer_qos;
        consumer_qos.start_disjunction_group ();
        // The types int the range [0,ACE_ES_EVENT_UNDEFINED) are
        // reserved for the EC...
        consumer_qos.insert_time (ACE_ES_EVENT_INTERVAL_TIMEOUT,
                                  time,
                                  0);

        interval_consumer.connect (consumer_admin.in (),
                                   consumer_qos.get_ConsumerQOS ()
                                   ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;
      }

      // ****************************************************************

      EC_Counting_Consumer conjunction_consumer ("Consumer/conjunction");
      // Create a consumer, intialize its RT_Info structures, and
      // connnect to the event channel....

      {
        // Let's say that the execution time for event 2 is 1
        // milliseconds...
        ACE_Time_Value tv (0, 200000);
        TimeBase::TimeT time;
        ORBSVCS_Time::Time_Value_to_TimeT (time, tv);

        ACE_ConsumerQOS_Factory consumer_qos;
        consumer_qos.start_conjunction_group ();
        consumer_qos.insert_time (ACE_ES_EVENT_INTERVAL_TIMEOUT,
                                  time,
                                  0);
        consumer_qos.insert_type (20,
                                  0);

        conjunction_consumer.connect (consumer_admin.in (),
                                      consumer_qos.get_ConsumerQOS ()
                                      ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;
      }

      // ****************************************************************

      EC_Counting_Consumer deadline_consumer ("Consumer/deadline");

      {
        ACE_Time_Value tv (0, 40000);
        TimeBase::TimeT time;
        ORBSVCS_Time::Time_Value_to_TimeT (time, tv);

        ACE_ConsumerQOS_Factory consumer_qos;
        consumer_qos.start_disjunction_group ();
        consumer_qos.insert_time (ACE_ES_EVENT_DEADLINE_TIMEOUT,
                                  time,
                                  0);
        consumer_qos.insert_type (20,
                                  0);

        deadline_consumer.connect (consumer_admin.in (),
                                   consumer_qos.get_ConsumerQOS ()
                                   ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;
      }

      // ****************************************************************

      ACE_Time_Value tv (5, 0);
      // Wait for events, using work_pending()/perform_work() may help
      // or using another thread, this example is too simple for that.
      orb->run (tv);

      // ****************************************************************

      deadline_consumer.disconnect (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      conjunction_consumer.disconnect (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      interval_consumer.disconnect (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      supplier.deactivate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      supplier.disconnect (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // ****************************************************************

      event_channel->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // ****************************************************************

      poa->destroy (1, 1 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // ****************************************************************

      interval_consumer.dump_results (50, 5);
      conjunction_consumer.dump_results (25, 5);
      deadline_consumer.dump_results (200, 5);
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

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
