// $Id$

#include "Command_Handler.h"
#include "ctr.cpp"

ACE_RCSID(mpeg_client, Command_Handler, "$Id$")

  static int temp_argc = 5;
static char *temp_argv [] = {"Command_Handler",
                              "-ORBobjrefstyle",
                              "url",
                         "-ORBport",
                         "0"
  };

Command_Handler::Command_Handler (ACE_HANDLE command_handle)
  :command_suspended_ (UNSUSPENDED),
   busy_ (0),
   video_data_handle_ (-1),
   audio_data_handle_ (-1),
   command_handle_ (command_handle),
   video_control_ (0),
   video_reactive_strategy_ (0),
   video_client_mmdevice_ (0),
   video_streamctrl_ (0),
   audio_control_ (0),
   audio_reactive_strategy_ (0),
   audio_client_mmdevice_ (0),
   audio_streamctrl_ (0),
   receiver_ (this),
   data_host_ (0),
   client_sig_handler_ (this),
   notification_handler_ (this)
{
}

Command_Handler::~Command_Handler (void)
{
  this->remove_handlers ();
  if (ACE_OS::getpid () == CTRpid)
    ::remove_all_semaphores ();
}

int
Command_Handler::parse_args (int argc,char **argv)
{
  ACE_Get_Opt get_opts (argc,argv,"h:");
  int c;
  
  while ((c= get_opts ()) != -1)
    switch (c)
      {
      case 'h':
        this->data_host_ = ACE_OS::strdup (get_opts.optarg);
        ACE_DEBUG ((LM_DEBUG,"data_host is %s\n",this->data_host_));
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,"Unknown argument %c\n",c),-1);
      }
  return 0;
}

TAO_ORB_Manager *
Command_Handler::orb_manager (void)
{
  return &this->orb_manager_;
}

void
Command_Handler::wait_for_display (Suspended reason)
{
  this->command_suspended_ = reason;
  TAO_ORB_Core_instance ()->reactor ()->suspend_handler (this->command_handle_);
}

// initialize the command handler.

int
Command_Handler::init (int argc,
                       char *argv[])
{
  this->argc_ = temp_argc;
  this->argv_ = temp_argv;
  
  // Increase the debug_level so that we can see the output
  this->parse_args (argc, argv);
  TAO_debug_level++;
  CORBA::String_var ior;
  TAO_TRY
    {
      this->orb_manager_.init_child_poa (this->argc_,
                                         this->argv_,
                                         "child_poa",
                                         TAO_TRY_ENV);
      TAO_CHECK_ENV;
      // activate the client video mmdevice under the child poa.
      ior = this->orb_manager_.activate (&this->receiver_,
                                         TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Initialize the naming services
      if (my_name_client_.init (orb_manager_.orb ()) != 0)
	ACE_ERROR_RETURN ((LM_ERROR,
			   " (%P|%t) Unable to initialize "
			   "the TAO_Naming_Client. \n"),
			  -1);

      char receiver_file[BUFSIZ];

      ACE_OS::sprintf (receiver_file,
                       "/tmp/receiver_ior.%d",
                       getuid ());
      // print the ior of the receiver to a file
      FILE *fp = ACE_OS::fopen (receiver_file,"w");
      if (fp != 0)
        {
          ACE_OS::fprintf (fp,"%s",ior.in ());
          ACE_OS::fclose (fp);
        }
      
      // initialize the client signal handler.
      if (this->client_sig_handler_.register_handler () < 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) register_handler for sig_handler failed\n"),
                          -1);

      if (TAO_ORB_Core_instance ()->reactor ()->register_handler (&this->notification_handler_,
                                                                  ACE_Event_Handler::READ_MASK) < 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) register_handler for notification handler failed\n"),
                          -1);
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Command_Handler::init");
      return -1;
    }
  TAO_ENDTRY;
  return 0;
}

// Runs the ORB event loop
int
Command_Handler::run (void)
{
  int result;
  TAO_TRY
    {
      result = this->orb_manager_.run (TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("orb:run ()");
      return -1;
    }
  TAO_ENDTRY;
  return result;
}


int
Command_Handler::handle_timeout (const ACE_Time_Value &,
                                  const void *arg)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t)Command_Handler::handle_timeout called \n"));
  return 0;
}


int
Command_Handler::resolve_audio_reference (void)
{
  TAO_TRY
    {
      if (this->audio_mmdevice_ior_.in ()!=0)
        {
          CORBA::Object_var mmdevice_obj = this->orb_manager_.orb ()->string_to_object (this->audio_mmdevice_ior_,
                                                                                        TAO_TRY_ENV);
          TAO_CHECK_ENV;
          if (CORBA::is_nil (mmdevice_obj.in ()) == 0)
            {
              this->audio_server_mmdevice_ = AVStreams::MMDevice::_narrow (mmdevice_obj.in (),
                                                                           TAO_TRY_ENV);
            }
          this->audio_mmdevice_ior_ = (char *)0;
          return 0;
        }

      CosNaming::Name audio_server_mmdevice_name (1);

      audio_server_mmdevice_name.length (1);
      audio_server_mmdevice_name [0].id = CORBA::string_dup ("Audio_Server_MMDevice");
      CORBA::Object_var audio_server_mmdevice_obj =
        this->my_name_client_->resolve (audio_server_mmdevice_name,
                                        TAO_TRY_ENV);
      TAO_CHECK_ENV;

      this->audio_server_mmdevice_ =
        AVStreams::MMDevice::_narrow (audio_server_mmdevice_obj.in (),
                                      TAO_TRY_ENV);
      TAO_CHECK_ENV;

      if (CORBA::is_nil (this->audio_server_mmdevice_.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " could not resolve Audio_Server_Mmdevice in Naming service\n"),
                          -1);
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) Audio_MMDevice successfully resolved: %s\n",
                  this->orb_manager_.orb ()->object_to_string (this->audio_server_mmdevice_.in (),
                                                               TAO_TRY_ENV)));
      TAO_CHECK_ENV;

    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Command_Handler::resolve_audio_reference");
      return -1;
    }
  TAO_ENDTRY;

  return 0;
}

int
Command_Handler::resolve_video_reference (void)
{
  TAO_TRY
    {
      if (this->video_mmdevice_ior_.in ()!= 0)
        {
          CORBA::Object_var mmdevice_obj = this->orb_manager_.orb ()->string_to_object (this->video_mmdevice_ior_,
                                                                                        TAO_TRY_ENV);
          TAO_CHECK_ENV;
          if (CORBA::is_nil (mmdevice_obj.in ()) == 0)
            {
              this->video_server_mmdevice_ = AVStreams::MMDevice::_narrow (mmdevice_obj.in (),
                                                                           TAO_TRY_ENV);
            }
          this->video_mmdevice_ior_ = (char *) 0;
          return 0;
        }
      CosNaming::Name video_server_mmdevice_name (1);

      video_server_mmdevice_name.length (1);
      video_server_mmdevice_name [0].id = CORBA::string_dup ("Video_Server_MMDevice");
      CORBA::Object_var video_server_mmdevice_obj =
        this->my_name_client_->resolve (video_server_mmdevice_name,
                                 TAO_TRY_ENV);
      TAO_CHECK_ENV;

      this->video_server_mmdevice_ =
        AVStreams::MMDevice::_narrow (video_server_mmdevice_obj.in (),
                                      TAO_TRY_ENV);
      TAO_CHECK_ENV;

      if (CORBA::is_nil (this->video_server_mmdevice_.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " could not resolve Video_Server_Mmdevice in Naming service\n"),
                          -1);
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) Video_MMDevice successfully resolved: %s\n",
                  this->orb_manager_.orb ()->object_to_string (this->video_server_mmdevice_.in (),
                                                               TAO_TRY_ENV)));
      TAO_CHECK_ENV;

    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Command_Handler::resolve_video_reference");
      return -1;
    }
  TAO_ENDTRY;

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) MMDevice successfully resolved.\n"));
  return 0;
}

ACE_HANDLE
Command_Handler::get_handle (void) const
{
  return this->command_handle_;
}

// handle the command sent to us by the GUI process
// this is a reactor callback method
int
Command_Handler::handle_input (ACE_HANDLE fd)
{
  unsigned char cmd;
  int val;

  //  ACE_DEBUG ((LM_DEBUG, "handle_input, fd: %d\n", fd));

  if (!(this->busy_))
    {
      // Weird hack coming up:
      if (fd == cmdSocket)
	{
	  val = OurCmdRead ((char*)&cmd, 1);
	  this->busy_ = 1;
	  this->client_sig_handler_.TimerProcessing ();
	}
      //  ACE_DEBUG ((LM_DEBUG, "val: %d, cmd: %d\n", val, cmd));
      // if we get an interrupt while reading we go back to the event loop
      if (val == 1)
	{
	  this->busy_ = 0;
          return 0;
	}

      //      fprintf(stderr, "CTR: cmd received - %d\n", cmd);
      TAO_TRY
        {
          switch (cmd)
            {
            case CmdINIT:
              ACE_DEBUG ((LM_DEBUG,"(%P|%t) command_handler:CmdINIT received\n"));
              if (this->init_av () == -1)
                {
                  this->busy_ = 0;
                  ACE_ERROR_RETURN ((LM_ERROR,"(%P|%t) init_av failed\n"),0);
                }
              //              cerr << "init_av done\n";
              break;
            case CmdSTOP:
              this->stop();
              break;
            case CmdFF:
              this->fast_forward ();
              break;
            case CmdFB:
              this->fast_backward ();
              break;
            case CmdSTEP:
              this->step ();
              break;
            case CmdPLAY:
              if (this->play (0,
                              TAO_TRY_ENV) < 0)
                {
                  this->busy_ = 0;
                  ACE_ERROR_RETURN ((LM_ERROR,"(%P|%t)play failed\n"),0);
                }
              TAO_CHECK_ENV;
              break;
            case CmdPOSITION:
              this->position ();
              break;
            case CmdPOSITIONrelease:
              this->position_release ();
              break;
            case CmdVOLUME:
              this->volume ();
              break;
            case CmdBALANCE:
              this->balance ();
              break;
            case CmdSPEED:
              this->speed ();
              break;
            case CmdLOOPenable:
              {
                shared->loopBack = 1;
                break;
              }
            case CmdLOOPdisable:
              {
                shared->loopBack = 0;
                break;
              }
            default:
              fprintf(stderr, "CTR: unexpected command from UI: cmd = %d.\n", cmd);
              ACE_OS::exit (1);
              break;
            }
          this->busy_ = 0;
          // unset the busy flag,done with processing the command.
        }
      TAO_CATCHANY
        {
          TAO_TRY_ENV.print_exception ("Command_Handler::handle_input ()");
          return 0;
        }
      TAO_ENDTRY;
    }
  return 0;
}

int
Command_Handler::init_av (void)
{
  cerr << "inside init_av \n";
  int i, j;

  /* try to stop and close previous playing */
  if (audioSocket >= 0 || videoSocket >= 0)
    {
      //      ACE_DEBUG ((LM_DEBUG, "(%P|%t) Reached line %d in %s\n", __LINE__, __FILE__));
      // this may have to be taken care of afterwards.
      unsigned char tmp = CmdCLOSE;
      int result =
        this->stop_playing();
      if (result < 0)
        return result;

      if (audioSocket >= 0)
         {
           if (ABpid > 0) {
             ACE_DEBUG ((LM_DEBUG,"killing the abprocess\n"));
             char message[BUFSIZ];
             message [0] = EXIT;
             ACE_OS::write (asp[0],&message,BUFSIZ);
             ABpid = -1;
           }
           usleep(10000);
         }
       if (videoSocket >= 0)
         {
           if (VBpid > 0) {
             char message[BUFSIZ];
             message [0] = EXIT;
             ACE_OS::write (vsp[0],&message,BUFSIZ);
             VBpid = -1;
           }
           usleep(10000);

           videoSocket = -1;
           while ((!vbuffer->VBbufEmpty()) || !VDbufEmpty()) {
             while (VDpeekMsg() != NULL) {
               VDreclaimMsg(VDgetMsg());
             }
             usleep(10000);
           }
           usleep(10000);
         }
    }
   this->close ();

  int result;
  /* read in video/audio files */
  // set the vf and af to 0 , very important.
  vf [0]= 0;
  af [0]=0;

  NewCmd(CmdINIT);

  i = 0;
  result = OurCmdRead((char*)&i, 4);
  result = OurCmdRead(vf, i);
  vf[i] = 0;
  result = OurCmdRead((char*)&i, 4);
  result = OurCmdRead(af, i);
  af[i] = 0;
  /*
    fprintf(stderr, "INIT: vh-%s, vf-%s, ah-%s, af-%s\n", vh, vf, ah, af);
  */

  shared->live = 0;
  shared->audioMaxPktSize = !shared->config.audioConn;
  shared->videoMaxPktSize = !shared->config.videoConn;

  if (af[0] != 0)
    {
      if ((result =init_audio_channel(af)) > 0)
        return result;
      else if (result < 0)
        {
          audioSocket = -1;
          shared->totalSamples = 0;
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG,"(%P|%t) Initialized audio\n"));
          shared->nextSample = 0;
          if (shared->config.maxSPS < shared->audioPara.samplesPerSecond)
            shared->config.maxSPS = shared->audioPara.samplesPerSecond;
        }
    }
  else
    {
      shared->totalSamples = 0;
      audioSocket = -1;
    }
  if (vf[0] != 0)
    {
      ACE_DEBUG ((LM_DEBUG,"(%P|%t) Initializing video\n"));
      if ((result = this->init_video_channel(vf)) > 0)
        return result;
      else if (result < 0)
        {
          shared->totalFrames = 0;      /* disable video channel */
          videoSocket = -1;
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG,
                      "%s %d\n",
                      __FILE__,__LINE__));
          shared->nextFrame = 1;
          shared->nextGroup = 0;
          shared->currentFrame = shared->currentGroup = shared->currentDisplay = 0;
          if (shared->config.maxFPS < shared->framesPerSecond)
            shared->config.maxFPS = shared->framesPerSecond;
        }
    }
  else
    {
      videoSocket = -1;
      shared->totalFrames = 0;  /* disable video channel */
    }
  if (audioSocket < 0 && videoSocket < 0)  /* none of video/audio channels is setup */
    {
      unsigned char tmp = CmdFAIL;
      CmdWrite(&tmp, 1);
      /*
        fprintf(stderr, "CTR initialization failed.\n");
      */
      return -1;
    }
  else
    {
      unsigned char tmp = CmdDONE;
      set_speed();
      if (videoSocket >= 0)
        this->wait_for_display (INIT);
      CmdWrite(&tmp, 1);
      if (videoSocket < 0)
        {
          tmp = CmdVPclearScreen;
          CmdWrite(&tmp, 1);
        }
      return 0;
    }
  cerr << "returning from init_av \n";
  return 0;
}

