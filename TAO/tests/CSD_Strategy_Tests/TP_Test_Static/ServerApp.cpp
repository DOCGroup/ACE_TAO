// $Id$
#include "ServerApp.h"
#include "Foo_A_i.h"
#include "AppHelper.h"
#include "TestAppExceptionC.h"
#include "AppShutdown.h"
#include "ace/Get_Opt.h"
// To force static load the service.
#include "tao/PI/PI.h"
#include "tao/CSD_ThreadPool/CSD_ThreadPool.h"


ServerApp::ServerApp()
  : TestAppBase ("TP_Test_1_Server"),
    ior_filename_ ("test.ior"),
    num_clients_ (1)
{
}

ServerApp::~ServerApp()
{
}

int
ServerApp::run_i(int argc, ACE_TCHAR *argv[])
{
  // Initialize the ORB before parsing our own args.
  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

  // Parse the command-line args for this application.
  // * Returns -1 if problems are encountered.
  // * Returns 1 if the usage statement was explicitly requested.
  // * Returns 0 otherwise.
  int result = this->parse_args (argc, argv);
  if (result != 0)
    {
      return result;
    }

  TheAppShutdown->init(orb.in(), num_clients_);

  // Get the Root POA
  PortableServer::POA_var root_poa =
           RefHelper<PortableServer::POA>::resolve_initial_ref(orb.in(),
                                                               "RootPOA");

  // Get the POAManager from the Root POA.
  PortableServer::POAManager_var poa_manager
    = root_poa->the_POAManager();

  // Create the child POA Policies.
  CORBA::PolicyList policies(0);
  policies.length(0);

  // Create the child POA
  PortableServer::POA_var child_poa =
                              AppHelper::create_poa("ChildPoa",
                                                    root_poa.in(),
                                                    poa_manager.in(),
                                                    policies);

  // Create the servant object.
  Foo_A_i* servant = new Foo_A_i();

  // Local smart pointer variable to deal with releasing the reference
  // to the servant object when the variable falls out of scope.
  PortableServer::ServantBase_var servant_owner(servant);

  // Obtain the object reference using the servant
  CORBA::Object_var obj = AppHelper::activate_servant(child_poa.in(),
                                                      servant);

  // Stringify and save the object reference to a file
  AppHelper::ref_to_file(orb.in(),
                         obj.in(),
                         this->ior_filename_.c_str());

  // Activate the POA Manager
  poa_manager->activate();

  ACE_DEBUG((LM_DEBUG,
             "(%P|%t) ServerApp is ready.  Running the ORB event loop.\n"));

  // Run the ORB event loop.
  orb->run ();

  ACE_DEBUG((LM_DEBUG,
             "(%P|%t) ServerApp ORB event loop has completed.\n"));

  TheAppShutdown->wait ();

  // Calling wait on ACE_Thread_Manager singleton to avoid the problem
  // that the main thread might exit before all CSD Threads exit.

  // Wait for all CSD task threads exit.
  ACE_Thread_Manager::instance ()->wait ();

  ACE_DEBUG((LM_DEBUG,
             "(%P|%t) ServerApp is destroying the Root POA.\n"));

  root_poa->destroy(1, 1);

  ACE_DEBUG((LM_DEBUG,
             "(%P|%t) ServerApp is destroying the ORB.\n"));

  orb->destroy();

  ACE_DEBUG((LM_DEBUG,
             "(%P|%t) ServerApp has completed running successfully.\n"));

  return 0;
}


int
ServerApp::parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "o:n:");

  int c;

  while ((c = get_opts ()) != -1)
    {
      switch (c)
      {
        case 'o':
          this->ior_filename_ = ACE_TEXT_ALWAYS_CHAR (get_opts.opt_arg());
          break;

        case 'n':
          {
            int tmp = ACE_OS::atoi (get_opts.opt_arg());
            if (tmp < 1)
              {
                ACE_ERROR((LM_ERROR,
                          "(%P|%t) Error.  -n must be followed by an integer "
                          "value greater than 0.\n"));
                return -1;
              }

            this->num_clients_ = tmp;
          }
          break;

        case '?':
          ACE_ERROR((LM_ERROR,
                     "(%P|%t) usage:  %s -o <ior_filename> -n <num_clients>\n",
                     argv[0]));
          return 1;

        default:
          ACE_ERROR((LM_ERROR,
                     "(%P|%t) usage:  %s -o <ior_filename> -n <num_clients>\n",
                     argv[0]));
          return -1;
      }
    }

  return 0;
}
