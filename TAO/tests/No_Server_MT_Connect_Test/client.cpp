#include "testC.h"
#include "tao/debug.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdlib.h"
#include "ace/Task.h"

struct Client_Options
{
  ACE_TCHAR const *ior {ACE_TEXT ("file://test.ior")};
  int thread_count {5};
  int iteration_count {5};
  bool shutdown {};
};

static int
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
        options.thread_count = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case 'i':
        options.iteration_count = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case 'x':
        options.shutdown = true;
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage: %s [-k <ior>] [-n <threads>] "
                                     "[-i <iterations>] [-x]\n"),
                           argv [0]),
                          -1);
      }

  return 0;
}

class Client : public ACE_Task_Base
{
public:
  Client (Simple_Server_ptr server, int iteration_count)
    : server_ (Simple_Server::_duplicate (server))
    , iteration_count_ (iteration_count)
  {
  }

  Client (Client const &) = delete;
  Client &operator= (Client const &) = delete;
  Client (Client &&) = delete;
  Client &operator= (Client &&) = delete;

  int svc () override;

private:
  Simple_Server_var const server_;
  int const iteration_count_;
};

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var const orb (CORBA::ORB_init (argc, argv));
      Client_Options options {};

      if (parse_args (argc, argv, options) != 0)
        return 1;

      CORBA::Object_var const object (orb->string_to_object (options.ior));
      Simple_Server_var const server (Simple_Server::_narrow (object.in ()));

      if (CORBA::is_nil (server.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Object reference <%s> is nil.\n"),
                           options.ior),
                          1);

      if (options.shutdown)
        server->shutdown ();

      Client client (server.in (), options.iteration_count);
      if (client.activate (THR_NEW_LWP | THR_JOINABLE,
                           options.thread_count) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Cannot activate client threads\n")),
                          1);

      client.thr_mgr ()->wait ();

      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("threads finished\n")));

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return 1;
    }

  return 0;
}

int
Client::svc ()
{
  try
    {
      for (int i {}; i < this->iteration_count_; ++i)
        {
          this->server_->test_method ();

          if (TAO_debug_level > 0 && i % 100 == 0)
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("(%P|%t) iteration = %d\n"),
                        i));
        }
    }
  catch (CORBA::Exception const &)
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) Expected exceptions\n")));
    }
  return 0;
}
