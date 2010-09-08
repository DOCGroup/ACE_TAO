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
#include "ace/OS_NS_stdio.h"

static const ACE_TCHAR *ior_output_file = 0;
static const ACE_TCHAR *forward_to_ior = 0;

static int
parse_args (int argc, ACE_TCHAR **argv)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("f:o:"));
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
                           "\nusage:  %s\n"
                           "-f forward to IOR\n"
                           "-o output file for IOR\n"
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
setup_poa (PortableServer::POA_ptr root_poa)
{
  // Policies for the childPOA to be created.
  CORBA::PolicyList policies (2);
  policies.length (2);

  // Tell the POA to use a servant manager.
  policies[0] =
    root_poa->create_request_processing_policy (PortableServer::USE_SERVANT_MANAGER);

  // Allow implicit activation.
  policies[1] =
    root_poa->create_implicit_activation_policy (PortableServer::IMPLICIT_ACTIVATION);

  PortableServer::POAManager_var poa_manager =
    root_poa->the_POAManager ();

  // Create POA as child of RootPOA with the above policies.  This POA
  // will use a SERVANT_ACTIVATOR because of RETAIN policy.
  PortableServer::POA_var child_poa =
    root_poa->create_POA ("childPOA",
                          poa_manager.in (),
                          policies);

  // Creation of childPOAs is over. Destroy the Policy objects.
  for (CORBA::ULong i = 0;
       i < policies.length ();
       ++i)
    {
      policies[i]->destroy ();
    }

  return child_poa._retn ();
}

ServantActivator *
create_servant_manager (CORBA::ORB_ptr orb,
                        PortableServer::POA_ptr child_poa)
{
  CORBA::Object_var forward_to;
  if (forward_to_ior)
    {
      forward_to =
        orb->string_to_object (forward_to_ior);
    }

  ServantActivator *activator = 0;
  ACE_NEW_RETURN (activator,
                  ServantActivator (orb,
                                    forward_to.in ()),
                  0);

  // Set ServantActivator to be the servant activator.
  child_poa->set_servant_manager (activator);
  // For the code above, we're using the CORBA 3.0 servant manager
  // semantics supported by TAO.  For CORBA 2.x ORBs you'd need to
  // use the following code in place of the previous line:
  //
  // PortableServer::ServantManager_var servant_activator =
  //   activator->_this ();
  //
  // child_poa->set_servant_manager (servant_activator.in (),
  //);

  test_i *servant = 0;
  ACE_NEW_RETURN (servant,
                  test_i (orb,
                          child_poa,
                          *activator,
                          127),
                  0);

  PortableServer::ServantBase_var servant_var (servant);

  test_var test =
    servant->_this ();

  CORBA::String_var ior =
    orb->object_to_string (test.in ());

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
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      // Initialize the ORB first.
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      int result =
        parse_args (argc, argv);

      if (result == -1)
        return -1;

      // Obtain the RootPOA.
      CORBA::Object_var obj =
        orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (obj.in ());

      // Get the POAManager of the RootPOA.
      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      PortableServer::POA_var child_poa =
        setup_poa (root_poa.in ());

      ServantActivator *manager =
        create_servant_manager (orb.in (),
                                child_poa.in ());

      poa_manager->activate ();

      orb->run ();

      orb->destroy ();

      delete manager;
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught in server");
      return -1;
    }

  return 0;
}
