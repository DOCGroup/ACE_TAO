// $Id$

#include "Counting_Supplier.h"
#include "Counting_Consumer.h"

#include "orbsvcs/Time_Utilities.h"
#include "orbsvcs/CosEvent/CEC_EventChannel.h"
#include "orbsvcs/CosEvent/CEC_Default_Factory.h"

ACE_RCSID(CEC_Tests, Disconnect, "$Id$")

static void run_test (PortableServer::POA_ptr poa,
                      int use_callbacks
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

// ****************************************************************

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

void
run_test (PortableServer::POA_ptr poa,
          int use_callbacks
          TAO_ENV_ARG_DECL)
{
  TAO_CEC_EventChannel_Attributes attributes (poa, poa);
  attributes.disconnect_callbacks = use_callbacks;

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

  // and the supplier admin..
  CosEventChannelAdmin::SupplierAdmin_var supplier_admin =
    event_channel->for_suppliers (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // ****************************************************************

  int iterations = 100;

  CEC_Counting_Supplier supplier_0;
  CEC_Counting_Supplier supplier_1;
  CEC_Counting_Consumer consumer_0 ("Consumer/0");
  CEC_Counting_Consumer consumer_1 ("Consumer/1");

  for (int i = 0; i != iterations; ++i)
    {
      supplier_0.connect (supplier_admin.in ()
                          TAO_ENV_ARG_PARAMETER);
      ACE_CHECK;
      consumer_0.connect (consumer_admin.in ()
                          TAO_ENV_ARG_PARAMETER);
      ACE_CHECK;
      if (i % 2 == 1)
        {
          supplier_1.connect (supplier_admin.in ()
                              TAO_ENV_ARG_PARAMETER);
          ACE_CHECK;
          consumer_1.connect (consumer_admin.in ()
                              TAO_ENV_ARG_PARAMETER);
          ACE_CHECK;
        }
      supplier_0.disconnect (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
      consumer_0.disconnect (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
      if (i % 2 == 1)
        {
          consumer_1.disconnect (TAO_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK;
          supplier_1.disconnect (TAO_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK;
        }
    }

  supplier_0.connect (supplier_admin.in ()
                      TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
  consumer_0.connect (consumer_admin.in ()
                      TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  event_channel->destroy (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  deactivate_servant (&supplier_0 TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  deactivate_servant (&consumer_0 TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  deactivate_servant (&ec_impl TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

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

// ****************************************************************

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