int
Command_Handler::init_java_av (char *audio_ior,
                               char *video_ior,
                               char *audio_file,
                               char *video_file)
{
  if (audio_file != 0)
    ACE_DEBUG ((LM_DEBUG,"%s\n",audio_file));
  if (video_file != 0)
    ACE_DEBUG ((LM_DEBUG,"%s\n",video_file));
  int j;
  this->audio_mmdevice_ior_ = audio_ior;
  this->video_mmdevice_ior_ = video_ior;

  /* try to stop and close previous playing */
  if (audioSocket >= 0 || videoSocket >= 0)
    {
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) Reached line %d in %s\n", __LINE__, __FILE__));
      // this may have to be taken care of afterwards.
      unsigned char tmp = CmdCLOSE;
      int result =
        this->stop_playing();
      if (result < 0)
        ACE_ERROR_RETURN ((LM_ERROR,"stop_playing failed\n"),result);

      if (audioSocket >= 0)
        {
          if (ABpid > 0) {
            ACE_DEBUG ((LM_DEBUG,"Killing AB Process\n"));
            char message[BUFSIZ];
            message [0] = EXIT;
            ACE_OS::write (asp[0],&message,BUFSIZ);
            ABpid = -1;
          }
          usleep(10000);
        }

      if (videoSocket >= 0)
        {
          if (VBpid > 0) {
            char message[BUFSIZ];
            message [0] = EXIT;
            ACE_OS::write (vsp[0],&message,BUFSIZ);
            VBpid = -1;
          }
          usleep(10000);

          videoSocket = -1;
          while ((!vbuffer->VBbufEmpty()) || !VDbufEmpty()) {
            while (VDpeekMsg() != NULL) {
              VDreclaimMsg(VDgetMsg());
            }
            usleep(10000);
          }
          usleep(10000);
        }
    }
  this->close ();

  int result;
  NewCmd(CmdINIT);

  shared->live = 0;
  shared->audioMaxPktSize = !shared->config.audioConn;
  shared->videoMaxPktSize = !shared->config.videoConn;

  if (audio_file!= 0)
    {
      int result;
      if ((result = init_audio_channel(audio_file)) > 0)
        return result;
      else if (result < 0)
        {
          audioSocket = -1;
          shared->totalSamples = 0;
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG,"(%P|%t) Initialized audio\n"));
          shared->nextSample = 0;
          if (shared->config.maxSPS < shared->audioPara.samplesPerSecond)
            shared->config.maxSPS = shared->audioPara.samplesPerSecond;
        }
    }
  else
    {
      shared->totalSamples = 0;
      audioSocket = -1;
    }
  if (video_file!= 0)
    {
      ACE_DEBUG ((LM_DEBUG,"(%P|%t) Initializing video\n"));
      if ((result = this->init_video_channel(video_file)) > 0)
        return result;
      else if (result < 0)
        {
          shared->totalFrames = 0;      /* disable video channel */
          videoSocket = -1;
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG,
                      "%s %d\n",
                      __FILE__,__LINE__));
          shared->nextFrame = 1;
          shared->nextGroup = 0;
          shared->currentFrame = shared->currentGroup = shared->currentDisplay = 0;
          if (shared->config.maxFPS < shared->framesPerSecond)
            shared->config.maxFPS = shared->framesPerSecond;
        }
    }
  else
    {
      videoSocket = -1;
      shared->totalFrames = 0;  /* disable video channel */
    }
  if (audioSocket < 0 && videoSocket < 0)  /* none of video/audio channels is setup */
    {
      unsigned char tmp = CmdFAIL;
      CmdWrite(&tmp, 1);

      fprintf(stderr, "CTR initialization failed.\n");

      return -1;
    }
  else
    {
      unsigned char tmp = CmdDONE;
      set_speed();
      if (videoSocket >= 0)
        this->wait_for_display (INIT);
      CmdWrite(&tmp, 1);
      if (videoSocket < 0)
        {
          tmp = CmdVPclearScreen;
          CmdWrite(&tmp, 1);
        }
      return 0;
    }
  cerr << "returning from init_av \n";
  return 0;
}


int
Command_Handler::init_audio_channel (char *audiofile)
{
  ACE_DEBUG ((LM_DEBUG,"(%P|%t) init_audio_channel called\n"));
  this->audio_data_handle_ = -1;

  if (!hasAudioDevice)
  {
    fprintf(stderr, "CTR warning: Audio device not available, Audio ignored.\n");
    return -1;
  }

  if (this->connect_to_audio_server() == -1)
    return -1;

  shared->audioMaxPktSize = -INET_SOCKET_BUFFER_SIZE;
  /* Initialize with AS */
  {
    Audio_Control::INITaudioPara_var para (new
                                           Audio_Control::INITaudioPara);
    Audio_Control::INITaudioReply_var reply (new Audio_Control::INITaudioReply);


    para->sn = shared->cmdsn;
    para->version = VERSION;
    para->para.encodeType = shared->AFPara.encodeType;
    para->para.channels = shared->AFPara.channels;
    para->para.samplesPerSecond = shared->AFPara.samplesPerSecond;
    para->para.bytesPerSample = shared->AFPara.bytesPerSample;
    para->nameLength =ACE_OS::strlen (audiofile)+1;

    para->audiofile.length (strlen(audiofile));

    for (u_int i=0;i<para->audiofile.length ();i++)
      para->audiofile [i] = audiofile [i];

    // CORBA call
    TAO_TRY
      {
        CORBA::Boolean result;
        result = this->audio_control_->init_audio (para.in (),
                                                   reply.out (),
                                                   TAO_TRY_ENV);
        TAO_CHECK_ENV;
        if (result == (0))
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%P|%) init_audio failed\n"),
                            -1);
        else
          ACE_DEBUG ((LM_DEBUG,"(%P|%t) init_audio success \n"));
      }
    TAO_CATCHANY
      {
        TAO_TRY_ENV.print_exception ("audio_control_->init_audio (..)");
        return -1;
      }
    TAO_ENDTRY;

    /*
    fprintf(stderr, "AF Audio para: encode %d, ch %d, sps %d, bps %d.\n",
	    para.para.encodeType, para.para.channels,
	    para.para.samplesPerSecond, para.para.bytesPerSample);
     */
    {
      int flag = 1;

      shared->live += reply->live;
      shared->audioFormat = reply->format;
      shared->audioPara.encodeType = reply->para.encodeType;
      shared->audioPara.channels = reply->para.channels;
      shared->audioPara.samplesPerSecond = reply->para.samplesPerSecond;
      shared->audioPara.bytesPerSample = reply->para.bytesPerSample;
      shared->totalSamples = reply->totalSamples;

      fprintf(stderr, "Audio: samples %d, sps %d, bps %d\n",
	      shared->totalSamples, shared->AFPara.samplesPerSecond,
	      shared->AFPara.bytesPerSample);

      SetAudioParameter(&shared->audioPara);
    }

    /* create AB */
    {
      if (ABpid == -1)
        {
          switch (ABpid = ACE_OS::fork ())
            {
            case -1:
             ACE_OS::perror ("CTR error on forking AB process");
              ACE_OS::exit (1);
              break;
            case 0:
              {
                if (realTimeFlag) {
                  SetRTpriority("AB", -1);
                }
                ACE_OS::free (vh);
                ACE_OS::free (vf);
                ACE_OS::free (ah);
                ACE_OS::free (audiofile);
                vbuffer->VBdeleteBuf();
                VDdeleteBuf();
                if (cmdSocket >= 0)
                  ACE_OS::close(cmdSocket);
                if (realTimeFlag >= 2) {
#ifdef __svr4__
                  if (SetRTpriority("AB", 0)) realTimeFlag = 0;
#elif defined(_HPUX_SOURCE)
                  if (SetRTpriority("AB", 1)) realTimeFlag = 0;
#endif
                }
                abuffer->ABprocess(this->audio_data_handle_);
                this->remove_handlers ();
                // Before shutting down the orb remove all the handlers registered with it.
                ACE_DEBUG ((LM_DEBUG,"(%P|%t) Before shutdown"));
                this->orb_manager_.orb ()->shutdown ();
                ACE_DEBUG ((LM_DEBUG,"(%P|%t) After shutdown"));
                return 1;
              }
              break;
            default:
              abuffer->ABflushBuf(0);
              break;
            }
        }
    }
  }
  ACE_DEBUG ((LM_DEBUG,"(%P) Init_audio_channel done\n"));
  return 0;
}

int
Command_Handler::remove_handlers (void)
{
  int result;
  result = this->client_sig_handler_.remove_handler ();
  
  if (TAO_ORB_Core_instance ()->reactor ()->remove_handler (&this->notification_handler_,
                                                              ACE_Event_Handler::READ_MASK) < 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) remove_handler for notification handler failed\n"),
                      -1);
  return 0;
  
}

void
Command_Handler::set_mmdevice (CORBA::Object_ptr audio_mmdevice,
                               const char *audio_file,
                               CORBA::Object_ptr video_mmdevice,
                               const char *video_file)

{
  CORBA::String audio_mmdevice_ior (0);
  CORBA::String video_mmdevice_ior (0);
  TAO_TRY
    {
      audio_mmdevice_ior =
	this->orb_manager_.orb ()->object_to_string (audio_mmdevice,
						     TAO_TRY_ENV);
      TAO_CHECK_ENV;
      ACE_DEBUG ((LM_DEBUG,"audio_mmdevice ior is %s\n",audio_mmdevice_ior));
      video_mmdevice_ior =
        this->orb_manager_.orb ()->object_to_string (video_mmdevice,
                                                     TAO_TRY_ENV);
      TAO_CHECK_ENV;
      ACE_DEBUG ((LM_DEBUG,"audio_mmdevice ior is %s\n",video_mmdevice_ior));
    }
  TAO_CATCHANY
    {
      ACE_ERROR ((LM_ERROR,"set_audio_mmdevice failed\n"));
    }
  TAO_ENDTRY;
  this->init_java_av (audio_mmdevice_ior,video_mmdevice_ior,(char *)audio_file,(char *)video_file);
  return;
}

void
Command_Handler::set_video_data_handle (ACE_HANDLE data_fd)
{
  videoSocket = this->video_data_handle_= data_fd;
}

void
Command_Handler::set_video_control (Video_Control_ptr video_control)
{
  ACE_DEBUG ((LM_DEBUG,"(%P|%t) Setting the video control\n"));
  this->video_control_ = video_control;
}

void
Command_Handler::set_audio_data_handle (ACE_HANDLE data_fd)
{
  audioSocket = this->audio_data_handle_= data_fd;
}

void
Command_Handler::set_audio_control (Audio_Control_ptr audio_control)
{
  ACE_DEBUG ((LM_DEBUG,"(%P|%t) setting the audio control\n"));
  this->audio_control_ = audio_control;
}

char*
Command_Handler::get_data_host (void)
{
  return this->data_host_;
}

