// $Id$

#include "MT_Disconnect.h"
#include "Counting_Consumer.h"
#include "Counting_Supplier.h"

#include "orbsvcs/Time_Utilities.h"
#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/Event/EC_Event_Channel.h"
#include "orbsvcs/Event/EC_Default_Factory.h"

ACE_RCSID(EC_Tests, Disconnect, "$Id$")

static void run_test (PortableServer::POA_ptr poa,
                      int use_callbacks,
                      CORBA::Environment &ACE_TRY_ENV);

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

      run_test (poa.in (), 0, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      run_test (poa.in (), 1, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // ****************************************************************

      poa->destroy (1, 1, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      orb->destroy (ACE_TRY_ENV);
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

void
deactivate_servant (PortableServer::Servant servant,
                    CORBA::Environment &ACE_TRY_ENV)
{
  PortableServer::POA_var poa =
    servant->_default_POA (ACE_TRY_ENV);
  ACE_CHECK;
  PortableServer::ObjectId_var id =
    poa->servant_to_id (servant, ACE_TRY_ENV);
  ACE_CHECK;
  poa->deactivate_object (id.in (), ACE_TRY_ENV);
  ACE_CHECK;
}

void
run_test (PortableServer::POA_ptr poa,
          int use_callbacks,
          CORBA::Environment &ACE_TRY_ENV)
{
  TAO_EC_Event_Channel_Attributes attributes (poa, poa);
  attributes.disconnect_callbacks = use_callbacks;

  TAO_EC_Event_Channel ec_impl (attributes);
  ec_impl.activate (ACE_TRY_ENV);
  ACE_CHECK;

  RtecEventChannelAdmin::EventChannel_var event_channel =
    ec_impl._this (ACE_TRY_ENV);
  ACE_CHECK;

  Task task (event_channel.in (), use_callbacks);

  if (task.activate (THR_BOUND|THR_NEW_LWP, 1) != 0)
    {
      ACE_ERROR ((LM_ERROR, "Cannot activate the tasks\n"));
    }

  // Wait for all the threads to complete and the return
  ACE_Thread_Manager::instance ()->wait ();

  event_channel->destroy (ACE_TRY_ENV);
  ACE_CHECK;

  deactivate_servant (&ec_impl, ACE_TRY_ENV);
  ACE_CHECK;
}

Task::Task (RtecEventChannelAdmin::EventChannel_ptr ec,
            int callbacks)
  :  event_channel (RtecEventChannelAdmin::EventChannel::_duplicate (ec)),
     use_callbacks (callbacks)
{
}


int
Task::svc ()
{
  for (int i = 0; i < 10; ++i)
    {
      ACE_TRY_NEW_ENV
        {
          this->run_iteration (ACE_TRY_ENV);
          ACE_TRY_CHECK;
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
Task::run_iteration (CORBA::Environment &ACE_TRY_ENV)
{
  // Obtain the consumer admin..
  RtecEventChannelAdmin::ConsumerAdmin_var consumer_admin =
    this->event_channel->for_consumers (ACE_TRY_ENV);
  ACE_CHECK;

  // and the supplier admin..
  RtecEventChannelAdmin::SupplierAdmin_var supplier_admin =
    this->event_channel->for_suppliers (ACE_TRY_ENV);
  ACE_CHECK;

  // ****************************************************************

  int iterations = 100;

  EC_Counting_Supplier supplier_0;
  EC_Counting_Supplier supplier_1;
  EC_Counting_Consumer consumer_0 ("Consumer/0");
  EC_Counting_Consumer consumer_1 ("Consumer/1");

  const int event_type = 20;
  const int event_source = 10;

  ACE_ConsumerQOS_Factory consumer_qos;
  consumer_qos.start_disjunction_group ();
  consumer_qos.insert (event_source, event_type, 0);

  ACE_SupplierQOS_Factory supplier_qos;
  supplier_qos.insert (event_source,
                       event_type,
                       0, 1);

  for (int i = 0; i != iterations; ++i)
    {
      supplier_0.connect (supplier_admin.in (),
                          supplier_qos.get_SupplierQOS (),
                          ACE_TRY_ENV);
      ACE_CHECK;
      consumer_0.connect (consumer_admin.in (),
                          consumer_qos.get_ConsumerQOS (),
                          ACE_TRY_ENV);
      ACE_CHECK;
      if (i % 2 == 1)
        {
          supplier_1.connect (supplier_admin.in (),
                              supplier_qos.get_SupplierQOS (),
                              ACE_TRY_ENV);
          ACE_CHECK;
          consumer_1.connect (consumer_admin.in (),
                              consumer_qos.get_ConsumerQOS (),
                              ACE_TRY_ENV);
          ACE_CHECK;
        }
      supplier_0.disconnect (ACE_TRY_ENV);
      ACE_CHECK;
      consumer_0.disconnect (ACE_TRY_ENV);
      ACE_CHECK;
      if (i % 2 == 1)
        {
          consumer_1.disconnect (ACE_TRY_ENV);
          ACE_CHECK;
          supplier_1.disconnect (ACE_TRY_ENV);
          ACE_CHECK;
        }
    }

  deactivate_servant (&supplier_0, ACE_TRY_ENV);
  ACE_CHECK;

  deactivate_servant (&consumer_0, ACE_TRY_ENV);
  ACE_CHECK;

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

// ****************************************************************

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
