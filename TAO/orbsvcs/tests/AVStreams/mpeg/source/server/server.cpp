// $Id$

#include "server.h"
#include "ace/Process.h"

ACE_RCSID(server, server, "$Id$")

int AV_Server::done_;
pid_t AV_Server::current_pid_ = -1;


Audio_MMDevice::Audio_MMDevice (TAO_AV_Endpoint_Process_Strategy *endpoint_strategy)
  :TAO_MMDevice (endpoint_strategy),
   connections_ (0)
{
}

AVStreams::StreamEndPoint_B_ptr  
Audio_MMDevice::create_B (AVStreams::StreamCtrl_ptr the_requester, 
                        AVStreams::VDev_out the_vdev, 
                        AVStreams::streamQoS &the_qos, 
                        CORBA::Boolean_out met_qos, 
                        char *&named_vdev, 
                        const AVStreams::flowSpec &the_spec,  
                        CORBA::Environment &env)
{
  ACE_DEBUG ((LM_DEBUG,"(%P|%t) Audio_MMDevice::create_B called \n"));
  AVStreams::StreamEndPoint_B_ptr stream_ptr;
  stream_ptr = TAO_MMDevice::create_B (the_requester,
                                       the_vdev,
                                       the_qos,
                                       met_qos,
                                       named_vdev,
                                       the_spec,
                                       env);
  if (stream_ptr != 0)
    this->connections_;
  return stream_ptr;
}

int
Audio_MMDevice::connections (void)
{
  return this->connections_;
}

// AV_Server_Sig_Handler routines

AV_Server_Sig_Handler::AV_Server_Sig_Handler (void)
{
}

int
AV_Server_Sig_Handler::register_handler (void)
{
  // Assign the Sig_Handler a dummy I/O descriptor.  Note that even
  // though we open this file "Write Only" we still need to use the
  // ACE_Event_Handler::NULL_MASK when registering this with the
  // ACE_Reactor (see below).
  this->handle_ = ACE_OS::open (ACE_DEV_NULL, O_WRONLY);
  ACE_ASSERT (this->handle_ != -1);
  //  ACE_DEBUG ((LM_DEBUG,"(%P|%t) sig_handler == %d\n",this->handle_));

  // Register signal handler object.  Note that NULL_MASK is used to
  // keep the ACE_Reactor from calling us back on the "/dev/null"
  // descriptor.

   if (TAO_ORB_Core_instance ()->reactor ()->register_handler
       (this, ACE_Event_Handler::NULL_MASK) == -1)
     ACE_ERROR_RETURN ((LM_ERROR, 
                        "%p\n", 
                        "register_handler"),
                       -1);

  // handles these signals.
   //   this->sig_set.fill_set ();
   this->sig_set.sig_add (SIGCHLD);  
   this->sig_set.sig_add (SIGBUS); 
   this->sig_set.sig_add (SIGINT); 
   this->sig_set.sig_add (SIGTERM);

  // Register the signal handler object to catch the signals.  if
  if (TAO_ORB_Core_instance ()->reactor ()->register_handler 
      (this->sig_set, this) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, 
                       "%p\n", 
                       "register_handler"),
                      -1);
  return 0;
}

// Called by the ACE_Reactor to extract the fd.
ACE_HANDLE
AV_Server_Sig_Handler::get_handle (void) const
{
  return this->handle_;
}

int 
AV_Server_Sig_Handler::handle_input (ACE_HANDLE)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) handling asynchonrous input...\n"));
  return 0;
}

int 
AV_Server_Sig_Handler::shutdown (ACE_HANDLE, ACE_Reactor_Mask)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) closing down Sig_Handler...\n"));
  return 0;
}

// This method handles all the signals that are being caught by this
// object.  

int
AV_Server_Sig_Handler::handle_signal (int signum, siginfo_t *, ucontext_t *)
{
  //  ACE_DEBUG ((LM_DEBUG, "(%P|%t) received signal %S\n", signum));

  switch (signum)
    {
    case SIGCHLD:
      // Handle the death of child signal.
      this->clear_child (SIGCHLD);
      break;
    case SIGBUS:
      // Handle the Bus error signal
    case SIGINT:
      // Handle the interrupt signal
    case SIGTERM:
      // Handle the process termination signal.
      this->int_handler (signum);
      break;
    default:
      //      ACE_DEBUG ((LM_DEBUG, "(%P|%t) %S: not handled, returning to program\n", signum));
      break;
    }
  return 0;
}

