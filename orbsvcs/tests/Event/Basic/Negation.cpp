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
      attributes.consumer_reconnect = 1;
      attributes.supplier_reconnect = 1;

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

      const int event_type = 20;
      const int event_source = 10;
      const int milliseconds = 50;

      EC_Counting_Supplier first_supplier;

      first_supplier.activate (consumer_admin.in (),
                               milliseconds);
      first_supplier.connect (supplier_admin.in (),
                              event_source,
                              event_type,
                              event_source,
                              event_type);

      EC_Counting_Supplier second_supplier;

      second_supplier.activate (consumer_admin.in (),
                                milliseconds);
      second_supplier.connect (supplier_admin.in (),
                               event_source,
                               event_type + 1,
                               event_source,
                               event_type + 1);

      EC_Counting_Supplier third_supplier;

      third_supplier.activate (consumer_admin.in (),
                               milliseconds);
      third_supplier.connect (supplier_admin.in (),
                              event_source,
                              event_type + 1,
                              event_source,
                              event_type + 1);

      // ****************************************************************

      EC_Counting_Consumer regular_consumer ("Consumer/regular");
      // Create a consumer, intialize its RT_Info structures, and
      // connnect to the event channel....


      {
        ACE_ConsumerQOS_Factory consumer_qos;
        consumer_qos.start_disjunction_group ();
        consumer_qos.insert (event_source, event_type, 0);

        regular_consumer.connect (consumer_admin.in (),
                                  consumer_qos.get_ConsumerQOS ());
      }

      // ****************************************************************

      EC_Counting_Consumer negation_consumer ("Consumer/negation");
      // Create a consumer, intialize its RT_Info structures, and
      // connnect to the event channel....

      {
        ACE_ConsumerQOS_Factory consumer_qos;
        consumer_qos.start_negation ();
        consumer_qos.start_disjunction_group ();
        consumer_qos.insert (event_source, event_type, 0);

        negation_consumer.connect (consumer_admin.in (),
                                   consumer_qos.get_ConsumerQOS ());
      }

      // ****************************************************************

      ACE_Time_Value tv (5, 0);
      // Wait for events, using work_pending()/perform_work() may help
      // or using another thread, this example is too simple for that.
      orb->run (tv);

      // ****************************************************************

      negation_consumer.disconnect ();

      // ****************************************************************

      regular_consumer.disconnect ();

      // ****************************************************************

      third_supplier.deactivate ();
      third_supplier.disconnect ();
      second_supplier.deactivate ();
      second_supplier.disconnect ();
      first_supplier.deactivate ();
      first_supplier.disconnect ();

      // ****************************************************************

      event_channel->destroy ();

      // ****************************************************************

      poa->destroy (1, 1);

      // ****************************************************************

      CORBA::ULong expected =
        third_supplier.event_count
        + second_supplier.event_count;
      negation_consumer.dump_results (expected, 5);
      expected =
        first_supplier.event_count;
      regular_consumer.dump_results (expected, 5);

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Service");
      return 1;
    }
  return 0;
}
