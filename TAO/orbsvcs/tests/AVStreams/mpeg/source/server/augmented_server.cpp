// $Id$

#include "augmented_server.h"
#include "ace/Process.h"

int AV_Server::done_;
pid_t AV_Server::current_pid_ = -1;

AV_Audio_MMDevice::
AV_Audio_MMDevice (TAO_AV_Endpoint_Process_Strategy *endpoint_strategy)
  :TAO_MMDevice (endpoint_strategy),
   connections_ (0)
{
}

AVStreams::StreamEndPoint_B_ptr  
AV_Audio_MMDevice::
create_B (AVStreams::StreamCtrl_ptr the_requester, 
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
AV_Audio_MMDevice::connections (void) const
{
  return this->connections_;
}

void
AV_Audio_MMDevice::
export_properties (TAO_Property_Exporter& prop_exporter) const
{
  CORBA::Any connections;

  connections <<= this->connections_;
  prop_exporter.add_static_property (AV_Property_Names[AV_NUMBER_OF_CONNECTIONS],
				     connections);
}

CORBA::Object_ptr
AV_Audio_MMDevice::
get_object (CORBA::Environment& _env)
{
  CORBA::Object_ptr object_ptr = this->_this (_env);
  TAO_CHECK_ENV_RETURN (_env, object_ptr);

  return object_ptr;
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
  CORBA::Object_var naming_obj = TAO_ORB_Core_instance ()->orb ()->resolve_initial_references ("NameService");
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
  TAO_ORB_Core_instance ()->reactor ()->remove_handler (this->sig_set);
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
  
  // Resolve the Naming service reference. 

  CORBA::Object_var naming_obj = orb->resolve_initial_references ("NameService");
  if (CORBA::is_nil (naming_obj.in ()))
    ACE_ERROR_RETURN ((LM_ERROR,
                       " (%P|%t) Unable to resolve the Name Service.\n"),
                      -1);

  this->naming_context_ =
    CosNaming::NamingContext::_narrow (naming_obj.in (),
                                       env);
  TAO_CHECK_ENV_RETURN (env,-1);

  // Register the video mmdevice object with the ORB
  ACE_NEW_RETURN (this->video_mmdevice_,
                  TAO_MMDevice (&this->video_process_strategy_),
                  -1);

  // create the video server mmdevice with the naming service pointer.
  this->orb_manager_.activate_under_child_poa ("Video_Server_MMDevice",
                                               this->video_mmdevice_,
                                               env);
  TAO_CHECK_ENV_RETURN (env,-1);
  
  // Register the video_mmdevice with the naming service.
  CosNaming::Name video_server_mmdevice_name (1);
  video_server_mmdevice_name.length (1);
  video_server_mmdevice_name [0].id = CORBA::string_dup ("Video_Server_MMDevice");
  
  // Register the video control object with the naming server.
  this->naming_context_->bind (video_server_mmdevice_name,
                               this->video_mmdevice_->_this (env),
                               env);
  
  if (env.exception () != 0)
    {
      env.clear ();
      this->naming_context_->rebind (video_server_mmdevice_name,
				     this->video_mmdevice_->_this (env),
				     env);
      TAO_CHECK_ENV_RETURN (env,-1);
    }
  
  
  // Register the audio mmdevice object with the ORB
  ACE_NEW_RETURN (this->audio_mmdevice_,
                  AV_Audio_MMDevice (&this->audio_process_strategy_),
                  -1);
  
  // create the audio server mmdevice with the naming service pointer.
  this->orb_manager_.activate_under_child_poa ("Audio_Server_MMDevice",
                                               this->audio_mmdevice_,
                                               env);
  TAO_CHECK_ENV_RETURN (env,-1);

  // Register the audio_mmdevice with the naming service.

  CosNaming::Name audio_server_mmdevice_name (1);
  audio_server_mmdevice_name.length (1);
  audio_server_mmdevice_name [0].id = CORBA::string_dup ("Audio_Server_MMDevice");
  
  // Register the audio control object with the naming server.
  this->naming_context_->bind (audio_server_mmdevice_name,
                        this->audio_mmdevice_->_this (env),
                        env);

  if (env.exception () != 0)
    {
      env.clear ();
      this->naming_context_->rebind (audio_server_mmdevice_name,
                              this->audio_mmdevice_->_this (env),
                              env);
      TAO_CHECK_ENV_RETURN (env,-1);
    }

  // Invoke this once, passing in an object for each trading service
  // service type.  
  this->resolve_trader (this->audio_mmdevice_, env);
  TAO_CHECK_ENV_RETURN (env, -1);

  // Invoke this for each offer.
  this->export_properties (this->audio_mmdevice_, env);
  TAO_CHECK_ENV_RETURN (env, -1);
  
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
  this->orb_manager_.run (env);

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) AV_Server::run () "
              "came out of the (ORB) "
              "event loop %p\n",
              "run_event_loop"));
  return 0;
  
}

