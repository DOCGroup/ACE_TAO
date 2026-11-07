#include "MT_Disconnect.h"
#include "Counting_Consumer.h"
#include "Counting_Supplier.h"

#include "orbsvcs/CosEvent/CEC_EventChannel.h"
#include "orbsvcs/CosEvent/CEC_Default_Factory.h"

static void run_test (PortableServer::POA_ptr poa,
                      bool use_callbacks);
static void deactivate_servant (PortableServer::Servant servant);

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

      run_test (poa.in (), false);

      run_test (poa.in (), true);

      // ****************************************************************

      poa->destroy (true, true);

      orb->destroy ();
    }
  catch (CORBA::Exception const& ex)
    {
      ex._tao_print_exception ("Service");
      return 1;
    }
  return 0;
}

// ****************************************************************

static void
deactivate_servant (PortableServer::Servant servant)
{
  PortableServer::POA_var poa =
    servant->_default_POA ();
  PortableServer::ObjectId_var id =
    poa->servant_to_id (servant);
  poa->deactivate_object (id.in ());
}

void
run_test (PortableServer::POA_ptr poa,
          bool use_callbacks)
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

  deactivate_servant (&ec_impl);
}

MTD_Task::MTD_Task (CosEventChannelAdmin::EventChannel_ptr ec,
                    bool callbacks)
  : event_channel (CosEventChannelAdmin::EventChannel::_duplicate (ec)),
    use_callbacks_ (callbacks)
{
}

int
MTD_Task::svc ()
{
  for (int i {}; i < 10; ++i)
    {
      try
        {
          this->run_iteration ();
        }
      catch (CORBA::Exception const&)
        {
          return -1;
        }
    }
  return 0;
}

void
MTD_Task::run_iteration ()
{
  // Obtain the consumer admin..
  CosEventChannelAdmin::ConsumerAdmin_var consumer_admin =
    this->event_channel->for_consumers ();

  // and the supplier admin..
  CosEventChannelAdmin::SupplierAdmin_var supplier_admin =
    this->event_channel->for_suppliers ();

  // ****************************************************************

  constexpr int iterations {100};

  CEC_Counting_Supplier supplier_0;
  CEC_Counting_Supplier supplier_1;
  CEC_Counting_Consumer consumer_0 ("Consumer/0");
  CEC_Counting_Consumer consumer_1 ("Consumer/1");

  for (int i {}; i != iterations; ++i)
    {
      supplier_0.connect (supplier_admin.in ());
      consumer_0.connect (consumer_admin.in ());
      if (i % 2 == 1)
        {
          supplier_1.connect (supplier_admin.in ());
          consumer_1.connect (consumer_admin.in ());
        }
      supplier_0.disconnect ();
      consumer_0.disconnect ();
      if (i % 2 == 1)
        {
          consumer_1.disconnect ();
          supplier_1.disconnect ();
        }
    }

  deactivate_servant (&supplier_0);

  deactivate_servant (&consumer_0);

  CORBA::ULong count_0 {};
  CORBA::ULong count_1 {};
  if (this->use_callbacks_)
    {
      count_0 += iterations;
      count_1 += iterations / 2;
    }

  if (consumer_0.disconnect_count != count_0)
    ACE_ERROR ((LM_ERROR,
                "ERROR: incorrect number of disconnect calls (%d/%d) for "
                "consumer 0 (%d)\n",
                consumer_0.disconnect_count, count_0,
                this->use_callbacks_));
  if (supplier_0.disconnect_count != count_0)
    ACE_ERROR ((LM_ERROR,
                "ERROR: incorrect number of disconnect calls (%d/%d) for "
                "supplier 0 (%d)\n",
                supplier_0.disconnect_count, count_0,
                this->use_callbacks_));

  if (consumer_1.disconnect_count != count_1)
    ACE_ERROR ((LM_ERROR,
                "ERROR: incorrect number of disconnect calls (%d/%d) for "
                "consumer 1 (%d)\n",
                consumer_1.disconnect_count, count_1,
                this->use_callbacks_));
  if (supplier_1.disconnect_count != count_1)
    ACE_ERROR ((LM_ERROR,
                "ERROR: incorrect number of disconnect calls (%d/%d) for "
                "supplier 1 (%d)\n",
                supplier_1.disconnect_count, count_1,
                this->use_callbacks_));
}
