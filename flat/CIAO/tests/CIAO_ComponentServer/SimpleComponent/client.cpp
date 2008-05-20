// $Id$

#include <ace/Get_Opt.h>
#include <ciao/ComponentServer/CIAO_CS_ClientS.h>
#include <ciao/ComponentServer/CIAO_ComponentServerC.h>
#include <ciao/ComponentServer/CIAO_ServerActivator_Impl.h>
#include <ciao/Valuetype_Factories/ConfigValue.h>
#include <ciao/Logger/Logger_Service.h>

#include "SimpleComponentC.h"

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
ACE_TMAIN (int argc,  char **argv)
{
  using namespace CIAO::Deployment;
  
  CIAO::Logger_Service logger;
  
  logger.init (argc, argv);
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
  ACE_DEBUG ((LM_DEBUG, "foo\n"));

      CIAO_ServerActivator_i *sa_tmp = new CIAO_ServerActivator_i (spawn_delay,
                                                                cs_path,
                                                                false,
                                                                orb.in (),
                                                                root_poa.in ());
  ACE_DEBUG ((LM_DEBUG, "bar\n"));
      
      PortableServer::ServantBase_var safe = sa_tmp;
      
      ServerActivator_var sa = sa_tmp->_this ();

      //ACE_DEBUG ((LM_DEBUG, "Attempting to create componentserver with no configvalues\n"));
      // Make a componentserver with no configvalues
      ComponentServer_var server1 (ComponentServer::_narrow (sa->create_component_server (0)));
        
      if (CORBA::is_nil (server1.in ()))
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

      // Make our configvalues
      // ::Components::ConfigValues_var configs = new 
      CORBA::Any val;
      ::Components::ConfigValues configs(3);
      configs.length (3);
      
      val <<= "create_Foo_SimpleHome_Servant";
      configs[0] = new CIAO::ConfigValue_impl ("edu.vanderbilt.dre.ServantEntrypoint",
                                               val);
      val <<= "SimpleComponent_svnt";
      configs[1] = new CIAO::ConfigValue_impl ("edu.vanderbilt.dre.ServantArtifact",
                                               val);
      val <<= "SimpleComponent_exec";
      configs[2] = new CIAO::ConfigValue_impl ("edu.vanderbilt.dre.ExecutorArtifact",
                                               val);

      // Install Home
      Components::CCMHome_var home = cont1a->install_home ("MyNameIsEarl",
                                                           "create_Foo_SimpleHome_Impl",
                                                           configs);
      
      if (CORBA::is_nil (home))
        {
          ACE_ERROR ((LM_ERROR, "Got back a nil home ref from install_home\n"));
          return -1;
        }
      
      Foo::SimpleHome_var shome = Foo::SimpleHome::_narrow (home.in ());
      
      if (CORBA::is_nil (shome))
        {
          ACE_ERROR ((LM_ERROR, "Narrow failed from CCM_Home to SimpleHome\n"));
          return -1;
        }
      
      
      Foo::SimpleComponent_var sc = shome->create ();
      
      if (CORBA::is_nil (sc))
        {
          ACE_ERROR ((LM_ERROR, "Home failed to make a component\n "));
          return -1;
        }
      
      sc->trigger ();
      
      cont1a->remove_home (home.in ());
      
      server1->remove_container (cont1a.in ());
      
            //ACE_DEBUG ((LM_DEBUG, "Removing component server\n"));
      sa->remove_component_server (server1.in ());
            //ACE_DEBUG ((LM_DEBUG, "Componentserver removed\n"));

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