int
Command_Handler::init_video_channel (char *videofile)
{
  //  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Reached line %d in %s\n",__LINE__, __FILE__,

  fprintf (stderr," File Name is %s\n",videofile);

  if (this->connect_to_video_server () == -1)
    return -1;
  // set the max_pkt size for video ?? very minimum
  shared->videoMaxPktSize = -INET_SOCKET_BUFFER_SIZE;
  /* Initialize with VS */
  {
    //    ACE_DEBUG ((LM_DEBUG, "(%P|%t) Reached line %d in %s\n", __LINE__, __FILE__));
    Video_Control::INITvideoPara_var  para (new
                                            Video_Control::INITvideoPara);
    Video_Control::INITvideoReply *reply_ptr = 0;
    Video_Control::INITvideoReply_out reply (reply_ptr);

    para->sn = shared->cmdsn;
    para->version = VERSION;
    para->videofile.length (strlen(videofile));

    // string to sequence <char>
    for (int i=0;i<para->videofile.length ();i++)
      para->videofile [i] = videofile [i];

    // CORBA call
    TAO_TRY
      {
        CORBA::Boolean result;
        result = this->video_control_->init_video (para.in (),
                                                   reply,
                                                   TAO_TRY_ENV);
        TAO_CHECK_ENV;
        if (result == (0))
          return -1;
        else
          ACE_DEBUG ((LM_DEBUG,"(%P|%t) init_video success \n"));
      }
    TAO_CATCHANY
      {
        TAO_TRY_ENV.print_exception ("video_control_->init_video (..)");
        return -1;
      }
    TAO_ENDTRY;
    shared->live += reply->live;
    shared->videoFormat = reply->format;
    shared->totalHeaders = reply->totalHeaders;
    shared->totalFrames = reply->totalFrames;
    shared->totalGroups = reply->totalGroups;
    shared->averageFrameSize = reply->averageFrameSize;
    shared->horizontalSize = reply->horizontalSize;
    shared->verticalSize = reply->verticalSize;
    shared->pelAspectRatio = reply->pelAspectRatio;
    shared->pictureRate = ((double)reply->pictureRate1000) / 1000.0;
    shared->vbvBufferSize = reply->vbvBufferSize;
    shared->firstGopFrames = reply->firstGopFrames;
    shared->patternSize = reply->pattern.length ();
    if (shared->patternSize == 0) {

      Fprintf(stderr, "CTR warning: patternsize %d\n", shared->patternSize);

      shared->patternSize = 1;
      shared->pattern[0]  = 'I';
      shared->pattern[1] = 0;
      shared->IframeGap = 1;
    }
    else if (shared->patternSize < PATTERN_SIZE)
      {
        int i;
        char * ptr = shared->pattern + shared->patternSize;
        //       strncpy(shared->pattern, reply->pattern, shared->patternSize);
        for (i=0;i<shared->patternSize;i++)
          shared->pattern[i] = reply->pattern [i];
        for (i = 1; i < PATTERN_SIZE / shared->patternSize; i ++) {
          //         ACE_OS::memcpy (ptr, shared->pattern, shared->patternSize);
          for (int j=0; j < shared->patternSize ;j++)
            ptr [j] = shared->pattern [j];
          ptr += shared->patternSize;
        }
        shared->IframeGap = 1;
        while (shared->IframeGap < shared->patternSize)
          {
            if (shared->pattern[shared->IframeGap] == 'I')
              break;
            else
              shared->IframeGap ++;
          }
      }
    else
      {
        fprintf(stderr, "CTR Error: patternSize %d greater than PATTERN_SIZE %d.\n",
                shared->patternSize, PATTERN_SIZE);
        ACE_OS::exit (1);
      }
    fprintf(stderr, "Video: %s, %s\n",
            shared->videoFormat == VIDEO_SIF ? "SIF" :
            shared->videoFormat == VIDEO_JPEG ? "JPEG" :
            shared->videoFormat == VIDEO_MPEG1 ? "MPEG1" :
            shared->videoFormat == VIDEO_MPEG2 ? "MPEG2" : "UNKOWN format",
            reply->live ? "live source" : "stored source");

    fprintf(stderr, "Video: numS-%d, numG-%d, numF-%d, aveFrameSize-%d\n",
            reply->totalHeaders, reply->totalGroups, reply->totalFrames,
            reply->averageFrameSize);
    fprintf(stderr, "Video: maxS-%d, maxG-%d, maxI-%d, maxP-%d, maxB-%d\n",
            reply->sizeSystemHeader, reply->sizeGop,
            reply->sizeIFrame, reply->sizePFrame, reply->sizeBFrame);
    fprintf(stderr,
            "Video: SHinfo: hsize-%d, vsize-%d, pelAspect-%d, rate-%f, vbv-%d.\n",
            reply->horizontalSize, reply->verticalSize, reply->pelAspectRatio,
            shared->pictureRate, reply->vbvBufferSize);
    shared->pattern[shared->patternSize] = 0;
    fprintf(stderr, "Video: firstGopFrames %d, IframeGap %d\n",
            reply->firstGopFrames,  shared->IframeGap);
    shared->pattern[shared->patternSize] = 'I';
    if (reply->totalFrames > MAX_FRAMES && (!shared->live))
      {
        fprintf(stderr,
                "Error: totalFrames %d > MAX_FRAMES %d, needs change and recompile.\n",
                reply->totalFrames, MAX_FRAMES);
        ComCloseConn(this->video_data_handle_);
        //  ComCloseConn(videoSocket);
        videoSocket = -1;
        return -1;
      }
    //    ACE_DEBUG ((LM_DEBUG, "(%P|%t) Reached line %d in %s\n", __LINE__, __FILE__));

    /* create VB, and put INIT frame to VB*/
    {
          int sp[2];  /* sp[0] is for CTR and sp[1] is for VB */

          /* create command socket pair for sending INIT frame to VB, the pipe
             should be discard/non-discard in consistent with videoSocket*/
          if (socketpair(AF_UNIX,
                         shared->videoMaxPktSize >= 0 ? SOCK_STREAM :
                         SOCK_DGRAM, 0, sp) == -1)
            {
             ACE_OS::perror ("CTR error on open CTR-VB socketpair");
              ACE_OS::exit (1);
            }
    
      if (VBpid == -1)
        {
          switch (VBpid = ACE_OS::fork ())
            {
            case -1:
             ACE_OS::perror ("CTR error on forking VB process");
              ACE_OS::exit (1);
              break;
            case 0:
              ACE_DEBUG ((LM_DEBUG, "(%P|%t) Reached line %d in %s\n", __LINE__, __FILE__));
              if (realTimeFlag) {
                SetRTpriority("VB", -1);
              }
              ACE_OS::free (vh);
              ACE_OS::free (videofile);
              ACE_OS::free (ah);
              ACE_OS::free (af);
              ::close(sp[0]);
              //::close (videoSocket);
              //          if (audioSocket >= 0)
              //            ComCloseFd(audioSocket);
              ::close (audioSocket);
              abuffer->ABdeleteBuf();
              VDdeleteBuf();
              if (cmdSocket >= 0)
                ::close(cmdSocket);
              if (realTimeFlag >= 2) {
#ifdef __svr4__
                if (SetRTpriority("VB", 0)) realTimeFlag = 0;
#elif defined(_HPUX_SOURCE)
                if (SetRTpriority("VB", 1)) realTimeFlag = 0;
#endif
              }
              if (vbuffer->VBprocess(sp[1], this->video_data_handle_) == -1)
                {
                  ACE_DEBUG ((LM_DEBUG,"VBprocess exiting as init failed\n"));
                  ACE_OS::exit (2);
                }
              else
                {
                  this->remove_handlers ();
                  this->orb_manager_.orb ()->shutdown ();
                  return 1;
                }
              break;
            default:
              ::close(sp[1]);
              //          ::close(dataSocket);
              {
                int bytes, res;
                /* passing all messages of INIT frame to VB here. */
                char * buf = (char *)ACE_OS::malloc(INET_SOCKET_BUFFER_SIZE);
                VideoMessage *msg = (VideoMessage *)buf;
                int pkts = 1, msgo = 0, msgs = 0;

                if (buf == NULL) {
                 ACE_OS::perror ("CTR error on ACE_OS::malloc() for INIT frame");
                  ACE_OS::exit (1);
                }
                while (msgo + msgs < pkts) {
                  //              ACE_DEBUG ((LM_DEBUG, "(%P|%t) Reached line %d in %s\n", __LINE__, __FILE__));
                  //              cerr << "expecting a packet of size " << sizeof (*msg)                   << endl;

                  VideoRead(buf, sizeof(*msg));
                  //~~ we need to read the first frame from the
                  //  data socket instead of control socket.
                  //              SocketRecv(dataSocket, buf, size);
                  //              ACE_DEBUG ((LM_DEBUG,"packetsn = %d,msgsn = %d\n",msg->packetsn,msg->msgsn));
                  pkts = ntohl(msg->packetSize);
                  msgo = ntohl(msg->msgOffset);
                  msgs = ntohl(msg->msgSize);
                  if (shared->videoMaxPktSize >= 0) {  /* non-discard mode */
                    //                ACE_DEBUG ((LM_DEBUG, "(%P|%t) Reached line %d in %s\n", __LINE__, __FILE__));
                    write_bytes(sp[0], buf, sizeof(*msg));
                    bytes = msgs;
                    while (bytes > 0) {
                      int size = min(bytes, INET_SOCKET_BUFFER_SIZE);
                      VideoRead(buf, size);
                      write_bytes(sp[0], buf, size);
                      bytes -= size;
                    }
                  }
                  else {
                    //                cerr << "expecting a packet of size " << msgs                     << endl;

                    VideoRead(buf + sizeof(*msg), msgs);
                    bytes = sizeof(*msg) + msgs;
                    while ((res = ACE_OS::write (sp[0], buf, bytes)) == -1) {
                      if (errno == EINTR || errno == ENOBUFS) continue;
                     ACE_OS::perror ("CTR error on sending INIT frame to VB");
                      ACE_OS::exit (1);
                    }
                    if (res < bytes) {
                      fprintf(stderr, "CTR warn: send() res %dB < bytes %dB\n", res, bytes);
                    }
                    /*
                      Fprintf(stderr,
                      "CTR transferred INIT frame to VB: pkts %d, msgo %d, msgs %d\n",
                      pkts, msgo, msgs);
                    */
                  }
                }
                ACE_OS::read (sp[0], buf, 1); /* read a garbage byte, to sync with VB */
                fprintf (stderr,"(%P|%t) Command process read a garbage bytes\n");
                ::close(sp[0]);
                ACE_OS::free (buf);
              }
              break;
            }
        }
    }
  }
  return 0;
}


int
Command_Handler::stat_stream (CORBA::Char_out ch,
                              CORBA::Long_out size)
{
  return 0;
}


int
Command_Handler::close (void)
{
  TAO_TRY
    {
      if (CORBA::is_nil (this->audio_control_) == 0)
        {
          // one way function call.
          this->audio_control_->close (TAO_TRY_ENV);

          TAO_CHECK_ENV;
          if (ABpid > 0) {
            char message[BUFSIZ];
            message [0] = EXIT;
            ACE_OS::write (asp[0],&message,BUFSIZ);
            ABpid = -1;
          }
          ACE_DEBUG ((LM_DEBUG,"(%P|%t) audio close done \n"));
        }

      if (CORBA::is_nil (this->video_control_) == 0)
        {
          // one way function call.
          this->video_control_->close (TAO_TRY_ENV);
          TAO_CHECK_ENV;
          if (VBpid > 0) {
            char message[BUFSIZ];
            message [0] = EXIT;
            ACE_OS::write (vsp[0],&message,BUFSIZ);
            VBpid = -1;
          }
          ACE_DEBUG ((LM_DEBUG,"(%P|%t) video close done \n"));
        }
      this->orb_manager_.child_poa ()->destroy (1,0,TAO_TRY_ENV);
      TAO_CHECK_ENV;
      ACE_DEBUG ((LM_DEBUG,"(%P|%t)destroy childpoa done\n"));

      this->orb_manager_.init_child_poa (argc_,
                                         argv_,
                                         "child_poa",
                                         TAO_TRY_ENV);
      TAO_CHECK_ENV;
      ACE_DEBUG ((LM_DEBUG,"(%P)New child poa initialized\n"));
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("video_control_->close (..)");
      return -1;
    }
  TAO_ENDTRY;
  delete this->audio_reactive_strategy_;
  ACE_DEBUG ((LM_DEBUG,"(%P)%N,%l"));
  delete this->audio_client_mmdevice_;
  ACE_DEBUG ((LM_DEBUG,"(%P)%N,%l"));
  //      delete this->audio_streamctrl_;
  ACE_DEBUG ((LM_DEBUG,"(%P)%N,%l"));
  delete this->video_reactive_strategy_;
  ACE_DEBUG ((LM_DEBUG,"(%P)%N,%l"));
  delete this->video_client_mmdevice_;
  ACE_DEBUG ((LM_DEBUG,"(%P)%N,%l"));
  //      delete this->video_streamctrl_;
  ACE_DEBUG ((LM_DEBUG,"(%P)%N,%l"));
  return 0;
}


int
Command_Handler::stat_sent (void)
{
  return 0;
}


int
Command_Handler::fast_forward (void)

