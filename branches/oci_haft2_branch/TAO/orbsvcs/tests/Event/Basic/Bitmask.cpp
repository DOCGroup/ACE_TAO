// $Id$

#include "Counting_Consumer.h"
#include "Counting_Supplier.h"

#include "orbsvcs/Time_Utilities.h"
#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/Event/EC_Event_Channel.h"
#include "orbsvcs/Event/EC_Default_Factory.h"

ACE_RCSID(EC_Tests, Bitmask, "$Id$")

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

      const int milliseconds = 50;

      EC_Counting_Supplier first_supplier;

      first_supplier.activate (consumer_admin.in (),
                               milliseconds
                               ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      first_supplier.connect (supplier_admin.in (),
                              0x00001111UL,
                              0x11110000UL,
                              0x00001111UL,
                              0x11110000UL
                              ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      EC_Counting_Supplier second_supplier;

      second_supplier.activate (consumer_admin.in (),
                                milliseconds
                                ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      second_supplier.connect (supplier_admin.in (),
                               0x01100000UL,
                               0x00000110UL,
                               0x01100000UL,
                               0x00000110UL
                               ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // ****************************************************************

      EC_Counting_Consumer consumer_bitmask_reject ("Consumer/bitmask/reject");
      // Create a consumer, intialize its RT_Info structures, and
      // connnect to the event channel....

      {
        ACE_ConsumerQOS_Factory consumer_qos;
        consumer_qos.start_bitmask (0x00001111, 0x11110000);
        consumer_qos.start_disjunction_group (1);
        consumer_qos.insert (0x01100000, 0x00000110, 0);

        consumer_bitmask_reject.connect (consumer_admin.in (),
                                         consumer_qos.get_ConsumerQOS ()
                                         ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;
      }

      // ****************************************************************

      EC_Counting_Consumer consumer_bitmask_accept ("Consumer/bitmask/accept");
      // Create a consumer, intialize its RT_Info structures, and
      // connnect to the event channel....

      {
        ACE_ConsumerQOS_Factory consumer_qos;
        consumer_qos.start_bitmask (0x01100110, 0x01100110);
        consumer_qos.insert_null_terminator ();

        consumer_bitmask_accept.connect (consumer_admin.in (),
                                         consumer_qos.get_ConsumerQOS ()
                                         ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;
      }

      // ****************************************************************

      EC_Counting_Consumer consumer_bitmask_filter ("Consumer/bitmask/filter");
      // Create a consumer, intialize its RT_Info structures, and
      // connnect to the event channel....

      {
        ACE_ConsumerQOS_Factory consumer_qos;
        consumer_qos.start_bitmask (0x00000110, 0x01100000);
        consumer_qos.insert_null_terminator ();

        consumer_bitmask_filter.connect (consumer_admin.in (),
                                         consumer_qos.get_ConsumerQOS ()
                                         ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;
      }

      // ****************************************************************

      EC_Counting_Consumer consumer_bitmask_value ("Consumer/bitmask/value");
      // Create a consumer, intialize its RT_Info structures, and
      // connnect to the event channel....

      {
        ACE_ConsumerQOS_Factory consumer_qos;
        consumer_qos.start_disjunction_group (1);
        consumer_qos.insert_bitmasked_value (0x11110000, 0x00001111,
                                             0x01100000, 0x00000110);

        consumer_bitmask_value.connect (consumer_admin.in (),
                                        consumer_qos.get_ConsumerQOS ()
                                        ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;
      }

      // ****************************************************************

      EC_Counting_Consumer consumer_bitmask_loose ("Consumer/bitmask/loose");
      // Create a consumer, intialize its RT_Info structures, and
      // connnect to the event channel....

      {
        ACE_ConsumerQOS_Factory consumer_qos;
        consumer_qos.start_disjunction_group (1);
        consumer_qos.insert_bitmasked_value (0x11111111, 0x11111111,
                                             0x01100000, 0x00000110);

        consumer_bitmask_loose.connect (consumer_admin.in (),
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

      consumer_bitmask_loose.disconnect (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      consumer_bitmask_value.disconnect (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      consumer_bitmask_filter.disconnect (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      consumer_bitmask_accept.disconnect (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      consumer_bitmask_reject.disconnect (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // ****************************************************************

      second_supplier.deactivate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      second_supplier.disconnect (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      first_supplier.deactivate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      first_supplier.disconnect (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // ****************************************************************

      event_channel->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // ****************************************************************

      poa->destroy (1, 1 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // ****************************************************************

      consumer_bitmask_reject.dump_results (0, 5);
      CORBA::ULong expected =
        first_supplier.event_count
        + second_supplier.event_count;
      consumer_bitmask_accept.dump_results (expected, 5);

      expected = second_supplier.event_count;
      consumer_bitmask_filter.dump_results (expected, 5);
      expected = second_supplier.event_count;
      consumer_bitmask_value.dump_results (expected, 5);
      expected = second_supplier.event_count;
      consumer_bitmask_loose.dump_results (expected, 5);

      orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
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
