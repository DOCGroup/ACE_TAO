// $Id$

#include "Sender.h"
#include "Receiver.h"
#include "ace/Get_Opt.h"

ACE_RCSID(LongWrites, client, "$Id$")

const char *ior = "file://test.ior";

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "k:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
	ior = get_opts.optarg;
	break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
			   "-k <ior>"
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

int
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "", ACE_TRY_ENV);
      ACE_TRY_CHECK;

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA", ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var tmp =
        orb->string_to_object(ior, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      Test::Coordinator_var coordinator =
        Test::Coordinator::_narrow(tmp.in (), ACE_TRY_ENV);
      if (CORBA::is_nil (coordinator.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil coordinator reference <%s>\n",
                             ior),
                            1);
        }

      Receiver *receiver_impl;
      ACE_NEW_RETURN (receiver_impl,
                      Receiver,
                      1);
      PortableServer::ServantBase_var receiver_owner_transfer(receiver_impl);

      Test::Receiver_var receiver =
        receiver_impl->_this (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      Sender *sender_impl;
      ACE_NEW_RETURN (sender_impl,
                      Sender,
                      1);
      PortableServer::ServantBase_var sender_owner_transfer(sender_impl);

      Test::Sender_var sender =
        sender_impl->_this (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      poa_manager->activate (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      coordinator->add_pair (receiver.in (), sender.in (),
                             ACE_TRY_ENV);
      ACE_TRY_CHECK;

      while (!sender_impl->shutdown_called ())
        {
          ACE_Time_Value tv(1, 0);
          orb->run (tv, ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }

      root_poa->destroy (1, 1, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      orb->destroy (ACE_TRY_ENV);
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
