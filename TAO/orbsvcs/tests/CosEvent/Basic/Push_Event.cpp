// $Id$

#include "Counting_Consumer.h"
#include "Counting_Supplier.h"
#include "orbsvcs/CosEvent/CEC_EventChannel.h"
#include "orbsvcs/CosEvent/CEC_Default_Factory.h"

ACE_RCSID(CEC_Tests_Basic, Push_Event, "$Id$")

int
main (int argc, char* argv[])
{
  TAO_CEC_Default_Factory::init_svcs ();

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

      TAO_CEC_EventChannel_Attributes attributes (poa.in (),
                                                  poa.in ());

      TAO_CEC_EventChannel ec_impl (attributes);
      ec_impl.activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CosEventChannelAdmin::EventChannel_var event_channel =
        ec_impl._this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // ****************************************************************

      // Obtain the consumer admin..
      CosEventChannelAdmin::ConsumerAdmin_var consumer_admin =
        event_channel->for_consumers (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Obtain the supplier admin..
      CosEventChannelAdmin::SupplierAdmin_var supplier_admin =
        event_channel->for_suppliers (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // ****************************************************************

      const int milliseconds = 50;

      CEC_Counting_Supplier supplier_00;

      supplier_00.connect (supplier_admin.in ()
                           ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // ****************************************************************

      CEC_Counting_Supplier supplier_01;

      supplier_01.connect (supplier_admin.in ()
                           ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // ****************************************************************

      CEC_Counting_Supplier supplier_10;

      supplier_10.connect (supplier_admin.in ()
                           ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // ****************************************************************

      CEC_Counting_Supplier supplier_11;

      supplier_11.connect (supplier_admin.in ()
                           ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // ****************************************************************

      // Create a consumer, intialize its RT_Info structures, and
      // connnect to the event channel....
      CEC_Counting_Consumer consumer_00 ("Consumer/00");
      consumer_00.connect (consumer_admin.in ()
                           ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // ****************************************************************

      CEC_Counting_Consumer consumer_01 ("Consumer/01");
      consumer_01.connect (consumer_admin.in ()
                           ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // ****************************************************************

      CEC_Counting_Supplier_Task task_00 (&supplier_00, milliseconds);
      CEC_Counting_Supplier_Task task_01 (&supplier_01, milliseconds);
      CEC_Counting_Supplier_Task task_10 (&supplier_10, milliseconds);
      CEC_Counting_Supplier_Task task_11 (&supplier_11, milliseconds);

      if (task_00.activate (THR_BOUND|THR_NEW_LWP, 1) != 0)
        {
          ACE_ERROR ((LM_ERROR, "Cannot activate task 00\n"));
        }
      if (task_01.activate (THR_BOUND|THR_NEW_LWP, 1) != 0)
        {
          ACE_ERROR ((LM_ERROR, "Cannot activate task 01\n"));
        }
      if (task_10.activate (THR_BOUND|THR_NEW_LWP, 1) != 0)
        {
          ACE_ERROR ((LM_ERROR, "Cannot activate task 10\n"));
        }
      if (task_11.activate (THR_BOUND|THR_NEW_LWP, 1) != 0)
        {
          ACE_ERROR ((LM_ERROR, "Cannot activate task 11\n"));
        }

      ACE_Time_Value tv (5, 0);
      ACE_OS::sleep (tv);

      task_00.stop ();
      task_01.stop ();
      task_10.stop ();
      task_11.stop ();

      // Wait for all the threads to complete and the return
      ACE_Thread_Manager::instance ()->wait ();

      // ****************************************************************

      consumer_01.disconnect (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      consumer_00.disconnect (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      supplier_11.disconnect (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      supplier_10.disconnect (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      supplier_01.disconnect (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      supplier_00.disconnect (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // ****************************************************************

      event_channel->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // ****************************************************************

      poa->destroy (1, 1 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // ****************************************************************

      CORBA::ULong expected =
        task_00.push_count ()
        + task_01.push_count ()
        + task_10.push_count ()
        + task_11.push_count ();

      consumer_00.dump_results (expected, 5);
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
