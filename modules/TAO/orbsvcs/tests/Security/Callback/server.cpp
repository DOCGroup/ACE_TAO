#include "ace/Log_Msg.h"
#include "server_i.h"
#include "ace/OS_NS_stdio.h"
#include "ace/SString.h"

ACE_RCSID (Callback,
           server,
           "$Id$")

const char *cert_file = "cacert.pem";

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      ACE_CString env ("SSL_CERT_FILE=");
      env += cert_file;
      ACE_OS::putenv (env.c_str ());

      //
      // Initialize the ORB
      //
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

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
      FILE *output_file= ACE_OS::fopen ("server.ior", "w");
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