// SIGCHLD handler
void
AV_Server_Sig_Handler::clear_child (int sig)
{
  int pid;
  int status;
  
  //  ACE_DEBUG ((LM_DEBUG,"(%P|%t) AV_Server: Reaping the children\n"));
  // reap the children
  while ((pid = ACE_OS::waitpid (-1, 
                                 &status, 
                                 WNOHANG)) > 0)
  {
    if (pid == AV_Server::current_pid_)
      {
        cerr << "The child currently being waited for has died\n";
        //        ACE_DEBUG ((LM_DEBUG,"(%P|%t) The child currently being waited for has died.\n"));
        AV_Server::done_ = 1;
      }

    // decrement the count of number of active children
    Mpeg_Global::session_num --;
    
    if (status == 0) 
      continue;
    
    //    ACE_DEBUG ((LM_DEBUG, "(%P|%t) AV_Server: child %d (status %d)\n", pid, status));
  }
  return;
}

int
AV_Server_Sig_Handler::remove_names (void)
{
  TAO_TRY
    {
  CORBA::Object_var naming_obj =
    TAO_ORB_Core_instance ()->orb ()->resolve_initial_references ("NameService");
  if (CORBA::is_nil (naming_obj.in ()))
    ACE_ERROR_RETURN ((LM_ERROR,
                       " (%P|%t) Unable to resolve the Name Service.\n"),
                      -1);

  CosNaming::NamingContext_var naming_context =
    CosNaming::NamingContext::_narrow (naming_obj.in (),
                                       TAO_TRY_ENV);
  TAO_CHECK_ENV;

  // Unregister the video_mmdevice with the naming service.

  CosNaming::Name video_server_mmdevice_name (1);
  video_server_mmdevice_name.length (1);
  video_server_mmdevice_name [0].id = CORBA::string_dup ("Video_Server_MMDevice");
  
  // Register the video control object with the naming server.
  naming_context->unbind (video_server_mmdevice_name,
                          TAO_TRY_ENV);
  // Unregister the audio_mmdevice with the naming service.

  CosNaming::Name audio_server_mmdevice_name (1);
  audio_server_mmdevice_name.length (1);
  audio_server_mmdevice_name [0].id = CORBA::string_dup ("Audio_Server_MMDevice");
  
  // Register the audio control object with the naming server.
  naming_context->unbind (audio_server_mmdevice_name,
                          TAO_TRY_ENV);

    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("AV_Server_Sig_Handler::remove_names ()");
      return -1;
    }
  TAO_ENDTRY;
  return 0;
}

//  ctrl-c, Bus error, interrupt sig handler
void
AV_Server_Sig_Handler::int_handler (int sig)
{
  // unbind the names from the naming service.

  this->remove_names ();
  TAO_ORB_Core_instance ()->orb ()->shutdown ();
  ACE_DEBUG ((LM_DEBUG, 
              "(%P|%t) AV server killed by signal %d\n",
              sig));
}

AV_Server_Sig_Handler::~AV_Server_Sig_Handler (void)
{
  //  TAO_ORB_Core_instance ()->reactor ()->remove_handler (this->sig_set);
}

// AV_Server routines

// Default Constructor
AV_Server::AV_Server (void)
  :video_process_strategy_ (&video_process_options_),
   audio_process_strategy_ (&audio_process_options_)
{
  this->video_process_options_.command_line ("./vs -ORBport 0 -ORBobjrefstyle url");
  this->audio_process_options_.command_line ("./as -ORBport 0 -ORBobjrefstyle url");
}

// %% move to the destructor or sig handler
void
AV_Server::on_exit_routine (void)
{
  // %% what does the following do
  if (Mpeg_Global::parentpid != ACE_OS::getpid ()) 
    {
      ACE_DEBUG ((LM_DEBUG, 
                  "(%P|%t) Process is exiting\n"));
      return;
    }
  
  // %% what does the following do
  if (Mpeg_Global::live_audio > 1)
    ExitLiveAudio ();
  if (Mpeg_Global::live_video > 1)
    ExitLiveVideo ();
  //  ComCloseServer();

  // Remove the elements from the Naming service.
}


