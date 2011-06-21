// $Id$

#include "Counting_Supplier.h"
#include "Counting_Consumer.h"

#include "orbsvcs/Time_Utilities.h"
#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/Event/EC_Event_Channel.h"
#include "orbsvcs/Event/EC_Default_Factory.h"

static void run_test (PortableServer::POA_ptr poa,
                      int use_callbacks);

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

      run_test (poa.in (), 0);

      run_test (poa.in (), 1);

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

// ****************************************************************

void
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
          int use_callbacks)
{
  TAO_EC_Event_Channel_Attributes attributes (poa, poa);
  attributes.disconnect_callbacks = use_callbacks;

  TAO_EC_Event_Channel ec_impl (attributes);
  ec_impl.activate ();

  RtecEventChannelAdmin::EventChannel_var event_channel =
    ec_impl._this ();

  // ****************************************************************

  // Obtain the consumer admin..
  RtecEventChannelAdmin::ConsumerAdmin_var consumer_admin =
    event_channel->for_consumers ();

  // and the supplier admin..
  RtecEventChannelAdmin::SupplierAdmin_var supplier_admin =
    event_channel->for_suppliers ();

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
                          supplier_qos.get_SupplierQOS ());
      consumer_0.connect (consumer_admin.in (),
                          consumer_qos.get_ConsumerQOS ());
      if (i % 2 == 1)
        {
          supplier_1.connect (supplier_admin.in (),
                              supplier_qos.get_SupplierQOS ());
          consumer_1.connect (consumer_admin.in (),
                              consumer_qos.get_ConsumerQOS ());
        }
      supplier_0.disconnect ();
      consumer_0.disconnect ();
      if (i % 2 == 1)
        {
          consumer_1.disconnect ();
          supplier_1.disconnect ();
        }
    }

  supplier_0.connect (supplier_admin.in (),
                      supplier_qos.get_SupplierQOS ());
  consumer_0.connect (consumer_admin.in (),
                      consumer_qos.get_ConsumerQOS ());

  event_channel->destroy ();

  deactivate_servant (&supplier_0);

  deactivate_servant (&consumer_0);

  deactivate_servant (&ec_impl);

  CORBA::ULong count_0 = 1;
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
