
// $Id$

#include "ace/Thread_Manager.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Get_Opt.h"

#include "TestS.h"
#include "TestC.h"

int num_calls = 10; // total calls client is going to make
const int sleep_time = 1; // sleep for  1 sec on each call

// This should equal num_calls within 'sleep * num_calls' seconds
int calls_received = 0;

const ACE_TCHAR *ior      = ACE_TEXT("file://test.ior");
const ACE_TCHAR *ior_file = ACE_TEXT("test.ior");

/***************************/
/*** Servant Declaration ***/
/***************************/

class ST_AMH_Servant : public virtual POA_Test::AMH_Roundtrip
{
public:
  ST_AMH_Servant (CORBA::ORB_ptr orb);

  void test_method (Test::AMH_RoundtripResponseHandler_ptr _tao_rh,
                    Test::Timestamp send_time);

protected:
  CORBA::ORB_ptr orb_;
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
  ST_AMH_Server (void);
  virtual ~ST_AMH_Server ();

  /// ORB inititalisation stuff
  int start_orb_and_poa (const CORBA::ORB_var &_orb);

  /// register the servant with the poa
  virtual void register_servant (ST_AMH_Servant *servant);

  /// orb-perform_work () abstraction
  virtual void run_event_loop ();

public:

protected:
  CORBA::ORB_ptr orb_;
  PortableServer::POA_var root_poa_;

private:
  /// Write servant IOR to file specified with the '-o' option
  int write_ior_to_file (CORBA::String_var ior);
};

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
//
int parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("n:o:k:"));
  int c;

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
ST_AMH_Servant::ST_AMH_Servant (CORBA::ORB_ptr orb)
  : orb_(CORBA::ORB::_duplicate(orb))
{
}

// ------------------------------------------------------------------------
//
void
ST_AMH_Servant::test_method (Test::AMH_RoundtripResponseHandler_ptr,
                             Test::Timestamp)
{
  ACE_OS::printf("Received Timestamp # %d\n", calls_received);
  ACE_OS::sleep(1);
  ++calls_received;

  // When _tao_rh destructor is called, it shouldn't send anything to
  // the client as well
}

/*** Server Declaration ***/

// ------------------------------------------------------------------------
//
ST_AMH_Server::ST_AMH_Server (void)
{
}

// ------------------------------------------------------------------------
//
ST_AMH_Server::~ST_AMH_Server ()
{
  try
    {
      this->root_poa_->destroy (1, 1);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
    }
}

// ------------------------------------------------------------------------
//
int ST_AMH_Server::start_orb_and_poa (const CORBA::ORB_var &_orb)
{
  try
    {
      this->orb_ = CORBA::ORB::_duplicate(_orb.in ());

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

// ------------------------------------------------------------------------
//
void ST_AMH_Server::register_servant (ST_AMH_Servant *servant)
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

      Test::Roundtrip_var roundtrip = Test::Roundtrip::_narrow (object.in ());

      CORBA::String_var iorstr = this->orb_->object_to_string(roundtrip.in ());

      (void) this->write_ior_to_file(iorstr);
    }
  catch (const CORBA::Exception& ex)
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
      while (1)
        {
          this->orb_->perform_work (&period);

          // when all calls from client have been received, exit
          if (calls_received == num_calls )
            return;
        }
    }
  catch (const CORBA::Exception&)
    {
    }
}

// ------------------------------------------------------------------------
//
int
ST_AMH_Server::write_ior_to_file (CORBA::String_var iorstr)
{
  // If the ior_output_file exists, output the ior to it
  FILE *output_file= ACE_OS::fopen (ior_file, "w");
  if (output_file == 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "Cannot open output file for writing IOR: %s",
                  ior_file));
      return -1;
    }

  ACE_OS::fprintf (output_file, "%s", iorstr.in ());
  ACE_OS::fclose (output_file);
  return 0;
}


// ------------------------------------------------------------------------
//
static ACE_THR_FUNC_RETURN start_server(void* _arg)
{
  ST_AMH_Server *amh_server = static_cast<ST_AMH_Server*>(_arg);
  amh_server->run_event_loop();
  return 0;
}

// ------------------------------------------------------------------------
//
static ACE_THR_FUNC_RETURN start_client(void* _arg)
{
  Test::Roundtrip_var roundtrip(static_cast<Test::Roundtrip_ptr>(_arg));

  // Do a couple of calls on the server. If the sever is trying to
  // do something stupid like sending an exception to us, then it
  // won't be able to handle more than 1 request from us.
  Test::Timestamp time = 10;

  for (int i = 0; i < num_calls; i++)
    {
      roundtrip->test_method(time);
      ACE_DEBUG ((LM_DEBUG, "Sent call # %d\n", i));
    }

  return 0;
}


// ------------------------------------------------------------------------
//
int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  if (parse_args (argc, argv) != 0)
    return 1;

  ST_AMH_Server amh_server;
  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

  amh_server.start_orb_and_poa(orb);

  ST_AMH_Servant servant(orb.in());

  amh_server.register_servant(&servant);

  CORBA::Object_var object =  orb->string_to_object(ior);

  Test::Roundtrip_var roundtrip = Test::Roundtrip::_narrow(object.in ());

  if (CORBA::is_nil(roundtrip.in()))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Nil Test::Roundtrip reference <%s>\n",
                         ior),
                        1);
    }

  ACE_thread_t serverThr;
  ACE_thread_t clientThr;

  ACE_Thread_Manager::instance()->spawn(start_server,
                                        (void*)&amh_server,
                                        THR_NEW_LWP | THR_JOINABLE,
                                        &serverThr);

  ACE_Thread_Manager::instance()->spawn(start_client,
                                        (void*)roundtrip.in (),
                                        THR_NEW_LWP | THR_JOINABLE,
                                        &clientThr);

  ACE_Thread_Manager::instance()->join(clientThr);
  ACE_OS::printf("End client\n");
  ACE_Thread_Manager::instance()->join(serverThr);
  ACE_OS::printf("End server\n");

  orb->destroy();

  return 0;
}
