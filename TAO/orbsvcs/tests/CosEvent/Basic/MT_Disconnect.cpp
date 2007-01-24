// $Id$

#include "MT_Disconnect.h"
#include "Counting_Consumer.h"
#include "Counting_Supplier.h"

#include "orbsvcs/CosEvent/CEC_EventChannel.h"
#include "orbsvcs/CosEvent/CEC_Default_Factory.h"

ACE_RCSID (CEC_Tests,
           MT_Disconnect,
           "$Id$")

static void run_test (PortableServer::POA_ptr poa,
                      int use_callbacks
                      ACE_ENV_ARG_DECL);

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

      CORBA::Object_var object =
        orb->resolve_initial_references ("RootPOA" ACE_ENV_ARG_PARAMETER);
      PortableServer::POA_var poa =
        PortableServer::POA::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
      PortableServer::POAManager_var poa_manager =
        poa->the_POAManager ();
      poa_manager->activate ();

      // ****************************************************************

      run_test (poa.in (), 0 ACE_ENV_ARG_PARAMETER);

      run_test (poa.in (), 1 ACE_ENV_ARG_PARAMETER);

      // ****************************************************************

      poa->destroy (1, 1 ACE_ENV_ARG_PARAMETER);

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

// ****************************************************************

void
deactivate_servant (PortableServer::Servant servant
                    ACE_ENV_ARG_DECL)
{
  PortableServer::POA_var poa =
    servant->_default_POA ();
  PortableServer::ObjectId_var id =
    poa->servant_to_id (servant ACE_ENV_ARG_PARAMETER);
  poa->deactivate_object (id.in () ACE_ENV_ARG_PARAMETER);
}

void
run_test (PortableServer::POA_ptr poa,
          int use_callbacks
          ACE_ENV_ARG_DECL)
{
  TAO_CEC_EventChannel_Attributes attributes (poa, poa);
  attributes.disconnect_callbacks = use_callbacks;

  TAO_CEC_EventChannel ec_impl (attributes);
  ec_impl.activate ();

  CosEventChannelAdmin::EventChannel_var event_channel =
    ec_impl._this ();

  MTD_Task task (event_channel.in (), use_callbacks);

  if (task.activate (THR_BOUND|THR_NEW_LWP, 1) != 0)
    {
      ACE_ERROR ((LM_ERROR, "Cannot activate the tasks\n"));
    }

  // Wait for all the threads to complete and the return
  ACE_Thread_Manager::instance ()->wait ();

  event_channel->destroy ();

  deactivate_servant (&ec_impl ACE_ENV_ARG_PARAMETER);
}

MTD_Task::MTD_Task (CosEventChannelAdmin::EventChannel_ptr ec,
            int callbacks)
  :  event_channel (CosEventChannelAdmin::EventChannel::_duplicate (ec)),
     use_callbacks (callbacks)
{
}


int
MTD_Task::svc ()
{
  for (int i = 0; i < 10; ++i)
    {
      ACE_TRY_NEW_ENV
        {
          this->run_iteration ();
        }
      ACE_CATCHANY
        {
          return -1;
        }
      ACE_ENDTRY;
    }
  return 0;
}

void
MTD_Task::run_iteration (void)
{
  // Obtain the consumer admin..
  CosEventChannelAdmin::ConsumerAdmin_var consumer_admin =
    this->event_channel->for_consumers ();

  // and the supplier admin..
  CosEventChannelAdmin::SupplierAdmin_var supplier_admin =
    this->event_channel->for_suppliers ();

  // ****************************************************************

  int iterations = 100;

  CEC_Counting_Supplier supplier_0;
  CEC_Counting_Supplier supplier_1;
  CEC_Counting_Consumer consumer_0 ("Consumer/0");
  CEC_Counting_Consumer consumer_1 ("Consumer/1");

  for (int i = 0; i != iterations; ++i)
    {
      supplier_0.connect (supplier_admin.in ()
                          ACE_ENV_ARG_PARAMETER);
      consumer_0.connect (consumer_admin.in ()
                          ACE_ENV_ARG_PARAMETER);
      if (i % 2 == 1)
        {
          supplier_1.connect (supplier_admin.in ()
                              ACE_ENV_ARG_PARAMETER);
          consumer_1.connect (consumer_admin.in ()
                              ACE_ENV_ARG_PARAMETER);
        }
      supplier_0.disconnect ();
      consumer_0.disconnect ();
      if (i % 2 == 1)
        {
          consumer_1.disconnect ();
          supplier_1.disconnect ();
        }
    }

  deactivate_servant (&supplier_0 ACE_ENV_ARG_PARAMETER);

  deactivate_servant (&consumer_0 ACE_ENV_ARG_PARAMETER);

  CORBA::ULong count_0 = 0;
  CORBA::ULong count_1 = 0;
  if (use_callbacks)
    {
      count_0 += iterations;
      count_1 += iterations / 2;
    }

  if (consumer_0.disconnect_count != count_0)
    ACE_ERROR ((LM_ERROR,
                "ERROR: incorrect number of disconnect calls (%d/%d) for "
                "consumer 0 (%d)\n",
                consumer_0.disconnect_count, count_0,
                use_callbacks));
  if (supplier_0.disconnect_count != count_0)
    ACE_ERROR ((LM_ERROR,
                "ERROR: incorrect number of disconnect calls (%d/%d) for "
                "supplier 0 (%d)\n",
                supplier_0.disconnect_count, count_0,
                use_callbacks));

  if (consumer_1.disconnect_count != count_1)
    ACE_ERROR ((LM_ERROR,
                "ERROR: incorrect number of disconnect calls (%d/%d) for "
                "consumer 1 (%d)\n",
                consumer_1.disconnect_count, count_1,
                use_callbacks));
  if (supplier_1.disconnect_count != count_1)
    ACE_ERROR ((LM_ERROR,
                "ERROR: incorrect number of disconnect calls (%d/%d) for "
                "supplier 1 (%d)\n",
                supplier_1.disconnect_count, count_1,
                use_callbacks));
}
