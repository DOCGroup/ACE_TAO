#include "testC.h"
#include "ace/Task.h"
#include "ace/Get_Opt.h"

int
parse_args (int argc, ACE_TCHAR *argv[], ACE_TCHAR const *&ior, int &n_tasks,
            bool &server_shutdown)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:c:x"));
  int option {};

  while ((option = get_opts ()) != -1)
    switch (option)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;
      case 'c':
        n_tasks = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case 'x':
        server_shutdown = false;
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "[-x] [-c concurrent_tasks]"
                           "\n",
                           argv [0]),
                          -1);
      }
  return 0;
}


class Client : public ACE_Task_Base
{
public:
  // ctor
  explicit Client (test_ptr server);
  Client (Client const &) = delete;
  Client &operator= (Client const &) = delete;
  Client (Client &&) = delete;
  Client &operator= (Client &&) = delete;

  int svc () override;

  // The server.
  test_var const server_;
};

Client::Client (test_ptr server)
  : server_ (test::_duplicate (server))
{
}

int
Client::svc ()
{
  try
    {
      this->server_->test_method ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Client: exception raised");
    }
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_TCHAR const *ior = ACE_TEXT ("file://test.ior");
  int n_tasks = 45;
  bool server_shutdown = true;
  try
    {
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv, ior, n_tasks, server_shutdown) != 0)
        return 1;

      CORBA::Object_var const obj = orb->string_to_object (ior);
      test_var const server = test::_narrow (obj.in ());
      if (CORBA::is_nil (server.in ()))
        ACE_ERROR_RETURN ((LM_DEBUG, "Nil reference <%s>\n", ior), 1);

      Client client (server.in ());
      if (client.activate (THR_NEW_LWP | THR_JOINABLE, n_tasks) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                            "Cannot activate client threads\n"),
                          1);

      client.thr_mgr ()->wait ();

      if (server_shutdown)
        {
          server->shutdown ();
        }

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught");
      return -1;
    }

  return 0;
}
