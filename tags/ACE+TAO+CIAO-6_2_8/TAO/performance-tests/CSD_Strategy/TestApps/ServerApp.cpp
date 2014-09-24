// $Id$
#include "ServerApp.h"
#include "TestInf/AppHelper.h"
#include "TestInf/OrbRunner.h"
#include "TestInf/AppShutdown.h"
#include "TestInf/TestAppExceptionC.h"
#include "TestServant/Foo_ClientEngine.h"
#include "ace/Get_Opt.h"
#include "ace/Time_Value.h"
#include "ace/High_Res_Timer.h"
// To force static load the service.
#include "tao/PI/PI.h"
#include "tao/CSD_ThreadPool/CSD_ThreadPool.h"


ServerApp::ServerApp()
  : TestAppBase("CSD_PT_ServerApp"),
    ior_filename_prefix_(ACE_TEXT("foo")),
    num_servants_(1),
    num_csd_threads_(1),
    num_orb_threads_(1),
    num_remote_clients_(1),
    num_collocated_clients_(0),
    num_loops_(1),
    use_csd_(1),
    scenario_id_("UnknownScenarioId"),
    trial_id_(0)
{
}


ServerApp::~ServerApp()
{
}


int
ServerApp::run_i(int argc, ACE_TCHAR* argv[])
{
  int result = this->init(argc, argv);
  if (result != 0)
    {
      return result;
    }

  this->poa_setup();
  this->csd_setup();
  this->servant_setup();
  this->collocated_setup();
  this->poa_activate();

  ACE_High_Res_Timer timer;
  timer.start();

  this->run_collocated_clients();
  this->run_orb_event_loop();


  timer.stop();

  ACE_Time_Value tv;
  timer.elapsed_time(tv);

  // Calling wait on ACE_Thread_Manager singleton to avoid the problem
  // that the main thread might exit before all CSD Threads exit.

  // Wait for all CSD task threads exit.
  ACE_Thread_Manager::instance ()->wait ();

  unsigned num_operations = this->stats_.total();

  double ops_per_msec = (1.0 * num_operations) / tv.msec();

  ACE_DEBUG((LM_DEBUG, "%s,%d,%d,%d,%.2f\n",
             this->scenario_id_.c_str(),
             this->trial_id_,
             num_operations,
             tv.msec(),
             ops_per_msec));

  this->cleanup();
  return this->check_results () ? 0 : -1;
}


int
ServerApp::init(int argc, ACE_TCHAR* argv[])
{
  this->orb_ = CORBA::ORB_init(argc, argv);

  // Parse the command-line args for this application.
  // * Raises -1 if problems are encountered.
  // * Returns 1 if the usage statement was explicitly requested.
  // * Returns 0 otherwise.
  int result = this->parse_args(argc, argv);

  if (result != 0)
    {
      return result;
    }

  TheAppShutdown->init(this->orb_.in(),
                       this->num_remote_clients_ +
                       this->num_collocated_clients_);

  return 0;
}


void
ServerApp::poa_setup(void)
{
  this->poa_ = this->create_poa(this->orb_.in(),
                                "ChildPoa");
}


void
ServerApp::csd_setup(void)
{
  this->tp_strategy_ = new TAO::CSD::TP_Strategy(this->num_csd_threads_);

  if (this->use_csd_ > 0)
    {
      if (!this->tp_strategy_->apply_to(this->poa_.in()))
        {
          ACE_ERROR((LM_ERROR,
                     "Failed to apply CSD strategy to poa.\n"));
          throw TestAppException();
        }
    }
}


void
ServerApp::servant_setup(void)
{
  this->servants_.create_and_activate(this->num_servants_,
                                      this->orb_.in (),
                                      this->poa_.in (),
                                      this->ior_filename_prefix_.c_str());
}


void
ServerApp::collocated_setup()
{
  int client_id = this->num_remote_clients_;

  for (unsigned i = 0; i < this->num_collocated_clients_; i++)
    {
      ++client_id;

      // Dole out the servant object references in a round-robin fashion.
      unsigned servant_index = i % this->num_servants_;

      ServantListType::T_stub_var obj = this->servants_.objref(servant_index);

      ClientEngine_Handle engine = new Foo_ClientEngine(obj.in(), client_id);

      this->collocated_client_task_.add_engine(engine.in());
    }

  this->collocated_client_task_.num_loops(this->num_loops_);
}


void
ServerApp::poa_activate(void)
{
  PortableServer::POAManager_var poa_manager
    = this->poa_->the_POAManager();

  poa_manager->activate();
}


