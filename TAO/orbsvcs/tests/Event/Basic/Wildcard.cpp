// $Id$

#include "Counting_Consumer.h"
#include "Counting_Supplier.h"
#include "orbsvcs/Time_Utilities.h"
#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/Event/EC_Event_Channel.h"
#include "orbsvcs/Event/EC_Default_Factory.h"

ACE_RCSID (EC_Tests,
           Wildcard,
           "$Id$")

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

      CORBA::Object_var object =
        orb->resolve_initial_references ("RootPOA" ACE_ENV_ARG_PARAMETER);
      PortableServer::POA_var poa =
        PortableServer::POA::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
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

      const int event_type = 20;
      const int event_source = 10;
      const int milliseconds = 50;

      EC_Counting_Supplier supplier;

      supplier.activate (consumer_admin.in (),
                         milliseconds
                         ACE_ENV_ARG_PARAMETER);
      supplier.connect (supplier_admin.in (),
                        event_source,
                        event_type,
                        event_source,
                        event_type
                        ACE_ENV_ARG_PARAMETER);

      // ****************************************************************

      EC_Counting_Supplier other_supplier;

      other_supplier.activate (consumer_admin.in (),
                               milliseconds
                               ACE_ENV_ARG_PARAMETER);
      other_supplier.connect (supplier_admin.in (),
                              event_source + 1,
                              event_type + 1,
                              event_source + 1,
                              event_type + 1
                              ACE_ENV_ARG_PARAMETER);

      // ****************************************************************

      EC_Counting_Supplier any_source_supplier;

      any_source_supplier.activate (consumer_admin.in (),
                                    milliseconds
                                    ACE_ENV_ARG_PARAMETER);
      any_source_supplier.connect (supplier_admin.in (),
                                   0,
                                   event_type,
                                   event_source,
                                   event_type
                                   ACE_ENV_ARG_PARAMETER);

      // ****************************************************************

      EC_Counting_Supplier any_type_supplier;

      any_type_supplier.activate (consumer_admin.in (),
                                  milliseconds
                                  ACE_ENV_ARG_PARAMETER);
      any_type_supplier.connect (supplier_admin.in (),
                                 event_source,
                                 0,
                                 event_source,
                                 event_type
                                 ACE_ENV_ARG_PARAMETER);

      // ****************************************************************

      EC_Counting_Supplier wildcard_supplier;

      wildcard_supplier.activate (consumer_admin.in (),
                                  milliseconds
                                  ACE_ENV_ARG_PARAMETER);
      wildcard_supplier.connect (supplier_admin.in (),
                                 0,
                                 0,
                                 event_source,
                                 event_type
                                 ACE_ENV_ARG_PARAMETER);

      // ****************************************************************

      EC_Counting_Consumer regular_consumer ("Consumer/regular");
      // Create a consumer, intialize its RT_Info structures, and
      // connnect to the event channel....


      {
        ACE_ConsumerQOS_Factory consumer_qos;
        consumer_qos.start_disjunction_group ();
        consumer_qos.insert (event_source, event_type, 0);

        regular_consumer.connect (consumer_admin.in (),
                                  consumer_qos.get_ConsumerQOS ()
                                  ACE_ENV_ARG_PARAMETER);
      }

      // ****************************************************************

      EC_Counting_Consumer any_type_consumer ("Consumer/any_type");
      // Create a consumer, intialize its RT_Info structures, and
      // connnect to the event channel....


      {
        ACE_ConsumerQOS_Factory consumer_qos;
        consumer_qos.start_disjunction_group ();
        consumer_qos.insert (event_source, 0, 0);

        any_type_consumer.connect (consumer_admin.in (),
                                   consumer_qos.get_ConsumerQOS ()
                                   ACE_ENV_ARG_PARAMETER);
      }

      // ****************************************************************

      EC_Counting_Consumer any_source_consumer ("Consumer/any_source");
      // Create a consumer, intialize its RT_Info structures, and
      // connnect to the event channel....


      {
        ACE_ConsumerQOS_Factory consumer_qos;
        consumer_qos.start_disjunction_group ();
        consumer_qos.insert (0, event_type, 0);

        any_source_consumer.connect (consumer_admin.in (),
                                     consumer_qos.get_ConsumerQOS ()
                                     ACE_ENV_ARG_PARAMETER);
      }

      // ****************************************************************

      EC_Counting_Consumer wildcard_consumer ("Consumer/wildcard");
      // Create a consumer, intialize its RT_Info structures, and
      // connnect to the event channel....


      {
        ACE_ConsumerQOS_Factory consumer_qos;
        consumer_qos.start_disjunction_group ();
        consumer_qos.insert (0, 0, 0);

        wildcard_consumer.connect (consumer_admin.in (),
                                   consumer_qos.get_ConsumerQOS ()
                                   ACE_ENV_ARG_PARAMETER);
      }

      // ****************************************************************

      ACE_Time_Value tv (5, 0);
      // Wait for events, using work_pending()/perform_work() may help
      // or using another thread, this example is too simple for that.
      orb->run (tv);

      // ****************************************************************

      wildcard_consumer.disconnect ();

      any_source_consumer.disconnect ();

      any_type_consumer.disconnect ();

      regular_consumer.disconnect ();

      wildcard_supplier.deactivate ();
      wildcard_supplier.disconnect ();

      any_type_supplier.deactivate ();

      any_source_supplier.deactivate ();
      any_source_supplier.disconnect ();

      other_supplier.deactivate ();
      other_supplier.disconnect ();

      supplier.deactivate ();
      supplier.disconnect ();

      // ****************************************************************

      event_channel->destroy ();

      // ****************************************************************

      poa->destroy (1, 1 ACE_ENV_ARG_PARAMETER);

      orb->destroy ();

      // ****************************************************************

      CORBA::ULong expected =
        wildcard_supplier.event_count
        + any_type_supplier.event_count
        + any_source_supplier.event_count
        + other_supplier.event_count
        + supplier.event_count;
      wildcard_consumer.dump_results (expected, 5);

      expected =
        wildcard_supplier.event_count
        + any_type_supplier.event_count
        + any_source_supplier.event_count
        // NOT THIS ONE + other_supplier.event_count
        + supplier.event_count;
      any_source_consumer.dump_results (expected, 5);

      expected =
        wildcard_supplier.event_count
        + any_type_supplier.event_count
        + any_source_supplier.event_count
        // NOT THIS ONE + other_supplier.event_count
        + supplier.event_count;
      any_type_consumer.dump_results (expected, 5);

      expected =
        wildcard_supplier.event_count
        + any_type_supplier.event_count
        + any_source_supplier.event_count
        // NOT THIS ONE + other_supplier.event_count
        + supplier.event_count;
      regular_consumer.dump_results (expected, 5);
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Service");
      return 1;
    }
  ACE_ENDTRY;
  return 0;
}