{
   // CORBA call
  unsigned char tmp;
  Video_Control::FFpara_var para (new Video_Control::FFpara);
  /*
  fprintf(stderr, "CTR: FF . . .\n");
  */
  if (shared->live) {
    beep();
  }
  else {
    this->stop_playing ();
    if (shared->nextGroup < 0)
      shared->nextGroup = 0;
    if (videoSocket >= 0 && shared->nextGroup < shared->totalGroups)
      {
        NewCmd(CmdFF);
        shared->needHeader = 0;
        shared->framesPerSecond = shared->config.ffFPS /
          shared->patternSize;
        shared->usecPerFrame = (int)(1000000.0 / (float)shared->config.ffFPS) *
          shared->patternSize;

        shared->VStimeAdvance =
	  max(shared->config.VStimeAdvance, DEFAULT_VStimeAdvance) * 1000;
        if (shared->VStimeAdvance < shared->usecPerFrame)
          shared->VStimeAdvance = shared->usecPerFrame;

        para->VStimeAdvance = shared->VStimeAdvance;
        para->sn = shared->cmdsn;
        para->nextGroup = shared->nextGroup;
        para->usecPerFrame = shared->usecPerFrame;
        para->framesPerSecond = shared->framesPerSecond;
        startTime = get_usec();
        TAO_TRY
          {
            CORBA::Boolean result;
            result = this->video_control_->fast_forward (para.in (),
                                                         TAO_TRY_ENV);
            TAO_CHECK_ENV;
            if (result == (0))
              return -1;
            ACE_DEBUG ((LM_DEBUG,"(%P|%t) fast_forward done \n"));
          }
        TAO_CATCHANY
          {
            TAO_TRY_ENV.print_exception ("video_control_->fast_forward_video (..)");
            return -1;
          }
        TAO_ENDTRY;
        start_timer();
      }
  }
  tmp = CmdDONE;
  CmdWrite(&tmp, 1);
  return 0;
}


int
Command_Handler::fast_backward (void)

{
  unsigned char tmp;
  Video_Control::FBpara_var para (new Video_Control::FBpara);
  /*
  fprintf(stderr, "CTR: FB . . .\n");
  */
  if (shared->live) {
    beep();
  }
  else {
    this->stop_playing();
    if (shared->nextGroup >= shared->totalGroups)
      shared->nextGroup = shared->totalGroups - 1;
    if (videoSocket >= 0 && shared->nextGroup >= 0)
    {
      NewCmd(CmdFB);
      shared->needHeader = 0;
      shared->framesPerSecond = shared->config.fbFPS /
			     shared->patternSize;
      shared->usecPerFrame = (int)(1000000.0 / (float)shared->config.fbFPS) *
			     shared->patternSize;

      shared->VStimeAdvance =
	  max(shared->config.VStimeAdvance, DEFAULT_VStimeAdvance) * 1000;
      if (shared->VStimeAdvance < shared->usecPerFrame)
	shared->VStimeAdvance = shared->usecPerFrame;

      para->VStimeAdvance = shared->VStimeAdvance;
      para->sn = shared->cmdsn;
      para->nextGroup = shared->nextGroup;
      para->usecPerFrame = shared->usecPerFrame;
      para->framesPerSecond = shared->framesPerSecond;
      startTime = get_usec();
        TAO_TRY
          {
            CORBA::Boolean result;
            result = this->video_control_->fast_backward (para.in (),
                                                          TAO_TRY_ENV);
            TAO_CHECK_ENV;
            if (result == (0))
              return -1;
            ACE_DEBUG ((LM_DEBUG,"(%P|%t) fast_backward done \n"));
          }
        TAO_CATCHANY
          {
            TAO_TRY_ENV.print_exception ("video_control_->fast_forward_video (..)");
            return -1;
          }
        TAO_ENDTRY;

        start_timer();
    }
  }
  tmp = CmdDONE;
  CmdWrite(&tmp, 1);
  return 0;
}


int
Command_Handler::step (void)

{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Command_Handler::step ()\n"));
  Video_Control::STEPpara_var para (new Video_Control::STEPpara);
  this->stop_playing ();
  NewCmd (CmdSTEP);
  if (videoSocket >= 0 && shared->nextFrame <= shared->totalFrames)
    { /* when shared->nextFrame == shared->totalFrame, it will force VS to send a ENDSEQ,
         to let VD give out the remaining frame in its ring[] buffer */
      para->sn = shared->cmdsn;
      para->nextFrame = shared->nextFrame;
      TAO_TRY
        {
          CORBA::Boolean result;
          result = this->video_control_->step (para.in (),
                                               TAO_TRY_ENV);
          ACE_DEBUG ((LM_DEBUG,"(%P|%t) step done \n"));
          TAO_CHECK_ENV;
          if (result == (0))
            return -1;
          }
        TAO_CATCHANY
          {
            TAO_TRY_ENV.print_exception ("video_control_->step (..)");
            return -1;
          }
        TAO_ENDTRY;
     /*
        fprintf(stderr, "CTR: STEP . . . frame-%d\n", para.nextFrame);
    */
    this->wait_for_display (STEP);
  }
//   unsigned char tmp = CmdDONE;
//   CmdWrite(&tmp, 1);
  return 0;
}

int
Command_Handler::play (int auto_exp,
                       CORBA::Environment& env)
{
  CORBA::Long vts;
  CORBA::Long ats;
  CORBA::Boolean result;
  unsigned char tmp;
  int cmdstarted = 0;
  int stuffsamples = 0;

  //  this->timer_.start ();
  fprintf (stderr, "CTR: PLAY . . .\n");

  this->stop_playing ();

  if (!shared->live && !shared->config.rt && videoSocket >= 0) {
    /* rtplay turned off only when video avaible and not want RT play */
    rtplay = 0;
    fprintf (stderr, "VCR is not playing at in realtime mode, audio disabled\n");
  }
  else {
    rtplay = 1;
  }

  if (shared->live) {
    rtplay = 1;
    shared->nextFrame = 0;
    shared->nextSample = 0;
  }

  shared->rtplay = rtplay;

  if (shared->nextFrame < 0)
    shared->nextFrame = 0;
  else if (shared->nextFrame >= shared->totalFrames)
    {
      shared->nextFrame = shared->totalFrames - 1;
    }

  //  ACE_DEBUG ((LM_DEBUG,"(%P|%t) nextFrame=%d,totalFrames=%d",
  //              shared->nextFrame,shared->totalFrames));
  if (audioSocket >= 0 && shared->nextSample < shared->totalSamples && rtplay)
    {
      //      ACE_DEBUG ((LM_DEBUG,"(%P|%t) %s,%d\n",__FILE__,__LINE__));
      Audio_Control::PLAYPara_var para (new Audio_Control::PLAYPara);
      if (cmdstarted == 0)
        {
          NewCmd (CmdPLAY);
          if (!auto_exp) set_speed ();
          cmdstarted = 1;
        }

      if (videoSocket >= 0 && rtplay && !shared->live) {
        /* video channel also active, recompute nextSample */
        shared->nextSample = (int) ( (double)shared->audioPara.samplesPerSecond *
                                     ( (double)shared->nextFrame / shared->pictureRate));
        shared->nextSample += shared->config.audioOffset;
        if (shared->nextSample < 0) {
          stuffsamples = (- shared->nextSample);
          shared->nextSample = 0;
        }
        else if (shared->nextSample >= shared->totalSamples)
          shared->nextSample = shared->totalSamples - 1;
      }

      abuffer->ABflushBuf (shared->nextSample);

      para->sn = shared->cmdsn;
      para->nextSample = shared->nextSample;
      para->samplesPerSecond = shared->samplesPerSecond;
      para->samplesPerPacket = 1024 / shared->audioPara.bytesPerSample;
      para->ABsamples = AB_BUF_SIZE / shared->audioPara.bytesPerSample;
      para->spslimit = 32000;

      startTime = get_usec ();
      // CORBA call.

      result =this->audio_control_->play (para.in (),
                                          ats,
                                          env);

      if (result == 0)
        return -1;
      TAO_CHECK_ENV_RETURN (env,-1);
    }
  if (videoSocket >= 0 && shared->nextFrame < shared->totalFrames)
    {
      Video_Control::PLAYpara_var para (new Video_Control::PLAYpara);

      if (cmdstarted == 0)
        {
          NewCmd (CmdPLAY);
          if (!auto_exp) set_speed ();
          cmdstarted = 1;
        }
      shared->VBheadFrame = -1;
      shared->needHeader = 0;
      {
        int i = shared->config.maxSPframes;
        i = (int) ( (double)i * (1000000.0 / (double)shared->usecPerFrame) /
                    shared->pictureRate);
        shared->sendPatternGops = max (min (i, PATTERN_SIZE) / shared->patternSize, 1);
      }
      cmdstarted = 1;
#ifdef STAT
      shared->collectStat = (shared->config.collectStat && (!shared->live));
      if (shared->collectStat)
        {
          int i;
          memset (& (shared->stat), 0, sizeof (shared->stat));
          shared->stat.VDlastFrameDecoded = (unsigned)-1;
          for (i = 0; i < MAX_FRAMES; i++)
            shared->stat.VBfillLevel[i] = SHRT_MIN;
          speedPtr = 0;
        }
#endif
      shared->VStimeAdvance =
        max (shared->config.VStimeAdvance, DEFAULT_VStimeAdvance) * 1000;
      if (shared->VStimeAdvance < shared->usecPerFrame)
        shared->VStimeAdvance = shared->usecPerFrame;

      para->VStimeAdvance = shared->VStimeAdvance;
      para->sn = shared->cmdsn;
      para->nextFrame = shared->nextFrame;
      para->usecPerFrame = shared->usecPerFrame;
      para->framesPerSecond = shared->framesPerSecond;
      para->collectStat = shared->collectStat;
      frate = shared->config.frameRateLimit;
      if (frate <= 0.0) {
        frate = 1.0;
      }
      shared->frameRateLimit = frate;
      para->frameRateLimit1000 =
         (long) (shared->frameRateLimit * 1000.0);
      compute_sendPattern ();
      para->sendPatternGops = shared->sendPatternGops;
      //      memcpy (para->sendPattern, shared->sendPattern, PATTERN_SIZE);

      // Sequence of chars

      para->sendPattern.length (PATTERN_SIZE);

      for (int i=0; i<PATTERN_SIZE ; i++)
        para->sendPattern [i] = shared->sendPattern [i];

      startTime = get_usec ();
      // CORBA call
      result =this->video_control_->play (para.in (),
                                          vts,
                                          env);
      if (result == 0)
        return -1;
      TAO_CHECK_ENV_RETURN (env,-1);
      ACE_DEBUG ((LM_DEBUG,"(%P|%t)Reached line %d in %s",__LINE__,__FILE__));
      if (shared->config.qosEffective) {
        /*
          fprintf (stderr, "CTR start FeedBack with init frameRateLimit %lf\n",
          frate);
        */
        maxfr = frate;  /* max frame rate all the time during one playback */
        minupf = (int) (1000000.0 / maxfr); /* min usec-per-frame all the time
                                               during one playback */
        maxrate = (double)minupf / (double)max (shared->usecPerFrame, minupf);
        /* this is current max frame rate in percentage against 'maxfr',
           current max frame rate is the lower of 'maxfr' and play speed */
        frate = 1.0; /* current sending frame rate, in percentage against 'maxfr'
                        This value is set with init value as 1.0, so that if current
                        speed is lower than frameRateLimit, no frames are dropped,
                        then when play speed increases frame rate will increase
                        accordingly until frames are dropped*/
        adjstep = ( (double)minupf / (double)shared->usecPerFrame) /
          (double)max (shared->patternSize * shared->sendPatternGops, 5);
        /* adjust step for current play speed, in percentage against
           'maxfr' */

        fbstate = 1;
        fb_startup = 1;

    
          fprintf (stderr, "CTR init frate: %f minupf %d, shared->upf %d\n",
          frate, minupf, shared->usecPerFrame);
    
      }
    }

  if (shared->live && (videoSocket >= 0) && (audioSocket >= 0)) {
    int gap = get_duration (ats, vts);
    if (gap < 0 || gap >= 5000000) {
      Fprintf (stderr, "Error for live source: ats %u, vts %u, gap %d\n",
               ats, vts, gap);
    }
    else {
      int skipped = gap * shared->audioPara.samplesPerSecond / 1000000;
      skipped += shared->config.audioOffset;
      abuffer->ABskipSamples (skipped);
      Fprintf (stderr, "Live source: skipped %d audio samples\n", skipped);
    }
  }
  else if (stuffsamples) {
    abuffer->ABskipSamples (-stuffsamples);
  }
  if (cmdstarted)
    start_timer ();
  tmp = CmdDONE;
  CmdWrite (&tmp, 1);
  return 0;
}

