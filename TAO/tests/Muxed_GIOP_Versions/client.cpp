#include "testC.h"
#include "tao/debug.h"
#include "ace/Get_Opt.h"
#include "ace/Task.h"

struct Client_Options
{
  ACE_TCHAR const *ior {ACE_TEXT ("file://test.ior")};
  ACE_TCHAR const *corbaloc {ACE_TEXT ("corbaloc:iiop:1.0@localhost:12000/ObjectName")};
  int thread_count {5};
  int iterations {5};
  bool shutdown_server {};
};

static int
parse_args (int argc, ACE_TCHAR *argv[], Client_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("l:k:n:i:x"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        options.ior = get_opts.opt_arg ();
        break;
      case 'l':
        options.corbaloc = get_opts.opt_arg ();
        break;
      case 'n':
        options.thread_count = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case 'i':
        options.iterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case 'x':
        options.shutdown_server = true;
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage:  %s "
                                     "-k <ior> "
                                     "-l <corbaloc> "
                                     "-n <nthreads> "
                                     "-i <niterations> "
                                     "[-x] "
                                     "\n"),
                           argv [0]),
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
  Client (Simple_Server_ptr server, Client_Options const &options);

  Client (Client const &) = delete;
  Client (Client &&) = delete;
  Client &operator= (Client const &) = delete;
  Client &operator= (Client &&) = delete;

  int svc () override;

private:
  void validate_connection () const;

  Simple_Server_var server_ {};
  ACE_TString corbaloc_;
  int const iterations_;
};

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      Client_Options options {};
      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv, options) != 0)
        return 1;

      CORBA::Object_var const object =
        orb->string_to_object (options.ior);

      Simple_Server_var const server =
        Simple_Server::_narrow (object.in ());

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Object reference <%s> is nil.\n"),
                             options.ior),
                            1);
        }

      Client client (server.in (), options);
      if (client.activate (THR_NEW_LWP | THR_JOINABLE,
                           options.thread_count) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Cannot activate client threads\n")),
                          1);

      // While those threads are busy sending 1.2 messages
      // send some corba loc requests, don't really care if there is an
      // object there to find
      if (!options.shutdown_server)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("starting string_to_object %s\n"),
                      options.corbaloc));

          for (int c {}; c < (options.iterations * 2); ++c)
            {
              try
                {
                  CORBA::Object_var const probably_not_exist =
                    orb->string_to_object (options.corbaloc);

                  if (CORBA::is_nil (probably_not_exist.in ()))
                    {
                      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("not found\n")));
                    }
                  else
                    {
                      Simple_Server_var const new_server =
                        Simple_Server::_narrow (probably_not_exist.in ());

                      // should throw an exception
                      if (CORBA::is_nil (new_server.in ()))
                        {
                          ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("not found it\n")));
                        }
                      else
                        {
                          ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("found it\n")));
                        }
                    }
                }
              catch (CORBA::Exception const &)
                {
                  // The test intentionally tries to resolve a nonexistent object.
                }
            }
        }
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) waiting for threads\n")));

      client.thr_mgr ()->wait ();

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) threads finished\n")));

      if (options.shutdown_server)
        {
          server->shutdown ();
        }

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return 1;
    }

  return 0;
}

// ****************************************************************

Client::Client (Simple_Server_ptr server,
                Client_Options const &options)
  : server_ (Simple_Server::_duplicate (server)),
    corbaloc_ (options.corbaloc),
    iterations_ (options.iterations)
{
}

void
Client::validate_connection () const
{
  // Ping the object 100 times, ignoring all exceptions.
  // It would be better to use validate_connection() but the test must
  // run on minimum CORBA builds too!
  for (int j {}; j != 100; ++j)
    {
      try
        {
          this->server_->test_method (j);
        }
      catch (CORBA::Exception const &)
        {
        }
    }
}

int
Client::svc ()
{
  try
    {
      this->validate_connection ();

      for (int i {}; i < this->iterations_; ++i)
        {
          this->server_->test_method (i);

          if (TAO_debug_level > 0 && i % 100 == 0)
            ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) iteration = %d\n"),
                        i));
        }
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("MT_Client: exception raised");
    }
  return 0;
}
