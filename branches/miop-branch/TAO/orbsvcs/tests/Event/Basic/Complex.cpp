// $Id$

#include "Counting_Consumer.h"
#include "Counting_Supplier.h"
#include "orbsvcs/Time_Utilities.h"
#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/Event/EC_Event_Channel.h"
#include "orbsvcs/Event/EC_Default_Factory.h"

ACE_RCSID(EC_Tests, Complex, "$Id$")

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

      EC_Counting_Supplier supplier_00;

      supplier_00.activate (consumer_admin.in (),
                            milliseconds
                            TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      supplier_00.connect (supplier_admin.in (),
                           event_source,
                           event_type,
                           event_source,
                           event_type
                           TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // ****************************************************************

      EC_Counting_Supplier supplier_01;

      supplier_01.activate (consumer_admin.in (),
                            milliseconds
                            TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      supplier_01.connect (supplier_admin.in (),
                           event_source,
                           event_type + 1,
                           event_source,
                           event_type + 1
                           TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // ****************************************************************

      EC_Counting_Supplier supplier_10;

      supplier_10.activate (consumer_admin.in (),
                            milliseconds
                            TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      supplier_10.connect (supplier_admin.in (),
                           event_source + 1,
                           event_type,
                           event_source + 1,
                           event_type
                           TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // ****************************************************************

      EC_Counting_Supplier supplier_11;

      supplier_11.activate (consumer_admin.in (),
                            milliseconds
                            TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      supplier_11.connect (supplier_admin.in (),
                           event_source + 1,
                           event_type + 1,
                           event_source + 1,
                           event_type + 1
                           TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // ****************************************************************

      EC_Counting_Consumer consumer_00 ("Consumer/00");
      // Create a consumer, intialize its RT_Info structures, and
      // connnect to the event channel....


      {
        ACE_ConsumerQOS_Factory consumer_qos;
        consumer_qos.start_logical_and_group (2);
        consumer_qos.start_negation ();
        consumer_qos.insert_source (event_source + 1, 0);
        consumer_qos.insert_type (event_type, 0);

        consumer_00.connect (consumer_admin.in (),
                             consumer_qos.get_ConsumerQOS ()
                             TAO_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;
      }

      // ****************************************************************

      EC_Counting_Consumer consumer_01 ("Consumer/01");
      // Create a consumer, intialize its RT_Info structures, and
      // connnect to the event channel....


      {
        ACE_ConsumerQOS_Factory consumer_qos;
        consumer_qos.start_logical_and_group (2);
        consumer_qos.insert_null_terminator (); // Any event
        consumer_qos.start_negation (); // but for (source,type)
        consumer_qos.insert (event_source, event_type, 0);

        consumer_01.connect (consumer_admin.in (),
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

      consumer_01.disconnect (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      consumer_00.disconnect (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      supplier_11.deactivate (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      supplier_10.deactivate (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      supplier_01.deactivate (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      supplier_00.deactivate (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      supplier_11.disconnect (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      supplier_10.disconnect (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      supplier_01.disconnect (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      supplier_00.disconnect (TAO_ENV_SINGLE_ARG_PARAMETER);
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
        supplier_00.event_count;
      consumer_00.dump_results (expected, 5);

      expected =
        supplier_01.event_count
        + supplier_10.event_count
        + supplier_11.event_count;
      consumer_01.dump_results (expected, 5);
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Service");
      return 1;
    }
  ACE_ENDTRY;
  return 0;
}
