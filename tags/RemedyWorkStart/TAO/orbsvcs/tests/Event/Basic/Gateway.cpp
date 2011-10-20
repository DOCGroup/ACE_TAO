// $Id$

#include "Counting_Consumer.h"
#include "Counting_Supplier.h"
#include "orbsvcs/Time_Utilities.h"
#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/Event/EC_Event_Channel.h"
#include "orbsvcs/Event/EC_Default_Factory.h"
#include "orbsvcs/Event/EC_Gateway_IIOP.h"

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

      TAO_EC_Event_Channel ec_impl_1 (attributes);
      ec_impl_1.activate ();

      RtecEventChannelAdmin::EventChannel_var event_channel_1 =
        ec_impl_1._this ();

      // ****************************************************************

      TAO_EC_Event_Channel ec_impl_2 (attributes);
      ec_impl_2.activate ();

      RtecEventChannelAdmin::EventChannel_var event_channel_2 =
        ec_impl_2._this ();

      // ****************************************************************

      // Obtain the consumer admin..
      RtecEventChannelAdmin::ConsumerAdmin_var consumer_admin_1 =
        event_channel_1->for_consumers ();

      // Obtain the supplier admin..
      RtecEventChannelAdmin::SupplierAdmin_var supplier_admin_1 =
        event_channel_1->for_suppliers ();

      // ****************************************************************

      // Obtain the consumer admin..
      RtecEventChannelAdmin::ConsumerAdmin_var consumer_admin_2 =
        event_channel_2->for_consumers ();

      // Obtain the supplier admin..
      RtecEventChannelAdmin::SupplierAdmin_var supplier_admin_2 =
        event_channel_2->for_suppliers ();

      // ****************************************************************

      TAO_EC_Gateway_IIOP gateway;
      gateway.init (event_channel_1.in (),
                    event_channel_2.in ());

      RtecEventChannelAdmin::Observer_var obs =
        gateway._this ();

      RtecEventChannelAdmin::Observer_Handle h =
        event_channel_2->append_observer (obs.in ());

      gateway.observer_handle (h);

      // ****************************************************************

      const int event_type = 20;
      const int event_source = 10;
      const int milliseconds = 50;

      EC_Counting_Supplier supplier_00;

      supplier_00.activate (consumer_admin_1.in (),
                            milliseconds);
      supplier_00.connect (supplier_admin_1.in (),
                           event_source,
                           event_type,
                           event_source,
                           event_type);

      EC_Counting_Supplier supplier_01;

      supplier_01.activate (consumer_admin_1.in (),
                            milliseconds);
      supplier_01.connect (supplier_admin_1.in (),
                           event_source,
                           event_type + 1,
                           event_source,
                           event_type + 1);

      // ****************************************************************

      EC_Counting_Consumer consumer_00 ("Consumer/00");
      // Create a consumer, intialize its RT_Info structures, and
      // connnect to the event channel....


      {
        ACE_ConsumerQOS_Factory consumer_qos;
        consumer_qos.start_disjunction_group (1);
        consumer_qos.insert (event_source, event_type, 0);

        consumer_00.connect (consumer_admin_2.in (),
                             consumer_qos.get_ConsumerQOS ());
      }

      // ****************************************************************

      ACE_Time_Value tv (5, 0);
      // Wait for events, using work_pending()/perform_work() may help
      // or using another thread, this example is too simple for that.
      orb->run (tv);

      // ****************************************************************

      CORBA::ULong expected =
        supplier_00.event_count;
      consumer_00.dump_results (expected, 5);

      CORBA::ULong last_count = consumer_00.event_count;
      CORBA::ULong last_supplier_00 = supplier_00.event_count;
      CORBA::ULong last_supplier_01 = supplier_01.event_count;

      // ****************************************************************

      {
        ACE_ConsumerQOS_Factory consumer_qos;
        consumer_qos.start_disjunction_group (2);
        consumer_qos.insert (event_source, event_type,     0);
        consumer_qos.insert (event_source, event_type + 1, 0);

        consumer_00.connect (consumer_admin_2.in (),
                             consumer_qos.get_ConsumerQOS ());
      }

      // ****************************************************************

      tv = ACE_Time_Value (5, 0);
      orb->run (tv);

      expected = last_count
        + supplier_00.event_count - last_supplier_00
        + supplier_01.event_count - last_supplier_01;
      consumer_00.dump_results (expected, 5);

      last_count = consumer_00.event_count;
      last_supplier_00 = supplier_00.event_count;
      last_supplier_01 = supplier_01.event_count;

      // ****************************************************************

      {
        ACE_ConsumerQOS_Factory consumer_qos;
        consumer_qos.start_disjunction_group (1);
        consumer_qos.insert (event_source, event_type, 0);

        consumer_00.connect (consumer_admin_2.in (),
                             consumer_qos.get_ConsumerQOS ());
      }

      // ****************************************************************

      tv = ACE_Time_Value (5, 0);
      orb->run (tv);

      expected = last_count
        + supplier_00.event_count - last_supplier_00;
      consumer_00.dump_results (expected, 5);

      last_count = consumer_00.event_count;
      last_supplier_00 = supplier_00.event_count;
      last_supplier_01 = supplier_01.event_count;

      // ****************************************************************

      consumer_00.disconnect ();

      // ****************************************************************

      {
        ACE_ConsumerQOS_Factory consumer_qos;
        consumer_qos.start_disjunction_group (1);
        consumer_qos.insert_type (event_type, 0);

        consumer_00.connect (consumer_admin_2.in (),
                             consumer_qos.get_ConsumerQOS ());
      }

      // ****************************************************************

      tv = ACE_Time_Value (5, 0);
      // Wait for events, using work_pending()/perform_work() may help
      // or using another thread, this example is too simple for that.
      orb->run (tv);

      // ****************************************************************

      expected = last_count
        + supplier_00.event_count - last_supplier_00;
      consumer_00.dump_results (expected, 5);

      last_count = consumer_00.event_count;
      last_supplier_00 = supplier_00.event_count;
      last_supplier_01 = supplier_01.event_count;

      // ****************************************************************

      {
        ACE_ConsumerQOS_Factory consumer_qos;
        consumer_qos.start_disjunction_group (2);
        consumer_qos.insert_type (event_type,     0);
        consumer_qos.insert_type (event_type + 1, 0);

        consumer_00.connect (consumer_admin_2.in (),
                             consumer_qos.get_ConsumerQOS ());
      }

      // ****************************************************************

      tv = ACE_Time_Value (5, 0);
      orb->run (tv);

      expected = last_count
        + supplier_00.event_count - last_supplier_00
        + supplier_01.event_count - last_supplier_01;
      consumer_00.dump_results (expected, 5);

      last_count = consumer_00.event_count;
      last_supplier_00 = supplier_00.event_count;
      last_supplier_01 = supplier_01.event_count;

      // ****************************************************************

      {
        ACE_ConsumerQOS_Factory consumer_qos;
        consumer_qos.start_disjunction_group (1);
        consumer_qos.insert_type (event_type, 0);

        consumer_00.connect (consumer_admin_2.in (),
                             consumer_qos.get_ConsumerQOS ());
      }

      // ****************************************************************

      tv = ACE_Time_Value (5, 0);
      orb->run (tv);

      expected = last_count
        + supplier_00.event_count - last_supplier_00;
      consumer_00.dump_results (expected, 5);

      last_count = consumer_00.event_count;
      last_supplier_00 = supplier_00.event_count;
      last_supplier_01 = supplier_01.event_count;

      // ****************************************************************

      consumer_00.disconnect ();

      supplier_01.deactivate ();
      supplier_00.deactivate ();

      supplier_01.disconnect ();
      supplier_00.disconnect ();

      gateway.shutdown ();

      // ****************************************************************

      event_channel_1->destroy ();

      event_channel_2->destroy ();

      // ****************************************************************

      poa->destroy (1, 1);

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Service");
      return 1;
    }
  return 0;
}
