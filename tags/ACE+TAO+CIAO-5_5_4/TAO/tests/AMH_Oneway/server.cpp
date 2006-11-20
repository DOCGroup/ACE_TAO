
// $Id$

#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Get_Opt.h"
#include "TestS.h"

int num_calls = 10; // total calls client is going to make
const int sleep_time = 1; // sleep for  1 sec on each call

// This should equal num_calls within 'sleep * num_calls' seconds
int calls_received = 0;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "n:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'n':
        num_calls = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      default:
        break;
      }
  return 0;
}


/***************************/
/*** Servant Declaration ***/

class ST_AMH_Servant
  : public virtual POA_Test::AMH_Roundtrip
{
public:
  ST_AMH_Servant (CORBA::ORB_ptr orb);

  void test_method (Test::AMH_RoundtripResponseHandler_ptr _tao_rh,
                    Test::Timestamp send_time
                    ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

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
                             Test::Timestamp send_time
                             ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_OS::sleep (1);
  ACE_DEBUG ((LM_DEBUG, "Recieved Timestamp # %d \n", calls_received));
  calls_received++;

  ACE_UNUSED_ARG (send_time);
  ACE_UNUSED_ARG (_tao_rh);

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
  ST_AMH_Server (int *argc, char **argv);
  virtual ~ST_AMH_Server ();

  /// ORB inititalisation stuff
  int start_orb_and_poa (void);

  /// register the servant with the poa
  virtual void register_servant (ST_AMH_Servant *servant);

  /// orb-perform_work () abstraction
  virtual void run_event_loop ();

  /// do post-run cleanup. This is necessary here because the servant
  /// supplied to regiser_servant happens to be allocated on the
  /// stack, and done after the instance of ST_AMH_Server is
  /// created. This leads to the servant being destroyed before this
  /// class's destructor. And alternative solution would be allocate
  /// the servant on the stack and rely on the reference count to
  /// delete it.
  virtual void cleanup ();

public:
  /// Accesor method (for servants) to the initialised ORB
  CORBA::ORB_ptr orb () { return this->orb_.in (); }

protected:
  int *argc_;
  char **argv_;
  char *ior_output_file_;
  CORBA::ORB_var orb_;
  PortableServer::POA_var root_poa_;

private:
  /// Write servant IOR to file specified with the '-o' option
  int write_ior_to_file (CORBA::String_var ior);
};



/*** Server Declaration ***/

ST_AMH_Server::ST_AMH_Server (int* argc, char **argv)
  : argc_ (argc)
  , argv_ (argv)
{
  this->ior_output_file_ = const_cast<char*> ("test.ior");
}

ST_AMH_Server::~ST_AMH_Server ()
{
}

void
ST_AMH_Server::cleanup ()
{
  ACE_TRY_NEW_ENV
    {
      this->root_poa_->destroy (1, 1 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      this->orb_->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Exception caught:");
    }
  ACE_ENDTRY;

}

int
ST_AMH_Server::start_orb_and_poa (void)
{
  ACE_TRY_NEW_ENV
    {
      this->orb_ = CORBA::ORB_init (*(this->argc_),
                                    this->argv_,
                                    "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var poa_object =
        this->orb_->resolve_initial_references("RootPOA" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      this->root_poa_ = PortableServer::POA::_narrow (poa_object.in ()
                                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var poa_manager =
        this->root_poa_->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Exception caught:");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

void
ST_AMH_Server::register_servant (ST_AMH_Servant *servant)
{
  ACE_TRY_NEW_ENV
    {
      Test::Roundtrip_var roundtrip =
        servant->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::String_var ior =
        this->orb_->object_to_string (roundtrip.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      (void) this->write_ior_to_file (ior);
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Exception caught:");
    }
  ACE_ENDTRY;
}

void
ST_AMH_Server::run_event_loop ()
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      ACE_Time_Value period (0, 11000);
      while (1)
        {
              this->orb_->perform_work (&period);
              ACE_TRY_CHECK;

              // when all calls from client have been recieved, exit
              if (calls_received == num_calls )
                return;
        }
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {}
  ACE_ENDTRY;
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
main (int argc, char *argv[])
{
  if (parse_args (argc, argv) != 0)
    return 1;

  ST_AMH_Server amh_server (&argc, argv);

  amh_server.start_orb_and_poa ();

  ST_AMH_Servant servant (amh_server.orb ());

  amh_server.register_servant (&servant);

  amh_server.run_event_loop ();

  amh_server.cleanup ();

  return 0;
}
