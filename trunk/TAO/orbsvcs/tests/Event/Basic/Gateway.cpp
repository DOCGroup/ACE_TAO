// $Id$

#include "Counting_Consumer.h"
#include "Counting_Supplier.h"
#include "orbsvcs/Time_Utilities.h"
#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/Event/EC_Event_Channel.h"
#include "orbsvcs/Event/EC_Default_Factory.h"
#include "orbsvcs/Event/EC_Gateway.h"

ACE_RCSID(EC_Tests, Gateway, "$Id$")

int
main (int argc, char* argv[])
{
  TAO_EC_Default_Factory::init_svcs ();

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // ORB initialization boiler plate...
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "", ACE_TRY_ENV);
      ACE_TRY_CHECK;

      CORBA::Object_var object =
        orb->resolve_initial_references ("RootPOA", ACE_TRY_ENV);
      ACE_TRY_CHECK;
      PortableServer::POA_var poa =
        PortableServer::POA::_narrow (object.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;
      PortableServer::POAManager_var poa_manager =
        poa->the_POAManager (ACE_TRY_ENV);
      ACE_TRY_CHECK;
      poa_manager->activate (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // ****************************************************************

      TAO_EC_Event_Channel_Attributes attributes (poa.in (),
                                                  poa.in ());
      attributes.consumer_reconnect = 1;
      attributes.supplier_reconnect = 1;

      TAO_EC_Event_Channel ec_impl_1 (attributes);
      ec_impl_1.activate (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      RtecEventChannelAdmin::EventChannel_var event_channel_1 =
        ec_impl_1._this (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // ****************************************************************

      TAO_EC_Event_Channel ec_impl_2 (attributes);
      ec_impl_2.activate (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      RtecEventChannelAdmin::EventChannel_var event_channel_2 =
        ec_impl_2._this (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // ****************************************************************

      // Obtain the consumer admin..
      RtecEventChannelAdmin::ConsumerAdmin_var consumer_admin_1 =
        event_channel_1->for_consumers (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Obtain the supplier admin..
      RtecEventChannelAdmin::SupplierAdmin_var supplier_admin_1 =
        event_channel_1->for_suppliers (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // ****************************************************************

      // Obtain the consumer admin..
      RtecEventChannelAdmin::ConsumerAdmin_var consumer_admin_2 =
        event_channel_2->for_consumers (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Obtain the supplier admin..
      RtecEventChannelAdmin::SupplierAdmin_var supplier_admin_2 =
        event_channel_2->for_suppliers (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // ****************************************************************

      TAO_EC_Gateway_IIOP gateway;
      gateway.init (event_channel_1.in (),
                    event_channel_2.in (),
                    RtecScheduler::Scheduler::_nil (),
                    RtecScheduler::Scheduler::_nil (),
                    0, 0,
                    ACE_TRY_ENV);

      RtecEventChannelAdmin::Observer_var obs =
        gateway._this (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      RtecEventChannelAdmin::Observer_Handle h =
        event_channel_2->append_observer (obs.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      gateway.observer_handle (h);

      // ****************************************************************

      const int event_type = 20;
      const int event_source = 10;
      const int milliseconds = 50;

      EC_Counting_Supplier supplier_00;

      supplier_00.activate (consumer_admin_1.in (),
                            milliseconds,
                            ACE_TRY_ENV);
      ACE_TRY_CHECK;
      supplier_00.connect (supplier_admin_1.in (),
                           event_source,
                           event_type,
                           event_source,
                           event_type,
                           ACE_TRY_ENV);
      ACE_TRY_CHECK;

      EC_Counting_Supplier supplier_01;

      supplier_01.activate (consumer_admin_1.in (),
                            milliseconds,
                            ACE_TRY_ENV);
      ACE_TRY_CHECK;
      supplier_01.connect (supplier_admin_1.in (),
                           event_source,
                           event_type + 1,
                           event_source,
                           event_type + 1,
                           ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // ****************************************************************

      EC_Counting_Consumer consumer_00 ("Consumer/00");
      // Create a consumer, intialize its RT_Info structures, and
      // connnect to the event channel....


      {
        ACE_ConsumerQOS_Factory consumer_qos;
        consumer_qos.start_disjunction_group (1);
        consumer_qos.insert_type (event_type, 0);

        consumer_00.connect (consumer_admin_2.in (),
                             consumer_qos.get_ConsumerQOS (),
                             ACE_TRY_ENV);
        ACE_TRY_CHECK;
      }

      // ****************************************************************

      ACE_Time_Value tv (5, 0);
      // Wait for events, using work_pending()/perform_work() may help
      // or using another thread, this example is too simple for that.
      orb->run (tv);

      // ****************************************************************

      CORBA::ULong expected =
        supplier_00.event_count;
      CORBA::ULong discarded =
        supplier_01.event_count;
      consumer_00.dump_results (expected, 5);

      // ****************************************************************

      {
        ACE_ConsumerQOS_Factory consumer_qos;
        consumer_qos.start_disjunction_group (2);
        consumer_qos.insert_type (event_type, 0);
        consumer_qos.insert_type (event_type + 1, 0);

        consumer_00.connect (consumer_admin_2.in (),
                             consumer_qos.get_ConsumerQOS (),
                             ACE_TRY_ENV);
        ACE_TRY_CHECK;
      }

      // ****************************************************************

      tv = ACE_Time_Value (5, 0);
      orb->run (tv);

      // ****************************************************************

      consumer_00.disconnect (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      supplier_01.deactivate (ACE_TRY_ENV);
      ACE_TRY_CHECK;
      supplier_00.deactivate (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      supplier_01.disconnect (ACE_TRY_ENV);
      ACE_TRY_CHECK;
      supplier_00.disconnect (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      gateway.shutdown (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // ****************************************************************

      event_channel_1->destroy (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      event_channel_2->destroy (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // ****************************************************************

      poa->destroy (1, 1, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      orb->destroy (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      expected =
        supplier_00.event_count
        + supplier_01.event_count - discarded;
      consumer_00.dump_results (expected, 5);
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Service");
      return 1;
    }
  ACE_ENDTRY;
  return 0;
}
