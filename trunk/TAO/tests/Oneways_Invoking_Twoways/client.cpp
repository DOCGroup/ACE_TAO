// $Id$

#include "Receiver_i.h"
#include "Client_Task.h"
#include "Server_Task.h"
#include "ace/Get_Opt.h"

const ACE_TCHAR *ior = ACE_TEXT("file://test.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
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
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      if (parse_args (argc, argv) != 0)
        return 1;

      // Get the sender reference..
      CORBA::Object_var tmp =
        orb->string_to_object(ior);

      Test::Sender_var sender =
        Test::Sender::_narrow(tmp.in ());

      if (CORBA::is_nil (sender.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil coordinator reference <%s>\n",
                             ior),
                            1);
        }

      Receiver_i *receiver_impl;
      ACE_NEW_RETURN (receiver_impl,
                      Receiver_i (orb.in (),
                                  sender.in (),
                                  10),
                      1);

      PortableServer::ServantBase_var receiver_owner_transfer(receiver_impl);

      PortableServer::ObjectId_var id =
        root_poa->activate_object (receiver_impl);

      CORBA::Object_var object = root_poa->id_to_reference (id.in ());

      Test::Receiver_var receiver =
        Test::Receiver::_narrow (object.in ());

      // Activate poa manager
      poa_manager->activate ();

      Client_Task client_task (sender.in (),
                               receiver.in (),
                               ACE_Thread_Manager::instance (),
                               receiver_impl);

      Server_Task server_task (orb.in (),
                               ACE_Thread_Manager::instance ());

      // Before creating threads we will let the sender know that we
      // will have two threads that would make invocations..
      sender->active_objects ((CORBA::Short) 2);

      if (server_task.activate (THR_NEW_LWP | THR_JOINABLE, 2,1) == -1)
        {
          ACE_ERROR ((LM_ERROR, "Error activating server task\n"));
        }

      if (client_task.activate (THR_NEW_LWP | THR_JOINABLE, 2, 1) == -1)
        {
          ACE_ERROR ((LM_ERROR, "Error activating client task\n"));
        }

      // wait for oneway sends and twoway replies to be processed
      client_task.wait ();

      // shutdown server
      sender->shutdown ();

      // shutdown ourself
      receiver->shutdown ();

      // wait for event loop to finish
      server_task.wait ();

      ACE_DEBUG ((LM_DEBUG,
                  "Event Loop finished\n"));

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
