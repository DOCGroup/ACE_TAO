#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Get_Opt.h"
#include "TestS.h"
#include <atomic>
#include <memory>

static std::atomic<int> calls_received {};

struct Server_Options
{
  ACE_TCHAR const *ior_output_file {ACE_TEXT ("test.ior")};
  int num_calls {10};
};

static int
parse_args (int argc, ACE_TCHAR *argv[], Server_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("n:o:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'n':
        options.num_calls = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case 'o':
        options.ior_output_file = get_opts.opt_arg ();
        break;
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage: %s [-n <number_of_calls>] [-o <iorfile>]\n"),
                           argv [0]),
                          -1);
      }

  return 0;
}

/***************************/
/*** Servant Declaration ***/

class ST_AMH_Servant
  : public virtual POA_Test::AMH_Roundtrip
{
public:
  explicit ST_AMH_Servant (CORBA::ORB_ptr orb);
  ST_AMH_Servant (ST_AMH_Servant const &) = delete;
  ST_AMH_Servant &operator= (ST_AMH_Servant const &) = delete;
  ST_AMH_Servant (ST_AMH_Servant &&) = delete;
  ST_AMH_Servant &operator= (ST_AMH_Servant &&) = delete;

  void test_method (Test::AMH_RoundtripResponseHandler_ptr _tao_rh,
                    Test::Timestamp send_time) override;

protected:
  CORBA::ORB_var orb_;
};

/***************************/
/*** Servant Definition ***/

ST_AMH_Servant::ST_AMH_Servant (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
ST_AMH_Servant::test_method (Test::AMH_RoundtripResponseHandler_ptr,
                             Test::Timestamp)
{
  ACE_OS::sleep (1);
  int const call_number (calls_received.fetch_add (1));
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Received Timestamp #%d on %T\n"), call_number));

  // When _tao_rh destructor is called, it shouldn't send anything to
  // the client as well
}

/*** Server Declaration ***/

/**
 Class that performs all 'dirty' initialisation work that is common to
 all the AMH servers and 'hides' all the common ORB functions.
 */
class ST_AMH_Server
{
public:
  ST_AMH_Server (int *argc, ACE_TCHAR **argv);
  ~ST_AMH_Server () = default;
  ST_AMH_Server (ST_AMH_Server const &) = delete;
  ST_AMH_Server &operator= (ST_AMH_Server const &) = delete;
  ST_AMH_Server (ST_AMH_Server &&) = delete;
  ST_AMH_Server &operator= (ST_AMH_Server &&) = delete;

  /// ORB initialization stuff
  int start_orb_and_poa ();

  /// register the servant with the poa
  int register_servant (ST_AMH_Servant *servant, ACE_TCHAR const *ior_output_file);

  /// orb-perform_work () abstraction
  void run_event_loop (int expected_calls);

  /// do post-run cleanup. This is necessary here because the servant
  /// supplied to register_servant happens to be allocated on the
  /// stack, and done after the instance of ST_AMH_Server is
  /// created. This leads to the servant being destroyed before this
  /// class's destructor. And alternative solution would be allocate
  /// the servant on the stack and rely on the reference count to
  /// delete it.
  void cleanup ();

  /// Accesor method (for servants) to the initialized ORB
  CORBA::ORB_ptr orb () const { return this->orb_.in (); }

protected:
  int * const argc_;
  ACE_TCHAR ** const argv_;
  CORBA::ORB_var orb_;
  PortableServer::POA_var root_poa_;

private:
  /// Write servant IOR to file specified with the '-o' option
  int write_ior_to_file (CORBA::String_var const &ior,
                         ACE_TCHAR const *ior_output_file);
};

/*** Server Declaration ***/

ST_AMH_Server::ST_AMH_Server (int *argc, ACE_TCHAR **argv)
  : argc_ (argc)
  , argv_ (argv)
{
}

void
ST_AMH_Server::cleanup ()
{
  try
    {
      this->root_poa_->destroy (true, true);

      this->orb_->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
    }
}

int
ST_AMH_Server::start_orb_and_poa ()
{
  try
    {
      this->orb_ = CORBA::ORB_init (*(this->argc_),
                                    this->argv_);

      CORBA::Object_var const poa_object (
        this->orb_->resolve_initial_references ("RootPOA"));

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT (" (%P|%t) Unable to initialize the POA.\n")),
                          1);

      this->root_poa_ = PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var const poa_manager (
        this->root_poa_->the_POAManager ());

      poa_manager->activate ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return -1;
    }

  return 0;
}

int
ST_AMH_Server::register_servant (ST_AMH_Servant *servant,
                                 ACE_TCHAR const *ior_output_file)
{
  try
    {
      PortableServer::ObjectId_var const id (
        this->root_poa_->activate_object (servant));

      CORBA::Object_var const object (this->root_poa_->id_to_reference (id.in ()));

      Test::Roundtrip_var const roundtrip (Test::Roundtrip::_narrow (object.in ()));

      CORBA::String_var const ior (this->orb_->object_to_string (roundtrip.in ()));

      return this->write_ior_to_file (ior, ior_output_file);
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return -1;
    }
}

void
ST_AMH_Server::run_event_loop (int expected_calls)
{
  try
    {
      while (true)
        {
          ACE_Time_Value period (0, 11000);
          this->orb_->perform_work (&period);

          // when all calls from client have been received, exit
          if (calls_received.load () == expected_calls)
            return;
        }
    }
  catch (CORBA::Exception const &)
    {
    }
}

int
ST_AMH_Server::write_ior_to_file (CORBA::String_var const &ior,
                                  ACE_TCHAR const *ior_output_file)
{
  // If the ior_output_file exists, output the ior to it
  std::unique_ptr<FILE, int (*) (FILE *)> output_file (
    ACE_OS::fopen (ior_output_file, "w"), ACE_OS::fclose);
  if (!output_file)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Cannot open output file for writing IOR: %s"),
                  ior_output_file));
      return -1;
    }

  ACE_OS::fprintf (output_file.get (), "%s", ior.in ());
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ST_AMH_Server amh_server (&argc, argv);

  if (amh_server.start_orb_and_poa () != 0)
    return 1;

  Server_Options options {};
  if (parse_args (argc, argv, options) != 0)
    return 1;

  ST_AMH_Servant servant (amh_server.orb ());

  if (amh_server.register_servant (&servant, options.ior_output_file) != 0)
    {
      amh_server.cleanup ();
      return 1;
    }

  amh_server.run_event_loop (options.num_calls);

  amh_server.cleanup ();

  return 0;
}
