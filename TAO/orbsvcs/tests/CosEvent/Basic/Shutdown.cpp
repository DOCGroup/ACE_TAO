#include "Counting_Consumer.h"
#include "Counting_Supplier.h"
#include "orbsvcs/CosEvent/CEC_EventChannel.h"
#include "orbsvcs/CosEvent/CEC_Default_Factory.h"

#include <array>
#include <cstdio>

static void run_test (PortableServer::POA_ptr poa,
                      bool with_callbacks);
static void deactivate_servant (PortableServer::Servant servant);

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  TAO_CEC_Default_Factory::init_svcs ();

  try
    {
      // ORB initialization boiler plate...
      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var const object =
        orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var const poa =
        PortableServer::POA::_narrow (object.in ());
      PortableServer::POAManager_var const poa_manager =
        poa->the_POAManager ();
      poa_manager->activate ();

      run_test (poa.in (), false);

      run_test (poa.in (), true);

      poa->destroy (true, true);

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Service");
      return 1;
    }
  return 0;
}

static void
deactivate_servant (PortableServer::Servant servant)
{
  PortableServer::POA_var const poa =
    servant->_default_POA ();
  PortableServer::ObjectId_var const id =
    poa->servant_to_id (servant);
  poa->deactivate_object (id.in ());
}

static void
run_test (PortableServer::POA_ptr poa,
          bool with_callbacks)
{
  TAO_CEC_EventChannel_Attributes attributes (poa,
                                              poa);
  attributes.disconnect_callbacks = with_callbacks;

  TAO_CEC_EventChannel ec_impl (attributes);
  ec_impl.activate ();

  CosEventChannelAdmin::EventChannel_var const event_channel =
    ec_impl._this ();

  // Obtain the consumer admin..
  CosEventChannelAdmin::ConsumerAdmin_var const consumer_admin =
    event_channel->for_consumers ();

  // Obtain the supplier admin..
  CosEventChannelAdmin::SupplierAdmin_var const supplier_admin =
    event_channel->for_suppliers ();

  constexpr int n {200};
  // Servants are manually owned until they are deactivated from the POA.
  std::array<CEC_Counting_Consumer *, n> consumers {};
  std::array<CEC_Counting_Supplier *, n> suppliers {};

  for (int i {}; i != n; ++i)
    {
      std::array<char, 64> consumer_name {};
      std::snprintf (consumer_name.data (), consumer_name.size (),
                     "Consumer/%4.4d", i);
      ACE_NEW (consumers[i],
               CEC_Counting_Consumer (consumer_name.data ()));

      ACE_NEW (suppliers[i], CEC_Counting_Supplier ());
    }

  for (int i {}; i != n; ++i)
    {
      consumers[i]->connect (consumer_admin.in ());

      suppliers[i]->connect (supplier_admin.in ());
    }

  // Destroy the event channel, *before* disconnecting the
  // clients.
  event_channel->destroy ();

  for (int i {}; i != n; ++i)
    {
      if (consumers[i]->disconnect_count != 1)
        ACE_DEBUG ((LM_DEBUG,
                    "ERRROR unexpected "
                    "disconnect count (%u) in Consumer/%04.4d\n",
                    consumers[i]->disconnect_count,
                    i));

      if (suppliers[i]->disconnect_count != 1)
        ACE_DEBUG ((LM_DEBUG,
                    "ERRROR unexpected "
                    "disconnect count (%u) in Supplier/%04.4d\n",
                    suppliers[i]->disconnect_count,
                    i));
    }

  for (int i {}; i != n; ++i)
    {
      deactivate_servant (suppliers[i]);
      delete suppliers[i];

      deactivate_servant (consumers[i]);
      delete consumers[i];
    }

  deactivate_servant (&ec_impl);
}
