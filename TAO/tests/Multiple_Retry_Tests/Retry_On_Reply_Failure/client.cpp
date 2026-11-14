#include "testC.h"
#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include "ace/streams.h"
#include "tao/Invocation_Utils.h"
#include "ace/OS_NS_unistd.h"

struct Client_Options
{
  ACE_TString ior {ACE_TEXT ("file://test.ior")};
  int expected_exception {TAO::FOE_NON};
};

constexpr int nthreads = 1;
constexpr ACE_TCHAR corbaloc_prefix[] = ACE_TEXT ("corbaloc:");

static bool
parse_args (int argc, ACE_TCHAR *argv[], Client_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:e:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        options.ior = get_opts.opt_arg ();
        break;

      case 'e':
        options.expected_exception = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior>"
                           "-e <expected exception kind> "
                           "\n",
                           argv [0]),
                          false);
      }

  if (ACE_OS::strncmp (options.ior.c_str (),
                       corbaloc_prefix,
                       ACE_OS::strlen (corbaloc_prefix)) != 0)
    return false;

  // Indicates successful parsing of the command line
  return true;
}

class Worker : public ACE_Task_Base
{
public:
  Worker (CORBA::ORB_ptr orb, ACE_TString const &ior);

  Worker (Worker const &) = delete;
  Worker (Worker &&) = delete;
  Worker &operator= (Worker const &) = delete;
  Worker &operator= (Worker &&) = delete;

  // = The Task_Base methods
  int svc () override;

  // Caught any exception ?
  int received_ex_kind () const;

  // Return number of received exceptions.
  int num_received_ex () const;

  // Indicate if the invocation completed.
  bool invocation_completed () const;

  // Is test done ?
  void done ();

private:
  // The ORB reference
  CORBA::ORB_var orb_;
  ACE_TString ior_;
  // The exceptions caught.
  int received_ex_kind_;
  // The number of received exceptions.
  int num_received_ex_;
  // Flag indicating that the invocation was completed.
  bool invocation_completed_;
  // Flag for test completion. The result is
  // collected before done.
  bool done_;
};

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      Client_Options options;
      if (!parse_args (argc, argv, options))
        return 1;

      Worker worker (orb.in (), options.ior);

      if (worker.activate (THR_NEW_LWP | THR_JOINABLE,
                           nthreads) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) Cannot activate worker threads\n"),
                          1);

      int timeout = 30;
      int now = 0;
      while (now < timeout &&
             ((options.expected_exception == TAO::FOE_NON && !worker.invocation_completed ()) ||
              (options.expected_exception != TAO::FOE_NON &&
               options.expected_exception != worker.received_ex_kind ()))
             )
        {
          std::cout << "." << std::flush;
          now += 1;
          ACE_Time_Value tv (1, 0);
          orb->run (tv);
        }
      ACE_ASSERT (now != 0);

      std::cout << std::endl;

      worker.done ();

      CORBA::Object_var const object =
        orb->string_to_object (options.ior.c_str ());

      Simple_Server_var const server =
        Simple_Server::_narrow (object.in ());

      server->shutdown ();

      ACE_OS::sleep (1);

      orb->destroy ();

      worker.thr_mgr ()->wait ();

      bool const expect_no_ex = options.expected_exception == TAO::FOE_NON &&
        worker.num_received_ex () == 0 && worker.invocation_completed ();
      bool const expect_ex_received = options.expected_exception == worker.received_ex_kind () &&
        worker.num_received_ex () > 0 && !worker.invocation_completed ();
      if (expect_no_ex || expect_ex_received)
        {
          ACE_DEBUG ((LM_DEBUG, "(%P|%t)client: test passed.\n"));
          return 0;
        }
      else
        {
          ACE_DEBUG ((LM_ERROR, "(%P|%t)client: test failed.\n"));
          return 1;
        }
    }

  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught in main:");
      return 1;
    }

  return 0;
}

// ****************************************************************

Worker::Worker (CORBA::ORB_ptr orb, ACE_TString const &ior)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    ior_ (ior),
    received_ex_kind_ (TAO::FOE_NON),
    num_received_ex_ (0),
    invocation_completed_ (false),
    done_ (false)
{
}

int
Worker::svc ()
{
  try
    {
      CORBA::Object_var const object =
        this->orb_->string_to_object (this->ior_.c_str ());

      Simple_Server_var const server =
        Simple_Server::_narrow (object.in ());

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR ((LM_ERROR,
                      "Object reference <%s> is nil.\n",
                      this->ior_.c_str ()));
          return 0;
        }

      try
        {
          CORBA::Boolean const result = server->test_is_a ("IDL:Foo:1.0");

          this->invocation_completed_ = true;

          if (result != 0)
            ACE_DEBUG ((LM_DEBUG,
                        "(%P|%t) unexpected result = %d\n",
                        result));
        }
      catch (CORBA::OBJECT_NOT_EXIST const &)
        {
          ACE_DEBUG ((LM_DEBUG, "(%P|%t)received OBJECT_NOT_EXIST \n"));
          if (!this->done_)
            {
              ++ this->num_received_ex_;
              this->received_ex_kind_ |= TAO::FOE_OBJECT_NOT_EXIST;
            }
        }
      catch (CORBA::COMM_FAILURE const &)
        {
          ACE_DEBUG ((LM_DEBUG, "(%P|%t)received COMM_FAILURE \n"));
          if (!this->done_)
            {
              ++ this->num_received_ex_;
              this->received_ex_kind_ |= TAO::FOE_COMM_FAILURE;
            }
        }
      catch (CORBA::TRANSIENT const &)
        {
          ACE_DEBUG ((LM_DEBUG, "(%P|%t)received TRANSIENT \n"));
          if (!this->done_)
            {
              ++ this->num_received_ex_;
              this->received_ex_kind_ |= TAO::FOE_TRANSIENT;
            }
        }
      catch (CORBA::INV_OBJREF const &)
        {
          ACE_DEBUG ((LM_DEBUG, "(%P|%t)received INV_OBJREF \n"));
          if (!this->done_)
            {
              ++ this->num_received_ex_;
              this->received_ex_kind_ |= TAO::FOE_INV_OBJREF;
            }
        }
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Unexpected exception caught");
    }

  return 0;
}

int
Worker::received_ex_kind () const
{
  return received_ex_kind_;
}

int
Worker::num_received_ex () const
{
  return num_received_ex_;
}

bool
Worker::invocation_completed () const
{
  return invocation_completed_;
}

void
Worker::done ()
{
  done_ = true;
}
