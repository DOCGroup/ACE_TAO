// $Id$

#include "testC.h"
#include "ace/Get_Opt.h"
#include "ace/Task.h"

ACE_RCSID(Forwarding, client, "$Id$")

const char *ior = "file://test.ior";
int niterations = 10;
int nthreads = 1;
int do_shutdown = 0;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "xk:i:");
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
  // Indicates sucessful parsing of the command line
  return 0;
}

class Worker : public ACE_Task_Base
{
public:
  Worker (CORBA::ORB_ptr orb);
  // Constructor

  virtual void run_test (TAO_ENV_SINGLE_ARG_DECL);
  // The actual implementation of the test

  // = The Task_Base methods
  virtual int svc (void);

private:
  CORBA::ORB_var orb_;
  // The ORB reference
};

int
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return 1;

      Worker worker (orb.in ());

      if (worker.activate (THR_NEW_LWP | THR_JOINABLE,
                           nthreads) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) Cannot activate worker threads\n"),
                          1);

      ACE_Time_Value tv (5, 0);

      orb->run (tv TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Now run a test in the main thread, just to confuse matters a
      // little more.
      worker.run_test (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      worker.thr_mgr ()->wait ();

      if (do_shutdown)
        {
          CORBA::Object_var object =
            orb->string_to_object (ior TAO_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          Simple_Server_var server =
            Simple_Server::_narrow (object.in () TAO_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          server->shutdown (TAO_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      orb->destroy (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception caught in main:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}

// ****************************************************************

Worker::Worker (CORBA::ORB_ptr orb)
  :  orb_ (CORBA::ORB::_duplicate (orb))
{
}

int
Worker::svc (void)
{
  ACE_TRY_NEW_ENV
    {
      this->run_test (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception caught in thread (%t)\n");
    }
  ACE_ENDTRY;

  return 0;
}

void
Worker::run_test (TAO_ENV_SINGLE_ARG_DECL)
{
  for (int j = 0; j != niterations; ++j)
    {
      CORBA::Object_var object =
        this->orb_->string_to_object (ior TAO_ENV_ARG_PARAMETER);
      ACE_CHECK;

      CORBA::Boolean is_simple_server =
        object->_is_a ("IDL:Simple_Server:1.0" TAO_ENV_ARG_PARAMETER);
      ACE_CHECK;
      if (!is_simple_server)
        ACE_DEBUG ((LM_DEBUG,
                    "(%P|%t) unexpected result from _is_a()\n"));
    }

  CORBA::Object_var object =
    this->orb_->string_to_object (ior TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  Simple_Server_var server =
    Simple_Server::_narrow (object.in () TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (CORBA::is_nil (server.in ()))
    {
      ACE_ERROR ((LM_ERROR,
                  "Object reference <%s> is nil\n",
                  ior));
      return;
    }

  for (int i = 0; i != niterations; ++i)
    {
      CORBA::Boolean r =
        server->test_is_a ("IDL:Foo:1.0" TAO_ENV_ARG_PARAMETER);
      ACE_CHECK;

      if (r != 0)
        ACE_DEBUG ((LM_DEBUG,
                    "(%P|%t) unexpected result = %d\n",
                    r));
    }
}
