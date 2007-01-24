// $Id$

#include "Counting_Consumer.h"
#include "Counting_Supplier.h"

#include "orbsvcs/Time_Utilities.h"
#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/Event/EC_Event_Channel.h"
#include "orbsvcs/Event/EC_Default_Factory.h"

ACE_RCSID (EC_Tests,
           Bitmask,
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

      const int milliseconds = 50;

      EC_Counting_Supplier first_supplier;

      first_supplier.activate (consumer_admin.in (),
                               milliseconds
                               ACE_ENV_ARG_PARAMETER);
      first_supplier.connect (supplier_admin.in (),
                              0x00001111UL,
                              0x11110000UL,
                              0x00001111UL,
                              0x11110000UL
                              ACE_ENV_ARG_PARAMETER);

      EC_Counting_Supplier second_supplier;

      second_supplier.activate (consumer_admin.in (),
                                milliseconds
                                ACE_ENV_ARG_PARAMETER);
      second_supplier.connect (supplier_admin.in (),
                               0x01100000UL,
                               0x00000110UL,
                               0x01100000UL,
                               0x00000110UL
                               ACE_ENV_ARG_PARAMETER);

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
      }

      // ****************************************************************

      ACE_Time_Value tv (5, 0);
      // Wait for events, using work_pending()/perform_work() may help
      // or using another thread, this example is too simple for that.
      orb->run (tv);

      // ****************************************************************

      consumer_bitmask_loose.disconnect ();
      consumer_bitmask_value.disconnect ();
      consumer_bitmask_filter.disconnect ();
      consumer_bitmask_accept.disconnect ();
      consumer_bitmask_reject.disconnect ();

      // ****************************************************************

      second_supplier.deactivate ();
      second_supplier.disconnect ();
      first_supplier.deactivate ();
      first_supplier.disconnect ();

      // ****************************************************************

      event_channel->destroy ();

      // ****************************************************************

      poa->destroy (1, 1 ACE_ENV_ARG_PARAMETER);

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

      orb->destroy ();
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Service");
      return 1;
    }
  ACE_ENDTRY;
  return 0;
}
