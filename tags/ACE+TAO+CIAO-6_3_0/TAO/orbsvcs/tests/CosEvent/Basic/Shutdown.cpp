// $Id$

#include "Counting_Consumer.h"
#include "Counting_Supplier.h"
#include "orbsvcs/CosEvent/CEC_EventChannel.h"
#include "orbsvcs/CosEvent/CEC_Default_Factory.h"

static void run_test (PortableServer::POA_ptr poa,
                      int with_callbacks);

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

void
deactivate_servant (PortableServer::Servant servant)
{
  PortableServer::POA_var poa =
    servant->_default_POA ();
  PortableServer::ObjectId_var id =
    poa->servant_to_id (servant);
  poa->deactivate_object (id.in ());
}

static void
run_test (PortableServer::POA_ptr poa,
          int with_callbacks)
{
  TAO_CEC_EventChannel_Attributes attributes (poa,
                                              poa);
  attributes.disconnect_callbacks = with_callbacks;

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

  CEC_Counting_Consumer **consumer = 0;
  CEC_Counting_Supplier **supplier = 0;

  const int n = 200;
  ACE_NEW (consumer, CEC_Counting_Consumer*[n]);
  ACE_NEW (supplier, CEC_Counting_Supplier*[n]);

  int i = 0;
  for (i = 0; i != n; ++i)
    {
      char consumer_name[64];
      ACE_OS::sprintf (consumer_name, "Consumer/%4.4d", i);
      ACE_NEW (consumer[i],
               CEC_Counting_Consumer (consumer_name));

      ACE_NEW (supplier[i], CEC_Counting_Supplier ());
    }

  for (i = 0; i != n; ++i)
    {
      consumer[i]->connect (consumer_admin.in ());

      supplier[i]->connect (supplier_admin.in ());
    }

  // ****************************************************************

  // Destroy the event channel, *before* disconnecting the
  // clients.
  event_channel->destroy ();

  // ****************************************************************

  for (i = 0; i != n; ++i)
    {
      if (consumer[i]->disconnect_count != 1)
        ACE_DEBUG ((LM_DEBUG,
                    "ERRROR unexpected "
                    "disconnect count (%d) in Consumer/%04.4d\n",
                    consumer[i]->disconnect_count,
                    i));

      if (supplier[i]->disconnect_count != 1)
        ACE_DEBUG ((LM_DEBUG,
                    "ERRROR unexpected "
                    "disconnect count (%d) in Supplier/%04.4d\n",
                    supplier[i]->disconnect_count,
                    i));
    }

  // ****************************************************************

  for (i = 0; i != n; ++i)
    {
      deactivate_servant (supplier[i]);
      delete supplier[i];

      deactivate_servant (consumer[i]);
      delete consumer[i];
    }
  delete[] supplier;
  delete[] consumer;

  deactivate_servant (&ec_impl);
}
