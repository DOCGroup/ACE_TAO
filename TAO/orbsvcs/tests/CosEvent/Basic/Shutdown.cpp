// $Id$

#include "Counting_Consumer.h"
#include "Counting_Supplier.h"
#include "orbsvcs/CosEvent/CEC_EventChannel.h"
#include "orbsvcs/CosEvent/CEC_Default_Factory.h"

ACE_RCSID(CEC_Tests_Basic, Shutdown, "$Id$")

static void run_test (PortableServer::POA_ptr poa,
                      int with_callbacks
                      TAO_ENV_ARG_DECL);

int
main (int argc, char* argv[])
{
  TAO_CEC_Default_Factory::init_svcs ();

  TAO_ENV_DECLARE_NEW_ENV;
  ACE_TRY
    {
      // ORB initialization boiler plate...
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var object =
        orb->resolve_initial_references ("RootPOA" TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      PortableServer::POA_var poa =
        PortableServer::POA::_narrow (object.in () TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      PortableServer::POAManager_var poa_manager =
        poa->the_POAManager (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      poa_manager->activate (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // ****************************************************************

      run_test (poa.in (), 0 TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      run_test (poa.in (), 1 TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // ****************************************************************

      poa->destroy (1, 1 TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      orb->destroy (TAO_ENV_SINGLE_ARG_PARAMETER);
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

void
deactivate_servant (PortableServer::Servant servant
                    TAO_ENV_ARG_DECL)
{
  PortableServer::POA_var poa =
    servant->_default_POA (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  PortableServer::ObjectId_var id =
    poa->servant_to_id (servant TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
  poa->deactivate_object (id.in () TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

static void
run_test (PortableServer::POA_ptr poa,
          int with_callbacks
          TAO_ENV_ARG_DECL)
{
  TAO_CEC_EventChannel_Attributes attributes (poa,
                                              poa);
  attributes.disconnect_callbacks = with_callbacks;

  TAO_CEC_EventChannel ec_impl (attributes);
  ec_impl.activate (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CosEventChannelAdmin::EventChannel_var event_channel =
    ec_impl._this (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // ****************************************************************

  // Obtain the consumer admin..
  CosEventChannelAdmin::ConsumerAdmin_var consumer_admin =
    event_channel->for_consumers (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // Obtain the supplier admin..
  CosEventChannelAdmin::SupplierAdmin_var supplier_admin =
    event_channel->for_suppliers (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // ****************************************************************

  CEC_Counting_Consumer **consumer;
  CEC_Counting_Supplier **supplier;

  const int n = 200;
  ACE_NEW (consumer, CEC_Counting_Consumer*[n]);
  ACE_NEW (supplier, CEC_Counting_Supplier*[n]);

  int i;
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
      consumer[i]->connect (consumer_admin.in () TAO_ENV_ARG_PARAMETER);
      ACE_CHECK;

      supplier[i]->connect (supplier_admin.in () TAO_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }

  // ****************************************************************

  // Destroy the event channel, *before* disconnecting the
  // clients.
  event_channel->destroy (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

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
      deactivate_servant (supplier[i] TAO_ENV_ARG_PARAMETER);
      ACE_CHECK;
      delete supplier[i];

      deactivate_servant (consumer[i] TAO_ENV_ARG_PARAMETER);
      ACE_CHECK;
      delete consumer[i];
    }
  delete[] supplier;
  delete[] consumer;

  deactivate_servant (&ec_impl TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