void
AV_Server::export_properties (AV_Exportable* mmdevice,
			      CORBA::Environment& _env)
{
  CORBA::Object_ptr object_ptr = mmdevice->get_object (_env);
  TAO_CHECK_ENV_RETURN_VOID (_env);
  
  CosPropertyService::PropertySet_ptr prop_set =
    CosPropertyService::PropertySet::_narrow (object_ptr, _env);
  TAO_CHECK_ENV_RETURN_VOID (_env);
  
  // Instantiate the property exporter helper class.
  TAO_Property_Exporter prop_exporter (this->trader_, prop_set);

  // Add properties to server description.
  mmdevice->export_properties (prop_exporter);
      
  // Add the machine properties.
  ACE_DEBUG ((LM_ERROR, "Adding machine properties.\n"));
  for (int i = (int) TAO_Machine_Properties::CPU;
       i <= (int) TAO_Machine_Properties::LOAD; i++)
    {
      CORBA::Any extra_info;
      const char* name = TAO_Machine_Properties::PROP_NAMES[i];
      const CORBA::TypeCode_ptr prop_type = CORBA::_tc_float;

      extra_info <<= name;
      CosTradingDynamic::DynamicProp* dp_struct = 
	this->dp_.construct_dynamic_prop (name, prop_type, extra_info);
      
      this->dp_.register_handler (name, &this->mach_props_);
      prop_exporter.add_dynamic_property (name, dp_struct);
    }
  
  ACE_DEBUG ((LM_ERROR, "Exporting to the Trader.\n"));
  
  CosTrading::OfferId_var offer_id =
    prop_exporter.export (CORBA::Object::_duplicate (object_ptr),
			  (const CosTrading::ServiceTypeName) AV_MMDEVICE,
			  _env);
  TAO_CHECK_ENV_RETURN_VOID (_env);
}

void
AV_Server::resolve_trader (const AV_Audio_MMDevice* mmdevice,
			   CORBA::Environment& _env)
{
  if (this->trader_.ptr () == 0)
    {
      // Bootstrap to the Lookup interface.
      ACE_DEBUG ((LM_ERROR, "Bootstrap to the Lookup interface.\n"));
      CORBA::Object_var trading_obj =
	this->orb_manager_.orb ()->resolve_initial_references ("TradingService");
      
      if (CORBA::is_nil (trading_obj.in ()))
	{
	  ACE_ERROR ((LM_ERROR,
		      " (%P|%t) Unable to bootstrap to the Trading Service.\n"));
	  return;
	}
      
      // Narrow the lookup interface.
      ACE_DEBUG ((LM_DEBUG, "Narrowing the lookup interface.\n"));
      this->trader_ = CosTrading::Lookup::_narrow (trading_obj.in (), _env);
      TAO_CHECK_ENV_RETURN_VOID (_env);
      
      // Export the service types, filling in the service type
      // information first. 
      CosTradingRepos::ServiceTypeRepository::PropStructSeq prop_seq;
      CosTradingRepos::ServiceTypeRepository::ServiceTypeNameSeq type_name_seq;
      
      prop_seq.length (11);
      
      for (int i = 0; i <= TAO_Machine_Properties::NUM_PROPERTIES; i++)
	{
	  prop_seq[i].name = TAO_Machine_Properties::PROP_NAMES[i];
	  prop_seq[i].value_type = CORBA::TypeCode::_duplicate (CORBA::_tc_float);
	  prop_seq[i].mode = CosTradingRepos::ServiceTypeRepository::PROP_NORMAL;
	}
      
      prop_seq[10].name = AV_Property_Names[AV_NUMBER_OF_CONNECTIONS];
      prop_seq[10].value_type = CORBA::TypeCode::_duplicate (CORBA::_tc_long);
      prop_seq[10].mode = CosTradingRepos::ServiceTypeRepository::PROP_MANDATORY;
      
      CosTrading::TypeRepository_ptr obj = this->trader_->type_repos (_env);
      CosTradingRepos::ServiceTypeRepository_var str =
	CosTradingRepos::ServiceTypeRepository::_narrow (obj, _env);
      TAO_CHECK_ENV_RETURN_VOID (_env);
      
      TAO_TRY
	{
	  ACE_DEBUG ((LM_DEBUG, "Attempting add_type.\n"));
	  str->add_type (AV_MMDEVICE,
			 mmdevice->_interface_repository_id (),
			 prop_seq,
			 type_name_seq,
			 TAO_TRY_ENV);
	  TAO_CHECK_ENV;
	}
      TAO_CATCH (CosTradingRepos::ServiceTypeRepository::ServiceTypeExists, ste)
	{
	  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Ah, well the service type exists already."));
	}
      TAO_CATCHANY
	{
	  TAO_RETHROW;
	}
      TAO_ENDTRY;
    }
}

AV_Server::~AV_Server (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) AV_Server: Removing handlers from the Reactor\n"));

  if (this->video_mmdevice_ != 0)
    delete this->video_mmdevice_;
  if (this->audio_mmdevice_ != 0)
    delete this->audio_mmdevice_;
  
}

int
main (int argc, char **argv)
{
  TAO_TRY
    {
      int return_value =
	AV_SERVER::instance ()->init (argc, argv, TAO_TRY_ENV);
      TAO_CHECK_ENV;

      if (return_value != -1)
	{
	  AV_SERVER::instance ()->run (TAO_TRY_ENV);
	  TAO_CHECK_ENV;
	}
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Exception");
      return -1;
    }
  TAO_ENDTRY;
  
  return 0;
}



