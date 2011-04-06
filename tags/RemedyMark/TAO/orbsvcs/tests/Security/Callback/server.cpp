// $Id$

#include "ace/Log_Msg.h"
#include "server_i.h"
#include "ace/OS_NS_stdio.h"
#include "ace/SString.h"
#include "ace/Get_Opt.h"

const ACE_TCHAR *ior_output_file = ACE_TEXT ("server.ior");
const ACE_TCHAR *cert_file = ACE_TEXT("cacert.pem");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:"));
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
                           "usage:  %s "
                           "-o <iorfile>"
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      ACE_TString env (ACE_TEXT("SSL_CERT_FILE="));
      env += cert_file;
      ACE_OS::putenv ( ACE_TEXT_ALWAYS_CHAR(env.c_str ()) );

      //
      // Initialize the ORB
      //
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      //
      // Get the Root POA.
      //
      CORBA::Object_var obj =
        orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var poa =
        PortableServer::POA::_narrow (obj.in ());

      //
      // Create the server, get object reference,
      // and create "stringified" IOR.
      //
      server_i *tmp = 0;
      ACE_NEW_RETURN (tmp,
                      server_i (orb.in ()),
                      -1);
      PortableServer::ServantBase_var theServer = tmp;

      PortableServer::ObjectId_var oid =
        poa->activate_object (theServer.in ());
      CORBA::Object_var server_obj =
        poa->id_to_reference (oid.in ());
      CORBA::String_var server_IORString =
        orb->object_to_string (server_obj.in ());

      //
      // Write the IOR to a file.
      //
      // Output the IOR to the <ior_output_file>
      FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file for writing IOR\n"),
                            -1);
      ACE_OS::fprintf (output_file, "%s", server_IORString.in ());
      ACE_OS::fclose (output_file);

      //
      // Activate the POA manager.
      //
      PortableServer::POAManager_var mgr =
        poa->the_POAManager ();
      mgr->activate ();

      ACE_DEBUG ((LM_INFO,
                  ACE_TEXT ("(%P) calling orb->run () ...\n")));

      orb->run ();

      poa->destroy (1, 1);

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (ACE_TEXT ("Caught exception\n"));

      return -1;
    }

  return 0;
}
