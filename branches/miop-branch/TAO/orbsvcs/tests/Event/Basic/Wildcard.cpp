// $Id$

#include "Counting_Consumer.h"
#include "Counting_Supplier.h"
#include "orbsvcs/Time_Utilities.h"
#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/Event/EC_Event_Channel.h"
#include "orbsvcs/Event/EC_Default_Factory.h"

ACE_RCSID(EC_Tests, Wildcard, "$Id$")

int
main (int argc, char* argv[])
{
  TAO_EC_Default_Factory::init_svcs ();

  TAO_ENV_DECLARE_NEW_ENV;
  ACE_TRY
    {
      // ORB initialization boiler plate...
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var object =
        orb->resolve_initial_references ("RootPOA" TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      PortableServer::POA_var poa =
        PortableServer::POA::_narrow (object.in () TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      PortableServer::POAManager_var poa_manager =
        poa->the_POAManager (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      poa_manager->activate (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // ****************************************************************

      TAO_EC_Event_Channel_Attributes attributes (poa.in (),
                                                  poa.in ());

      TAO_EC_Event_Channel ec_impl (attributes);
      ec_impl.activate (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      RtecEventChannelAdmin::EventChannel_var event_channel =
        ec_impl._this (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;


      // ****************************************************************

      // Obtain the consumer admin..
      RtecEventChannelAdmin::ConsumerAdmin_var consumer_admin =
        event_channel->for_consumers (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Obtain the supplier admin..
      RtecEventChannelAdmin::SupplierAdmin_var supplier_admin =
        event_channel->for_suppliers (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // ****************************************************************

      const int event_type = 20;
      const int event_source = 10;
      const int milliseconds = 50;

      EC_Counting_Supplier supplier;

      supplier.activate (consumer_admin.in (),
                         milliseconds
                         TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      supplier.connect (supplier_admin.in (),
                        event_source,
                        event_type,
                        event_source,
                        event_type
                        TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // ****************************************************************

      EC_Counting_Supplier other_supplier;

      other_supplier.activate (consumer_admin.in (),
                               milliseconds
                               TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      other_supplier.connect (supplier_admin.in (),
                              event_source + 1,
                              event_type + 1,
                              event_source + 1,
                              event_type + 1
                              TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // ****************************************************************

      EC_Counting_Supplier any_source_supplier;

      any_source_supplier.activate (consumer_admin.in (),
                                    milliseconds
                                    TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      any_source_supplier.connect (supplier_admin.in (),
                                   0,
                                   event_type,
                                   event_source,
                                   event_type
                                   TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // ****************************************************************

      EC_Counting_Supplier any_type_supplier;

      any_type_supplier.activate (consumer_admin.in (),
                                  milliseconds
                                  TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      any_type_supplier.connect (supplier_admin.in (),
                                 event_source,
                                 0,
                                 event_source,
                                 event_type
                                 TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // ****************************************************************

      EC_Counting_Supplier wildcard_supplier;

      wildcard_supplier.activate (consumer_admin.in (),
                                  milliseconds
                                  TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      wildcard_supplier.connect (supplier_admin.in (),
                                 0,
                                 0,
                                 event_source,
                                 event_type
                                 TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

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
                                  TAO_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;
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
                                   TAO_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;
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
                                     TAO_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;
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
                                   TAO_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;
      }

      // ****************************************************************

      ACE_Time_Value tv (5, 0);
      // Wait for events, using work_pending()/perform_work() may help
      // or using another thread, this example is too simple for that.
      orb->run (tv);

      // ****************************************************************

      wildcard_consumer.disconnect (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      any_source_consumer.disconnect (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      any_type_consumer.disconnect (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      regular_consumer.disconnect (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      wildcard_supplier.deactivate (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      wildcard_supplier.disconnect (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      any_type_supplier.deactivate (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      any_source_supplier.deactivate (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      any_source_supplier.disconnect (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      other_supplier.deactivate (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      other_supplier.disconnect (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      supplier.deactivate (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      supplier.disconnect (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // ****************************************************************

      event_channel->destroy (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // ****************************************************************

      poa->destroy (1, 1 TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      orb->destroy (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

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
