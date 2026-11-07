#include "testC.h"
#include "tao/debug.h"
#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include "ace/SString.h"

struct Client_Options
{
  ACE_TString ior {ACE_TEXT ("file://test.ior")};
  int nthreads {5};
  int niterations {5};
  bool server_shutdown {};
};

int
parse_args (int argc, ACE_TCHAR *argv[], Client_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:n:i:x"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        options.ior = get_opts.opt_arg ();
        break;
      case 'n':
        options.nthreads = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case 'i':
        options.niterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case 'x':
        options.server_shutdown = true;
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage: %s -k <ior> -n <nthreads> -i <niterations> -x\n"),
                           argv[0]),
                          -1);
      }
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
  Client (int niterations, ACE_TString const &ior);
  // ctor
  Client (Client const &) = delete;
  Client (Client &&) = delete;
  Client &operator= (Client const &) = delete;
  Client &operator= (Client &&) = delete;

  int svc () override;
  // The thread entry point.

private:
  int const niterations_;
  // The number of iterations on each client thread.

  ACE_TString const ior_;
  // The IOR that we should use.
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

      CORBA::Object_var const object = orb->string_to_object (options.ior.c_str ());

      Simple_Server_var const server =
        Simple_Server::_narrow (object.in ());

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Object reference <%s> is nil.\n"),
                             options.ior.c_str ()),
                            1);
        }

      Client client (options.niterations, options.ior);
      if (client.activate (THR_NEW_LWP | THR_JOINABLE,
                           options.nthreads) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Cannot activate client threads\n")),
                          1);

      client.thr_mgr ()->wait ();

      ACE_DEBUG ((LM_DEBUG, "threads finished\n"));

      if (options.server_shutdown)
        {
          server->shutdown ();
        }
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}

// ****************************************************************

Client::Client (int niterations,
                ACE_TString const &ior)
  : niterations_ (niterations),
    ior_ (ior)
{
}

int
Client::svc ()
{
  try
    {
      for (int i {}; i < this->niterations_; ++i)
        {
          // If we are using a global ORB this is a nop, otherwise it
          // initializes the ORB resources for this thread.
          int argc {1};
          ACE_TCHAR argv0[] = ACE_TEXT ("dummy_argv");
          ACE_TCHAR *argv[] {argv0};
          CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

          CORBA::Object_var const object = orb->string_to_object (this->ior_.c_str ());

          Simple_Server_var const server =
            Simple_Server::_narrow (object.in ());

          if (CORBA::is_nil (server.in ()))
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 ACE_TEXT ("(%P|%t) Object reference <%s> is nil\n"),
                                 this->ior_.c_str ()),
                                1);
            }

          server->test_method ();
          if (TAO_debug_level > 0 && i % 100 == 0)
            ACE_DEBUG ((LM_DEBUG, "(%P|%t) iteration = %d\n", i));
        }
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("MT_Client: exception raised");
    }
  return 0;
}