int
Command_Handler::position_action (int operation_tag)
{
  int val;
  unsigned char tmp = CmdDONE;
  OurCmdRead ((char*)&val, 4);

  this->operation_tag_ = operation_tag;
  this->position_val_ = val;

  //  ACE_DEBUG ((LM_DEBUG,"(%P|%t) position_action called\n"));
  if (shared->live) {
    beep();
  }
  else {
    shared->locationPosition = val;
    this->stop_playing ();
    NewCmd(CmdPOSITION);
    if (videoSocket >= 0)
    {
      int gop = shared->nextGroup;
      Video_Control::POSITIONpara_var para (new Video_Control::POSITIONpara);
      shared->nextGroup = ((shared->totalGroups-1) * val) / POSITION_RANGE;
      /*
      fprintf(stderr, "CTR: POSITION%s %d (nextGop %d). . .\n",
              operation_tag ? "_released" : "", val, shared->nextGroup);
      */
      if (gop != shared->nextGroup || operation_tag)
      {
        shared->nextFrame = ((shared->totalFrames-1) * val) / POSITION_RANGE;
        para->sn = shared->cmdsn;
        para->nextGroup = shared->nextGroup;
        tmp = CmdPOSITION;
        TAO_TRY
          {
            CORBA::Boolean result;
            result = this->video_control_->position (para.in (),
                                                     TAO_TRY_ENV);
            TAO_CHECK_ENV;
            if (result == (0))
              return -1;
            //            ACE_DEBUG ((LM_DEBUG,"(%P|%t) position done \n"));
          }
        TAO_CATCHANY
          {
            TAO_TRY_ENV.print_exception ("video_control_->position (..)");
            return -1;
          }
        TAO_ENDTRY;
        if (operation_tag)  /* release or LOOPrewind */
          this->wait_for_display (POSITION);
      }
//       if (operation_tag && audioSocket >= 0) /* needs to adjust audio position */
//       {

//         shared->nextSample = (int)((double)shared->audioPara.samplesPerSecond *
//                              ((double)shared->nextFrame / shared->pictureRate));
//         //        ACE_DEBUG ((LM_DEBUG,"shared->nextsample = %d\n",shared->nextSample));
//       }
    }
//     else if (audioSocket >= 0)
//       shared->nextSample  = (shared->totalSamples-1) * val / POSITION_RANGE;
  }
//   tmp = CmdDONE;
//   CmdWrite(&tmp, 1);
  return 0;
}

int
Command_Handler::position (void)

{
  return this->position_action (0);
}

int
Command_Handler::position_release (void)

{
  return this->position_action (1);
}

int
Command_Handler::volume (void)

{
  OurCmdRead((char *)&shared->volumePosition, 4);
  if (audioSocket >= 0) {
    SetAudioGain();
  }
  /*
  unsigned char tmp = CmdDONE;
  tmp = CmdDONE;
  CmdWrite(&tmp, 1);
  */
  return 0;
}

int
Command_Handler::balance (void)

{
  OurCmdRead((char *)&shared->balancePosition, 4);
  /*
  unsigned char tmp = CmdDONE;
  tmp = CmdDONE;
  CmdWrite(&tmp, 1);
  */
  return 0;
}

int
Command_Handler::speed (void)

{
  ACE_DEBUG ((LM_DEBUG,"COmmand_Handler::speed ()\n"));
  unsigned char tmp;
  CORBA::Boolean result;
  OurCmdRead((char *)&shared->speedPosition, 4);
  set_speed();
  TAO_TRY
    {
      if (!shared->live && shared->cmd == CmdPLAY)
        {
          if (videoSocket >= 0)
            {
              Video_Control::SPEEDpara_var para (new Video_Control::SPEEDpara);
              para->sn = shared->cmdsn;
              para->usecPerFrame = shared->usecPerFrame;
              para->framesPerSecond = shared->framesPerSecond;
              para->frameRateLimit1000 =
                (long)(shared->frameRateLimit * 1000.0);
              {
                int i = shared->config.maxSPframes;
                i = (int) ((double)i * (1000000.0 / (double)shared->usecPerFrame) /
                           shared->pictureRate);
                shared->sendPatternGops = max(min(i, PATTERN_SIZE) / shared->patternSize, 1);
              }
              compute_sendPattern();
              para->sendPatternGops = shared->sendPatternGops;
              para->sendPattern.length (PATTERN_SIZE);
              for (int i=0; i< PATTERN_SIZE ; i++)
                para->sendPattern[i]=shared->sendPattern[i];
              // CORBA call
              result = this->video_control_->speed (para.in (),
                                                    TAO_TRY_ENV);
              TAO_CHECK_ENV;
              if (result == (0))
                return -1;
              if (fbstate) {
                maxrate = (double)minupf / (double)max(shared->usecPerFrame, minupf);
                adjstep = ((double)minupf / (double)shared->usecPerFrame) /
                  (double)max(shared->patternSize * shared->sendPatternGops, 5);
                fbstate = 1;
              }
            }
          if (audioSocket >= 0)
            {
              Audio_Control::SPEEDPara_var para (new Audio_Control::SPEEDPara);
              para->sn = shared->cmdsn;
              para->samplesPerSecond = shared->samplesPerSecond;
              para->samplesPerPacket = 1024 / shared->audioPara.bytesPerSample;
              para->spslimit = 32000;
              // CORBA call
              result =
                this->audio_control_->speed (para.in (),
                                             TAO_TRY_ENV);
              TAO_CHECK_ENV;
              if (result == (0))
                return -1;
            }
        }
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("audio_control->speed ()");
      return -1;
    }
  TAO_ENDTRY;

  return 0;
}


int
Command_Handler::stop (void)

{
#ifdef STAT
  unsigned char preCmd = shared->cmd;
#endif
  /*
    fprintf(stderr, "CTR: STOP . . .\n");
  */
  this->stop_playing ();

  if (shared->live && videoSocket >= 0) {
    Fprintf(stderr, "CTR live video stat: average disp frame rate: %5.2f fps\n",
	    shared->pictureRate * displayedFrames / shared->nextFrame);
  }
  unsigned char tmp = CmdDONE;
  //  ACE_DEBUG ((LM_DEBUG,"(%P|%t) command_handler::Stop :nextFrame=%d\n",shared->nextFrame));
  CmdWrite(&tmp, 1);

  return 0;
}

int
Command_Handler::stop_playing (void)
{
  unsigned char precmd = shared->cmd;

  TAO_TRY
    {
      if (precmd == CmdFF || precmd == CmdFB || precmd == CmdPLAY)
        {
          //          ACE_DEBUG ((LM_DEBUG, "(%P|%t) Reached line %d in %s\n", __LINE__, __FILE__));
          unsigned char tmp = CmdSTOP;
          NewCmd(CmdSTOP);

          /* notify AS and/or VS */
          if ((CORBA::is_nil (this->audio_control_) == 0)
              && precmd == CmdPLAY
              && rtplay)
            {
              //  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Reached line %d in %s\n", __LINE__, __FILE__));
              // CORBA call
              CORBA::Boolean result =
                this->audio_control_->stop (shared->cmdsn,
                                            TAO_TRY_ENV);
              //              cerr << "audio_control_->stop result is " << result << endl;
              if (result == (0))
                  return -1;
              TAO_CHECK_ENV;
            }
          if (CORBA::is_nil (this->video_control_) == 0)
            {
              //              ACE_DEBUG ((LM_DEBUG, "(%P|%t) Reached line %d in %s\n", __LINE__, __FILE__));
              // CORBA call
              CORBA::Boolean result =
                this->video_control_->stop (shared->cmdsn,
                                            TAO_TRY_ENV);
              if (result == (0))
                return -1;
              TAO_CHECK_ENV;
            }
          //          ACE_DEBUG ((LM_DEBUG, "(%P|%t) Reached line %d in %s\n", __LINE__, __FILE__));

          /* stop timer and sleep for a while */
          //          cerr << "stopping timer" << endl;
          this->client_sig_handler_.stop_timer ();
          usleep(100000);

          /* purge VDbuf and audio channel from AS*/
          if (videoSocket >= 0)
            {
              while (VDpeekMsg() != NULL)
                VDreclaimMsg(VDgetMsg());
              /*
                Fprintf(stderr, "CTR: VDbuf purged.\n");
              */
              fbstate = 0;

            }

          /* adjust some info */
          if (precmd == CmdPLAY && videoSocket >= 0)
            shared->nextFrame = shared->currentFrame+1;
          else
            shared->nextGroup = shared->currentGroup + 1;
        }
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Command_Handler.stop(..)");
      return -1;
    }
  TAO_ENDTRY;
  return 0;
}

// connects and handshakes with the server
int
Command_Handler::connect_to_video_server (void)
{
  // set the pointers to the correct values
  //  *max_pkt_size = -INET_SOCKET_BUFFER_SIZE;
  // initialize the command handler , ORB
  TAO_TRY
    {
  if (this->resolve_video_reference () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) command_handler: resolve_video_reference returned -1"),
                       -1);
  AVStreams::streamQoS_var the_qos (new AVStreams::streamQoS);
  AVStreams::flowSpec_var the_flows (new AVStreams::flowSpec);
  // Bind the client and server mmdevices.

  ACE_NEW_RETURN (this->video_reactive_strategy_,
                  Video_Endpoint_Reactive_Strategy_A (&this->orb_manager_,
                                                      this),
                  -1);
  ACE_NEW_RETURN (this->video_client_mmdevice_,
                  TAO_MMDevice (this->video_reactive_strategy_),
                  -1);
  
  this->orb_manager_.activate_under_child_poa ("Video_Client_MMDevice",
                                               this->video_client_mmdevice_,
                                               TAO_TRY_ENV);
  TAO_CHECK_ENV;

  ACE_NEW_RETURN (this->video_streamctrl_,
                  TAO_StreamCtrl,
                  -1);
  this->video_streamctrl_->bind_devs
    (this->video_client_mmdevice_->_this (TAO_TRY_ENV),
         this->video_server_mmdevice_.in (),
         the_qos.inout (),
         the_flows.in (),
         TAO_TRY_ENV);

      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("video_streamctrl.bind_devs:");
      return -1;
    }
  TAO_ENDTRY;

  return 0;

}

// connects and handshakes with the server
int
Command_Handler::connect_to_audio_server (void)
{

  // // set the pointers to the correct values
  //  *max_pkt_size = -INET_SOCKET_BUFFER_SIZE;

  TAO_TRY
    {

  if (this->resolve_audio_reference () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) command_handler: resolve_audio_reference returned -1"),
                       -1);
  AVStreams::streamQoS_var the_qos (new AVStreams::streamQoS);
  AVStreams::flowSpec_var the_flows (new AVStreams::flowSpec);

  ACE_NEW_RETURN (this->audio_reactive_strategy_,
                  Audio_Endpoint_Reactive_Strategy_A (&this->orb_manager_,
                                                      this),
                  -1);

  ACE_NEW_RETURN (this->audio_client_mmdevice_,
                  TAO_MMDevice (this->audio_reactive_strategy_),
                  -1);

  // activate the client audio mmdevice under the child poa.
  this->orb_manager_.activate_under_child_poa ("Audio_Client_MMDevice",
                                               this->audio_client_mmdevice_,
                                               TAO_TRY_ENV);
  TAO_CHECK_ENV;

  ACE_NEW_RETURN (this->audio_streamctrl_,
                  TAO_StreamCtrl,
                  -1);

  // Bind the client and server mmdevices.
      this->audio_streamctrl_->bind_devs
        (this->audio_client_mmdevice_->_this (TAO_TRY_ENV),
         this->audio_server_mmdevice_.in (),
         the_qos.inout (),
         the_flows.in (),
         TAO_TRY_ENV);

      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("audio_streamctrl.bind_devs:");
      return -1;
    }
  TAO_ENDTRY;

  return 0;
}

// ========================================
// Decode_Notification_Handler methods.

Decode_Notification_Handler::Decode_Notification_Handler (Command_Handler *command_handler)
  :command_handler_ (command_handler)
{
}

ACE_HANDLE
Decode_Notification_Handler::get_handle (void) const
{
  return vdsp[0];
}

int
Decode_Notification_Handler::handle_input (ACE_HANDLE fd) 
{
  ACE_DEBUG ((LM_DEBUG,"Decode_Notification_Handler::handle_input \n"));
  char message[BUFSIZ];
  int result =
    ACE_OS::read (fd,&message,BUFSIZ);
  if (result == -1)
    ACE_ERROR_RETURN ((LM_ERROR,"Decode_Notification_Handler::handle_input read failed\n"),0);
  if (result == 0)
    ACE_ERROR_RETURN ((LM_ERROR,"Decode_Notification_Handler::handle_input socket closed\n"),0);

  if (message[0] != DECODED)
    ACE_ERROR_RETURN ((LM_ERROR,"Decode_Notification_Handler:: Not a decoded message\n"),0);

  // Resume the command handler.
  if (this->command_handler_->command_suspended_ != UNSUSPENDED)
    {
      unsigned char tmp;
      switch (this->command_handler_->command_suspended_)
        {
        case INIT:
          tmp = CmdDONE;
          CmdWrite(&tmp, 1);
          if (videoSocket < 0)
            {
              tmp = CmdVPclearScreen;
              CmdWrite(&tmp, 1);
            }
          break;
        case STEP:
          tmp = CmdDONE;
          CmdWrite(&tmp, 1);
          break;
        case POSITION:
          if (this->command_handler_->operation_tag_ && audioSocket >= 0) /* needs to adjust audio position */
            {

              shared->nextSample = (int)((double)shared->audioPara.samplesPerSecond *
                                         ((double)shared->nextFrame / shared->pictureRate));
              //        ACE_DEBUG ((LM_DEBUG,"shared->nextsample = %d\n",shared->nextSample));
            }
          if (audioSocket >= 0)
            shared->nextSample  = (shared->totalSamples-1) * (this->command_handler_->position_val_) / POSITION_RANGE;
          tmp = CmdDONE;
          CmdWrite(&tmp, 1);
          break;
        default:
          ACE_DEBUG ((LM_DEBUG,"INVALID STATE for command suspension\n"));
          break;
        }
      TAO_ORB_Core_instance ()->reactor ()->resume_handler (this->command_handler_->get_handle ());
    }
  ::usr1_handler (0);
  return 0;
}


