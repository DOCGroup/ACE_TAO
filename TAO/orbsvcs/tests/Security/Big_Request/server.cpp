
#include "TX_Object_i.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"
#include "ace/SString.h"

ACE_RCSID (Big_Request,
           server,
           "$Id$")

const char *ior_output_file = 0;
const char *cert_file = "cacert.pem";

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "o:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Usage:  %s "
                           "-o <iorfile>"
                           "\n",
                           argv [0]),
                          -1);
      }

  // Indicates sucessful parsing of the command line
  return 0;
}


int
main (int argc, char * argv[])
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      ACE_TString env ("SSL_CERT_FILE=");
      env += cert_file;
      ACE_OS::putenv (env.c_str ());

      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);

      CORBA::Object_var poaObj =
        orb->resolve_initial_references ("RootPOA" ACE_ENV_ARG_PARAMETER);

      PortableServer::POA_var rootPoa =
        PortableServer::POA::_narrow (poaObj.in () ACE_ENV_ARG_PARAMETER);

      PortableServer::POAManager_var poa_manager =
        rootPoa->the_POAManager ();

      poa_manager->activate ();

      if (::parse_args (argc, argv) != 0)
        return 1;

      TX_Object_i implObject (orb.in ());

      TX_Object_var txObject = implObject._this ();

      CORBA::String_var ior =
        orb->object_to_string (txObject.in () ACE_ENV_ARG_PARAMETER);

      ACE_DEBUG ((LM_DEBUG, "Activated as <%s>\n", ior.in ()));

      // If the ior_output_file exists, output the ior to it.
      if (ior_output_file != 0)
        {
          FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
          if (output_file == 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Cannot open output file for writing IOR: %s\n",
                               ior_output_file),
                              1);
          ACE_OS::fprintf (output_file, "%s", ior.in ());
          ACE_OS::fclose (output_file);
        }

      orb->run ();

      ACE_DEBUG ((LM_DEBUG,
                  "\n"
                  "Event loop finished.\n"));

      rootPoa->destroy (1, 1 ACE_ENV_ARG_PARAMETER);

      orb->destroy ();
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "ERROR");

      return -1;
    }
  ACE_ENDTRY;


  return 0;
}
