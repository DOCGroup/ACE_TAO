// $Id$
#include "ClientApp.h"
#include "AppHelper.h"
#include "OrbRunner.h"
#include "AppShutdown.h"
#include "TestAppExceptionC.h"
#include "Foo_B_ClientEngine.h"
#include "ace/Get_Opt.h"
// To force static load the service.
#include "tao/PI/PI.h"
#include "tao/CSD_ThreadPool/CSD_ThreadPool.h"


ClientApp::ClientApp()
: TestAppBase("TP_Test_3_Client"),
  client_task_ (true), // shutdown orb after client is done.
  num_servants_ (1),
  num_csd_threads_ (1),
  num_orb_threads_ (1),
  ior_("Not Set"),
  client_kind_(0),
  client_id_(0)
{
}


ClientApp::~ClientApp()
{
}


int
ClientApp::run_i(int argc, char* argv[] ACE_ENV_ARG_DECL)
{
  int result = this->init(argc, argv ACE_ENV_ARG_PARAMETER);
  if (result != 0)   
    {
      return result;
    }
  ACE_CHECK_RETURN (-1);

  this->poa_setup(ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
  this->csd_setup(ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
  this->client_setup(ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
  this->poa_activate(ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
  this->run_clients(ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
  this->run_orb_event_loop(ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Calling wait on ACE_Thread_Manager singleton to avoid the problem
  // that the main thread might exit before all CSD Threads exit.

  // Wait for all CSD task threads exit.
  ACE_Thread_Manager::instance ()->wait ();

  this->cleanup();

  return this->check_validity () ? 0 : -1;
}


int
ClientApp::init(int argc, char* argv[] ACE_ENV_ARG_DECL)
{
  this->orb_ = CORBA::ORB_init(argc, argv, "" ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Parse the command-line args for this application.
  // * Raises -1 if problems are encountered.
  // * Returns 1 if the usage statement was explicitly requested.
  // * Returns 0 otherwise.
  int result = this->parse_args(argc, argv);
  if (result != 0)
    {
      return result;
    }

  TheAppShutdown->init(this->orb_.in(), num_servants_ ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  return 0;
}


void
ClientApp::poa_setup(ACE_ENV_SINGLE_ARG_DECL)
{
  this->poa_ = this->create_poa(this->orb_.in(), 
                                "ChildPoa" 
                                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}


void
ClientApp::csd_setup(ACE_ENV_SINGLE_ARG_DECL)
{
  this->tp_strategy_ = new TAO::CSD::TP_Strategy(this->num_csd_threads_);

  if (!this->tp_strategy_->apply_to(this->poa_.in() ACE_ENV_ARG_PARAMETER))
    {
      ACE_ERROR((LM_ERROR,
                 "Failed to apply CSD strategy to poa.\n"));
      ACE_THROW(TestAppException());
    }
  ACE_CHECK;
}


void
ClientApp::client_setup(ACE_ENV_SINGLE_ARG_DECL)
{
  // Turn the ior_ into a Foo_B obj ref.
  Foo_B_var foo = RefHelper<Foo_B>::string_to_ref(this->orb_.in(),
                                                  this->ior_.c_str()
                                                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
 
  this->servants_.create_and_activate(1, // number of callback servants
                                      this->poa_.in()
                                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  ServantListType::T_stub_var cb = this->servants_.objref(0);

  // Create the ClientEngine object, and give it the Foo_B and Callback object 
  // references.
  ClientEngine_Handle engine 
    = new Foo_B_ClientEngine(foo.in(), cb.in (), this->client_id_);
  this->client_task_.add_engine(engine.in());
}


void
ClientApp::poa_activate(ACE_ENV_SINGLE_ARG_DECL)
{
  PortableServer::POAManager_var poa_manager 
    = this->poa_->the_POAManager(ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  poa_manager->activate(ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}


void
ClientApp::run_clients(ACE_ENV_SINGLE_ARG_DECL)
{
  if (this->client_task_.open() != 0)
    {
      ACE_THROW (TestAppException ());
    }
}


void
ClientApp::run_orb_event_loop(ACE_ENV_SINGLE_ARG_DECL)
{
  OrbRunner orb_runner(this->orb_.in(), this->num_orb_threads_);
  orb_runner.run(ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  TheAppShutdown->wait ();
}


PortableServer::POA_ptr
ClientApp::create_poa(CORBA::ORB_ptr orb, const char* poa_name ACE_ENV_ARG_DECL)
{
  // Get the Root POA.
  PortableServer::POA_var root_poa
    = RefHelper<PortableServer::POA>::resolve_initial_ref(orb, 
                                                          "RootPOA"
                                                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (PortableServer::POA::_nil ());

  // Get the POAManager from the Root POA.
  PortableServer::POAManager_var poa_manager 
    = root_poa->the_POAManager(ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (PortableServer::POA::_nil ());

  // Create the child POA Policies.
  CORBA::PolicyList policies(0);
  policies.length(0);

  // Create the child POA
  PortableServer::POA_var poa = AppHelper::create_poa(poa_name,
                                                      root_poa.in(),
                                                      poa_manager.in(),
                                                      policies
                                                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (PortableServer::POA::_nil ());

  // Give away the child POA_ptr from the POA_var variable.
  return poa._retn();
}


void
ClientApp::cleanup()
{
}


int
ClientApp::parse_args(int argc, char* argv[])
{
  this->exe_name_ = argv[0];

  ACE_Get_Opt get_opts(argc, argv, "i:k:n:");

  int c;

  while ((c = get_opts()) != -1)
    {
      int result = 0;
      switch (c)
      {
        case 'i':
          this->ior_ = get_opts.opt_arg();
          break;

        case 'k':
          result = set_arg(this->client_kind_,
                  get_opts.opt_arg(),
                  c,
                  "client_kind");
          break;

        case 'n':
           result = set_arg(this->client_id_,
                  get_opts.opt_arg(),
                  c,
                  "client_id");
          break;

        case '?':
          this->usage_statement();
          return 1;

        default:
          this->usage_statement();
          return -1;
      }

      if (result != 0)
        {
          return result;
        }
    }

  return this->arg_dependency_checks();
}

void
ClientApp::usage_statement()
{
  ACE_ERROR((LM_ERROR,
             "Usage:  %s [options]\n\n"
             "OPTIONS:\n\n"
             "\t[-i <ior>]\n"
             "\t[-k <client_kind>]\n"
             "\t[-n <client_id>]\n"
             "\t[-?]\n\n",
             this->exe_name_.c_str()));
}


int
ClientApp::arg_dependency_checks()
{
  if (this->ior_ == "Not Set")
    {
      ACE_ERROR((LM_ERROR,
                 "Error: Missing required command-line option (-i <ior>).\n"));
      this->usage_statement();
      return -1;
    }
  if (this->client_id_ <= 0)
    {
      ACE_ERROR((LM_ERROR,
                 "Error: Invalid command-line option (-n <client id>). \n"
                 "       The client id should be positive integer. \n"));
      this->usage_statement();
      return -1;
    }

  return 0;
}


int
ClientApp::set_arg(unsigned&   value,
                   const char* arg,
                   char        opt,
                   const char* name,
                   int         min)
{
  int tmp = ACE_OS::atoi(arg);

  if (tmp < min)
    {
      ACE_ERROR((LM_ERROR,
                 "Error: -%c <%s> must be integer type with a value of, "
                 "at least, %d.\n", opt, name, min));
      this->usage_statement();
      return -1;
    }

  value = tmp;
  return 0;
}


bool
ClientApp::check_validity ()
{
  // Check whether the clients return any errors.
  if (this->client_task_.failure_count () > 0)
    {
      return false;
    }

  return true;
}