void
ServerApp::run_collocated_clients(void)
{
  if (this->num_collocated_clients_ > 0)
    {
      if (this->collocated_client_task_.open() == -1)
        {
          throw TestAppException ();
        }
    }
}


void
ServerApp::run_orb_event_loop(void)
{
  OrbRunner orb_runner(this->orb_.in(), this->num_orb_threads_);
  orb_runner.run();
  TheAppShutdown->wait ();
}


void
ServerApp::cleanup()
{
}


int
ServerApp::parse_args(int argc, ACE_TCHAR* argv[])
{
  this->exe_name_ = argv[0];

  ACE_Get_Opt get_opts(argc, argv, ACE_TEXT("p:s:n:t:r:c:l:u:x:z:"));

  int c;

  while ((c = get_opts()) != -1)
    {
      int result = 0;
      switch (c)
      {
        case 'p':
          this->ior_filename_prefix_ = get_opts.opt_arg();
          break;

        case 's':
          result = this->set_arg(this->num_servants_,
                  get_opts.opt_arg(),
                  c,
                  "num_servants",
                  1);
          break;

        case 'n':
          result = this->set_arg(this->num_csd_threads_,
                  get_opts.opt_arg(),
                  c,
                  "num_servants",
                  1);
          break;

        case 't':
          result = this->set_arg(this->num_orb_threads_,
                  get_opts.opt_arg(),
                  c,
                  "num_orb_threads",
                  1);
          break;

        case 'r':
          result = this->set_arg(this->num_remote_clients_,
                  get_opts.opt_arg(),
                  c,
                  "num_remote_clients");
          break;

        case 'c':
          result = this->set_arg(this->num_collocated_clients_,
                  get_opts.opt_arg(),
                  c,
                  "num_collocated_clients");
          break;

        case 'l':
          result = this->set_arg(this->num_loops_,
                        get_opts.opt_arg(),
                        c,
                        "num_loops",
                        1);
          break;

        case 'u':
          result = this->set_arg(this->use_csd_,
                        get_opts.opt_arg(),
                        c,
                        "use_csd_flag");
          break;

        case 'x':
          this->scenario_id_ = ACE_TEXT_ALWAYS_CHAR(get_opts.opt_arg());
          break;

        case 'z':
          result = this->set_arg(this->trial_id_,
                        get_opts.opt_arg(),
                        c,
                        "trial_id_number",
                        1);
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
ServerApp::usage_statement()
{
  ACE_ERROR((LM_ERROR,
             "Usage:  %s [options]\n\n"
             "OPTIONS:\n\n"
             "\t[-p <ior_filename_prefix>]\n"
             "\t[-s <num_servants>]\n"
             "\t[-n <num_csd_threads>]\n"
             "\t[-t <num_orb_threads>]\n"
             "\t[-r <num_remote_clients>]\n"
             "\t[-c <num_collocated_clients>]\n"
             "\t[-l <num_loops>]\n"
             "\t[-u <use_csd_flag>]\n"
             "\t[-x <scenario_id_string>]\n"
             "\t[-z <trial_id_number>]\n"
             "\t[-?]\n\n",
             this->exe_name_.c_str()));
}


int
ServerApp::arg_dependency_checks()
{
  return (this->num_remote_clients_
         + this->num_collocated_clients_) > 0 ? 0 : -1;
}


int
ServerApp::set_arg(unsigned&   value,
                   const ACE_TCHAR* arg,
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


PortableServer::POA_ptr
ServerApp::create_poa(CORBA::ORB_ptr orb,
                      const char* poa_name)
{
  // Get the Root POA.
  PortableServer::POA_var root_poa
    = RefHelper<PortableServer::POA>::resolve_initial_ref(orb,
                                                          "RootPOA");

  // Get the POAManager from the Root POA.
  PortableServer::POAManager_var poa_manager
    = root_poa->the_POAManager();

  // Create the child POA Policies.
  CORBA::PolicyList policies(0);
  policies.length(0);

  // Create the child POA
  PortableServer::POA_var poa = AppHelper::create_poa(poa_name,
                                                      root_poa.in(),
                                                      poa_manager.in(),
                                                      policies);

  // Give away the child POA_ptr from the POA_var variable.
  return poa._retn();
}


bool
ServerApp::check_results()
{

  this->stats_.init(this->num_remote_clients_ +
                    this->num_collocated_clients_,
                    this->num_loops_);

  Foo_ClientEngine::expected_results(this->stats_);

  for (unsigned i = 0; i < this->num_servants_; i++)
    {
      this->servants_.servant(i)->gather_stats(this->stats_);
    }

  this->stats_.actual_vs_expected();

  return true;
}
