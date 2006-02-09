// -*- C++ -*-

#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "Foo_i.h"
#include "ace/SString.h"
#include "orbsvcs/SecurityC.h"

ACE_RCSID (Secure_Invocation,
           server,
           "$Id$")

const char *ior_output_file = 0;
const char *cert_file = "cacert.pem";
bool protection_required = false;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "o:s:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;
      case 's':
        protection_required = (get_opts.opt_arg () [0] == '1');
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Usage:  %s "
                           "-o <iorfile> "
                           "[-s 0|1] "
                           "\n",
                           argv [0]),
                          -1);
      }

  // Indicates sucessful parsing of the command line
  return 0;
}



PortableServer::POA_ptr
create_child_poa (CORBA::ORB_ptr orb,
                  PortableServer::POA_ptr poa,
                  Security::QOP qop
                  ACE_ENV_ARG_DECL)
{
  CORBA::Any policy_value;
  policy_value <<= qop;

  // Create the Security::QOPPolicy.
  CORBA::Policy_var policy =
    orb->create_policy (Security::SecQOPPolicy,
                        policy_value
                        ACE_ENV_ARG_PARAMETER);
  ACE_TRY_CHECK;

  CORBA::PolicyList policy_list (1);
  policy_list.length (1);
  policy_list[0] = CORBA::Policy::_duplicate (policy.in ());

  PortableServer::POAManager_var poa_manager =
    poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_TRY_CHECK;


  PortableServer::POA_var child_poa =
    poa->create_POA ("childPOA",
                     poa_manager.in (),
                     policy_list
                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  policy_list[0]->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_TRY_CHECK;

  return child_poa._retn ();
}

int
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      ACE_TString env ("SSL_CERT_FILE=");
      env += cert_file;
      ACE_OS::putenv (env.c_str ());

      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var obj =
        orb->resolve_initial_references ("RootPOA" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (obj.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) ERROR: Unable to initialize the "
                           "POA.\n"),
                          1);

      PortableServer::POA_var rootp =
        PortableServer::POA::_narrow (obj.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;


      if (parse_args (argc, argv) != 0)
        return 1;

      PortableServer::POA_var poa =
        create_child_poa (orb.in (),
                          rootp.in (),
                          protection_required ? ::Security::SecQOPIntegrityAndConfidentiality
                                              : ::Security::SecQOPNoProtection);

      obj =
        orb->resolve_initial_references ("SecurityLevel3:SecurityCurrent"
                                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      SecurityLevel3::SecurityCurrent_var security_current =
        SecurityLevel3::SecurityCurrent::_narrow (obj.in ()
                                                  ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (security_current.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) ERROR: SecurityCurrent reference "
                           "is nil.\n"),
                          1);

      Foo_i *server_impl = 0;

      ACE_NEW_RETURN (server_impl,
                      Foo_i (orb.in (),
                             security_current.in ()),
                      -1);

      PortableServer::ServantBase_var owner_transfer (server_impl);

      PortableServer::ObjectId_var server_id =
        poa->activate_object (server_impl ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var server =
        poa->id_to_reference (server_id.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::String_var ior =
        orb->object_to_string (server.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) Activated as <%s>\n", ior.in ()));

      // If the ior_output_file exists, output the ior to it
      if (ior_output_file != 0)
        {
          FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
          if (output_file == 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "(%P|%t) Cannot open output file for writing IOR: %s",
                               ior_output_file),
                              1);
          ACE_OS::fprintf (output_file, "%s", ior.in ());
          ACE_OS::fclose (output_file);
        }

      PortableServer::POAManager_var poa_manager =
        poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      orb->run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) Event loop finished.\n"));

      poa->destroy (1, 1 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      rootp->destroy (1, 1 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Caught server-side exception:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
