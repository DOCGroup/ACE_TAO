/**
 * @file client.cpp
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

#include "Peer.h"
#include "Servant_var.h"

#include "orbsvcs/Event/EC_Default_Factory.h"

#include "tao/Messaging.h"

#include "ace/Get_Opt.h"
#include "ace/Sched_Params.h"

ACE_RCSID(EC_Federated_Scalability, client, "$Id$")

const char *ior = "file://server.ior";

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "k:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <IOR> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

int main (int argc, char *argv[])
{
  TAO_EC_Default_Factory::init_svcs ();

  int priority =
    (ACE_Sched_Params::priority_min (ACE_SCHED_FIFO)
     + ACE_Sched_Params::priority_max (ACE_SCHED_FIFO)) / 2;
  priority = ACE_Sched_Params::next_priority (ACE_SCHED_FIFO,
                                              priority);
  // Enable FIFO scheduling, e.g., RT scheduling class on Solaris.

  if (ACE_OS::sched_params (ACE_Sched_Params (ACE_SCHED_FIFO,
                                              priority,
                                              ACE_SCOPE_PROCESS)) != 0)
    {
      if (ACE_OS::last_error () == EPERM)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "client (%P|%t): user is not superuser, "
                      "test runs in time-shared class\n"));
        }
      else
        ACE_ERROR ((LM_ERROR,
                    "client (%P|%t): sched_params failed\n"));
    }

  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return 1;

      // Get the event channel object reference
      CORBA::Object_var coordinator_object =
        orb->string_to_object (ior TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Control::Coordinator_var coordinator =
        Control::Coordinator::_narrow (coordinator_object.in ()
                                       TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (coordinator.in ()))
        {
          ACE_ERROR ((LM_ERROR,
                      "(%P|%t) Invalid or nil coordinator\n"));
          return 1;
        }

      CORBA::Object_var manager_object =
        orb->resolve_initial_references ("ORBPolicyManager"
                                         TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::PolicyManager_var policy_manager =
        CORBA::PolicyManager::_narrow (manager_object.in ()
                                       TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Any sync_scope;
      sync_scope <<= Messaging::SYNC_WITH_TARGET;

      CORBA::PolicyList policy_list (1);
      policy_list.length (1);
      policy_list[0] =
        orb->create_policy (Messaging::SYNC_SCOPE_POLICY_TYPE,
                            sync_scope
                            TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      policy_manager->set_policy_overrides (policy_list,
                                            CORBA::SET_OVERRIDE
                                            TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

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

      poa_manager->activate (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      poa_manager->activate (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Servant_var<ECFS_Peer> peer_impl (new ECFS_Peer (orb.in ()));

      peer_impl->init (root_poa.in () TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Control::Peer_var peer (peer_impl->_this (TAO_ENV_SINGLE_ARG_PARAMETER));
      ACE_TRY_CHECK;

      coordinator->join (peer.in () TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      orb->run (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - event loop finished\n"));

      orb->destroy (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception caught:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class Servant_var<ECFS_Peer>;

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate Servant_var<ECFS_Peer>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
