#include "ace/Log_Msg.h"
#include "ace/streams.h"
#include "serverC.h"
#include "client_i.h"


ACE_RCSID (Callback,
           client,
           "$Id$")


int
main (int argc, char *argv[])
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      //
      // Initialize the ORB
      //
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, ""
                         TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      //
      // Get the Root POA.
      //
      CORBA::Object_var obj =
        orb->resolve_initial_references ("RootPOA"
                                         TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POA_var poa =
        PortableServer::POA::_narrow (obj.in () 
                                      TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      //
      // Get a reference to the server.
      //
      obj = orb->string_to_object ("file://server.ior"
                                   TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

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
                         TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

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

      client_var client_ref = servant->_this (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      //
      // Activate the POA manager.
      //
      PortableServer::POAManager_var mgr =
        poa->the_POAManager (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      mgr->activate (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      //
      // Set the server's callback and invoke the test request.
      //
      server_obj->set_client (client_ref.in ()
                              TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      server_obj->test_request ("first secure callback to client"
                                TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      //
      // Repeat the callback test.
      //
      server_obj->set_client (client_ref.in () TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      server_obj->test_request ("second secure callback to client"
                                TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      server_obj->shutdown (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      poa->destroy (1, 1
                    TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      orb->destroy (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
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
