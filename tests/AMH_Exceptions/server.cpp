// -*- C++ -*-
// $Id$

#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_time.h"
#include "ace/Get_Opt.h"
#include "TestS.h"
#include "tao/ORB_Core.h"

/***************************/
/*** Servant Declaration ***/

class ST_AMH_Servant
  : public virtual POA_Test::AMH_Roundtrip
{
public:
  ST_AMH_Servant (CORBA::ORB_ptr orb);

  void test_method (Test::AMH_RoundtripResponseHandler_ptr _tao_rh,
                    Test::Timestamp send_time);

  //FUZZ: disable check_for_lack_ACE_OS
  void shutdown (Test::AMH_RoundtripResponseHandler_ptr _tao_rh);
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

  Test::ServerOverload *ts = new Test::ServerOverload;

  // Calee owns the memory now.  Need not delete 'ts'
  Test::AMH_RoundtripExceptionHolder holder (ts);

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
  this->orb_->shutdown (0);
}

/*** Server Declaration ***/

/**
 Class that performs all 'dirty' initialisation work that is common to
 all the AMH servers and 'hides' all the common ORB functions.
 */
class ST_AMH_Server
{
public:
  ST_AMH_Server (int argc, ACE_TCHAR **argv);
  virtual ~ST_AMH_Server ();

  /// ORB inititalisation stuff
  int start_orb_and_poa (void);

  /// register the servant with the poa
  virtual void register_servant (ST_AMH_Servant *servant);

  /// orb-perform_work () abstraction
  virtual void run_event_loop ();

public:
  /// Accesor method (for servants) to the initialised ORB
  CORBA::ORB_ptr orb () { return this->orb_.in (); }

protected:
  int argc_;
  ACE_TCHAR **argv_;
  const ACE_TCHAR *ior_output_file_;
  CORBA::ORB_var orb_;
  PortableServer::POA_var root_poa_;

private:
  /// Write servant IOR to file specified with the '-o' option
  int write_ior_to_file (CORBA::String_var ior);
};



/*** Server Declaration ***/

ST_AMH_Server::ST_AMH_Server (int argc, ACE_TCHAR **argv)
  : argc_ (argc)
  , argv_ (argv)
{
  this->ior_output_file_ = ACE_TEXT ("test.ior");
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        this->ior_output_file_ = get_opts.opt_arg ();
        break;
      }
}

ST_AMH_Server::~ST_AMH_Server ()
{
  try
    {
      this->root_poa_->destroy (1, 1);

      this->orb_->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
    }

}

int
ST_AMH_Server::start_orb_and_poa (void)
{
  try
    {
      this->orb_ = CORBA::ORB_init (this->argc_, this->argv_);

      CORBA::Object_var poa_object =
        this->orb_->resolve_initial_references("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      this->root_poa_ = PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var poa_manager =
        this->root_poa_->the_POAManager ();

      poa_manager->activate ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return -1;
    }

  return 0;
}

void
ST_AMH_Server::register_servant (ST_AMH_Servant *servant)
{
  try
    {
      CORBA::Object_var poa_object =
        this->orb_->resolve_initial_references("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::ObjectId_var id =
        root_poa->activate_object (servant);

      CORBA::Object_var object = root_poa->id_to_reference (id.in ());

      Test::Roundtrip_var roundtrip =
        Test::Roundtrip::_narrow (object.in ());

      CORBA::String_var ior =
        this->orb_->object_to_string (roundtrip.in ());

      (void) this->write_ior_to_file (ior);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
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
  catch (const CORBA::Exception&)
    {}
}

int
ST_AMH_Server::write_ior_to_file (CORBA::String_var ior)
{
  // If the ior_output_file exists, output the ior to it
  FILE *output_file= ACE_OS::fopen (ST_AMH_Server::ior_output_file_,
                                    "w");
  if (output_file == 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "Cannot open output file for writing IOR: %s",
                  ST_AMH_Server::ior_output_file_));
      return -1;
    }

  ACE_OS::fprintf (output_file, "%s", ior.in ());
  ACE_OS::fclose (output_file);
  return 0;
}


int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  ST_AMH_Server amh_server (argc, argv);

  amh_server.start_orb_and_poa ();

  ST_AMH_Servant servant (amh_server.orb ());

  amh_server.register_servant (&servant);

  amh_server.run_event_loop ();

  return 0;
}