// Parses the command line arguments
int
AV_Server::parse_args (int argc,
                         char **argv)
{
  ACE_Get_Opt get_opts (argc, argv, "rd:s:vamh");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'r': // real time flag
        Mpeg_Global::rttag = 1;
        break;
      case 'd': // clock drift in ppm
        Mpeg_Global::drift_ppm = ACE_OS::atoi (get_opts.optarg);
        break;
      case 's':// limit on number of sessions
        Mpeg_Global::session_limit = ACE_OS::atoi (get_opts.optarg);
        break;
      case 'v':// live video flag
        Mpeg_Global::live_video = 1;
        break;
      case 'a':// live audio flag
        Mpeg_Global::live_audio = 1;
        break;
      case '?':
      case 'h':// help flag
        ACE_DEBUG ((LM_DEBUG,
                    "Usage: %s [-r ] [-m]\n"
                    "          [-d#int(clock drift in ppm)]\n"
                    "          [-s#int(limit on number of sessions)]\n"
                    "          [-v] [-a] [-?] [-h]",
                    argv [0]));
        return -1;
      }
  return 0;
}

        
// Initializes the mpeg server
int
AV_Server::init (int argc,
                 char **argv,
                 CORBA::Environment& env)
{
  int result;

  // Initialize the orb_manager
  this->orb_manager_.init_child_poa (argc,
                                     argv,
                                     "child_poa",
                                     env);
  TAO_CHECK_ENV_RETURN (env,
                        -1);
  
  CORBA::ORB_var orb = 
    this->orb_manager_.orb ();

  PortableServer::POA_var child_poa = 
    this->orb_manager_.child_poa ();

  result = this->parse_args (argc, argv);
  if (result < 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) Error parsing arguments"),
                      -1);

  // Initialize the naming services
  if (my_name_client_.init (this->orb_manager_.orb ()) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
		       " (%P|%t) Unable to initialize "
		       "the TAO_Naming_Client. \n"),
		      -1);

  // Register the video mmdevice object with the ORB
  ACE_NEW_RETURN (this->video_mmdevice_,
                  TAO_MMDevice (&this->video_process_strategy_),
                  -1);

  // create the video server mmdevice with the naming service pointer.
  CORBA::String_var video_mmdevice_ior = this->orb_manager_.activate_under_child_poa ("Video_Server_MMDevice",
                                                                                      this->video_mmdevice_,
                                                                                      env);
  ACE_DEBUG ((LM_DEBUG,"(%P|%t) video_mmdevice_ior is :%s\n",video_mmdevice_ior.in ()));

  TAO_CHECK_ENV_RETURN (env,-1);

  // Register the video_mmdevice with the naming service.
  CosNaming::Name video_server_mmdevice_name (1);
  video_server_mmdevice_name.length (1);
  video_server_mmdevice_name [0].id = CORBA::string_dup ("Video_Server_MMDevice");
  
  // Register the video control object with the naming server.
  this->my_name_client_->bind (video_server_mmdevice_name,
                               this->video_mmdevice_->_this (env),
                               env);

  if (env.exception () != 0)
    {
      env.clear ();
      this->my_name_client_->rebind (video_server_mmdevice_name,
                              this->video_mmdevice_->_this (env),
                              env);
      TAO_CHECK_ENV_RETURN (env,-1);
    }
  

  // Register the audio mmdevice object with the ORB
  ACE_NEW_RETURN (this->audio_mmdevice_,
                  Audio_MMDevice (&this->audio_process_strategy_),
                  -1);

  // create the audio server mmdevice with the naming service pointer.
  CORBA::String_var audio_mmdevice_ior = this->orb_manager_.activate_under_child_poa ("Audio_Server_MMDevice",
                                                                                      this->audio_mmdevice_,
                                                                                      env);
  ACE_DEBUG ((LM_DEBUG,"Audio MMDevice ior is: %s\n",audio_mmdevice_ior.in ()));
  TAO_CHECK_ENV_RETURN (env,-1);

  // Register the audio_mmdevice with the naming service.

  CosNaming::Name audio_server_mmdevice_name (1);
  audio_server_mmdevice_name.length (1);
  audio_server_mmdevice_name [0].id = CORBA::string_dup ("Audio_Server_MMDevice");
  
  // Register the audio control object with the naming server.
  this->my_name_client_->bind (audio_server_mmdevice_name,
                        this->audio_mmdevice_->_this (env),
                        env);

  if (env.exception () != 0)
    {
      env.clear ();
      this->my_name_client_->rebind (audio_server_mmdevice_name,
                              this->audio_mmdevice_->_this (env),
                              env);
      TAO_CHECK_ENV_RETURN (env,-1);
    }
  


  // Register the various signal handlers with the reactor.
  result = this->signal_handler_.register_handler ();

  if (result < 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) Error registering signal handler"),
                      -1);

  ::atexit (on_exit_routine);
  return 0;
}

// Runs the mpeg server
int
AV_Server::run (CORBA::Environment& env){
  int result;
  // Run the ORB event loop
//   while (1)
//     {
      this->orb_manager_.run (env);
//       if (errno == EINTR)
//         continue;
//       else
//         break;
//     }

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) AV_Server::run () "
              "came out of the (ORB) "
              "event loop %p\n",
              "run_event_loop"));
  return 0;
  
}

AV_Server::~AV_Server (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) AV_Server: Removing handlers from the Reactor\n"));
  delete this->video_mmdevice_;
  delete this->audio_mmdevice_;
}

int
main (int argc, char **argv)
{
  TAO_TRY
    {
      if (AV_SERVER::instance ()->init (argc, argv, TAO_TRY_ENV) == -1)
        return 1;
      TAO_CHECK_ENV;
      AV_SERVER::instance ()->run (TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Exception");
      return -1;
    }
  TAO_ENDTRY;
  
  return 0;
}



