// $Id$

#include "Counting_Consumer.h"
#include "Counting_Supplier.h"
#include "orbsvcs/Time_Utilities.h"
#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/Event/EC_Event_Channel.h"
#include "orbsvcs/Event/EC_Default_Factory.h"

ACE_RCSID (EC_Tests,
           Complex,
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

      EC_Counting_Supplier supplier_00;

      supplier_00.activate (consumer_admin.in (),
                            milliseconds
                            ACE_ENV_ARG_PARAMETER);
      supplier_00.connect (supplier_admin.in (),
                           event_source,
                           event_type,
                           event_source,
                           event_type
                           ACE_ENV_ARG_PARAMETER);

      // ****************************************************************

      EC_Counting_Supplier supplier_01;

      supplier_01.activate (consumer_admin.in (),
                            milliseconds
                            ACE_ENV_ARG_PARAMETER);
      supplier_01.connect (supplier_admin.in (),
                           event_source,
                           event_type + 1,
                           event_source,
                           event_type + 1
                           ACE_ENV_ARG_PARAMETER);

      // ****************************************************************

      EC_Counting_Supplier supplier_10;

      supplier_10.activate (consumer_admin.in (),
                            milliseconds
                            ACE_ENV_ARG_PARAMETER);
      supplier_10.connect (supplier_admin.in (),
                           event_source + 1,
                           event_type,
                           event_source + 1,
                           event_type
                           ACE_ENV_ARG_PARAMETER);

      // ****************************************************************

      EC_Counting_Supplier supplier_11;

      supplier_11.activate (consumer_admin.in (),
                            milliseconds
                            ACE_ENV_ARG_PARAMETER);
      supplier_11.connect (supplier_admin.in (),
                           event_source + 1,
                           event_type + 1,
                           event_source + 1,
                           event_type + 1
                           ACE_ENV_ARG_PARAMETER);

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
                             ACE_ENV_ARG_PARAMETER);
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
                             ACE_ENV_ARG_PARAMETER);
      }

      // ****************************************************************

      ACE_Time_Value tv (5, 0);
      // Wait for events, using work_pending()/perform_work() may help
      // or using another thread, this example is too simple for that.
      orb->run (tv);

      // ****************************************************************

      consumer_01.disconnect ();

      consumer_00.disconnect ();

      supplier_11.deactivate ();
      supplier_10.deactivate ();
      supplier_01.deactivate ();
      supplier_00.deactivate ();

      supplier_11.disconnect ();
      supplier_10.disconnect ();
      supplier_01.disconnect ();
      supplier_00.disconnect ();

      // ****************************************************************

      event_channel->destroy ();

      // ****************************************************************

      poa->destroy (1, 1 ACE_ENV_ARG_PARAMETER);

      orb->destroy ();

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
