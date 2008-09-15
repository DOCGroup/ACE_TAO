#include "ace/Log_Msg.h"
#include "serverC.h"
#include "client_i.h"
#include "ace/SString.h"

ACE_RCSID (Callback,
           client,
           "$Id$")

const char *cert_file = "cacert.pem";

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      ACE_TString env ("SSL_CERT_FILE=");
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
      // Get a reference to the server.
      //
      obj = orb->string_to_object ("file://server.ior"
                                   );

      if (CORBA::is_nil (obj.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("(%P) orb->string_to_object ")
                             ACE_TEXT ("(\"file://server.ior\") failed.\n")),
                            -1);
        }

      //
      // Downcast the IOR to the appropriate object type.
      //
      server_var server_obj =
        server::_narrow (obj.in ());

      if (CORBA::is_nil (server_obj.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("(%P) server::_narrow(obj) failed.\n")),
                            -1);
        }

      //
      // Create and activate the client.
      //
      client_i *servant = 0;
      ACE_NEW_RETURN (servant,
                      client_i (server_obj.in ()),
                      -1);
      PortableServer::ServantBase_var theClient = servant;

      client_var client_ref = servant->_this ();

      //
      // Activate the POA manager.
      //
      PortableServer::POAManager_var mgr =
        poa->the_POAManager ();

      mgr->activate ();

      //
      // Set the server's callback and invoke the test request.
      //
      server_obj->set_client (client_ref.in ());

      server_obj->test_request ("first secure callback to client");

      //
      // Repeat the callback test.
      //
      server_obj->set_client (client_ref.in ());
      server_obj->test_request ("second secure callback to client");

      server_obj->shutdown ();

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
