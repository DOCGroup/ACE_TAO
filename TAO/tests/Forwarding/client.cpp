#include "testC.h"
#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include <string>

std::basic_string<ACE_TCHAR> ior = ACE_TEXT ("file://test.ior");
int niterations = 10;
constexpr int nthreads = 1;
bool do_shutdown {};

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("xk:i:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'x':
        do_shutdown = true;
        break;

      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case 'i':
        niterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "-i <niterations> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

class Worker : public ACE_Task_Base
{
public:
  explicit Worker (CORBA::ORB_ptr orb);
  // Constructor

  void run_test ();
  // The actual implementation of the test

  // = The Task_Base methods
  int svc () override;

  Worker (Worker const&) = delete;
  Worker (Worker&&) = delete;
  Worker& operator= (Worker const&) = delete;
  Worker& operator= (Worker&&) = delete;

private:
  CORBA::ORB_var const orb_;
  // The ORB reference
};

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      Worker worker (orb.in ());

      if (worker.activate (THR_NEW_LWP | THR_JOINABLE,
                           nthreads) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) Cannot activate worker threads\n"),
                          1);

      ACE_Time_Value tv (5);

      orb->run (tv);

      // Now run a test in the main thread, just to confuse matters a
      // little more.
      worker.run_test ();

      worker.thr_mgr ()->wait ();

      if (do_shutdown)
        {
          CORBA::Object_var const object =
            orb->string_to_object (ior.c_str ());

          Simple_Server_var const server =
            Simple_Server::_narrow (object.in ());

          server->shutdown ();
        }

      orb->destroy ();
    }
  catch (CORBA::Exception const& ex)
    {
      ex._tao_print_exception ("Exception caught in main:");
      return 1;
    }

  return 0;
}

// ****************************************************************

Worker::Worker (CORBA::ORB_ptr orb)
  :  orb_ (CORBA::ORB::_duplicate (orb))
{
}

int
Worker::svc ()
{
  try
    {
      this->run_test ();
    }
  catch (CORBA::Exception const& ex)
    {
      ex._tao_print_exception ("Exception caught in thread (%t)\n");
    }

  return 0;
}

void
Worker::run_test ()
{
  for (int j = 0; j != niterations; ++j)
    {
      CORBA::Object_var const object =
        this->orb_->string_to_object (ior.c_str ());

      CORBA::Boolean const is_simple_server =
        object->_is_a ("IDL:Simple_Server:1.0");
      if (!is_simple_server)
        ACE_DEBUG ((LM_DEBUG,
                    "(%P|%t) unexpected result from _is_a()\n"));
    }

  CORBA::Object_var const object =
    this->orb_->string_to_object (ior.c_str ());

  Simple_Server_var const server =
    Simple_Server::_narrow (object.in ());

  if (CORBA::is_nil (server.in ()))
    {
      ACE_ERROR ((LM_ERROR,
                  "Object reference <%s> is nil.\n",
                  ior.c_str ()));
      return;
    }

  for (int i = 0; i != niterations; ++i)
    {
      CORBA::Boolean const r =
        server->test_is_a ("IDL:Foo:1.0");

      if (r != 0)
        ACE_DEBUG ((LM_DEBUG,
                    "(%P|%t) unexpected result = %d\n",
                    r));
    }
}
