// $Id$

#include "Peer.h"
#include "ace/Get_Opt.h"
#include "ace/Thread_Manager.h"

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
                           "-k <ior> "
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
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic got a nil RootPOA\n"),
                          1);

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var tmp =
        orb->string_to_object(ior);

      Test::Coordinator_var coordinator =
        Test::Coordinator::_narrow(tmp.in ());

      if (CORBA::is_nil (coordinator.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil coordinator reference <%s>\n",
                             ior),
                            1);
        }

      Peer *peer_impl = 0;
      ACE_NEW_RETURN (peer_impl,
                      Peer (orb.in ()),
                      1);
      PortableServer::ServantBase_var peer_owner_transfer(peer_impl);

      PortableServer::ObjectId_var id =
        root_poa->activate_object (peer_impl);

      CORBA::Object_var object = root_poa->id_to_reference (id.in ());

      Test::Peer_var peer =
        Test::Peer::_narrow (object.in ());

      poa_manager->activate ();

      coordinator->add_peer (peer.in ());

      orb->run ();

      // Wait for all the threads.
      ACE_Thread_Manager::instance ()->wait ();

      root_poa->destroy (1, 1);

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