// ----------------------------------------------------------------------
// Client_Sig_Handler methods

void
Client_Sig_Handler::PlayAudioOnly(void)
{
  int maxSize;
  int size, csize;  /* all in samples */
  unsigned int AFtime;

  if (audioFirst)
  {
    audioFirst = 0;
    nextAFtime = GetAudioTime() + audioForward;
  }
  else
    if ((int)(nextAFtime - GetAudioTime()) >= bufferedSamples)
      return;

  if (timer_on >4)
  {
    // ~~we may need to uncomment this ??
    this->stop_timer ();

    // stop both the audio and video servers
    this->command_handler_->stop ();
    /* tries to rewind and play again */
    if (shared->loopBack)
    {
      /*
      fprintf(stderr, "CTR: trying loopBack().\n");
      */
      loopBack();
    }

    return;
  }
  if (nextASSample >= shared->totalSamples)
  {
    timer_on ++;
    return;
  }
  if (shared->samplesPerSecond >= shared->audioPara.samplesPerSecond)
    maxSize = (AudioBufSize/shared->audioPara.bytesPerSample);
  else
    maxSize = ((AudioBufSize/shared->audioPara.bytesPerSample) *
               shared->samplesPerSecond) /
              shared->audioPara.samplesPerSecond;
  for (;;)
  {
    size = abuffer->ABgetSamples(rawBuf, maxSize);
    csize = AudioConvert(size);
    AFtime = PlayAudioSamples(nextAFtime, convBuf, csize);
    nextASSample += size;
    shared->nextSample += size;
    nextAFtime += csize;
    if ((int)(nextAFtime - AFtime) < 0)
      nextAFtime = AFtime;
    if (nextASSample >= shared->totalSamples)
    {
      timer_on ++;
      break;
    }
    /*
    Fprintf(stderr, "CTR: nextAFtime:%d, AFtime:%d, bufferedSamples:%d\n",
            nextAFtime, AFtime, bufferedSamples);
            */
    if ((int)(nextAFtime - AFtime) >= bufferedSamples)
      break;
  }
  {
    unsigned char tmp = CmdVPaudioPosition;
    CmdWrite(&tmp, 1);
  }
}

// handles the timeout SIGALRM signal
Client_Sig_Handler::Client_Sig_Handler (Command_Handler *command_handler)
  : command_handler_ (command_handler)
{
}

Client_Sig_Handler::~Client_Sig_Handler (void)
{
  TAO_ORB_Core_instance ()->reactor ()->remove_handler (this,
                                            ACE_Event_Handler::NULL_MASK);

  TAO_ORB_Core_instance ()->reactor ()->remove_handler (this->sig_set);
}

int
Client_Sig_Handler::register_handler (void)
{
  // Assign the Sig_Handler a dummy I/O descriptor.  Note that even
  // though we open this file "Write Only" we still need to use the
  // ACE_Event_Handler::NULL_MASK when registering this with the
  // ACE_Reactor (see below).
  this->handle_ = ACE_OS::open (ACE_DEV_NULL, O_WRONLY);
  ACE_ASSERT (this->handle_ != -1);

  // Register signal handler object.  Note that NULL_MASK is used to
  // keep the ACE_Reactor from calling us back on the "/dev/null"
  // descriptor.
  if (TAO_ORB_Core_instance ()->reactor ()->register_handler
      (this, ACE_Event_Handler::NULL_MASK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "register_handler"),
                      -1);

  // Create a sigset_t corresponding to the signals we want to catch.

  this->sig_set.sig_add (SIGINT);
  this->sig_set.sig_add (SIGQUIT);
  this->sig_set.sig_add (SIGALRM);
  this->sig_set.sig_add (SIGUSR2);
  this->sig_set.sig_add (SIGSEGV);

  // Register the signal handler object to catch the signals.
  if (TAO_ORB_Core_instance ()->reactor ()->register_handler (sig_set,
                                                  this) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "register_handler"),
                      -1);
  return 0;
}

int
Client_Sig_Handler::remove_handler (void)
{
  if (TAO_ORB_Core_instance ()->reactor ()->remove_handler (this,
                                                            ACE_Event_Handler::NULL_MASK) == -1)
    ACE_DEBUG ((LM_DEBUG,"(%P)remove_handler failed for client_sig handler"));

  if (TAO_ORB_Core_instance ()->reactor ()->remove_handler (sig_set) == -1)
    ACE_DEBUG ((LM_DEBUG,"(%P)remove_handler failed for client_sig handler"));
  return 0;
}

// Called by the ACE_Reactor to extract the fd.
ACE_HANDLE
Client_Sig_Handler::get_handle (void) const
{
  return this->handle_;
}

int
Client_Sig_Handler::handle_input (ACE_HANDLE)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) handling asynchonrous input...\n"));
  return 0;
}

int
Client_Sig_Handler::shutdown (ACE_HANDLE, ACE_Reactor_Mask)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) closing down Sig_Handler...\n"));
  return 0;
}

// This method handles all the signals that are being caught by this
// object.  In our simple example, we are simply catching SIGALRM,
// SIGINT, and SIGQUIT.  Anything else is logged and ignored.
//
// There are several advantages to using this approach.  First,
// the behavior triggered by the signal is handled in the main event
// loop, rather than in the signal handler.  Second, the ACE_Reactor's
// signal handling mechanism eliminates the need to use global signal
// handler functions and data.

int
Client_Sig_Handler::handle_signal (int signum, siginfo_t *, ucontext_t *)
{
  int status;
  pid_t pid;
  //ACE_DEBUG ((LM_DEBUG, "(%P|%t) received signal %S\n", signum));

  switch (signum)
    {
    case SIGSEGV:
      ::remove_all_semaphores ();
      exit (0);
    case SIGALRM:
      // Handle the timeout
      this->TimerHandler (signum);
      // %% ??!!!
      break;
    case SIGCHLD:
      //      ACE_DEBUG ((LM_DEBUG, "(%P|%t) received signal %S\n", signum));
      pid = ACE_OS::wait (&status);
      return 0;
    case SIGINT:
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) received signal %S, removing signal handlers from the reactor\n", signum));
      this->command_handler_->close ();
      TAO_ORB_Core_instance ()->reactor ()->remove_handler (this->sig_set);
      TAO_ORB_Core_instance ()->orb ()->shutdown ();
      return 0;
    default:
      ACE_DEBUG ((LM_DEBUG,
                  "(%t) %S: not handled, returning to program\n",
                  signum));
      return 0;
    }
  this->TimerProcessing ();
  return 0;
}

void
Client_Sig_Handler::TimerHandler(int sig)
{
  int currentUPF = shared->currentUPF;
  /*
  Fprintf(stderr, "CTR in TimerHandler.\n");
  */
  if (videoSocket >= 0 && shared->cmd == CmdPLAY && currentUPF != timerUPF)
  {
    struct itimerval val;
    {
      val.it_interval.tv_sec =  val.it_value.tv_sec = currentUPF / 1000000;
      val.it_interval.tv_usec = val.it_value.tv_usec = currentUPF % 1000000;
      setitimer(ITIMER_REAL, &val, NULL);
      /*
      fprintf(stderr, "CTR: timer speed changed to %d upf.\n", shared->currentUPF);
      */
    }
    timerUPF = currentUPF;
#ifdef STAT
    if (shared->collectStat && speedPtr < SPEEDHIST_SIZE)
    {
      speedHistory[speedPtr].frameId = shared->nextFrame;
      speedHistory[speedPtr].usecPerFrame = timerUPF;
      speedHistory[speedPtr].frameRateLimit = shared->frameRateLimit;
      speedHistory[speedPtr].frames = shared->sendPatternGops * shared->patternSize;
      speedHistory[speedPtr].framesDropped = shared->framesDropped;
    }
    speedPtr ++;
#endif
  }
  /*
  fprintf(stderr, "+\n");
  */
  if (!timerCount) {
    int addedVSwat;
    if (shared->cmd == CmdPLAY) {
      addedVSwat = shared->usecPerFrame * (shared->VBheadFrame - shared->nextFrame);
    }
    else if (shared->cmd == CmdFF) {
      addedVSwat = shared->usecPerFrame * (shared->VBheadFrame - shared->nextGroup);
    }
    else { /* shared->cmd == CmdFB */
      addedVSwat = shared->usecPerFrame * (shared->nextGroup - shared->VBheadFrame);
    }
    shared->VStimeAdvance += addedVSwat;
  }
  timerCount += timer_signals_skip + 1;
  timer_signals_skip = 0;

  if (shared->live ) {
    if (audioSocket <= 0) {  /* video only */
      unsigned t = shared->VBheadFrame - startVSA;
      if (timerCount < t) {
        /*
        Fprintf(stderr, "CTR: (av) timerCount %d, t %d\n", timerCount, t);
        */
        timerCount = t;
      }
    }
  }
  double frames_per_group =
    (double)shared->totalFrames/shared->totalGroups;
  long nextframe = shared->nextGroup * frames_per_group;

  if (audioSocket > 0)
    {
      //      ACE_DEBUG ((LM_DEBUG,"(%P|%t) Adjusting audio nextsample value\n"));
      shared->nextSample = (int)((double)shared->audioPara.samplesPerSecond *
                                 ((double)nextframe / shared->pictureRate));

    /* audio involved,  TimerProcessing() will adjust the rate automatically */
    }
  if ((shared->cmd == CmdFF) || (shared->cmd == CmdFB))
    shared->nextFrame = shared->nextGroup * frames_per_group;
}

void
Client_Sig_Handler::stop_timer(void)
{
  struct itimerval val;

  if (!timer_on)
    return;

  timer_on = 0;

  //  setsignal(SIGALRM, SIG_IGN);

  val.it_interval.tv_sec =  val.it_value.tv_sec = 0;
  val.it_interval.tv_usec = val.it_value.tv_usec = 0;
  setitimer(ITIMER_REAL, &val, NULL);

  fprintf(stderr, "CTR: timer stopped.\n");

  //  this->command_handler_->stop_timer ();
  /*
  usleep(200000);
  */
}

