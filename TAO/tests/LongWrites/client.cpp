// $Id$

#include "Sender.h"
#include "Receiver.h"
#include "ace/Get_Opt.h"

ACE_RCSID(LongWrites, client, "$Id$")

const char *ior = "file://test.ior";

int test_type = Sender::TEST_ONEWAY;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "k:t:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.optarg;
        break;
      case 't':
        if (ACE_OS_String::strcasecmp(get_opts.optarg, "ONEWAY") == 0)
          test_type = Sender::TEST_ONEWAY;
        else if (ACE_OS_String::strcasecmp(get_opts.optarg, "WRITE") == 0)
          test_type = Sender::TEST_WRITE;
        else if (ACE_OS_String::strcasecmp(get_opts.optarg, "READ_WRITE") == 0)
          test_type = Sender::TEST_READ_WRITE;
        else
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unknown test type %s\n",
                             get_opts.optarg), 1);
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "-t <test_type (ONEWAY,WRITE,READ_WRITE)> "
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
      ACE_TRY_CHECK;

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
                      Sender (test_type),
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

      for (int i = 0; i != 600; ++i)
        {
          ACE_Time_Value tv(1, 0);
          orb->run (tv, ACE_TRY_ENV);
          ACE_TRY_CHECK;

          CORBA::ULong message_count =
            receiver_impl->message_count ();
          if (sender_impl->test_done (message_count))
            break;
        }
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) - client event loop done\n"));

      ACE_Thread_Manager::instance ()->wait ();

      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) client - threads finished\n"));

      root_poa->destroy (1, 1, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      orb->destroy (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      CORBA::ULong message_count =
        receiver_impl->message_count ();
      if (!sender_impl->test_done (message_count))
        {
          ACE_ERROR ((LM_ERROR,
                      "ERROR: missing messages, only received %d\n",
                      message_count));
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception caught:");
      return 1;
    }
  ACE_ENDTRY;

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) - client finished\n"));
  return 0;
}
