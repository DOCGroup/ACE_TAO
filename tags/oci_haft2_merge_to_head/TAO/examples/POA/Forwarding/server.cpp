// $Id$

// ========================================================================
// = LIBRARY
//    TAO/examples/POA/Forwarding
//
// = FILENAME
//    server.cpp
//
// = DESCRIPTION
//    Server that receives requests.  The server can be asked to
//    forward new requests to another server.
//
// = AUTHOR
//    Irfan Pyarali
//    Michael Kircher
//
// ========================================================================

#include "ace/Get_Opt.h"
#include "test_i.h"
#include "Servant_Activator.h"

ACE_RCSID(Forwarding, server, "$Id$")

static const char *ior_output_file = 0;
static const char *forward_to_ior = 0;

static int
parse_args (int argc, char **argv)
{
  ACE_Get_Opt get_opts (argc, argv, "f:o:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'f':
        forward_to_ior = get_opts.opt_arg ();
        break;

      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "\nusage:  %s \n"
                           "-f forward to IOR \n"
                           "-o output file for IOR \n"
                           "\n",
                           argv [0]),
                          -1);
      }

  if (ior_output_file == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "output IOR file not specified\n"),
                      -1);

  // Indicates successful parsing of command line.
  return 0;
}

PortableServer::POA_ptr
setup_poa (PortableServer::POA_ptr root_poa
           ACE_ENV_ARG_DECL)
{
  // Policies for the childPOA to be created.
  CORBA::PolicyList policies (2);
  policies.length (2);

  // Tell the POA to use a servant manager.
  policies[0] =
    root_poa->create_request_processing_policy (PortableServer::USE_SERVANT_MANAGER
                                                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (PortableServer::POA::_nil ());

  // Allow implicit activation.
  policies[1] =
    root_poa->create_implicit_activation_policy (PortableServer::IMPLICIT_ACTIVATION
                                                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (PortableServer::POA::_nil ());

  PortableServer::POAManager_var poa_manager =
    root_poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (PortableServer::POA::_nil ());

  // Create POA as child of RootPOA with the above policies.  This POA
  // will use a SERVANT_ACTIVATOR because of RETAIN policy.
  PortableServer::POA_var child_poa =
    root_poa->create_POA ("childPOA",
                          poa_manager.in (),
                          policies
                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (PortableServer::POA::_nil ());

  // Creation of childPOAs is over. Destroy the Policy objects.
  for (CORBA::ULong i = 0;
       i < policies.length ();
       ++i)
    {
      policies[i]->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (PortableServer::POA::_nil ());
    }

  return child_poa._retn ();
}

ServantActivator *
create_servant_manager (CORBA::ORB_ptr orb,
                        PortableServer::POA_ptr child_poa
                        ACE_ENV_ARG_DECL)
{
  CORBA::Object_var forward_to;
  if (forward_to_ior)
    {
      forward_to =
        orb->string_to_object (forward_to_ior
                               ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);
    }

  ServantActivator *activator = 0;
  ACE_NEW_RETURN (activator,
                  ServantActivator (orb,
                                    forward_to.in ()),
                  0);

  // Set ServantActivator to be the servant activator.
  child_poa->set_servant_manager (activator
                                  ACE_ENV_ARG_PARAMETER);
  // For the code above, we're using the CORBA 3.0 servant manager
  // semantics supported by TAO.  For CORBA 2.x ORBs you'd need to
  // use the following code in place of the previous line:
  //
  // PortableServer::ServantManager_var servant_activator =
  //   activator->_this ();
  //
  // child_poa->set_servant_manager (servant_activator.in (),
  //                                 ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  test_i *servant = 0;
  ACE_NEW_RETURN (servant,
                  test_i (orb,
                          child_poa,
                          *activator,
                          127),
                  0);

  PortableServer::ServantBase_var servant_var (servant);

  test_var test =
    servant->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  CORBA::String_var ior =
    orb->object_to_string (test.in ()
                           ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  FILE *output_file = ACE_OS::fopen (ior_output_file, "w");
  if (output_file == 0)
    ACE_ERROR ((LM_ERROR,
                "Cannot open output file for writing IOR: %s\n",
                ior_output_file));
  ACE_OS::fprintf (output_file,
                   "%s",
                   ior.in ());
  ACE_OS::fclose (output_file);

  return activator;
}

int
main (int argc,
      char **argv)
{
  int result =
    parse_args (argc, argv);

  if (result == -1)
    return -1;

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // Initialize the ORB first.
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc,
                         argv,
                         0
                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Obtain the RootPOA.
      CORBA::Object_var obj =
        orb->resolve_initial_references ("RootPOA"
                                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (obj.in ()
                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Get the POAManager of the RootPOA.
      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POA_var child_poa =
        setup_poa (root_poa.in ()
                   ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::ServantManager_var manager =
        create_servant_manager (orb.in (),
                                child_poa.in ()
                                ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      orb->run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Exception caught");
      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  return 0;
}