void
Client_Sig_Handler::DisplayPicture(void)
{
  //  ACE_DEBUG ((LM_DEBUG,"inside DisplayPicture\n"));
  int toDisplay = 1;
  int count = timerCount;

  if ((shared->cmd != CmdPLAY &&
       shared->cmd != CmdFF &&
       shared->cmd != CmdFB) ||
      lastCount == count)
    ACE_ERROR ((LM_ERROR,"Returning from first if in DisplayPicture\n"));
  if (timer_on >4)   /* 4 is a magic number */
    {
      this->stop_timer();

      // stop both the audio and video.
      this->command_handler_->stop ();

      /* tries to rewind and play again */
      if (shared->cmd == CmdPLAY && shared->loopBack)
        loopBack();

      ACE_ERROR ((LM_ERROR,"Returning because timer_on > 4\n"));
    }
  /*
    Fprintf(stderr, "CTR in diplayPicture().\n");
  */
  {
    int i, j;
    FrameBlock *buf, *next;
#if 0
    if (shared->cmd == CmdPLAY && rtplay &&
        (i = shared->nextFrame - shared->firstGopFrames) > 0) {
      j = (i / shared->patternSize) % shared->sendPatternGops;
      i %= shared->patternSize;
      if (shared->pattern[i] != 'B') {
        while (i > 0) {
          if (shared->pattern[--i] != 'B') break;
        }
      }
      toDisplay = (shared->sendPattern + j * shared->patternSize)[i];
    }
    else if (shared->cmd != CmdPLAY) {
      if (last_disp_fid != shared->nextGroup) toDisplay = 1;
      else toDisplay = 0;
    }
    if (toDisplay)
#endif
    {
      if (shared->cmd == CmdPLAY && !rtplay) {  /* if play with best effort */
        while (VDcheckMsg() <= 0)  /* keep sleeping for 10 millisec until a decoded
                                      frame show up in VD buffer */
          usleep(10000);
      }
#ifdef STAT
      if (shared->collectStat)
      {
        shared->stat.VBfillLevel[shared->nextFrame] =
            shared->VBheadFrame - shared->nextFrame;
        i = VDcheckMsg();
        if (i < 0) i = 0;
        else if (i >= MAX_VDQUEUE_SIZE) i = MAX_VDQUEUE_SIZE - 1;
        shared->stat.VDqueue[i] ++;
      }
#endif
      toDisplay = 0;
      for (;;)
      {
        buf = VDpeekMsg();

        if (buf == NULL) {
          //          ACE_DEBUG ((LM_DEBUG,"Null buffer  "));
          goto loop_end;
        }

        switch (shared->cmd)
        {
          //          ACE_DEBUG ((LM_DEBUG,"nextFrame = %d,nextGroup\n",shared->nextFrame,shared->nextGroup));
          int position;
        case CmdPLAY:
          position = shared->nextFrame;
          //          ACE_DEBUG ((LM_DEBUG,"nextFrame = %d,nextGroup\n",shared->nextFrame,shared->nextGroup));
          /*
          Fprintf(stderr, "CTR PLAY: buf->display %d, position %d\n",
                  buf->display, position);
          */
          if (buf->display == position)
          {  /* display it */
            buf = VDgetMsg();
            shared->nextGroup = buf->gop + 1;
#ifdef STAT
            shared->stat.CTRdispOnTime ++;
#endif
            goto display_picture;
          }
          else if (buf->display > position)
          {  /* too early, wait for future display */
            goto loop_end;
          }
          else  /* this picture too late */
          {
            // this line gets the buffer or dequeues from the shared memory queue
            buf = VDgetMsg();
            if (((next = VDpeekMsg()) == NULL || next->display > position) &&
                buf->display > last_disp_fid) {
#ifdef STAT
              shared->stat.CTRdispLate ++;
#endif
              /* buf is the last one, or next too to early, display it anyway */
              goto display_picture;
            }
            else   /* next not too early, discard buf */
            {
#ifdef STAT
              if (buf->display < last_disp_fid)
                shared->stat.CTRdropOutOrder ++;
              else
                shared->stat.CTRdropLate ++;
#endif
              /*
              Fprintf(stderr, "CTR drops frame display=%d, shared->nextFrame=%d\n",
                      buf->display, shared->nextFrame);
              */
              VDreclaimMsg(buf);
              continue;
            }
          }
          break;
        case CmdFF:
          position = shared->nextGroup;
          if (buf->gop == position)
          {  /* display it */
            buf = VDgetMsg();
            shared->nextFrame = buf->display;
            //            ACE_DEBUG ((LM_DEBUG,"nextFrame = %d\n",shared->nextFrame));
            goto display_picture;
          }
          else if (buf->gop > position)
          {  /* hold it for future display */
            goto loop_end;
          }
          else   /* discard late picture */
          {
            buf = VDgetMsg();
            if ((next = VDpeekMsg()) == NULL || next->gop > position)
              /* buf is the last one, or next too to early, display it anyway */
              goto display_picture;
            else   /* next not too early, discard buf */
            {
              VDreclaimMsg(buf);
              continue;
            }
          }
          break;
        case CmdFB:
          position = shared->nextGroup;
          if (buf->gop == position)
          {  /* display it */
            buf = VDgetMsg();
            shared->nextFrame = buf->display;
            //            ACE_DEBUG ((LM_DEBUG,"nextFrame = %d\n",shared->nextFrame));
            goto display_picture;
          }
          else if (buf->gop < position)
          {  /* hold it for future display */
            goto loop_end;
          }
          else   /* discard late picture */
          {
            buf = VDgetMsg();
            if ((next = VDpeekMsg()) == NULL || next->gop < position)
              /* buf is the last one, or next too to early, display it anyway */
              goto display_picture;
            else   /* next not too early, discard buf */
            {
              VDreclaimMsg(buf);
              continue;
            }
          }
          break;
        default:
          goto loop_end;
        }
      }
    display_picture:
      toDisplay = 1;
      if (shared->cmd == CmdPLAY) last_disp_fid = buf->display;
      else last_disp_fid = buf->gop;
#ifdef STAT
      if (shared->live) displayedFrames ++;
      if (shared->collectStat)
        shared->stat.VPframesDisplayed[buf->display >> 3] |= 1 << (buf->display % 8);
#endif
      {
        unsigned char tmp = CmdVPdisplayFrame;
        CmdWrite(&tmp, 1);
      }
      CmdWrite((unsigned char *)&buf, sizeof(char *));
    loop_end:;
    }
  }

  if (shared->cmd == CmdPLAY)
  {
    shared->nextFrame += rtplay ? count - lastCount : 1;
    if (shared->nextFrame >= shared->totalFrames)
    {
      timer_on ++;
      shared->nextFrame = shared->totalFrames;
    }

    /* following is frameRate feedback algorithm */
    if (fbstate && toDisplay && rtplay) {
      static Filter *fr = NULL;  /* frame-rate filter */
      static int start;  /* feedback action time, in microseconds */
      static int delay;  /* time to delay after each action, and to charge
                          the filter after action-delay */
      static int pretime; /* time of previous frame, in microseconds */
      static int throw_outlier = 0;
      /* tag to throw away outlier. In case there are outliers, this
         tag is flipped by the algorithm, so that If there are two
         consecutive sample deviating very much from the filtered mean
         value, the second is not considered outlier. This may mean
         that the frame rate has dropped significantly. */
      static double vr; /* filtered frame-rate value, in microseconds/frame */
      double r, nr;
      int t = get_usec();

      switch (fbstate) {
      case 3:  /* working, monitoring */
        {
          int interval = get_duration(pretime, t);
          if (throw_outlier) {
            if (interval >> 2 >= vr) {
              /* at least four times the filtered mean value to be
                 considered outlier */
              /* In case an outlier is detected and thrown away, then
                 the following sample will never be classified as an
                 outlier, and the current time is recorded */
              throw_outlier = 0;
              pretime = t;
              Fprintf(stderr, "CTR detected a gap %d (vr = %d) us\n",
                      interval, (int)vr);
              break;
            }
          }
          else {
            throw_outlier = 1;
          }
          vr = DoFilter(fr, (double)interval);
        }
        pretime = t;

        r = minupf / vr; /* convert the display fps to percentage of maxfr */

        nr = 0; /* This variable contains the newly computed server frame rate */

        /* let nr oscillate around 1.5 ~ 2.5 */
        if (min(frate, maxrate) - r >= 3.0 * adjstep) {
          /* pipeline is considered overloaded if server fps is more than 3 adjsteps
           higher than display fps */
          nr = min(frate, maxrate) - adjstep; /* slow down server frame rate
                                                 one step */
          if (fb_startup) {  /* startup feedback action: jump set the server fps
                                to a value close to actually measure display
                                frame rate */
            fb_startup = 0;
            while (nr >= r + 2.5 * adjstep) {
              nr -= adjstep;
            }
          }
        }
        else if (frate - r <= 0.5 * adjstep && frate < maxrate) {
          /* pipeline load is considered too light if the server frame rate
             is less than 0.5fps higher than display frame rate, while the server
             fps is no maximum yet. The server fps then is stepped up. */
          nr = min(frate + adjstep, maxrate);
        }
        if (nr > 0) { /* nr = 0 if not feedback action needs to be taken */
          shared->frameRateLimit = maxfr * nr;
          compute_sendPattern();
          /*
          fprintf(stderr,
            "CTR adjust frameRate to %lf, vr=%lf minupf=%d, r=%lf, frate=%lf, nr=%lf\n",
                  shared->frameRateLimit, vr, minupf, r, frate, nr);
           */
          frate = nr;  /* remember new server frame rate */
          shared->qosRecomputes ++;
          start = t;  /* remember the action time */
          /* delay for some time before restarts, to let feedback take effect */
          delay = shared->usecPerFrame * (shared->VBheadFrame - shared->nextFrame) +
                  shared->playRoundTripDelay;
          if (delay < 0) delay = shared->usecPerFrame;
          fbstate = 4;
        }
        break;
      case 4:  /* delay and reset after action*/
        if (get_duration(start, t) >= delay) {
          /*
          fprintf(stderr,
                  "CTR VB from s2 to s3, vr %lf, frate %lf, maxrate %lf, step %lf\n",
                  vr, frate, maxrate, adjstep);
           */
          fr = ResetFilter(fr, shared->config.filterPara >= 1 ?
                           shared->config.filterPara : 100);
          delay = shared->usecPerFrame *
                  max(shared->sendPatternGops * shared->patternSize,
                      shared->config.filterPara);
                  /* charge filter for time */
          start = pretime = t;
          throw_outlier = 0;
          fbstate = 2;
        }
        break;
      case 2:  /* charge the filter */
        {
          int interval = get_duration(pretime, t);
          if (throw_outlier) {
            if (interval >> 2 >= vr) {  /* at least four times the previous average */
              throw_outlier = 0;
              pretime = t;
              Fprintf(stderr, "CTR detected a gap %d (vr = %d) us\n",
                      interval, (int)vr);
              break;
            }
          }
          else {
            throw_outlier = 1;
          }
          vr = DoFilter(fr, (double)interval);
        }
        pretime = t;
        if (get_duration(start, t) >= delay) {
          /*
          fprintf(stderr,
                  "CTR VB from s2 to s3, vr %lf, frate %lf, maxrate %lf, step %lf\n",
                  vr, frate, maxrate, adjstep);
           */
          fbstate = 3;
        }
        break;
      case 1:  /* start or speed change, wait until speed data consistant */
        if (shared->currentUPF == shared->usecPerFrame) {
          if (fr == NULL) {
            fr = NewFilter(FILTER_LOWPASS, shared->config.filterPara >= 1 ?
                          shared->config.filterPara : 100);
          }
          else {
            fr = ResetFilter(fr, shared->config.filterPara >= 1 ?
                            shared->config.filterPara : 100);
          }
          if (fr == NULL) {
           ACE_OS::perror ("CTR failed to allocate space for fr filter");
            fbstate = 0;
          }
          vr = DoFilter(fr, (double)shared->usecPerFrame);
          delay = shared->usecPerFrame *
                  shared->sendPatternGops * shared->patternSize;
                  /* charge filter for some time */
          pretime = start = t;
          throw_outlier = 0;
          fbstate = 2;
        }
        break;
      default:
        fprintf(stderr, "CTR error: unknown feedback state: %d\n", fbstate);
        fbstate = 1;
        break;
      }
    }
    /* end of frame rate control algorithm */

  }
  else if (shared->cmd == CmdFF)
  {
    shared->nextGroup += count - lastCount;
    if (shared->nextGroup >= shared->totalGroups)
    {
      timer_on ++;
      shared->nextGroup = shared->totalGroups - 1;
    }
  }
  else
  {
    shared->nextGroup -= count - lastCount;
    if (shared->nextGroup < 0)
    {
      timer_on ++;
      shared->nextGroup = 0;
    }
  }
  lastCount = count;
}

void
Client_Sig_Handler::TimerProcessing (void)
{
  //  cerr << "Timerprocessing signal went off\n";
  if (audioSocket >= 0 && shared->cmd == CmdPLAY)
  {
    if (videoSocket < 0)
      this->PlayAudioOnly ();
    else if (rtplay)
    {
      //  cerr << "TimerProcessing: calling PlayAudio ()\n";
      int res = PlayAudio();
      /* and also tries to sync audio and video */
      if (res)
      {
        int jit;
        res = forward - audioForward;
        jit = (res>0 ? res : -res);
        jit = (int)(((double)jit / (double)shared->samplesPerSecond) * 1000000.0);
        if (res < -audioForward/2) /* needs to speedup the clock */
        {
          struct itimerval val;
          getitimer(ITIMER_REAL, &val);
          if ((int)val.it_value.tv_usec > jit)
            val.it_value.tv_usec -= jit;
          else
          {
            timer_signals_skip ++;
            val.it_value.tv_usec = 2;
          }
          setitimer(ITIMER_REAL, &val, NULL);
        }
        else if (res > audioForward) /* needs to slow down the clock */
        {
          struct itimerval val;
          /*
          val.it_interval.tv_sec =  val.it_value.tv_sec = 0;
          val.it_interval.tv_usec = val.it_value.tv_usec = 0;
          */
          getitimer(ITIMER_REAL, &val);
          val.it_value.tv_usec += (jit % 1000000);
          val.it_value.tv_sec += (jit / 1000000);
          setitimer(ITIMER_REAL, &val, NULL);
        }
        /*
        if (res < -audioForward || res > audioForward)
        */
        if (res < -16000 || res > 16000)
        {
          Fprintf(stderr, "Audio forward jit %d samples\n", res);
        }
      }
    }
  }
  if (videoSocket >= 0 &&
      (shared->cmd == CmdPLAY || shared->cmd == CmdFF || shared->cmd == CmdFB)) {
    //    cerr << "Timerprocessing signal-handler done\n";
    DisplayPicture();
  }

}

// -----------------------------------------------------------
// Audio_Client_StreamEndPoint methods

Audio_Client_StreamEndPoint::Audio_Client_StreamEndPoint (Command_Handler *command_handler,
                                                          char *host)
  :command_handler_ (command_handler),
   host_ (host)
{
}

int
Audio_Client_StreamEndPoint::handle_open (void)
{
  return -1;
}

int
Audio_Client_StreamEndPoint::handle_close (void)
{
  return -1;
}

