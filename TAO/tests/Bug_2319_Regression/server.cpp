
#include "ace/Thread_Manager.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Get_Opt.h"
#include <memory>

#include "TestS.h"
#include "TestC.h"

/***************************/
/*** Servant Declaration ***/
/***************************/

class ST_AMH_Servant : public virtual POA_Test::AMH_Roundtrip
{
public:
  ST_AMH_Servant () = default;
  ST_AMH_Servant (ST_AMH_Servant const &) = delete;
  ST_AMH_Servant &operator= (ST_AMH_Servant const &) = delete;
  ST_AMH_Servant (ST_AMH_Servant &&) = delete;
  ST_AMH_Servant &operator= (ST_AMH_Servant &&) = delete;

  void test_method (Test::AMH_RoundtripResponseHandler_ptr _tao_rh,
                    Test::Timestamp send_time) override;

  int calls_received () const;

private:
  int calls_received_ {};
};

/****************************/
/**** Server Declaration ****/
/****************************/

/**
 Class that performs all 'dirty' initialisation work that is common to
 all the AMH servers and 'hides' all the common ORB functions.
 */
class ST_AMH_Server
{
public:
  ST_AMH_Server (int num_calls, ACE_TCHAR const *ior_file);
  virtual ~ST_AMH_Server ();
  ST_AMH_Server (ST_AMH_Server const &) = delete;
  ST_AMH_Server &operator= (ST_AMH_Server const &) = delete;
  ST_AMH_Server (ST_AMH_Server &&) = delete;
  ST_AMH_Server &operator= (ST_AMH_Server &&) = delete;

  /// ORB inititalisation stuff
  int start_orb_and_poa (CORBA::ORB_ptr orb);

  /// register the servant with the poa
  virtual void register_servant (ST_AMH_Servant *servant);

  /// orb-perform_work () abstraction
  virtual void run_event_loop ();

protected:
  CORBA::ORB_var orb_;
  PortableServer::POA_var root_poa_;
  // Borrowed from main; it outlives both worker threads.
  ST_AMH_Servant *servant_ {};
  int const num_calls_;
  // Borrowed from main; it outlives both worker threads.
  ACE_TCHAR const *ior_file_;

private:
  /// Write servant IOR to file specified with the '-o' option
  int write_ior_to_file (CORBA::String_var const &ior,
                         ACE_TCHAR const *ior_file);
};

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
//
int parse_args (int argc,
                ACE_TCHAR *argv[],
                int &num_calls,
                ACE_TCHAR const *&ior,
                ACE_TCHAR const *&ior_file)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("n:o:k:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'n':
        num_calls = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case 'o':
        ior_file = get_opts.opt_arg ();
        break;
      case 'k':
        ior = get_opts.opt_arg ();
        break;
      default:
        break;
      }
  return 0;
}

/***************************/
/*** Servant Definition ***/
/***************************/

// ------------------------------------------------------------------------
//
void
ST_AMH_Servant::test_method (Test::AMH_RoundtripResponseHandler_ptr,
                             Test::Timestamp)
{
  ACE_OS::printf ("Received Timestamp # %d\n", this->calls_received_);
  ACE_OS::sleep (1);
  ++this->calls_received_;

  // When _tao_rh destructor is called, it shouldn't send anything to
  // the client as well
}

int
ST_AMH_Servant::calls_received () const
{
  return this->calls_received_;
}

/*** Server Declaration ***/

// ------------------------------------------------------------------------
//
ST_AMH_Server::ST_AMH_Server (int num_calls, ACE_TCHAR const *ior_file)
  : num_calls_ (num_calls)
    , ior_file_ (ior_file)
{
}

ST_AMH_Server::~ST_AMH_Server ()
{
  if (!CORBA::is_nil (this->root_poa_.in ()))
    {
      try
        {
          this->root_poa_->destroy (true, true);
        }
      catch (CORBA::Exception const &ex)
        {
          ex._tao_print_exception ("Exception caught:");
        }
    }
}

