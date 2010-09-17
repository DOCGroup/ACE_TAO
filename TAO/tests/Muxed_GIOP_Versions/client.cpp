// $Id$

#include "testC.h"
#include "tao/debug.h"
#include "ace/Get_Opt.h"
#include "ace/Task.h"

const ACE_TCHAR *ior = ACE_TEXT("file://test.ior");
const ACE_TCHAR *corbaloc_arg = ACE_TEXT("corbaloc:iiop:1.0@localhost:12000/ObjectName");
int nthreads = 5;
int niterations = 5;
int server_shutdown = 0;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("l:k:n:i:x"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;
      case 'l':
        corbaloc_arg = get_opts.opt_arg ();
        break;
      case 'n':
        nthreads = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case 'i':
        niterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case 'x':
        server_shutdown = 1;
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
  // Indicates successful parsing of the command line
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
  Client (Simple_Server_ptr server, int niterations);
  // ctor

  virtual int svc (void);
  // The thread entry point.

private:
  void validate_connection (void);
  // Validate the connection

private:
  Simple_Server_var server_;
  // The server.

  int niterations_;
  // The number of iterations on each client thread.
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

      Client client (server.in (), niterations);
      if (client.activate (THR_NEW_LWP | THR_JOINABLE,
                           nthreads) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot activate client threads\n"),
                          1);

      // While those threads are busy sending 1.2 messages
      // send some corba loc requests, don't really care if there is an
      // object there to find
      if (!server_shutdown)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "starting string_to_object %s\n",
                      corbaloc_arg));

          for (int c = 0; c < (niterations * 2); c++)
            {
              try
                {
                  CORBA::Object_var probably_not_exist =
                    orb->string_to_object(corbaloc_arg);


                  if (CORBA::is_nil(probably_not_exist.in()))
                    {
                      ACE_DEBUG ((LM_DEBUG, "not found\n", corbaloc_arg));
                    }
                  else
                    {
                      Simple_Server_var newserver =
                        Simple_Server::_narrow (probably_not_exist.in ());

                      // should throw an exception
                      if (CORBA::is_nil(newserver.in()))
                        {
                          ACE_DEBUG ((LM_DEBUG, "not found it\n", corbaloc_arg));
                        }
                      else
                        {
                          ACE_DEBUG ((LM_DEBUG, "found it\n", corbaloc_arg));
                        }
                    }
                }
              catch (const CORBA::Exception&)
                {
                  // ACE_DEBUG ((LM_DEBUG, "caught exception\n", corbaloc_arg));
                }
            }
        }
      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) waiting for threads\n"));

      client.thr_mgr ()->wait ();

      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) threads finished\n"));

      if (server_shutdown)
        {
          server->shutdown ();
        }

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return 1;
    }

  return 0;
}

// ****************************************************************

Client::Client (Simple_Server_ptr server,
                int niterations)
  :  server_ (Simple_Server::_duplicate (server)),
     niterations_ (niterations)
{
}

void
Client::validate_connection (void)
{
  // Ping the object 100 times, ignoring all exceptions.
  // It would be better to use validate_connection() but the test must
  // run on minimum CORBA builds too!
  for (int j = 0; j != 100; ++j)
    {
      try
        {
          this->server_->test_method (j);
        }
      catch (const CORBA::Exception&){}
    }
}

int
Client::svc (void)
{
  try
    {
      this->validate_connection ();

      for (int i = 0; i < this->niterations_; ++i)
        {
          this->server_->test_method (i);

          if (TAO_debug_level > 0 && i % 100 == 0)
            ACE_DEBUG ((LM_DEBUG, "(%P|%t) iteration = %d\n",
                        i));
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("MT_Client: exception raised");
    }
  return 0;
}
