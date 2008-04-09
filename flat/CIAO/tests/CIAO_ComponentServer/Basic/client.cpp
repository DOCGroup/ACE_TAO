// $Id$

#include <ace/Get_Opt.h>
#include <ciao/ComponentServer/CIAO_ComponentServerC.h>

const char *server_1 = "file://test1.ior";
const char *server_2 = "file://test2.ior";

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "k:j:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        server_1 = get_opts.opt_arg ();
        break;
        
      case 'j':
        server_2 = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "-j <ior> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

int
ACE_TMAIN (int argc,  char **argv)
{
  using namespace CIAO::Deployment;
  
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var tmp1 = orb->string_to_object(server_1);
      CORBA::Object_var tmp2 = orb->string_to_object(server_2);
      
      ComponentServer_var server1 (ComponentServer::_narrow (tmp1)),
        server2 (ComponentServer::_narrow (tmp2));
      
      if (CORBA::is_nil (server1.in ()) ||
          CORBA::is_nil (server1.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Nil componentserver references"), -1);
        }
      
      Components::Deployment::Container_var tmp = server1->create_container (0);
      Container_var cont1a = Container::_narrow (tmp.in ());
      if (CORBA::is_nil (cont1a.in ()))
        {
          ACE_ERROR ((LM_ERROR, "Error: Got nil object reference from first create op on server 1 %u %u\n",
                      tmp.in (), cont1a.in ()));
          return -1;
        }
      ACE_ERROR ((LM_DEBUG, "Got container from server 1a\n"));
      
      Container_var cont1b = Container::_narrow (server1->create_container (0));
      if (CORBA::is_nil (cont1b.in ()))
        {
          ACE_ERROR ((LM_ERROR, "Error: Got nil object reference from second create op on server 1\n"));
          return -1;
        }
      ACE_ERROR ((LM_DEBUG, "Got container from server 1b\n"));

      Container_var cont2a = Container::_narrow (server2->create_container (0));
      if (CORBA::is_nil (cont2a.in ()))
        {
          ACE_ERROR ((LM_ERROR, "Error: Got nil object reference from first create op on server 2\n"));
          return -1;
        }
      ACE_ERROR ((LM_DEBUG, "Got container from server 2a\n"));

      try
        {
          server2->remove_container (cont1a);
          ACE_ERROR ((LM_ERROR, "Error: remove_container removed a container not belonging to the component server\n"));
        }
      catch (::Components::RemoveFailure &)
        {
          ACE_DEBUG ((LM_DEBUG, "Server 2 Refused to remove container it didn't own\n"));
          // expected
        }
      
      ::Components::Deployment::Containers_var cses = server1->get_containers ();
      
      if (cses->length () != 2)
        ACE_ERROR ((LM_ERROR, "Error: get_containers returned the wrong number of containers, %u should be 2\n",
                    cses->length ()));
      else ACE_DEBUG ((LM_DEBUG, "Got %u containers\n", cses->length ()));
      
      server1->remove_container (cont1a);
      ACE_DEBUG ((LM_DEBUG, "Successfully removed container 1a\n"));
      
      cses = server1->get_containers ();

      if (cses->length () != 1)
        ACE_ERROR ((LM_ERROR, "Error: get_containers returned %u containers after removal, should be 1\n",
                    cses->length ()));
      
      server2->remove_container (cont2a);
      ACE_DEBUG ((LM_DEBUG, "Successfully removed container 2a\n"));
      server1->remove_container (cont1b);
      ACE_DEBUG ((LM_DEBUG, "Successfully removed container 1b\n"));
      
      server1->remove ();
      server1->shutdown ();
      server2->remove ();
      server2->shutdown ();
      
      orb->destroy ();
    }
  catch (::Components::CreateFailure &e)
    {
      ACE_ERROR ((LM_ERROR, "Error: Caught CreateFailure exception.\n"));
    }
  catch (::Components::RemoveFailure &e)
    {
      ACE_ERROR ((LM_ERROR, "Error: Caught RemoveFailure exception.\n"));
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, "Error: Caught unknown exception\n"));
    }
}
