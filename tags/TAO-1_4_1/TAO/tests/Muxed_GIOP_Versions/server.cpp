// $Id$

#include "test_i.h"
#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include "ace/Synch_T.h"

ACE_RCSID(MT_Server, server, "server.cpp,v 1.7 2002/01/29 20:21:08 okellogg Exp")

const char *ior_output_file = 0;
const char *corbaloc_arg = "corbaloc:iiop:1.0@localhost:12000/ObjectName";
int niterations = 5;

int nthreads = 4;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "l:i:o:n:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
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
                           "-o <iorfile>"
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
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
  void validate_connection (ACE_ENV_SINGLE_ARG_DECL_NOT_USED);
  // Validate the connection

private:

  /// The server.
  Simple_Server_var server_;

  CORBA::ORB_var orb_;

  /// The number of iterations on each client thread.
  int niterations_;


  ACE_SYNCH_MUTEX mutex_;
};

/***************************************************/
int
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var poa_object =
        orb->resolve_initial_references ("RootPOA" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return 1;

      Simple_Server_i *server_impl = 0;
      ACE_NEW_RETURN (server_impl,
                      Simple_Server_i (orb.in ()),
                      -1);

      PortableServer::ServantBase_var owner_transfer(server_impl);

      Simple_Server_var server =
        server_impl->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::String_var ior =
        orb->object_to_string (server.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "Activated as <%s>\n", ior.in ()));

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

      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Worker worker (orb.in ());
      if (worker.activate (THR_NEW_LWP | THR_JOINABLE,
                           nthreads) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) Cannot activate client threads\n"),
                          1);

      SelfClient selfabuse (orb.in(), server.in(), niterations);
      if (selfabuse.activate (THR_NEW_LWP | THR_JOINABLE,
                              nthreads) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) Cannot activate abusive threads\n"),
                          1);

      selfabuse.thr_mgr()->wait();

      worker.thr_mgr ()->wait ();

      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) event loop finished\n"));
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception caught:");
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
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      ACE_Time_Value tv (120, 0);
      this->orb_->run (tv ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
    }
  ACE_ENDTRY;
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
SelfClient::validate_connection (ACE_ENV_SINGLE_ARG_DECL)
{
  // Ping the object 100 times, ignoring all exceptions.
  // It would be better to use validate_connection() but the test must
  // run on minimum CORBA builds too!
  ACE_GUARD (ACE_SYNCH_MUTEX, guard, mutex_);

  for (int j = 0; j != 100; ++j)
    {
      ACE_TRY
        {
          this->server_->test_method (j ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      ACE_CATCHANY {} ACE_ENDTRY;
    }
}

int
SelfClient::svc (void)
{
  ACE_TRY_NEW_ENV
    {
      this->validate_connection (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      for (int i = 0; i < this->niterations_; ++i)
        {
          ACE_TRY_EX (CORBALOC)
            {
              CORBA::Object_var probably_not_exist =
                orb_->string_to_object (corbaloc_arg
                                        ACE_ENV_ARG_PARAMETER);

              ACE_TRY_CHECK_EX (CORBALOC);

              if (CORBA::is_nil (probably_not_exist.in()))
                {
                  ACE_DEBUG ((LM_DEBUG, "not found\n", corbaloc_arg));
                }
              else
                {
                  Simple_Server_var newserver =
                    Simple_Server::_narrow (probably_not_exist.in ()
                                            ACE_ENV_ARG_PARAMETER);
                  ACE_TRY_CHECK_EX (CORBALOC);

                  // should throw an exception
                  if (CORBA::is_nil (newserver.in()))
                    {
                      ACE_DEBUG ((LM_DEBUG,
                                  "(%P|%t)  Not found it \n"));

                    }
                  else
                    {
                      ACE_DEBUG ((LM_DEBUG,
                                  "(%P|%t) Found it \n"));
                    }
                }

            }
          ACE_CATCHANY
            {
              ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                                   "MT_SelfClient: exception raised");
            }
          ACE_ENDTRY;

          // Just make a call
          this->server_->test_method (i ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "MT_SelfClient: exception raised");
    }
  ACE_ENDTRY;
  return 0;
}
