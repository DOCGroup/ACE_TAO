// $Id$

#include "test_i.h"
#include "ace/Get_Opt.h"
#include "ace/Task.h"

const ACE_TCHAR *ior_output_file = 0;
const ACE_TCHAR *corbaloc_arg = ACE_TEXT("corbaloc:iiop:1.0@localhost:12000/ObjectName");
int niterations = 5;

int nthreads = 4;
int nclient_threads = nthreads;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("c:l:i:o:n:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'c':
        nclient_threads = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'l':
        corbaloc_arg = get_opts.opt_arg ();
        break;

      case 'i':
        niterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;

      case 'n':
        nthreads = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-c <# client threads> "
                           "-i <# iterations> "
                           "-l <corba loc> "
                           "-n <# server threads> "
                           "-o <iorfile> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

/*****************************************************/

class Worker : public ACE_Task_Base
{
  // = TITLE
  //   Run a server thread
  //
  // = DESCRIPTION
  //   Use the ACE_Task_Base class to run server threads
  //
public:
  /// ctor
  Worker (CORBA::ORB_ptr orb);

  /// The thread entry point.
  virtual int svc (void);

private:

  /// The orb
  CORBA::ORB_var orb_;
};

/*****************************************************/

class SelfClient : public ACE_Task_Base
{
  // = TITLE
  //   Run the client thread
  //
  // = DESCRIPTION
  //   Use the ACE_Task_Base class to run the client threads.
  //
public:
  SelfClient (CORBA::ORB_ptr orb, Simple_Server_ptr server, int niterations);
  // ctor

  virtual int svc (void);
  // The thread entry point.

private:
  void validate_connection (void);
  // Validate the connection

private:

  /// The server.
  Simple_Server_var server_;

  CORBA::ORB_var orb_;

  /// The number of iterations on each client thread.
  int niterations_;


  TAO_SYNCH_MUTEX mutex_;
};

/***************************************************/
int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var poa_object =
        orb->resolve_initial_references ("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      if (parse_args (argc, argv) != 0)
        return 1;

      Simple_Server_i *server_impl = 0;
      ACE_NEW_RETURN (server_impl,
                      Simple_Server_i (orb.in ()),
                      -1);

      PortableServer::ServantBase_var owner_transfer(server_impl);

      PortableServer::ObjectId_var id =
        root_poa->activate_object (server_impl);

      CORBA::Object_var object = root_poa->id_to_reference (id.in ());

      Simple_Server_var server =
        Simple_Server::_narrow (object.in ());

      CORBA::String_var ior =
        orb->object_to_string (server.in ());

      ACE_DEBUG ((LM_DEBUG, "Activated as <%C>\n", ior.in ()));

      // If the ior_output_file exists, output the ior to it
      if (ior_output_file != 0)
        {
          FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
          if (output_file == 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "(%P|%t) Cannot open output file for writing IOR: %s",
                               ior_output_file),
                              1);
          ACE_OS::fprintf (output_file, "%s", ior.in ());
          ACE_OS::fclose (output_file);
        }

      poa_manager->activate ();

      Worker worker (orb.in ());
      if (worker.activate (THR_NEW_LWP | THR_JOINABLE,
                           nthreads) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) Cannot activate client threads\n"),
                          1);

      SelfClient selfabuse (orb.in(), server.in(), niterations);
      if (selfabuse.activate (THR_NEW_LWP | THR_JOINABLE,
                              nclient_threads) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) Cannot activate abusive threads\n"),
                          1);

      selfabuse.thr_mgr()->wait();

      worker.thr_mgr ()->wait ();

      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) event loop finished\n"));
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
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
Worker::svc (void)
{
  try
    {
      ACE_Time_Value tv (140, 0);
      this->orb_->run (tv);
    }
  catch (const CORBA::Exception&)
    {
    }
  return 0;
}

// ****************************************************************

SelfClient::SelfClient (CORBA::ORB_ptr orb, Simple_Server_ptr server,
                int niterations)
  :  server_ (Simple_Server::_duplicate (server)),
         orb_ (CORBA::ORB::_duplicate (orb)),
     niterations_ (niterations)
{
}

void
SelfClient::validate_connection (void)
{
  // Ping the object 100 times, ignoring all exceptions.
  // It would be better to use validate_connection() but the test must
  // run on minimum CORBA builds too!
  ACE_GUARD (TAO_SYNCH_MUTEX, guard, mutex_);

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
SelfClient::svc (void)
{
  try
    {
      this->validate_connection ();

      for (int i = 0; i < this->niterations_; ++i)
        {
          try
            {
              CORBA::Object_var probably_not_exist =
                orb_->string_to_object (corbaloc_arg);


              if (CORBA::is_nil (probably_not_exist.in()))
                {
                  ACE_DEBUG ((LM_DEBUG, "not found\n", corbaloc_arg));
                }
              else
                {
                  Simple_Server_var newserver =
                    Simple_Server::_narrow (probably_not_exist.in ());

                  // should throw an exception
                  if (CORBA::is_nil (newserver.in()))
                    {
                      ACE_DEBUG ((LM_DEBUG,
                                  "(%P|%t)  Not found it\n"));

                    }
                  else
                    {
                      ACE_DEBUG ((LM_DEBUG,
                                  "(%P|%t) Found it\n"));
                    }
                }

            }
          catch (const CORBA::Exception& ex)
            {
              ex._tao_print_exception ("MT_SelfClient: exception raised");
            }

          // Just make a call
          this->server_->test_method (i);

        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("MT_SelfClient: exception raised");
    }
  return 0;
}
