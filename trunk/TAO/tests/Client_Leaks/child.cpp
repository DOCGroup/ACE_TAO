// $Id$

#include "Process.h"
#include "ace/Get_Opt.h"

ACE_RCSID(Client_Leaks, server, "$Id$")

int
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (argc < 2)
        {
          // Paranoia, we should have an auto_ptr-like gadget for
          // this.
          orb->destroy (TAO_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          ACE_ERROR_RETURN ((LM_ERROR,
                             "Usage: %s <ior>\n",
                             argv[0]), 1);
        }

      CORBA::Object_var object =
        orb->string_to_object (argv[1] TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      Test::Startup_Callback_var startup_callback =
        Test::Startup_Callback::_narrow (object.in () TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      if (CORBA::is_nil (startup_callback.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "ERROR: nil startup callback\n"),
                            1);
        }

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA" TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in () TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil RootPOA\n"),
                          1);

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Process *process_impl;
      ACE_NEW_RETURN (process_impl,
                      Process (orb.in ()),
                      1);
      PortableServer::ServantBase_var owner_transfer(process_impl);

      Test::Process_var process =
        process_impl->_this (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      poa_manager->activate (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      startup_callback->started (process.in () TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_Time_Value tv (50, 0);
      orb->run (tv TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      root_poa->destroy (1, 1 TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      orb->destroy (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      // Do not print error messages, they only make the test output
      // confusing.
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
