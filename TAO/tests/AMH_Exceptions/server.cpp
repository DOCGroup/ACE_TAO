// -*- C++ -*-
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_time.h"
#include "ace/Get_Opt.h"
#include "TestS.h"
#include "tao/ORB_Core.h"
#include <memory>

struct Server_Options
{
  ACE_TCHAR const *ior_output_file {ACE_TEXT ("test.ior")};
};

static int
parse_args (int argc, ACE_TCHAR *argv[], Server_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("o:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        options.ior_output_file = get_opts.opt_arg ();
        break;
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage: %s [-o <iorfile>]\n"),
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

  //FUZZ: disable check_for_lack_ACE_OS
  void shutdown (Test::AMH_RoundtripResponseHandler_ptr _tao_rh) override;
  //FUZZ: disable check_for_lack_ACE_OS

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
ST_AMH_Servant::test_method (Test::AMH_RoundtripResponseHandler_ptr _tao_rh,
                             Test::Timestamp)
{
  // Throw an overload exception
  std::unique_ptr<Test::ServerOverload> exception (
    std::make_unique<Test::ServerOverload> ());
  Test::AMH_RoundtripExceptionHolder holder (exception.get ());
  // The exception holder takes ownership of the exception.
  exception.release ();

  try
    {
      _tao_rh->test_method_excep (&holder);
    }
  catch (...)
    {
    }
}

void
ST_AMH_Servant::shutdown (Test::AMH_RoundtripResponseHandler_ptr /*_tao_rh*/)
{
  this->orb_->shutdown (false);
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
  ~ST_AMH_Server ();
  ST_AMH_Server (ST_AMH_Server const &) = delete;
  ST_AMH_Server &operator= (ST_AMH_Server const &) = delete;
  ST_AMH_Server (ST_AMH_Server &&) = delete;
  ST_AMH_Server &operator= (ST_AMH_Server &&) = delete;

  /// ORB initialisation stuff
  int start_orb_and_poa ();

  /// register the servant with the poa
  int register_servant (ST_AMH_Servant *servant);

  /// orb-perform_work () abstraction
  void run_event_loop ();

  /// Destroy the POA before the stack-allocated servant leaves scope.
  void cleanup ();

  /// Accesor method (for servants) to the initialized ORB
  CORBA::ORB_ptr orb () const { return this->orb_.in (); }

protected:
  int * const argc_;
  ACE_TCHAR ** const argv_;
  Server_Options options_ {};
  CORBA::ORB_var orb_;
  PortableServer::POA_var root_poa_;

private:
  /// Write servant IOR to file specified with the '-o' option
  int write_ior_to_file (CORBA::String_var const &ior);
};

/*** Server Declaration ***/

ST_AMH_Server::ST_AMH_Server (int *argc, ACE_TCHAR **argv)
  : argc_ (argc)
  , argv_ (argv)
{
}

ST_AMH_Server::~ST_AMH_Server ()
{
  this->cleanup ();
}

void
ST_AMH_Server::cleanup ()
{
  try
    {
      if (!CORBA::is_nil (this->root_poa_.in ()))
        {
          this->root_poa_->destroy (true, true);
          this->root_poa_ = PortableServer::POA::_nil ();
        }

      if (!CORBA::is_nil (this->orb_.in ()))
        {
          this->orb_->destroy ();
          this->orb_ = CORBA::ORB::_nil ();
        }
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
      this->orb_ = CORBA::ORB_init (*(this->argc_), this->argv_);
      if (parse_args (*(this->argc_), this->argv_, this->options_) != 0)
        return -1;

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
ST_AMH_Server::register_servant (ST_AMH_Servant *servant)
{
  try
    {
      PortableServer::ObjectId_var const id (
        this->root_poa_->activate_object (servant));

      CORBA::Object_var const object (this->root_poa_->id_to_reference (id.in ()));

      Test::Roundtrip_var const roundtrip (Test::Roundtrip::_narrow (object.in ()));

      CORBA::String_var const ior (this->orb_->object_to_string (roundtrip.in ()));

      return this->write_ior_to_file (ior);
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return -1;
    }
}

void
ST_AMH_Server::run_event_loop ()
{
  try
    {
      ACE_Time_Value period (0, 11000);
      while (!this->orb_->orb_core ()->has_shutdown ())
        {
          this->orb_->perform_work (&period);
        }
    }
  catch (CORBA::Exception const &)
    {
    }
}

int
ST_AMH_Server::write_ior_to_file (CORBA::String_var const &ior)
{
  // If the ior_output_file exists, output the ior to it
  std::unique_ptr<FILE, int (*) (FILE *)> output_file (
    ACE_OS::fopen (this->options_.ior_output_file, "w"), ACE_OS::fclose);
  if (!output_file)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Cannot open output file for writing IOR: %s\n"),
                  this->options_.ior_output_file));
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

  ST_AMH_Servant servant (amh_server.orb ());

  if (amh_server.register_servant (&servant) != 0)
    {
      amh_server.cleanup ();
      return 1;
    }

  amh_server.run_event_loop ();
  amh_server.cleanup ();

  return 0;
}