// called by the framework before calling connect. Here we create our
// flow spec which is nothing but hostname::port_number of the
// datagram.
CORBA::Boolean
Audio_Client_StreamEndPoint::handle_preconnect (AVStreams::flowSpec &the_spec)
{
  ACE_DEBUG ((LM_DEBUG,"(%P|%t) handle_preconnect called\n"));
  CORBA::UShort server_port;
  ACE_INET_Addr local_addr;

  // Get the local UDP address
  if (this->dgram_.open (ACE_Addr::sap_any) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,"(%P|%t) datagram open failed %p\n"),0);

      // set the socket buffer sizes to 64k.
  int sndbufsize = ACE_DEFAULT_MAX_SOCKET_BUFSIZ;
  int rcvbufsize = ACE_DEFAULT_MAX_SOCKET_BUFSIZ;

  if (this->dgram_.set_option (SOL_SOCKET,
                               SO_SNDBUF,
                               (void *) &sndbufsize,
                               sizeof (sndbufsize)) == -1
      && errno != ENOTSUP)
    return 0;
  else if (this->dgram_.set_option (SOL_SOCKET,
                                    SO_RCVBUF,
                                    (void *) &rcvbufsize,
                                    sizeof (rcvbufsize)) == -1
           && errno != ENOTSUP)
    return 0;

  if (this->dgram_.get_local_addr (local_addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,"(%P|%t)datagram get local addr failed %p"),-1);
  // form a string
  char client_address_string [BUFSIZ];
  ::sprintf (client_address_string,
             "%s:%d",
             this->host_ == 0 ? local_addr.get_host_name ():this->host_,
             local_addr.get_port_number ());
  the_spec.length (1);
  the_spec [0] = CORBA::string_dup (client_address_string);

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) client flow spec is %s\n",
              client_address_string));
  return 1;
}

// called by the A/V framework after calling connect. Passes the
// server streamendpoints' flowspec which we use to connect our
// datagram socket.
CORBA::Boolean
Audio_Client_StreamEndPoint::handle_postconnect (AVStreams::flowSpec& server_spec)
{
  ACE_DEBUG ((LM_DEBUG,"(%P|%t) handle_postconnect called \n"));

  // Take the first string of the sequence .
  ACE_INET_Addr server_udp_addr (server_spec [0]);

  server_udp_addr.dump ();
  if (ACE_OS::connect (this->dgram_.get_handle (),(sockaddr *) server_udp_addr.get_addr (),
                       server_udp_addr.get_size ()) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,"(%P|%t) datagram connect failed %p\n"),-1);
  // Now set the data handle of the command handler.

  this->command_handler_->set_audio_data_handle (this->dgram_.get_handle ());
  return 0;
}

int
Audio_Client_StreamEndPoint::handle_start (const AVStreams::flowSpec &the_spec,
                                           CORBA::Environment &env)

{
  return -1;
}

int
Audio_Client_StreamEndPoint::handle_stop (const AVStreams::flowSpec &the_spec,
                                          CORBA::Environment &env)

{
  return -1;
}

int
Audio_Client_StreamEndPoint::handle_destroy (const AVStreams::flowSpec &the_spec,
                                             CORBA::Environment &env)

{
  return -1;
}

ACE_HANDLE
Audio_Client_StreamEndPoint::get_handle (void)
{
  return this->dgram_.get_handle ();
}

// -----------------------------------------------------------
// Video_Client_StreamEndPoint methods

Video_Client_StreamEndPoint::Video_Client_StreamEndPoint (Command_Handler *command_handler,
                                                          char *host)
  :command_handler_ (command_handler),
   host_ (host)
{
}

int
Video_Client_StreamEndPoint::handle_open (void)
{
  return -1;
}

int
Video_Client_StreamEndPoint::handle_close (void)
{
  return -1;
}

CORBA::Boolean
Video_Client_StreamEndPoint::handle_preconnect (AVStreams::flowSpec &the_spec)
{
  ACE_DEBUG ((LM_DEBUG,"(%P|%t) handle_preconnect called\n"));
  CORBA::UShort server_port;
  ACE_INET_Addr local_addr;

  // Get the local UDP address
  if (this->dgram_.open (ACE_Addr::sap_any) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,"(%P|%t) datagram open failed %p\n"),0);

      // set the socket buffer sizes to 64k.
  int sndbufsize = ACE_DEFAULT_MAX_SOCKET_BUFSIZ;
  int rcvbufsize = ACE_DEFAULT_MAX_SOCKET_BUFSIZ;

  if (this->dgram_.set_option (SOL_SOCKET,
                               SO_SNDBUF,
                               (void *) &sndbufsize,
                               sizeof (sndbufsize)) == -1
      && errno != ENOTSUP)
    return 0;
  else if (this->dgram_.set_option (SOL_SOCKET,
                                    SO_RCVBUF,
                                    (void *) &rcvbufsize,
                                    sizeof (rcvbufsize)) == -1
           && errno != ENOTSUP)
    return 0;

  if (this->dgram_.get_local_addr (local_addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,"(%P|%t)datagram get local addr failed %p"),-1);
  // form a string
  char client_address_string [BUFSIZ];
  ::sprintf (client_address_string,
             "%s:%d",
             this->host_ == 0 ?local_addr.get_host_name ():this->host_,
             local_addr.get_port_number ());
  the_spec.length (1);
  the_spec [0] = CORBA::string_dup (client_address_string);

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) client flow spec is %s\n",
              client_address_string));
  return 1;
}

CORBA::Boolean
Video_Client_StreamEndPoint::handle_postconnect (AVStreams::flowSpec& server_spec)
{
  ACE_DEBUG ((LM_DEBUG,"(%P|%t) handle_postconnect called \n"));

  // Take the first string of the sequence .
  ACE_INET_Addr server_udp_addr (server_spec [0]);

  server_udp_addr.dump ();
  if (ACE_OS::connect (this->dgram_.get_handle (),(sockaddr *) server_udp_addr.get_addr (),
                       server_udp_addr.get_size ()) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,"(%P|%t) datagram connect failed %p\n"),-1);
  // Now set the data handle of the command handler.

  this->command_handler_->set_video_data_handle (this->dgram_.get_handle ());
  return 0;
}

int
Video_Client_StreamEndPoint::handle_start (const AVStreams::flowSpec &the_spec,
                                           CORBA::Environment &env)

{
  return -1;
}

int
Video_Client_StreamEndPoint::handle_stop (const AVStreams::flowSpec &the_spec,
                                          CORBA::Environment &env)

{
  return -1;
}

int
Video_Client_StreamEndPoint::handle_destroy (const AVStreams::flowSpec &the_spec,
                                             CORBA::Environment &env)

{
  return -1;
}

ACE_HANDLE
Video_Client_StreamEndPoint::get_handle (void)
{
  return this->dgram_.get_handle ();
}

// ---------------------------------------------------------
// Video_Client_VDev methods.

Video_Client_VDev::Video_Client_VDev (void)
  : video_control_ (0),
    command_handler_ (0)
{
}


Video_Client_VDev::Video_Client_VDev (Command_Handler *command_handler)
  :video_control_ (0),
   command_handler_ (command_handler)
{
}

CORBA::Boolean
Video_Client_VDev::set_media_ctrl (CORBA::Object_ptr media_ctrl,
                                   CORBA::Environment &env)
{
  ACE_DEBUG ((LM_DEBUG,"(%P|%t) Video_Client_VDev::set_media_ctrl ()\n"));
  this->video_control_ = Video_Control::_narrow (media_ctrl,
                                                 env);

  TAO_CHECK_ENV_RETURN (env,0);

  this->command_handler_->set_video_control (this->video_control_);

  return 1;
}

// -----------------------------------------------------------
// Audio_Client_VDev methods.

Audio_Client_VDev::Audio_Client_VDev (void)
  : audio_control_ (0),
    command_handler_ (0)
{
}


Audio_Client_VDev::Audio_Client_VDev (Command_Handler *command_handler)
  :audio_control_ (0),
   command_handler_ (command_handler)
{
}

CORBA::Boolean
Audio_Client_VDev::set_media_ctrl (CORBA::Object_ptr media_ctrl,
                                   CORBA::Environment &env)
{
  ACE_DEBUG ((LM_DEBUG,"(%P|%t) Audio_Client_VDev::set_media_ctrl ()\n"));
  this->audio_control_ = Audio_Control::_narrow (media_ctrl,
                                                 env);

  TAO_CHECK_ENV_RETURN (env,0);

  this->command_handler_->set_audio_control (this->audio_control_);

  return 1;
}

// -----------------------------------------------------------
// Video_Endpoint_Reactive_Strategy_A methods

Video_Endpoint_Reactive_Strategy_A::Video_Endpoint_Reactive_Strategy_A (TAO_ORB_Manager *orb_manager,
                                                                        Command_Handler *command_handler)
  : TAO_AV_Endpoint_Reactive_Strategy_A<Video_Client_StreamEndPoint,Video_Client_VDev,AV_Null_MediaCtrl>  (orb_manager),
    command_handler_ (command_handler)
{
}

Video_Endpoint_Reactive_Strategy_A::~Video_Endpoint_Reactive_Strategy_A (void)
{
  //  delete this->vdev_;
  //  delete this->endpoint_;
}

int
Video_Endpoint_Reactive_Strategy_A::make_vdev (Video_Client_VDev *&vdev)
{
  ACE_NEW_RETURN (this->vdev_,
                  Video_Client_VDev (this->command_handler_),
                  -1);
  vdev = this->vdev_;
  return 0;
}

int
Video_Endpoint_Reactive_Strategy_A::make_stream_endpoint (Video_Client_StreamEndPoint *&endpoint)
{
  ACE_NEW_RETURN (this->endpoint_,
                  Video_Client_StreamEndPoint (this->command_handler_,
                                               this->command_handler_->get_data_host ()),
                  -1);
  endpoint = this->endpoint_;
  return 0;
}

// ------------------------------------------------------------
// Audio_Endpoint_Reactive_Strategy_A methods

Audio_Endpoint_Reactive_Strategy_A::Audio_Endpoint_Reactive_Strategy_A (TAO_ORB_Manager *orb_manager,
                                                                        Command_Handler *command_handler)
  : TAO_AV_Endpoint_Reactive_Strategy_A<Audio_Client_StreamEndPoint,Audio_Client_VDev,AV_Null_MediaCtrl>  (orb_manager),
    command_handler_ (command_handler)
{
}

Audio_Endpoint_Reactive_Strategy_A::~Audio_Endpoint_Reactive_Strategy_A (void)
{
  //  delete this->vdev_;
  //  delete this->endpoint_;
}

int
Audio_Endpoint_Reactive_Strategy_A::make_vdev (Audio_Client_VDev *&vdev)
{
  ACE_NEW_RETURN (this->vdev_,
                  Audio_Client_VDev (this->command_handler_),
                  -1);
  vdev = this->vdev_;
  return 0;
}

int
Audio_Endpoint_Reactive_Strategy_A::make_stream_endpoint (Audio_Client_StreamEndPoint *&endpoint)
{
  ACE_NEW_RETURN (this->endpoint_,
                  Audio_Client_StreamEndPoint (this->command_handler_,
                                               this->command_handler_->get_data_host ()),
                  -1);
  endpoint = this->endpoint_;
  return 0;
}

//--------------------------------------------------------------------------------

Receiver_i::Receiver_i (Command_Handler *handler)
  :command_handler_ (handler)
{
}

void
Receiver_i::push_mmdevice (CORBA::Object_ptr audio_mmdevice,
                                 const char *audio_file,
                                 CORBA::Object_ptr video_mmdevice,
                                 const char *video_file,
                                 CORBA::Environment&)
{
  ACE_DEBUG ((LM_DEBUG,"(%P|%t)Receiver_i::push_audio_mmdevice"));
  this->command_handler_->set_mmdevice (audio_mmdevice,audio_file,video_mmdevice,video_file);
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class TAO_AV_Endpoint_Reactive_Strategy<Video_Client_StreamEndPoint,Video_Client_VDev,AV_Null_MediaCtrl>;
template class TAO_AV_Endpoint_Reactive_Strategy_A<Video_Client_StreamEndPoint,Video_Client_VDev,AV_Null_MediaCtrl>;
template class TAO_AV_Endpoint_Reactive_Strategy<Audio_Client_StreamEndPoint,Audio_Client_VDev,AV_Null_MediaCtrl>;
template class TAO_AV_Endpoint_Reactive_Strategy_A<Audio_Client_StreamEndPoint,Audio_Client_VDev,AV_Null_MediaCtrl>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate TAO_AV_Endpoint_Reactive_Strategy<Video_Client_StreamEndPoint,Video_Client_VDev,AV_Null_MediaCtrl>
#pragma instantiate TAO_AV_Endpoint_Reactive_Strategy_A<Video_Client_StreamEndPoint,Video_Client_VDev,AV_Null_MediaCtrl>
#pragma instantiate TAO_AV_Endpoint_Reactive_Strategy<Audio_Client_StreamEndPoint,Audio_Client_VDev,AV_Null_MediaCtrl>
#pragma instantiate TAO_AV_Endpoint_Reactive_Strategy_A<Audio_Client_StreamEndPoint,Audio_Client_VDev,AV_Null_MediaCtrl>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
