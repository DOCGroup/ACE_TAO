#include "testC.h"
#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include "ace/streams.h"
#include "tao/Invocation_Utils.h"
#include "ace/OS_NS_unistd.h"
#include <atomic>
#include <string>

struct Client_Options
{
  ACE_TString ior {ACE_TEXT ("file://test.ior")};
  int thread_count {1};
  bool shutdown {};
  int expected_exception_kind {TAO::FOE_NON};
  int num_requests {1};
  int expected_exceptions {1};
};

constexpr ACE_TCHAR corbaloc_prefix[] = ACE_TEXT ("corbaloc:");

int
parse_args (int argc, ACE_TCHAR *argv[], Client_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("xk:e:i:n:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'x':
        options.shutdown = true;
        break;

      case 'k':
        options.ior = get_opts.opt_arg ();
        break;

      case 'e':
        options.expected_exception_kind = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'i':
        options.num_requests = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'n':
        options.expected_exceptions = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> -x "
                           "-e <expected exception kind> "
                           "-i <num_requests> "
                           "-n <expected number of exceptions> "
                           "\n",
                           argv [0]),
                          -1);
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
  Worker (CORBA::ORB_ptr orb, Client_Options const &options);
  // Constructor

  // = The Task_Base methods
  int svc () override;

  // Caught any exception ?
  int received_ex_kind () const;

  // Return number of received exceptions.
  int num_received_ex () const;

  // Is test done ?
  void done ();

private:
  // The ORB reference
  CORBA::ORB_var orb_ {};
  // The exceptions caught.
  std::atomic_int received_ex_kind_ {TAO::FOE_NON};
  // The number of received exceptions.
  std::atomic_int num_received_ex_ {};
  // Flag for test completion. The result is
  // collected before done.
  std::atomic_bool done_ {};
  ACE_TString ior_ {};
  int num_requests_ {};
};

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
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

      constexpr int timeout = 30;
      int now {};
      while (now < timeout
             && ((options.expected_exception_kind != TAO::FOE_NON
                  && (worker.received_ex_kind () != options.expected_exception_kind
                      || worker.num_received_ex () != options.expected_exceptions))
                 || options.expected_exception_kind == TAO::FOE_NON))
        {
          std::cout << ".";
          ++now;
          ACE_Time_Value tv (1, 0);
          orb->run (tv);
        }
      std::cout << std::endl;

      worker.done ();

      if (options.shutdown)
        {
          CORBA::Object_var const object = orb->string_to_object (
            ACE_TEXT_ALWAYS_CHAR (options.ior.c_str ()));

          Simple_Server_var const server =
            Simple_Server::_narrow (object.in ());

          server->shutdown ();
        }

      ACE_OS::sleep (1);

      orb->destroy ();

      worker.thr_mgr ()->wait ();

      if (worker.received_ex_kind () != options.expected_exception_kind
          || worker.num_received_ex () != options.expected_exceptions)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                             "(%P|%t)client: test failed - expected is different from received. "
                             "expected %d/%d received %d/%d.\n",
                             options.expected_exception_kind,
                             options.expected_exceptions,
                             worker.received_ex_kind (),
                             worker.num_received_ex ()),
                          1);
      }

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
    ior_ (options.ior),
    num_requests_ (options.num_requests)
{
}

int
Worker::svc ()
{
  try
    {
      CORBA::Object_var const object = this->orb_->string_to_object (
        ACE_TEXT_ALWAYS_CHAR (this->ior_.c_str ()));

      Simple_Server_var const server =
        Simple_Server::_narrow (object.in ());

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR ((LM_ERROR,
                      "Object reference <%s> is nil.\n",
                      ACE_TEXT_ALWAYS_CHAR (this->ior_.c_str ())));
          return 0;
        }

      for (int i {}; i < this->num_requests_; ++i)
        {
          try
            {
              CORBA::Boolean const result = server->test_is_a ("IDL:Foo:1.0");
              if (result)
                ACE_DEBUG ((LM_DEBUG,
                            "(%P|%t) unexpected result = %d\n",
                            result));
            }
          catch (CORBA::OBJECT_NOT_EXIST const &)
          {
            if (!this->done_.load ())
            {
              this->num_received_ex_.fetch_add (1);
              this->received_ex_kind_.fetch_or (TAO::FOE_OBJECT_NOT_EXIST);
            }
          }
          catch (CORBA::COMM_FAILURE const &)
          {
            if (!this->done_.load ())
            {
              this->num_received_ex_.fetch_add (1);
              this->received_ex_kind_.fetch_or (TAO::FOE_COMM_FAILURE);
            }
          }
          catch (CORBA::TRANSIENT const &)
          {
            if (!this->done_.load ())
            {
              this->num_received_ex_.fetch_add (1);
              this->received_ex_kind_.fetch_or (TAO::FOE_TRANSIENT);
            }
          }
          catch (CORBA::INV_OBJREF const &)
          {
            if (!this->done_.load ())
            {
              this->num_received_ex_.fetch_add (1);
              this->received_ex_kind_.fetch_or (TAO::FOE_INV_OBJREF);
            }
          }
        }
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception (
        "Unexpected exception caught");
    }

  return 0;
}


int
Worker::received_ex_kind () const
{
  return this->received_ex_kind_.load ();
}

int
Worker::num_received_ex () const
{
  return this->num_received_ex_.load ();
}

void
Worker::done ()
{
  this->done_.store (true);
}

