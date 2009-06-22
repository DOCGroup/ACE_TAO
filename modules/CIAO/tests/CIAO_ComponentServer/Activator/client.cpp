// $Id$

#include "ace/Get_Opt.h"
#include "ciao/ComponentServer/CIAO_CS_ClientS.h"
#include "ciao/ComponentServer/CIAO_ComponentServerC.h"
#include "ciao/ComponentServer/CIAO_ServerActivator_Impl.h"
#include "ciao/ComponentServer/CIAO_ComponentInstallation_Impl.h"
#include "ciao/Valuetype_Factories/ConfigValue.h"

const char *cs_path = "ciao_componentserver";
CORBA::ULong spawn_delay = 30;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "s:d:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 's':
        cs_path = get_opts.opt_arg ();
        break;
        
      case 'd':
        spawn_delay = ACE_OS::atoi (get_opts.opt_arg ());
        break;
        
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-s <path> "
                           "-d <uint> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

int
ACE_TMAIN (int argc,  ACE_TCHAR **argv)
{
  using namespace CIAO::Deployment;
  
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;      
      
      CORBA::Object_var object =
        orb->resolve_initial_references ("RootPOA");
      
      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (object.in ());
      
      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();
      
      poa_manager->activate ();

      CIAO::Deployment::ComponentInstallation_Impl *tmp_ci;
      
      ACE_NEW_THROW_EX (tmp_ci, 
                        CIAO::Deployment::ComponentInstallation_Impl (),
                        CORBA::NO_MEMORY ());
      
      PortableServer::ServantBase_var safe_servant = tmp_ci;
      
      CIAO_ServerActivator_i *sa_tmp = new CIAO_ServerActivator_i (spawn_delay,
                                                                   cs_path,
                                                                   0,
                                                                   false,
                                                                   tmp_ci->_this (),
                                                                   orb.in (),
                                                                   root_poa.in ());
      
      PortableServer::ServantBase_var safe = sa_tmp;
      
      ServerActivator_var sa = sa_tmp->_this ();
      
      // Make our configvalues
      // ::Components::ConfigValues_var configs = new 
      ::Components::ConfigValues configs(2);
      //ACE_DEBUG ((LM_DEBUG, "Attempting to create componentserver with no configvalues\n"));
      // Make a componentserver with no configvalues
      ComponentServer_var server1 (ComponentServer::_narrow (sa->create_component_server (configs)));
      //ACE_DEBUG ((LM_DEBUG, "Componentserver with no configvalues created!\n")); 
      
      //ACE_DEBUG ((LM_DEBUG, "Attempting to create componentserver with UUID configvalues\n"));
      CORBA::Any val;
      val <<= "MyNameIsEarl";
      configs.length (1);
      ::Components::ConfigValue_var cv_tmp = new CIAO::ConfigValue_impl ("edu.vanderbilt.dre.ServerUUID", val);
      configs[0] = cv_tmp._retn ();
      
      ComponentServer_var server2 (ComponentServer::_narrow (sa->create_component_server (configs)));


      // Initialize servant
        
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
      //ACE_DEBUG ((LM_DEBUG, "Got container from server 1a\n"));
      
      Container_var cont1b = Container::_narrow (server1->create_container (0));
      if (CORBA::is_nil (cont1b.in ()))
        {
          ACE_ERROR ((LM_ERROR, "Error: Got nil object reference from second create op on server 1\n"));
          return -1;
        }
      //ACE_DEBUG ((LM_DEBUG, "Got container from server 1b\n"));

      Container_var cont2a = Container::_narrow (server2->create_container (0));
      if (CORBA::is_nil (cont2a.in ()))
        {
          ACE_ERROR ((LM_ERROR, "Error: Got nil object reference from first create op on server 2\n"));
          return -1;
        }
      //ACE_DEBUG ((LM_DEBUG, "Got container from server 2a\n"));

      try
        {
          server2->remove_container (cont1a);
          ACE_ERROR ((LM_ERROR, "Error: remove_container removed a container not belonging to the component server\n"));
        }
      catch (::Components::RemoveFailure &)
        {
          //ACE_DEBUG ((LM_DEBUG, "Server 2 Refused to remove container it didn't own\n"));
          // expected
        }
      
      ::Components::Deployment::Containers_var cses = server1->get_containers ();
      
      if (cses->length () != 2)
        ACE_ERROR ((LM_ERROR, "Error: get_containers returned the wrong number of containers, %u should be 2\n",
                    cses->length ()));
      else //ACE_DEBUG ((LM_DEBUG, "Got %u containers\n", cses->length ()));
      
      server1->remove_container (cont1a);
      //ACE_DEBUG ((LM_DEBUG, "Successfully removed container 1a\n"));
      
      cses = server1->get_containers ();

      if (cses->length () != 1)
        ACE_ERROR ((LM_ERROR, "Error: get_containers returned %u containers after removal, should be 1\n",
                    cses->length ()));
      
      server2->remove_container (cont2a);
      //ACE_DEBUG ((LM_DEBUG, "Successfully removed container 2a\n"));
      server1->remove_container (cont1b);
      //ACE_DEBUG ((LM_DEBUG, "Successfully removed container 1b\n"));

      //ACE_DEBUG ((LM_DEBUG, "Removing component server\n"));
      sa->remove_component_server (server1.in ());
      sa->remove_component_server (server2.in ());
      //ACE_DEBUG ((LM_DEBUG, "Componentserver removed\n"));

      orb->destroy ();
    }
  catch (::Components::CreateFailure &)
    {
      ACE_ERROR ((LM_ERROR, "Error: Caught CreateFailure exception.\n"));
    }
  catch (::Components::RemoveFailure &)
    {
      ACE_ERROR ((LM_ERROR, "Error: Caught RemoveFailure exception.\n"));
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, "Error: Caught unknown exception\n"));
    }
  return 0;
}
