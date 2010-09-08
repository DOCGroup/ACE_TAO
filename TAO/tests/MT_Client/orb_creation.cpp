// $Id$

#include "testC.h"
#include "tao/debug.h"
#include "ace/Get_Opt.h"
#include "ace/Task.h"

const ACE_TCHAR *ior = ACE_TEXT("file://test.ior");
int nthreads = 5;
int niterations = 5;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:n:i:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;
      case 'n':
        nthreads = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case 'i':
        niterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "-n <nthreads> "
                           "-i <niterations> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

class Client : public ACE_Task_Base
{
  // = TITLE
  //   Run the client thread
  //
  // = DESCRIPTION
  //   Use the ACE_Task_Base class to run the client threads.
  //
public:
  Client (int niterations,
          const char* ior);
  // ctor

  virtual int svc (void);
  // The thread entry point.

private:
  int niterations_;
  // The number of iterations on each client thread.

  const char* ior_;
  // The IOR that we should use.
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

      CORBA::Object_var object =
        orb->string_to_object (ior);

      Simple_Server_var server =
        Simple_Server::_narrow (object.in ());

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Object reference <%s> is nil.\n",
                             ior),
                            1);
        }

      Client client (niterations, ior);
      if (client.activate (THR_NEW_LWP | THR_JOINABLE,
                           nthreads) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot activate client threads\n"),
                          1);

      client.thr_mgr ()->wait ();

      ACE_DEBUG ((LM_DEBUG, "threads finished\n"));

      server->shutdown ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}

// ****************************************************************

Client::Client (int niterations,
                const char* ior)
  :  niterations_ (niterations),
     ior_ (ior)
{
}

int
Client::svc (void)
{
  try
    {
      for (int i = 0; i < this->niterations_; ++i)
        {
          // If we are using a global ORB this is a nop, otherwise it
          // initializes the ORB resources for this thread.
          int argc = 0;
          CORBA::String_var argv0 = CORBA::string_dup ("dummy_argv");
          char* argv[1] = { argv0.inout () };
          CORBA::ORB_var orb =
            CORBA::ORB_init (argc, argv);

          CORBA::Object_var object =
            orb->string_to_object (this->ior_);

          Simple_Server_var server =
            Simple_Server::_narrow (object.in ());

          if (CORBA::is_nil (server.in ()))
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%P|%t) Object reference <%s> is nil\n",
                                 ior),
                                1);
            }

          server->test_method ();
          if (TAO_debug_level > 0 && i % 100 == 0)
            ACE_DEBUG ((LM_DEBUG, "(%P|%t) iteration = %d\n", i));
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("MT_Client: exception raised");
    }
  return 0;
}
