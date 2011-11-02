// $Id$

#include "testC.h"
#include "ace/Get_Opt.h"
#include "ace/Task.h"

const ACE_TCHAR *ior = ACE_TEXT("file://test.ior");
int nthreads = 1;
int do_shutdown = 0;
static const ACE_TCHAR corbaloc_prefix[] = ACE_TEXT("corbaloc:");
int expected_object_not_exist = 0;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("xk:e:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'x':
        do_shutdown = 1;
        break;

      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case 'e':
        expected_object_not_exist = ACE_OS::atoi (get_opts.opt_arg ());
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

    if (ACE_OS::strncmp (ior,
                          corbaloc_prefix,
                          ACE_OS::strlen (corbaloc_prefix)) != 0)
      return 1;

  // Indicates successful parsing of the command line
  return 0;
}

class Worker : public ACE_Task_Base
{
public:
  Worker (CORBA::ORB_ptr orb);
  // Constructor

  virtual void run_test (void);
  // The actual implementation of the test

  // = The Task_Base methods
  virtual int svc (void);

  // Caught OBJECT_NOT_EXIST exception ?
  int got_object_not_exist () const;

private:
  CORBA::ORB_var orb_;
  // The ORB reference
  int got_object_not_exist_;
};

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      Worker worker (orb.in ());

      if (worker.activate (THR_NEW_LWP | THR_JOINABLE,
                           nthreads) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) Cannot activate worker threads\n"),
                          1);

      int timeout = 30;
      int now = 0;
      while (now < timeout && worker.got_object_not_exist () != expected_object_not_exist)
      {
        now += 5;
        ACE_Time_Value tv (5, 0);
        orb->run (tv);
      }

      if (do_shutdown)
        {
          CORBA::Object_var object =
            orb->string_to_object (ior);

          Simple_Server_var server =
            Simple_Server::_narrow (object.in ());

          server->shutdown ();
        }

      orb->destroy ();

      worker.thr_mgr ()->wait ();

      if (worker.got_object_not_exist () != expected_object_not_exist)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
          "(%P|%t)client: test failed.\n"),
                          1);
      }
      else
      {
        ACE_DEBUG ((LM_DEBUG, "(%P|%t)client: test passed.\n"));
      }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught in main:");
      return 1;
    }

  return 0;
}

// ****************************************************************

Worker::Worker (CORBA::ORB_ptr orb)
  :  orb_ (CORBA::ORB::_duplicate (orb)),
     got_object_not_exist_ (0)
{
}

int
Worker::svc (void)
{
  try
    {
      this->run_test ();
    }
  catch (const CORBA::OBJECT_NOT_EXIST &)
    {
      got_object_not_exist_ = 1;
    }
  catch (...)
    {
    }

  return 0;
}

void
Worker::run_test (void)
{
  CORBA::Object_var object =
    this->orb_->string_to_object (ior);

  Simple_Server_var server =
    Simple_Server::_narrow (object.in ());

  if (CORBA::is_nil (server.in ()))
    {
      ACE_ERROR ((LM_ERROR,
                  "Object reference <%s> is nil.\n",
                  ior));
      return;
    }

  CORBA::Boolean r =
    server->test_is_a ("IDL:Foo:1.0");

  if (r != 0)
    ACE_DEBUG ((LM_DEBUG,
                "(%P|%t) unexpected result = %d\n",
                r));
}

int
Worker::got_object_not_exist () const
{
  return got_object_not_exist_;
}
