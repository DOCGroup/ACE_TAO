// $Id$

#include "testC.h"
#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include "ace/OS.h"
#include "ace/streams.h"
#include "tao/Invocation_Utils.h"

const ACE_TCHAR *ior = ACE_TEXT("file://test.ior");
int nthreads = 1;
int do_shutdown = 0;
static const ACE_TCHAR corbaloc_prefix[] = ACE_TEXT("corbaloc:");
int expect_ex_kind = TAO::FOE_NON;
int num_requests = 1;
int expect_num_ex = 1;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("xk:e:i:n:"));
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
        expect_ex_kind = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'i':
        num_requests = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'n':
        expect_num_ex = ACE_OS::atoi (get_opts.opt_arg ());
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

    if (ACE_OS::strncmp (ior,
                          corbaloc_prefix,
                          ACE_OS::strlen(corbaloc_prefix)) != 0)
      return 1;

  // Indicates sucessful parsing of the command line
  return 0;
}

class Worker : public ACE_Task_Base
{
public:
  Worker (CORBA::ORB_ptr orb);
  // Constructor

  // = The Task_Base methods
  virtual int svc (void);

  // Caught any exception ?
  int received_ex_kind () const;

  // Return number of received exceptions.
  int num_received_ex () const;

  // Is test done ?
  void done ();

private:

  // The ORB reference
  CORBA::ORB_var orb_;
  // The exceptions caught.
  int received_ex_kind_;
  // The number of received exceptions.
  int num_received_ex_;
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
      while (now < timeout
        && ((expect_ex_kind != TAO::FOE_NON
             && worker.received_ex_kind () != expect_ex_kind
             && worker.num_received_ex () != expect_num_ex)
            || expect_ex_kind == TAO::FOE_NON))
      {
        std::cout << ".";
        now += 1;
        ACE_Time_Value tv (1, 0);
        orb->run (tv);
      }
      std::cout << std::endl;

      worker.done ();

      if (do_shutdown)
        {
          CORBA::Object_var object =
            orb->string_to_object (ior);

          Simple_Server_var server =
            Simple_Server::_narrow (object.in ());

          server->shutdown ();
        }

      ACE_OS::sleep (1);

      orb->destroy ();

      worker.thr_mgr ()->wait ();

      if (worker.received_ex_kind () != expect_ex_kind
        || worker.num_received_ex () != expect_num_ex)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
          ("(%P|%t)client: test failed - expected is different from received. "
          "expected %d/%d received %d/%d.\n"),
          expect_ex_kind, expect_num_ex, worker.received_ex_kind (), worker.num_received_ex()),
                          1);
      }

      ACE_DEBUG ((LM_DEBUG, "(%P|%t)client: test passed.\n"));
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
:    orb_ (CORBA::ORB::_duplicate (orb)),
     received_ex_kind_ (TAO::FOE_NON),
     num_received_ex_ (0),
     done_ (false)
{
}

int
Worker::svc (void)
{
  try
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
          return 0;
        }

      for (int i = 0; i < num_requests; ++i)
      {
        try {
          CORBA::Boolean r =
            server->test_is_a ("IDL:Foo:1.0");

          if (r != 0)
            ACE_DEBUG ((LM_DEBUG,
                        "(%P|%t) unexpected result = %d\n",
                        r));

        }
        catch (const CORBA::OBJECT_NOT_EXIST &)
          {
            //ACE_DEBUG ((LM_DEBUG, "(%P|%t)received OBJECT_NOT_EXIST \n"));
            if (!this->done_)
            {
              ++ this->num_received_ex_;
              received_ex_kind_ |= TAO::FOE_OBJECT_NOT_EXIST;
            }
          }
        catch (const CORBA::COMM_FAILURE &)
          {
            //ACE_DEBUG ((LM_DEBUG, "(%P|%t)received COMM_FAILURE \n"));
            if (!this->done_)
            {
              ++ this->num_received_ex_;
              received_ex_kind_ |= TAO::FOE_COMM_FAILURE;
            }
          }
        catch (const CORBA::TRANSIENT &)
          {
            //ACE_DEBUG ((LM_DEBUG, "(%P|%t)received TRANSIENT \n"));
            if (!this->done_)
            {
              ++ this->num_received_ex_;
              received_ex_kind_ |= TAO::FOE_TRANSIENT;
            }
          }
        catch (const CORBA::INV_OBJREF &)
          {
            //ACE_DEBUG ((LM_DEBUG, "(%P|%t)received INV_OBJREF \n"));
            if (!this->done_)
            {
              ++ this->num_received_ex_;
              received_ex_kind_ |= TAO::FOE_INV_OBJREF;
            }
          }
       }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "Unexpected exception caught");
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

void
Worker::done ()
{
  done_ = true;
}

