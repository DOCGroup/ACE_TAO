#include "testC.h"
#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include <atomic>

struct Client_Options
{
  ACE_TString ior {ACE_TEXT ("file://test.ior")};
  int thread_count {1};
  bool shutdown {};
  bool expected_object_not_exist {};
};

constexpr ACE_TCHAR corbaloc_prefix[] = ACE_TEXT ("corbaloc:");

int
parse_args (int argc, ACE_TCHAR *argv[], Client_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("xk:e:"));
  int c {};

  while ((c = get_opts ()) != -1)
    {
      switch (c)
        {
        case 'x':
          options.shutdown = true;
          break;

        case 'k':
          options.ior = get_opts.opt_arg ();
          break;

        case 'e':
          options.expected_object_not_exist = ACE_OS::atoi (get_opts.opt_arg ()) != 0;
          break;

        case '?':
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                            "usage:  %s "
                            "-k <ior> -x "
                            "-e <expected_object_not_exist> "
                            "\n",
                            argv [0]),
                            -1);
        }
    }

  if (ACE_OS::strncmp (options.ior.c_str (),
                        corbaloc_prefix,
                        ACE_OS::strlen (corbaloc_prefix)) != 0)
    return 1;

  // Indicates successful parsing of the command line
  return 0;
}

class Worker : public ACE_Task_Base
{
public:
  /// Constructor
  Worker (CORBA::ORB_ptr orb, Client_Options const &options);

  /// The actual implementation of the test
  void run_test ();

  int svc () override;

  /// Caught OBJECT_NOT_EXIST exception ?
  bool got_object_not_exist () const;
  bool finished () const;

private:
  Worker (Worker const &) = delete;
  Worker (Worker &&) = delete;
  Worker &operator= (Worker const &) = delete;
  Worker &operator= (Worker &&) = delete;

  CORBA::ORB_var orb_ {};
  // The ORB reference
  std::atomic_bool got_object_not_exist_ {};
  std::atomic_bool finished_ {};
  ACE_TString ior_ {};
};

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      Client_Options options;
      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv, options) != 0)
        return 1;

      Worker worker (orb.in (), options);

      if (worker.activate (THR_NEW_LWP | THR_JOINABLE,
                           options.thread_count) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) Cannot activate worker threads\n"),
                          1);

      int const timeout {30};
      int now {};
      while (now < timeout && !worker.finished ())
        {
          now += 5;
          ACE_Time_Value tv (5, 0);
          orb->run (tv);
        }

      if (options.shutdown)
        {
          CORBA::Object_var const object = orb->string_to_object (options.ior.c_str ());

          Simple_Server_var const server =
            Simple_Server::_narrow (object.in ());

          server->shutdown ();
        }

      orb->shutdown ();

      worker.thr_mgr ()->wait ();

      orb->destroy ();

      if (worker.got_object_not_exist () != options.expected_object_not_exist)
        ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t)client: test failed.\n"), 1);

      ACE_DEBUG ((LM_DEBUG, "(%P|%t)client: test passed.\n"));
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught in main:");
      return 1;
    }

  return 0;
}

// ****************************************************************

Worker::Worker (CORBA::ORB_ptr orb, Client_Options const &options)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    ior_ (options.ior)
{
}

int
Worker::svc ()
{
  try
    {
      this->run_test ();
    }
  catch (CORBA::OBJECT_NOT_EXIST const &)
    {
      this->got_object_not_exist_.store (true);
    }
  catch (...)
    {
    }

  this->finished_.store (true);
  return 0;
}

void
Worker::run_test ()
{
  CORBA::Object_var const object = this->orb_->string_to_object (this->ior_.c_str ());

  Simple_Server_var const server =
    Simple_Server::_narrow (object.in ());

  if (CORBA::is_nil (server.in ()))
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Object reference <%s> is nil.\n"),
                  this->ior_.c_str ()));
      return;
    }

  CORBA::Boolean const r =
    server->test_is_a ("IDL:Foo:1.0");

  if (r)
    ACE_DEBUG ((LM_DEBUG,
                "(%P|%t) unexpected result = %d\n",
                r));
}

bool
Worker::got_object_not_exist () const
{
  return this->got_object_not_exist_.load ();
}

bool
Worker::finished () const
{
  return this->finished_.load ();
}
