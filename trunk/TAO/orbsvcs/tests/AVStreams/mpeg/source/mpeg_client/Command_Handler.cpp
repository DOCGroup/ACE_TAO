// $Id$

#include "Command_Handler.h"
#include "tao/TAO.h"

// %% yikes!!!

#include "ctr.cpp"

const char *TAO_AV_ORB_ARGUMENTS = "-ORBobjrefstyle URL";

Command_Handler::Command_Handler (ACE_HANDLE command_handle)
  : command_handle_ (command_handle),
    orb_manager_ (0)
{
}

int
Command_Handler::init (void)
{
  ACE_ARGV orb_args (TAO_AV_ORB_ARGUMENTS);
  int argc = orb_args.argc ();

  ACE_NEW_RETURN (this->orb_manager_,
                  TAO_ORB_Manager,
                  -1);

  TAO_TRY
    {
      this->orb_manager_->init (argc,
                                orb_args.argv (),
                                TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Command_Handler::init");
      return -1;
    }
  TAO_ENDTRY;
      
}

int
Command_Handler::resolve_server_reference (void)
{
  TAO_TRY
    {
      CORBA::Object_var naming_obj =
        this->orb_manager_->orb ()->resolve_initial_references ("NameService");
      if (CORBA::is_nil (naming_obj.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to resolve the Name Service.\n"),
                          -1);
      CosNaming::NamingContext_var naming_context =
        CosNaming::NamingContext::_narrow (naming_obj.in (),
                                           TAO_TRY_ENV);
      TAO_CHECK_ENV;

      CosNaming::Name Video_Control_name (1);

      Video_Control_name.length (1);
      Video_Control_name [0].id = CORBA::string_dup ("Video_Control");
      CORBA::Object_var Video_Control_obj =
        naming_context->resolve (Video_Control_name,
                                 TAO_TRY_ENV);
      TAO_CHECK_ENV;

      this->video_control_ =
        Video_Control::_narrow (Video_Control_obj.in (),
                                TAO_TRY_ENV);
      TAO_CHECK_ENV;

      if (CORBA::is_nil (this->video_control_.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " could not resolve Video_Control in Naming service <%s>\n"),
                          -1);
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Command_Handler::resolve_server_reference");
      return -1;
    }
  TAO_ENDTRY;

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
  val = OurCmdRead ((char*)&cmd, 1);
  ::TimerProcessing ();

  // if we get an interrupt while reading we go back to the event loop    
  if (val == 1)
    return 0;

  FILE * fp = NULL;   /* file pointer for experiment plan */
  usr1_flag = 0;
    
    //    fprintf(stderr, "CTR: cmd received - %d\n", cmd);
    
  TAO_TRY
    {
      switch (cmd)
        {
        case CmdINIT:
          this->init_video ();
        
          /* following for automatic expriment plan when invoked by Developer */
          if (getuid() == DEVELOPER_UID && videoSocket >= 0) {
            fp = fopen(EXP_PLAN_FILE, "r");
            if (fp != NULL) {
              static char expCmd[6] = {CmdPOSITIONrelease, 0, 0, 0, 0, CmdPLAY};
              fprintf(stderr,
                      "Warning: Auto-exp plan is to be conducted as instructed by file %s\n",
                      EXP_PLAN_FILE);
              cmdBuffer = expCmd;
              cmdBytes = 6;
              cmdAcks = 2;
            }
          }
          else fp = NULL;
      
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

          /* following is for automatic experiment plan */
          if (fp != NULL) {
            char buf[64];
            while (fgets(buf, 64, fp) != NULL) {
              if (!strncmp("Delay", buf, 5)) {
                int val;
                sscanf(strchr(buf, ' '), "%d", &val);
                if (val < 0) val = 1;
                else if (val > 60) val = 60;
                fprintf(stderr, "Auto-exp: Delay for %d seconds\n", val);
                usleep(val * 1000000);
              }
              else if (!strncmp("Experiment", buf, 5)) {
                fprintf(stderr, "Auto-exp: to perform an experiment\n");
                while (fgets(buf, 64, fp) != NULL && buf[0] > ' ') {
                  if (!strncmp("playSpeed", buf, 5)) {
                    double fps;
                    sscanf(strchr(buf, ' '), "%lf", &fps);
                    /* following code is copied from definition of set_speed(void) */
                    shared->framesPerSecond = (int)fps;
                    shared->usecPerFrame = (int) (1000000.0/fps);
                    if (audioSocket >= 0)
                      {
                        double sps = shared->audioPara.samplesPerSecond *
                          fps / shared->pictureRate;
                        shared->samplesPerSecond = (int)sps;
                        shared->usecPerSample = (int)(1000000.0/sps);
                      }
                  }
                  else if (!strncmp("frameRateLimit", buf, 5)) {
                    sscanf(strchr(buf, ' '), "%f", &shared->config.frameRateLimit);
                  }
                  else if (!strncmp("maxSPframes", buf, 5)) {
                    sscanf(strchr(buf, ' '), "%d", &shared->config.maxSPframes);
                  }
                  else if (!strncmp("filterPara", buf, 5)) {
                    sscanf(strchr(buf, ' '), "%d", &shared->config.filterPara);
                  }
                  else if (!strncmp("collectStat", buf, 5)) {
                    sscanf(strchr(buf, ' '), "%d", &shared->config.collectStat);
                  }
                  else if (!strncmp("qosEffective", buf, 5)) {
                    sscanf(strchr(buf, ' '), "%d", &shared->config.qosEffective);
                  }
                  else if (!strncmp("syncEffective", buf, 5)) {
                    sscanf(strchr(buf, ' '), "%d", &shared->config.syncEffective);
                  }
                }
                usleep(500000);
                shared->loopBack = 1;
                break;
              }
              else if (!strncmp("EndExperiment", buf, 5)) {
                fprintf(stderr, "Auto-exp ends.\n");
                usleep(2000000);
                fclose(fp);
                exit(0);
              }
            }
          }

          this->play (fp != NULL,TAO_TRY_ENV);
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
          exit(1);
          break;
        }
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Command_Handler::handle_input ()");
      return -1;
    }
  TAO_ENDTRY;
  return 0;
}

CORBA::Boolean 
Command_Handler::init_video (void)
{
  ::init ();
  return 0;
}


CORBA::Boolean 
Command_Handler::stat_stream (CORBA::Char_out ch,
                              CORBA::Long_out size)
{
  return 0;
}


CORBA::Boolean 
Command_Handler::close (void)
{
  return 0;
}


CORBA::Boolean 
Command_Handler::stat_sent (void)
{
  return 0;
}


CORBA::Boolean 
Command_Handler::fast_forward (void)
                               
{
  ::ff ();
  return 0;
}


CORBA::Boolean 
Command_Handler::fast_backward (void)
                                
{
  ::fb ();
  return 0;
}


CORBA::Boolean 
Command_Handler::step (void)
                       
{
  ::step ();
  return 0;
}


CORBA::Boolean 
Command_Handler::play (int auto_exp,
                       CORBA::Environment& env)
{

  CORBA::Long vts;
  unsigned char tmp;
  unsigned int  ats;
  int cmdstarted = 0;
  int stuffsamples = 0;
  /*
 fprintf (stderr, "CTR: PLAY . . .\n");
 */
  stop_playing ();

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
    shared->nextFrame = shared->totalFrames - 1;

  if (audioSocket >= 0 && shared->nextSample < shared->totalSamples && rtplay)
    {
      PLAYaudioPara para;
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

      ABflushBuf (shared->nextSample);
  
      para.sn = htonl (shared->cmdsn);
      para.nextSample = htonl (shared->nextSample);
      para.samplesPerSecond = htonl (shared->samplesPerSecond);
      para.samplesPerPacket = htonl (1024 / shared->audioPara.bytesPerSample);
      para.ABsamples = htonl (AB_BUF_SIZE / shared->audioPara.bytesPerSample);
      para.spslimit = htonl (32000);
  
      startTime = get_usec ();
      tmp = CmdPLAY;
      AudioWrite (&tmp, 1);
      AudioWrite (&para, sizeof (para));
      read_int (audioSocket, (int *)&ats);
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
  
      para->VStimeAdvance = htonl (shared->VStimeAdvance);
      para->sn = htonl (shared->cmdsn);
      para->nextFrame = htonl (shared->nextFrame);
      para->usecPerFrame = htonl (shared->usecPerFrame);
      para->framesPerSecond = htonl (shared->framesPerSecond);
      para->collectStat = htonl (shared->collectStat);
      frate = shared->config.frameRateLimit;
      if (frate <= 0.0) {
        frate = 1.0;
      }
      shared->frameRateLimit = frate;
      para->frameRateLimit1000 =
        htonl ( (long) (shared->frameRateLimit * 1000.0));
      compute_sendPattern ();
      para->sendPatternGops = htonl (shared->sendPatternGops);
      //      memcpy (para->sendPattern, shared->sendPattern, PATTERN_SIZE);
      
      // Sequence of chars

      para->sendPattern.length (PATTERN_SIZE);

      for (int i=0; i<PATTERN_SIZE ; i++)
        para->sendPattern [i] = shared->sendPattern [i];
             
      startTime = get_usec ();
      /*
      tmp = CmdPLAY;
      VideoWrite (&tmp, 1);
      VideoWrite (&para, sizeof (para));
      read_int (videoSocket, (int *)&vts);
      */
      // CORBA call
      this->video_control_->play (para,
                                  vts,
                                  env);
      
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
   
        /*
          fprintf (stderr, "CTR init frate: %lf minupf %d, shared->upf %d\n",
          frate, minupf, shared->usecPerFrame);
        */
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
      ABskipSamples (skipped);
      Fprintf (stderr, "Live source: skipped %d audio samples\n", skipped);
    }
  }
  else if (stuffsamples) {
    ABskipSamples (-stuffsamples);
  }
  if (cmdstarted)
    start_timer ();
  tmp = CmdDONE;
  CmdWrite (&tmp, 1);
  return 0;
}


CORBA::Boolean 
Command_Handler::position (void)
                           
{
  ::position ();
  return 0;
}

int
Command_Handler::position_release (void)
                           
{
  ::position_release ();
  return 0;
}

int
Command_Handler::volume (void)
                           
{
  ::volume ();
  return 0;
}

int
Command_Handler::balance (void)
                           
{
  ::balance ();
  return 0;
}

CORBA::Boolean 
Command_Handler::speed (void)
                        
{
  ::speed ();
  return 0;
}


CORBA::Boolean 
Command_Handler::stop (void)
                       
{
  ::stop ();
  return 0;
}
// ----------------------------------------------------------------------

// Client_Sig_Handler methods
// handles the timeout SIGALRM signal
Client_Sig_Handler::Client_Sig_Handler ()
{
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
  if (ACE_Reactor::instance ()->register_handler 
      (this, ACE_Event_Handler::NULL_MASK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, 
                       "%p\n", 
                       "register_handler"),
                      -1);

  // Create a sigset_t corresponding to the signals we want to catch.
  ACE_Sig_Set sig_set;

  sig_set.sig_add (SIGINT);
  sig_set.sig_add (SIGQUIT);
  sig_set.sig_add (SIGALRM);  
  sig_set.sig_add (SIGUSR1);
  sig_set.sig_add (SIGUSR2);  

  // Register the signal handler object to catch the signals.
  if (ACE_Reactor::instance ()->register_handler (sig_set, 
                                                  this) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, 
                       "%p\n", 
                       "register_handler"),
                      -1);
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
  //  ACE_DEBUG ((LM_DEBUG, "(%t) received signal %S\n", signum));

  switch (signum)
    {
    case SIGALRM:
      // Handle the timeout
      ::TimerHandler (signum);
      // %% ??!!!
      break;
    case SIGUSR1:
      usr1_handler (signum);
      break;
    case SIGUSR2:
      default_usr2_handler (signum);
      break;
    case SIGCHLD:
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) received signal %S\n", signum));
      pid = ACE_OS::wait (&status);
      if (pid == UIpid)
        {
          cerr << "ui exited, im ending the event loop!" << endl;
          ACE_Reactor::instance ()->end_event_loop ();
        }
      return 0;
    case SIGINT:
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) received signal %S\n", signum));
      ACE_Reactor::instance ()->end_event_loop ();
      ::on_exit_routine ();
      return 0;
    default: 
      ACE_DEBUG ((LM_DEBUG, 
                  "(%t) %S: not handled, returning to program\n", 
                  signum));
      break;
    }
  TimerProcessing ();
  return 0;
}