// ------------------------------------------------------------------------
//
int ST_AMH_Server::start_orb_and_poa (CORBA::ORB_ptr orb)
{
  try
    {
      this->orb_ = CORBA::ORB::_duplicate (orb);

      CORBA::Object_var const poa_object =
        this->orb_->resolve_initial_references ("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      this->root_poa_ = PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var const poa_manager =
        this->root_poa_->the_POAManager ();

      poa_manager->activate ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return -1;
    }

  return 0;
}

// ------------------------------------------------------------------------
//
void ST_AMH_Server::register_servant (ST_AMH_Servant *servant)
{
  try
    {
      this->servant_ = servant;

      PortableServer::ObjectId_var const id =
        this->root_poa_->activate_object (servant);

      CORBA::Object_var const object = this->root_poa_->id_to_reference (id.in ());

      Test::Roundtrip_var const roundtrip = Test::Roundtrip::_narrow (object.in ());

      CORBA::String_var const ior = this->orb_->object_to_string (roundtrip.in ());

      (void) this->write_ior_to_file (ior, this->ior_file_);
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
    }
}

// ------------------------------------------------------------------------
//
void ST_AMH_Server::run_event_loop ()
{
  try
    {
      ACE_Time_Value period (1, 0);
      while (true)
        {
          this->orb_->perform_work (&period);

          // when all calls from client have been received, exit
          if (this->servant_->calls_received () == this->num_calls_)
            return;
        }
    }
  catch (CORBA::Exception const &)
    {
    }
}

// ------------------------------------------------------------------------
//
int
ST_AMH_Server::write_ior_to_file (CORBA::String_var const &ior,
                                  ACE_TCHAR const *ior_file)
{
  // If the ior_output_file exists, output the ior to it
  std::unique_ptr<FILE, int (*) (FILE *)> output_file (
    ACE_OS::fopen (ior_file, "w"), ACE_OS::fclose);
  if (!output_file)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Cannot open output file for writing IOR: %s",
                       ior_file),
                      -1);

  ACE_OS::fprintf (output_file.get (), "%s", ior.in ());
  return 0;
}


// ------------------------------------------------------------------------
//
static ACE_THR_FUNC_RETURN
start_server (void *arg)
{
  ST_AMH_Server * const amh_server = static_cast<ST_AMH_Server *> (arg);
  amh_server->run_event_loop ();
  return ACE_THR_FUNC_RETURN_NULL;
}

struct Client_Thread_Args
{
  // Borrowed from main; the client thread is joined before this reference expires.
  Test::Roundtrip_ptr roundtrip;
  int num_calls;
};

static ACE_THR_FUNC_RETURN
start_client (void *arg)
{
  Client_Thread_Args const * const thread_args = static_cast<Client_Thread_Args *> (arg);
  Test::Roundtrip_ptr const roundtrip = thread_args->roundtrip;

  // Do a couple of calls on the server. If the sever is trying to
  // do something stupid like sending an exception to us, then it
  // won't be able to handle more than 1 request from us.
  Test::Timestamp const time = 10;

  for (int i {}; i < thread_args->num_calls; ++i)
    {
      roundtrip->test_method (time);
      ACE_DEBUG ((LM_DEBUG, "Sent call # %d\n", i));
    }

  return ACE_THR_FUNC_RETURN_NULL;
}


// ------------------------------------------------------------------------
//
int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  int num_calls {10};
  ACE_TCHAR const *ior = ACE_TEXT ("file://test.ior");
  ACE_TCHAR const *ior_file = ACE_TEXT ("test.ior");

  if (parse_args (argc, argv, num_calls, ior, ior_file) != 0)
    return 1;

  CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

  {
    ST_AMH_Servant servant;
    ST_AMH_Server amh_server (num_calls, ior_file);
    if (amh_server.start_orb_and_poa (orb.in ()) != 0)
      return 1;

    amh_server.register_servant (&servant);

    CORBA::Object_var const object = orb->string_to_object (ior);

    Test::Roundtrip_var const roundtrip = Test::Roundtrip::_narrow (object.in ());

    if (CORBA::is_nil (roundtrip.in ()))
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Nil Test::Roundtrip reference <%s>\n",
                         ior),
                        1);

    Client_Thread_Args client_args {roundtrip.in (), num_calls};
    ACE_thread_t server_thread {};
    ACE_thread_t client_thread {};

    ACE_Thread_Manager * const thread_manager = ACE_Thread_Manager::instance ();
    if (thread_manager->spawn (start_server,
                               static_cast<void *> (&amh_server),
                               THR_NEW_LWP | THR_JOINABLE,
                               &server_thread) == -1)
      ACE_ERROR_RETURN ((LM_ERROR, "Unable to start server thread\n"), 1);

    if (thread_manager->spawn (start_client,
                               static_cast<void *> (&client_args),
                               THR_NEW_LWP | THR_JOINABLE,
                               &client_thread) == -1)
      {
        orb->shutdown (true);
        (void) thread_manager->join (server_thread);
        ACE_ERROR_RETURN ((LM_ERROR, "Unable to start client thread\n"), 1);
      }

    (void) thread_manager->join (client_thread);
    ACE_OS::printf ("End client\n");
    (void) thread_manager->join (server_thread);
    ACE_OS::printf ("End server\n");
  }

  orb->destroy ();

  return 0;
}
