// $Id$

#include "Counting_Consumer.h"
#include "Counting_Supplier.h"

#include "orbsvcs/Time_Utilities.h"
#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/Event/EC_Event_Channel.h"
#include "orbsvcs/Event/EC_Default_Factory.h"

ACE_RCSID(EC_Tests, Negation, "$Id$")

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
      attributes.consumer_reconnect = 1;
      attributes.supplier_reconnect = 1;

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

      EC_Counting_Supplier first_supplier;

      first_supplier.activate (consumer_admin.in (),
                               milliseconds
                               TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      first_supplier.connect (supplier_admin.in (),
                              event_source,
                              event_type,
                              event_source,
                              event_type
                              TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      EC_Counting_Supplier second_supplier;

      second_supplier.activate (consumer_admin.in (),
                                milliseconds
                                TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      second_supplier.connect (supplier_admin.in (),
                               event_source,
                               event_type + 1,
                               event_source,
                               event_type + 1
                               TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      EC_Counting_Supplier third_supplier;

      third_supplier.activate (consumer_admin.in (),
                               milliseconds
                               TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      third_supplier.connect (supplier_admin.in (),
                              event_source,
                              event_type + 1,
                              event_source,
                              event_type + 1
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

      EC_Counting_Consumer negation_consumer ("Consumer/negation");
      // Create a consumer, intialize its RT_Info structures, and
      // connnect to the event channel....

      {
        ACE_ConsumerQOS_Factory consumer_qos;
        consumer_qos.start_negation ();
        consumer_qos.start_disjunction_group ();
        consumer_qos.insert (event_source, event_type, 0);

        negation_consumer.connect (consumer_admin.in (),
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

      negation_consumer.disconnect (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // ****************************************************************

      regular_consumer.disconnect (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // ****************************************************************

      third_supplier.deactivate (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      third_supplier.disconnect (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      second_supplier.deactivate (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      second_supplier.disconnect (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      first_supplier.deactivate (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      first_supplier.disconnect (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // ****************************************************************

      event_channel->destroy (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // ****************************************************************

      poa->destroy (1, 1 TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // ****************************************************************

      CORBA::ULong expected =
        third_supplier.event_count
        + second_supplier.event_count;
      negation_consumer.dump_results (expected, 5);
      expected =
        first_supplier.event_count;
      regular_consumer.dump_results (expected, 5);

      orb->destroy (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
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
