#include "test_i.h"
#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include "ace/OS_NS_stdio.h"

#include <memory>

struct Server_Options
{
  ACE_TCHAR const *ior_output_file {};
  ACE_TCHAR const *corbaloc {ACE_TEXT ("corbaloc:iiop:1.0@localhost:12000/ObjectName")};
  int iterations {5};
  int client_threads {4};
  int server_threads {4};
};

static int
parse_args (int argc, ACE_TCHAR *argv[], Server_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("c:l:i:o:n:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'c':
        options.client_threads = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'l':
        options.corbaloc = get_opts.opt_arg ();
        break;

      case 'i':
        options.iterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'o':
        options.ior_output_file = get_opts.opt_arg ();
        break;

      case 'n':
        options.server_threads = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage:  %s "
                                     "-c <# client threads> "
                                     "-i <# iterations> "
                                     "-l <corba loc> "
                                     "-n <# server threads> "
                                     "-o <iorfile> "
                                     "\n"),
                           argv [0]),
                          -1);
      }
  return 0;
}

/*****************************************************/

/**
 * Run a server thread
 *
 * Use the ACE_Task_Base class to run server threads
 */
class Worker : public ACE_Task_Base
{
public:
  explicit Worker (CORBA::ORB_ptr orb);

  Worker (Worker const &) = delete;
  Worker (Worker &&) = delete;
  Worker &operator= (Worker const &) = delete;
  Worker &operator= (Worker &&) = delete;

  int svc () override;

private:
  /// The orb
  CORBA::ORB_var orb_ {};
};

/*****************************************************/

/**
 * Run a client thread
 *
 * Use the ACE_Task_Base class to run client threads
 */
class SelfClient : public ACE_Task_Base
{
public:
  SelfClient (CORBA::ORB_ptr orb,
              Simple_Server_ptr server,
              Server_Options const &options);

  SelfClient (SelfClient const &) = delete;
  SelfClient (SelfClient &&) = delete;
  SelfClient &operator= (SelfClient const &) = delete;
  SelfClient &operator= (SelfClient &&) = delete;

  int svc () override;

private:
  void validate_connection ();

  Simple_Server_var server_ {};
  CORBA::ORB_var orb_ {};
  ACE_TString corbaloc_;
  int const iterations_;
  TAO_SYNCH_MUTEX mutex_ {};
};

/***************************************************/
int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      Server_Options options {};
      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var const poa_object =
        orb->resolve_initial_references ("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT (" (%P|%t) Unable to initialize the POA.\n")),
                          1);

      PortableServer::POA_var const root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var const poa_manager =
        root_poa->the_POAManager ();

      if (parse_args (argc, argv, options) != 0)
        return 1;

      Simple_Server_i *server_impl {};
      ACE_NEW_RETURN (server_impl,
                      Simple_Server_i (orb.in ()),
                      -1);

      // ServantBase_var owns this TAO servant through its reference count.
      PortableServer::ServantBase_var const owner_transfer (server_impl);

      PortableServer::ObjectId_var const id =
        root_poa->activate_object (server_impl);

      CORBA::Object_var const object = root_poa->id_to_reference (id.in ());

      Simple_Server_var const server =
        Simple_Server::_narrow (object.in ());

      CORBA::String_var const ior =
        orb->object_to_string (server.in ());

      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Activated as <%C>\n"), ior.in ()));

      // If the ior_output_file exists, output the ior to it
      if (options.ior_output_file != nullptr)
        {
          std::unique_ptr<FILE, int (*) (FILE *)> output_file (
            ACE_OS::fopen (options.ior_output_file, ACE_TEXT ("w")),
            ACE_OS::fclose);
          if (!output_file)
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("(%P|%t) Cannot open output file for writing IOR: %s"),
                               options.ior_output_file),
                              1);
          ACE_OS::fprintf (output_file.get (), "%s", ior.in ());
        }

      poa_manager->activate ();

      Worker worker (orb.in ());
      if (worker.activate (THR_NEW_LWP | THR_JOINABLE,
                           options.server_threads) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("(%P|%t) Cannot activate server threads\n")),
                          1);

      SelfClient selfabuse (orb.in (), server.in (), options);
      if (selfabuse.activate (THR_NEW_LWP | THR_JOINABLE,
                              options.client_threads) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("(%P|%t) Cannot activate self-client threads\n")),
                          1);

      selfabuse.thr_mgr ()->wait ();

      worker.thr_mgr ()->wait ();

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) event loop finished\n")));
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}

Worker::Worker (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

int
Worker::svc ()
{
  try
    {
      ACE_Time_Value tv (140, 0);
      this->orb_->run (tv);
    }
  catch (CORBA::Exception const &)
    {
    }
  return 0;
}

SelfClient::SelfClient (CORBA::ORB_ptr orb,
                        Simple_Server_ptr server,
                        Server_Options const &options)
  : server_ (Simple_Server::_duplicate (server)),
    orb_ (CORBA::ORB::_duplicate (orb)),
    corbaloc_ (options.corbaloc),
    iterations_ (options.iterations)
{
}

void
SelfClient::validate_connection ()
{
  // Ping the object 100 times, ignoring all exceptions.
  // It would be better to use validate_connection() but the test must
  // run on minimum CORBA builds too!
  ACE_GUARD (TAO_SYNCH_MUTEX, guard, mutex_);

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
SelfClient::svc ()
{
  try
    {
      this->validate_connection ();

      for (int i {}; i < this->iterations_; ++i)
        {
          try
            {
              CORBA::Object_var const probably_not_exist =
                this->orb_->string_to_object (this->corbaloc_.c_str ());

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
                      ACE_DEBUG ((LM_DEBUG,
                                  ACE_TEXT ("(%P|%t) Not found it\n")));
                    }
                  else
                    {
                      ACE_DEBUG ((LM_DEBUG,
                                  ACE_TEXT ("(%P|%t) Found it\n")));
                    }
                }

            }
          catch (CORBA::Exception const &ex)
            {
              ex._tao_print_exception ("MT_SelfClient: exception raised");
            }

          // Just make a call
          this->server_->test_method (i);
        }
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("MT_SelfClient: exception raised");
    }
  return 0;
}
