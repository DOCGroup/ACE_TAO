#include <atomic>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include "tao/corba.h"
#include "tao/Utils/ORB_Manager.h"
#include "tao/AnyTypeCode/Any.h"
#include "tao/Messaging/Messaging.h"
#include "tao/ORBInitializer_Registry.h"
#include "tao/PI/PI.h"
#include "tao/LocalObject.h"
#include "tao/Utils/PolicyList_Destroyer.h"

struct Test_Options
{
  int n_threads {1};
  bool set_timeout {true};
  bool register_orb_initializer {true};
  bool init_in_main {};
};

int initORB (int thread_id, int argc, ACE_TCHAR *argv[], Test_Options const &options);

int
parse_args (int argc, ACE_TCHAR *argv[], Test_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("min:"));
  int option {};

  while ((option = get_opts ()) != -1)
    switch (option)
      {
      case 'm':
        options.init_in_main = true;
        break;

      case 'i':
        options.register_orb_initializer = false;
        break;

      case 'n':
        options.n_threads = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-m (Initialize an ORB from the main thread first) "
                           "-i (Do not register ORB initializer) "
                           "-n <numberOfThreads>"
                           "\n",
                           argv [0]),
                          -1);
      }
  return 0;
}

class Worker : public ACE_Task_Base
{
public:
  Worker (int argc, ACE_TCHAR *argv[], Test_Options const &options);
  Worker (Worker const &) = delete;
  Worker &operator= (Worker const &) = delete;
  Worker (Worker &&) = delete;
  Worker &operator= (Worker &&) = delete;

  int svc () override;

private:
  int argc_;
  ACE_TCHAR **argv_; // Borrowed from main; argv remains valid until the process exits.
  Test_Options const options_;
};

class MyORBinitializer
  : public virtual PortableInterceptor::ORBInitializer
  , public virtual CORBA::LocalObject
{
public:
  explicit MyORBinitializer (std::string orb_id)
    : orb_id_ (std::move (orb_id))
  {
  }
  MyORBinitializer (MyORBinitializer const &) = delete;
  MyORBinitializer &operator= (MyORBinitializer const &) = delete;
  MyORBinitializer (MyORBinitializer &&) = delete;
  MyORBinitializer &operator= (MyORBinitializer &&) = delete;

  void pre_init (PortableInterceptor::ORBInitInfo_ptr) override
  {
    ACE_DEBUG ((LM_DEBUG,
                "MyORBinitializer::pre_init() called for ORB \"%C\"\n",
                this->orb_id_.c_str ()));
  }

  void post_init (PortableInterceptor::ORBInitInfo_ptr) override
  {
    ACE_DEBUG ((LM_DEBUG,
                "MyORBinitializer::post_init() called for ORB \"%C\"\n",
                this->orb_id_.c_str ()));
  }

private:
  std::string const orb_id_;
};

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  Test_Options options;

  try
    {
      if (parse_args (argc, argv, options) != 0)
        return 1;

      if (options.init_in_main)
        initORB (0, argc, argv, options);

      Worker worker (argc, argv, options);
      if (worker.activate (THR_NEW_LWP | THR_JOINABLE, options.n_threads) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot activate ORB thread(s)\n"),
                          1);

      worker.thr_mgr ()->wait ();
      ACE_DEBUG ((LM_DEBUG, "Event loop finished\n"));
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}

// ****************************************************************

Worker::Worker (int argc, ACE_TCHAR *argv[], Test_Options const &options)
  : argc_ (argc)
  , argv_ (argv)
  , options_ (options)
{
}

int
Worker::svc ()
{
  static std::atomic<int> thread_id {};
  int const id = ++thread_id;
  initORB (id, this->argc_, this->argv_, this->options_);

  return 0;
}

int
initORB (int thread_id, int argc, ACE_TCHAR *argv[], Test_Options const &options)
{
  try
    {
      std::string const orb_id = "ORB_" + std::to_string (thread_id);
      std::vector<ACE_TCHAR *> argv_copy (argv, argv + argc);
      argv_copy.push_back (nullptr);
      int argc_copy = argc;

      ACE_DEBUG ((LM_DEBUG, "Initializing ORB \"%C\"\n", orb_id.c_str ()));

      if (options.register_orb_initializer)
        {
          ACE_DEBUG ((LM_DEBUG, "Creating ORB initializer\n"));
          PortableInterceptor::ORBInitializer_var const initializer (
            new MyORBinitializer (orb_id));
          PortableInterceptor::register_orb_initializer (initializer.in ());
        }

      ACE_DEBUG ((LM_DEBUG, "Creating TAO_ORB_Manager\n"));
      std::unique_ptr<TAO_ORB_Manager> orb_manager = std::make_unique<TAO_ORB_Manager> ();

      if (-1 == orb_manager->init (argc_copy, argv_copy.data (), orb_id.c_str ()))
        {
          ACE_DEBUG ((LM_DEBUG, "Failed to initialize ORB \"%C\"\n", orb_id.c_str ()));
          throw CORBA::INTERNAL ();
        }

      ACE_DEBUG ((LM_DEBUG, "ORB \"%C\" initialized\n", orb_id.c_str ()));

      if (options.set_timeout)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Setting connection timeout policy for ORB \"%C\"\n",
                      orb_id.c_str ()));

          TAO::Utils::PolicyList_Destroyer policy_list (1);
          policy_list.length (1);

          CORBA::ORB_var const orb = orb_manager->orb ();

          TimeBase::TimeT const connection_timeout = 100 * 10000;
          CORBA::Any any;
          any <<= connection_timeout;

          policy_list[0] = orb->create_policy (TAO::CONNECTION_TIMEOUT_POLICY_TYPE, any);

          ACE_DEBUG ((LM_DEBUG, "Connection timeout policy set for ORB \"%C\"\n",
                      orb_id.c_str ()));
        }

      orb_manager->fini ();
    }
  catch (CORBA::Exception const &ex)
    {
      ACE_ERROR ((LM_ERROR, "Caught exception: %C\n", ex._info ().c_str ()));
      return 1;
    }

  return 0;
}

