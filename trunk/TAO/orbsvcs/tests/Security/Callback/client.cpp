#include "ace/Log_Msg.h"
#include "serverC.h"
#include "client_i.h"
#include "ace/SString.h"

ACE_RCSID (Callback,
           client,
           "$Id$")

const char *cert_file = "cacert.pem";

int
main (int argc, char *argv[])
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      ACE_TString env ("SSL_CERT_FILE=");
      env += cert_file;
      ACE_OS::putenv (env.c_str ());

      //
      // Initialize the ORB
      //
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, ""
                         ACE_ENV_ARG_PARAMETER);

      //
      // Get the Root POA.
      //
      CORBA::Object_var obj =
        orb->resolve_initial_references ("RootPOA"
                                         ACE_ENV_ARG_PARAMETER);

      PortableServer::POA_var poa =
        PortableServer::POA::_narrow (obj.in ()
                                      ACE_ENV_ARG_PARAMETER);

      //
      // Get a reference to the server.
      //
      obj = orb->string_to_object ("file://server.ior"
                                   ACE_ENV_ARG_PARAMETER);

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
        server::_narrow (obj.in ()
                         ACE_ENV_ARG_PARAMETER);

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
      server_obj->set_client (client_ref.in ()
                              ACE_ENV_ARG_PARAMETER);

      server_obj->test_request ("first secure callback to client"
                                ACE_ENV_ARG_PARAMETER);

      //
      // Repeat the callback test.
      //
      server_obj->set_client (client_ref.in () ACE_ENV_ARG_PARAMETER);
      server_obj->test_request ("second secure callback to client"
                                ACE_ENV_ARG_PARAMETER);

      server_obj->shutdown ();

      poa->destroy (1, 1
                    ACE_ENV_ARG_PARAMETER);

      orb->destroy ();
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           ACE_TEXT ("Caught exception\n"));

      return -1;
    }
  ACE_ENDTRY;

  return 0;
}
