// $Id$

#include "Counting_Consumer.h"
#include "Counting_Supplier.h"
#include "orbsvcs/CosEvent/CEC_EventChannel.h"
#include "orbsvcs/CosEvent/CEC_Default_Factory.h"
#include "ace/OS_NS_unistd.h"

ACE_RCSID (CEC_Tests_Basic,
           Pull_Push_Event,
           "$Id$")

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  TAO_CEC_Default_Factory::init_svcs ();

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

      TAO_CEC_EventChannel_Attributes attributes (poa.in (),
                                                  poa.in ());

      TAO_CEC_EventChannel ec_impl (attributes);
      ec_impl.activate ();

      CosEventChannelAdmin::EventChannel_var event_channel =
        ec_impl._this ();

      // ****************************************************************

      // Obtain the consumer admin..
      CosEventChannelAdmin::ConsumerAdmin_var consumer_admin =
        event_channel->for_consumers ();

      // Obtain the supplier admin..
      CosEventChannelAdmin::SupplierAdmin_var supplier_admin =
        event_channel->for_suppliers ();

      // ****************************************************************

      const int milliseconds = 50;

      CEC_Counting_Supplier supplier_00;

      supplier_00.connect (supplier_admin.in ());

      // ****************************************************************

      CEC_Counting_Supplier supplier_01;

      supplier_01.connect (supplier_admin.in ());

      // ****************************************************************

      CEC_Pull_Counting_Supplier supplier_10;

      supplier_10.connect (supplier_admin.in ());

      // ****************************************************************

      CEC_Pull_Counting_Supplier supplier_11;

      supplier_11.connect (supplier_admin.in ());

      // ****************************************************************

      // Create a consumer, intialize its RT_Info structures, and
      // connnect to the event channel....
      CEC_Counting_Consumer consumer_00 ("Consumer/00");
      consumer_00.connect (consumer_admin.in ());

      // ****************************************************************

      CEC_Pull_Counting_Consumer consumer_01 ("Consumer/01");
      consumer_01.connect (consumer_admin.in ());

      // ****************************************************************

      CEC_Pull_Counting_Consumer consumer_10 ("Consumer/10");
      consumer_10.connect (consumer_admin.in ());

      // ****************************************************************

      CEC_Counting_Supplier_Task task_00 (&supplier_00,
                                          milliseconds);
      CEC_Counting_Supplier_Task task_01 (&supplier_01,
                                          3 * milliseconds);

      if (task_00.activate (THR_BOUND|THR_NEW_LWP, 1) != 0)
        {
          ACE_ERROR ((LM_ERROR, "Cannot activate task 00\n"));
        }
      if (task_01.activate (THR_BOUND|THR_NEW_LWP, 1) != 0)
        {
          ACE_ERROR ((LM_ERROR, "Cannot activate task 01\n"));
        }

      // ****************************************************************

      CEC_Counting_Consumer_Task consumer_task_01 (&consumer_01,
                                                   5 * milliseconds);
      if (consumer_task_01.activate (THR_BOUND|THR_NEW_LWP, 1) != 0)
        {
          ACE_ERROR ((LM_ERROR, "Cannot activate task 01\n"));
        }

      // ****************************************************************

      CEC_Counting_Consumer_Task consumer_task_10 (&consumer_10,
                                                   10);
      if (consumer_task_10.activate (THR_BOUND|THR_NEW_LWP, 1) != 0)
        {
          ACE_ERROR ((LM_ERROR, "Cannot activate task 10\n"));
        }

      // ****************************************************************

      ACE_Time_Value tv (15, 0);
      ACE_OS::sleep (tv);

      task_00.stop ();
      task_01.stop ();

      ACE_DEBUG ((LM_DEBUG,
                  "Events pulled (so far) by Consumer/01 = %d\n",
                  consumer_task_01.pull_count ()));

      tv = ACE_Time_Value (5, 0);
      ACE_OS::sleep (tv);

      consumer_task_10.stop ();
      consumer_task_01.stop ();

      // Wait for all the threads to complete and the return
      ACE_Thread_Manager::instance ()->wait ();

      // ****************************************************************

      consumer_10.disconnect ();

      consumer_01.disconnect ();

      consumer_00.disconnect ();

      supplier_11.disconnect ();
      supplier_10.disconnect ();
      supplier_01.disconnect ();
      supplier_00.disconnect ();

      // ****************************************************************

      event_channel->destroy ();

      // ****************************************************************

      poa->destroy (1, 1);

      orb->destroy ();

      // ****************************************************************

      CORBA::ULong expected =
        task_00.push_count ()
        + task_01.push_count ()
        + supplier_10.event_count
        + supplier_11.event_count;

      consumer_00.dump_results (expected, 5);

      expected =
        consumer_task_01.pull_count ();
      consumer_01.dump_results (expected, 5);
      expected =
        consumer_task_10.pull_count ();
      consumer_10.dump_results (expected, 5);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Service");
      return 1;
    }
  return 0;
}
