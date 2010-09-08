// $Id$

#include "Counting_Consumer.h"
#include "Counting_Supplier.h"

#include "orbsvcs/Time_Utilities.h"
#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/Event/EC_Event_Channel.h"
#include "orbsvcs/Event/EC_Default_Factory.h"

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  TAO_EC_Default_Factory::init_svcs ();

  try
    {
      // ORB initialization boiler plate...
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var object =
        orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var poa =
        PortableServer::POA::_narrow (object.in ());
      PortableServer::POAManager_var poa_manager =
        poa->the_POAManager ();
      poa_manager->activate ();

      // ****************************************************************

      TAO_EC_Event_Channel_Attributes attributes (poa.in (),
                                                  poa.in ());

      TAO_EC_Event_Channel ec_impl (attributes);
      ec_impl.activate ();

      RtecEventChannelAdmin::EventChannel_var event_channel =
        ec_impl._this ();


      // ****************************************************************

      // Obtain the consumer admin..
      RtecEventChannelAdmin::ConsumerAdmin_var consumer_admin =
        event_channel->for_consumers ();

      // Obtain the supplier admin..
      RtecEventChannelAdmin::SupplierAdmin_var supplier_admin =
        event_channel->for_suppliers ();

      // ****************************************************************

      EC_Counting_Supplier supplier;

      supplier.activate (consumer_admin.in (),
                         50);
      supplier.connect (supplier_admin.in (),
                        0, 20,
                        0, 20);

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
                                   consumer_qos.get_ConsumerQOS ());
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
                                      consumer_qos.get_ConsumerQOS ());
      }

      // ****************************************************************

      EC_Counting_Consumer deadline_consumer ("Consumer/deadline");

      {
        ACE_Time_Value tv (0, 80000);
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
                                   consumer_qos.get_ConsumerQOS ());
      }

      // ****************************************************************

      ACE_Time_Value tv (5, 0);
      // Wait for events, using work_pending()/perform_work() may help
      // or using another thread, this example is too simple for that.
      orb->run (tv);

      // ****************************************************************

      deadline_consumer.disconnect ();

      conjunction_consumer.disconnect ();

      interval_consumer.disconnect ();

      supplier.deactivate ();
      supplier.disconnect ();

      // ****************************************************************

      event_channel->destroy ();

      // ****************************************************************

      poa->destroy (1, 1);

      // ****************************************************************

      interval_consumer.dump_results (50, 5);
      conjunction_consumer.dump_results (25, 5);
      deadline_consumer.dump_results (100, 5);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Service");
      return 1;
    }
  return 0;
}
