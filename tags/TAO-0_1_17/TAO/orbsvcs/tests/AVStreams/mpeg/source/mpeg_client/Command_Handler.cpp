// $Id$

#include "Command_Handler.h"

// We have many if conditions based on audiosocket >=0 and
// videoSocket >=0 and these are the old TCP sockets used by him. With
// CORBA these have gone and being set to the UDP socket number and
// these will work fine for single files. The conditions may have to
// be changed to CORBA::is_nil on the CORBA objects and the
// connections closed if doing for multiple files.


// %% yikes!!!
#include "ctr.cpp"

const char *TAO_AV_ORB_ARGUMENTS = "-ORBobjrefstyle URL";

Command_Handler::Command_Handler (ACE_HANDLE command_handle)
  : command_handle_ (command_handle),
    orb_manager_ (0)
{
}

Command_Handler::~Command_Handler (void)
{
  ACE_Reactor::instance ()->remove_handler (this,
                                            ACE_Event_Handler::READ_MASK);
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
  return 0;
}

int
Command_Handler::resolve_video_reference (void)
{
  /*
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
      TAO_TRY_ENV.print_exception ("Command_Handler::resolve_video_reference");
      return -1;
    }
  TAO_ENDTRY;
  */
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

      CosNaming::Name video_mmdevice_name (1);

      video_mmdevice_name.length (1);
      video_mmdevice_name [0].id = CORBA::string_dup ("Video_MMDevice");
      video_mmdevice_name [0].id = CORBA::string_dup ("Video_MMDevice");
      CORBA::Object_var video_mmdevice_obj =
        naming_context->resolve (video_mmdevice_name,
                                 TAO_TRY_ENV);
      TAO_CHECK_ENV;

      this->video_mmdevice_ =
        AVStreams::MMDevice::_narrow (video_mmdevice_obj.in (),
                                      TAO_TRY_ENV);
      TAO_CHECK_ENV;

      if (CORBA::is_nil (this->video_mmdevice_.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " could not resolve Video_Mmdevice in Naming service <%s>\n"),
                          -1);
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

int
Command_Handler::resolve_audio_reference (void)
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

      CosNaming::Name Audio_Control_name (1);

      Audio_Control_name.length (1);
      Audio_Control_name [0].id = CORBA::string_dup ("Audio_Control");
      CORBA::Object_var Audio_Control_obj =
        naming_context->resolve (Audio_Control_name,
                                 TAO_TRY_ENV);
      TAO_CHECK_ENV;

      this->audio_control_ =
        Audio_Control::_narrow (Audio_Control_obj.in (),
                                TAO_TRY_ENV);
      TAO_CHECK_ENV;

      if (CORBA::is_nil (this->audio_control_.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " could not resolve Audio_Control in Naming service <%s>\n"),
                          -1);
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Command_Handler::resolve_video_reference");
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
          ACE_DEBUG ((LM_DEBUG,"(%P|%t) command_handler:CmdINIT received\n"));
          this->init_av ();
          // automatic experiment code zapped :-)
          fp = NULL;
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
          // automatic experiment code zapped :-)
          if (this->play (fp != NULL,
                          TAO_TRY_ENV) < 0)
            ACE_ERROR_RETURN ((LM_ERROR,"(%P|%t)play failed\n"),-1);
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

int 
Command_Handler::init_av (void)
{

  int i, j;

  //  if (this->orb_manager_ != 0)
  //    delete this->orb_manager_;

  // init the ORB manager.
  //  this->init ();
  /* try to stop and close previous playing */
  if (audioSocket >= 0 || videoSocket >= 0)
    {
      // this may have to be taken care of afterwards.
      unsigned char tmp = CmdCLOSE;
      int result = 
        this->stop_playing();
      if (result < 0)
        return result;
    
      if (audioSocket >= 0)
        {
          if (ABpid > 0) {
            kill(ABpid, SIGUSR1);
            ABpid = -1;
          }
          usleep(10000);
        }
    
      if (videoSocket >= 0)
        {
          if (VBpid > 0) {
            kill(VBpid, SIGUSR1);
            VBpid = -1;
          }
          usleep(10000);
          this->close ();
          //          ComCloseConn(videoSocket);
          videoSocket = -1;
          while ((!VBbufEmpty()) || !VDbufEmpty()) {
            while (VDpeekMsg() != NULL) {
              VDreclaimMsg(VDgetMsg());
            }
            usleep(10000);
          }
          usleep(10000);
        }
    }

  /* read in video/audio files */
  NewCmd(CmdINIT);
  CmdRead((char*)&i, 4);
  CmdRead(vh, i);
  vh[i] = 0;
  CmdRead((char*)&i, 4);
  CmdRead(vf, i);
  vf[i] = 0;
  CmdRead((char*)&i, 4);
  CmdRead(ah, i);
  ah[i] = 0;
  CmdRead((char*)&i, 4);
  CmdRead(af, i);
  af[i] = 0;
  /*
    fprintf(stderr, "INIT: vh-%s, vf-%s, ah-%s, af-%s\n", vh, vf, ah, af);
  */

  shared->live = 0;
  shared->audioMaxPktSize = !shared->config.audioConn;
  shared->videoMaxPktSize = !shared->config.videoConn;
  
  if (af[0] != 0)
    {
      if (init_audio_channel(ah, af))
        {
          audioSocket = -1;
          shared->totalSamples = 0;
        }
      else
        {      
          ACE_DEBUG ((LM_DEBUG,"(%P|%t) Initialized audio\n"));
          shared->nextSample = 0;
          if (shared->config.maxSPS < shared->audioPara.samplesPerSecond)
            shared->config.maxSPS < shared->audioPara.samplesPerSecond;
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
      if (this->init_video_channel(vh, vf))
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
      return 0;
    }
  else
    {
      unsigned char tmp = CmdDONE;
      set_speed();
      if (videoSocket >= 0)
        wait_display();
      CmdWrite(&tmp, 1);
      if (videoSocket < 0)
        {
          tmp = CmdVPclearScreen;
          CmdWrite(&tmp, 1);
        }
      return 0;
    }
  return 0;
}

int
Command_Handler::init_audio_channel (char *phostname, char *audiofile)
{
  ACE_DEBUG ((LM_DEBUG,"(%P|%t) init_audio_channel called\n"));
  int dataSocket = -1;

  if (!hasAudioDevice)
  {
    fprintf(stderr, "CTR warning: Audio device not available, Audio ignored.\n");
    return -1;
  }

  if (this->connect_to_audio_server(phostname, 
                                    &audioSocket,
                                    &dataSocket, 
                                    &shared->audioMaxPktSize) == -1) 
    return -1;
  
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
    para->nameLength = strlen(audiofile)+1;

    para->audiofile.length (strlen(audiofile));

    for (int i=0;i<para->audiofile.length ();i++)
      para->audiofile [i] = audiofile [i];

    // CORBA call
    TAO_TRY
      {
        CORBA::Boolean result;
        result = this->audio_control_->init_audio (para.in (),
                                                   reply.out (),
                                                   TAO_TRY_ENV);
        TAO_CHECK_ENV;
        if (result == (CORBA::B_FALSE))
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
      switch (ABpid = fork())
      {
      case -1:
	perror("CTR error on forking AB process");
	exit(1);
	break;
      case 0:
	if (realTimeFlag) {
	  SetRTpriority("AB", -1);
	}
	free(vh);
	free(vf);
	free(ah);
	free(audiofile);
        //	ComCloseFd(audioSocket);
        //	if (videoSocket >= 0)
          //	  ComCloseFd(videoSocket);
	VBdeleteBuf();
	VDdeleteBuf();
	if (cmdSocket >= 0)
	  ::close(cmdSocket);
	if (realTimeFlag >= 2) {
#ifdef __svr4__
	  if (SetRTpriority("AB", 0)) realTimeFlag = 0;
#elif defined(_HPUX_SOURCE)
	  if (SetRTpriority("AB", 1)) realTimeFlag = 0;
#endif
	}
	ABprocess(dataSocket);
	break;
      default:
        //	ComCloseFd(dataSocket);
	ABflushBuf(0);
	break;
      }
    }
  }
  return 0;
}

int
Command_Handler::init_video_channel (char *phostname, char *videofile)
{  
  //  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Reached line %d in %s\n", __LINE__, __FILE__));
  int dataSocket = -1;

  //  if (ComOpenConnPair(phostname, &videoSocket,
  //                      &dataSocket, &shared->videoMaxPktSize) == -1) {
  //    return -1;
  // }
  if (this->connect_to_video_server (phostname,
                               &videoSocket,
                               &dataSocket,
                               &shared->videoMaxPktSize) == -1)
    return -1;


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
        if (result == (CORBA::B_FALSE))
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
          //         memcpy(ptr, shared->pattern, shared->patternSize);
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
        exit(1);
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
        ComCloseConn(dataSocket);
        ComCloseConn(videoSocket);
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
          perror("CTR error on open CTR-VB socketpair");
          exit(1);
        }
      
      switch (VBpid = fork())
        {
        case -1:
          perror("CTR error on forking VB process");
          exit(1);
          break;
        case 0:
          //          ACE_DEBUG ((LM_DEBUG, "(%P|%t) Reached line %d in %s\n", __LINE__, __FILE__));
          if (realTimeFlag) {
            SetRTpriority("VB", -1);
          }
          free(vh);
          free(videofile);
          free(ah);
          free(af);
          ::close(sp[0]);
          //::close (videoSocket);
          //          if (audioSocket >= 0)
          //            ComCloseFd(audioSocket);
          ::close (audioSocket);
          ABdeleteBuf();
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
          VBprocess(sp[1], dataSocket);
          break;
        default:
          ::close(sp[1]);
          //          ::close(dataSocket);
          {
            int bytes, res;
            /* passing all messages of INIT frame to VB here. */
            char * buf = (char *)malloc(INET_SOCKET_BUFFER_SIZE);
            VideoMessage *msg = (VideoMessage *)buf;
            int pkts = 1, msgo = 0, msgs = 0;
	  
            if (buf == NULL) {
              perror("CTR error on malloc() for INIT frame");
              exit(1);
            }
            while (msgo + msgs < pkts) {
              //              ACE_DEBUG ((LM_DEBUG, "(%P|%t) Reached line %d in %s\n", __LINE__, __FILE__));
              //              cerr << "expecting a packet of size " << sizeof (*msg)                   << endl;

              VideoRead(buf, sizeof(*msg));
              //~~ we need to read the first frame from the 
              //  data socket instead of control socket.
              //              SocketRecv(dataSocket, buf, size);
              //              ACE_DEBUG ((LM_DEBUG,"packetsn = %d,msgsn = %d\n",
              //                          msg->packetsn,msg->msgsn));
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
                while ((res = write(sp[0], buf, bytes)) == -1) {
                  if (errno == EINTR || errno == ENOBUFS) continue;
                  perror("CTR error on sending INIT frame to VB");
                  exit(1);
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
            read(sp[0], buf, 1); /* read a garbage byte, to sync with VB */
            ::close(sp[0]);
            free(buf);
          }
          break;
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
  // flag to call close only once.
  //  static called = 0;
//    if (audioSocket >=0)
//      {
//        int result = 
//          this->stop_playing();
//        if (result < 0)
//          return result;
       
//        if (audioSocket >= 0)
//          {
//            if (ABpid > 0) {
//              kill(ABpid, SIGUSR1);
//              ABpid = -1;
//            }
//            usleep(10000);
//            ACE_DEBUG ((LM_DEBUG, "(%P|%t) Reached line %d in %s\n", __LINE__, __FILE__));
//          }
//      }
  //  if (!called)
  //    {
  //  called =1;
      TAO_TRY
        {
          if (CORBA::is_nil (this->audio_control_.in ()) == CORBA::B_FALSE)
            {
              // one way function call.
              this->audio_control_->close (TAO_TRY_ENV);
              
              TAO_CHECK_ENV;
              ACE_DEBUG ((LM_DEBUG,"(%P|%t) audio close done \n"));
            }
          
          if (CORBA::is_nil (this->video_control_.in ()) == CORBA::B_FALSE)
            {
              // one way function call.
              this->video_control_->close (TAO_TRY_ENV);
              TAO_CHECK_ENV;
              ACE_DEBUG ((LM_DEBUG,"(%P|%t) video close done \n"));
            }
        }
      TAO_CATCHANY
        {
          //      TAO_TRY_ENV.print_exception ("video_control_->close (..)");
          return -1;
        }
      TAO_ENDTRY;
      return 0;
      //    }
      //  return 0;
}


int 
Command_Handler::stat_sent (void)
{
  return 0;
}


int 
Command_Handler::fast_forward (void)
                               
{
  //  ::ff ();
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
            if (result == (CORBA::B_FALSE))
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
  //  ::fb ();
  //  return 0;
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
            if (result == (CORBA::B_FALSE))
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
  //  ::step ();
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
          if (result == (CORBA::B_FALSE))
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
    ::wait_display ();
  }
  unsigned char tmp = CmdDONE;
  CmdWrite(&tmp, 1);
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
      //      ACE_DEBUG ((LM_DEBUG,"(%P|%t) nextframe is > totalframes\n"));
      shared->nextFrame = shared->totalFrames - 1;
    }

  //  ACE_DEBUG ((LM_DEBUG,"(%P|%t) nextsample=%d,totalsamples=%d",
  //          shared->nextSample,shared->totalSamples));
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

      ABflushBuf (shared->nextSample);
  
      para->sn = shared->cmdsn;
      para->nextSample = shared->nextSample;
      para->samplesPerSecond = shared->samplesPerSecond;
      para->samplesPerPacket = 1024 / shared->audioPara.bytesPerSample;
      para->ABsamples = AB_BUF_SIZE / shared->audioPara.bytesPerSample;
      para->spslimit = 32000;
  
      startTime = get_usec ();
      // CORBA call.

      cerr << "Calling audio_control_->play ()" << endl;
      result =this->audio_control_->play (para,
                                          ats,
                                          env);

      cerr << "audio_control_ result is " << (int) result << endl;
      if (result == CORBA::B_FALSE)
        return -1;
      TAO_CHECK_ENV_RETURN (env,-1);
      //      ACE_DEBUG ((LM_DEBUG,"(%P|%t)Reached line %d in %s",__LINE__,__FILE__));
      //      read_int (audioSocket, (int *)&ats);
      //      ACE_DEBUG ((LM_DEBUG,"(%P|%t)Reached line %d in %s",__LINE__,__FILE__));
    }

  //  ACE_DEBUG ((LM_DEBUG,"(%P|%t) nextFrame=%d,totalFrames=%d",
  //          shared->nextFrame,shared->totalFrames));
  if (videoSocket >= 0 && shared->nextFrame < shared->totalFrames)
    {
      //      ACE_DEBUG ((LM_DEBUG,"(%P|%t) %s,%d\n",__FILE__,__LINE__));
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
      /*
      tmp = CmdPLAY;
      VideoWrite (&tmp, 1);
      VideoWrite (&para, sizeof (para));
      read_int (videoSocket, (int *)&vts);
      */
      // CORBA call
      result =this->video_control_->play (para,
                                          vts,
                                          env);
      if (result == CORBA::B_FALSE)
        return -1;
      TAO_CHECK_ENV_RETURN (env,-1);
      //      ACE_DEBUG ((LM_DEBUG,"(%P|%t)Reached line %d in %s",__LINE__,__FILE__));
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

int
Command_Handler::position_action (int operation_tag)
{
  int val;
  unsigned char tmp = CmdDONE;
  CmdRead ((char*)&val, 4);

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
            if (result == (CORBA::B_FALSE))
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
          wait_display();
      }
      if (operation_tag && audioSocket >= 0) /* needs to adjust audio position */
      {

        shared->nextSample = (int)((double)shared->audioPara.samplesPerSecond *
                             ((double)shared->nextFrame / shared->pictureRate));
        //        ACE_DEBUG ((LM_DEBUG,"shared->nextsample = %d\n",shared->nextSample));
      }
    }
    else if (audioSocket >= 0)
      shared->nextSample  = (shared->totalSamples-1) * val / POSITION_RANGE;
  }
  tmp = CmdDONE;
  CmdWrite(&tmp, 1);
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
  CmdRead((char *)&shared->volumePosition, 4);
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
  CmdRead((char *)&shared->balancePosition, 4);
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
  unsigned char tmp;
  CORBA::Boolean result;
  CmdRead((char *)&shared->speedPosition, 4);
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
        
              //        memcpy(para.sendPattern, shared->sendPattern, PATTERN_SIZE);
              para->sendPattern.length (PATTERN_SIZE);
              for (int i=0; i< PATTERN_SIZE ; i++)
                para->sendPattern[i]=shared->sendPattern[i];
              // CORBA call
              result = this->video_control_->speed (para.in (),
                                                    TAO_TRY_ENV);
              TAO_CHECK_ENV;
              //              ACE_DEBUG ((LM_DEBUG,
              //                          "(%P|%t) CORBA call returned %d\n",
              //                          result));
              if (result == (CORBA::B_FALSE))
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
              if (result == (CORBA::B_FALSE))
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
  /*
  ::stop ();
  return 0;
  */
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
  //  ACE_DEBUG ((LM_DEBUG,"(%P|%t) command_handler::Stop cmdone sent\n"));
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
          if ((CORBA::is_nil (this->audio_control_.in ()) == CORBA::B_FALSE) 
              && precmd == CmdPLAY
              && rtplay)
            {
              //  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Reached line %d in %s\n", __LINE__, __FILE__));
              // CORBA call
              CORBA::Boolean result =
                this->audio_control_->stop (shared->cmdsn,
                                            TAO_TRY_ENV);
              //              cerr << "audio_control_->stop result is " << result << endl;
              if (result == (CORBA::B_FALSE))
                  return -1;
              TAO_CHECK_ENV;
            }
          if (CORBA::is_nil (this->video_control_.in ()) == CORBA::B_FALSE)
            {
              //              ACE_DEBUG ((LM_DEBUG, "(%P|%t) Reached line %d in %s\n", __LINE__, __FILE__));
              // CORBA call
              CORBA::Boolean result =
                this->video_control_->stop (shared->cmdsn,
                                            TAO_TRY_ENV);
              if (result == (CORBA::B_FALSE))
                return -1;
              TAO_CHECK_ENV;
            }
          //          ACE_DEBUG ((LM_DEBUG, "(%P|%t) Reached line %d in %s\n", __LINE__, __FILE__));
          
          /* stop timer and sleep for a while */
          //          cerr << "stopping timer" << endl;
          stop_timer();
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
    
}

// connects and handshakes with the server
int
Command_Handler::connect_to_video_server (char *address, 
                                          int *ctr_fd, 
                                          int *data_fd, 
                                          int *max_pkt_size)
{

  // set the pointers to the correct values
  *max_pkt_size = -INET_SOCKET_BUFFER_SIZE;

  // construct the server addr
  ACE_INET_Addr server_addr (VCR_TCP_PORT,
                             address);

  this->video_stream_.close ();
  if (this->video_connector_.connect (this->video_stream_,
                                server_addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) Connection to server failed: %p\n",
                       "connect"),
                      -1);
  // Write the CmdINITvideo to tell the server that this is a video
  // client.
  int tmp;
  tmp = CmdINITvideo;
  this->video_stream_.send_n (&tmp, sizeof (tmp));
  int ack;
  this->video_stream_.recv_n (&ack, sizeof (ack));

  //  ACE_DEBUG ((LM_DEBUG, 
  //              "(%P|%t) got ack :%d\n", 
  //              ack));
  
  // initialize the command handler , ORB
  if (this->resolve_video_reference () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) command_handler: resolve_video_reference returned -1"),
                       -1);

  return 0;
  // ~~Short cut to just find the mmdevice from the Naming service.

  CORBA::UShort server_port;
  ACE_INET_Addr local_addr;
  CORBA::String client_address_string;
  
  TAO_TRY
    {
      ACE_NEW_RETURN (client_address_string,
                      char [BUFSIZ],
                      -1);
      this->video_dgram_.close ();
      // Get the local UDP address
      if (this->video_dgram_.open (ACE_Addr::sap_any) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,"(%P|%t)datagram open failed %p"),-1);

      // set the socket buffer sizes to 64k.
      int sndbufsize = ACE_DEFAULT_MAX_SOCKET_BUFSIZ;
      int rcvbufsize = ACE_DEFAULT_MAX_SOCKET_BUFSIZ;

      if (this->video_dgram_.set_option (SOL_SOCKET,
                                         SO_SNDBUF,
                                         (void *) &sndbufsize,
                                         sizeof (sndbufsize)) == -1
          && errno != ENOTSUP)
        return -1;
      else if (this->video_dgram_.set_option (SOL_SOCKET,
                                              SO_RCVBUF,
                                              (void *) &rcvbufsize,
                                              sizeof (rcvbufsize)) == -1
               && errno != ENOTSUP)
        return -1;

      if (this->video_dgram_.get_local_addr (local_addr) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,"(%P|%t)datagram get local addr failed %p"),-1);
      // form a string 
      ::sprintf (client_address_string,
                 "%s:%d",
                 local_addr.get_host_name (),
                 local_addr.get_port_number ());
      
      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) Client string is %s\n",
                  client_address_string));
      
      server_port = this->video_control_->set_peer (client_address_string,
                                                    TAO_TRY_ENV);
      TAO_CHECK_ENV;
      if (server_port == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) set_peer failed\n"),
                          -1);
      ACE_DEBUG ((LM_DEBUG,"(%P|%t) set_peer done:server port =%d\n",server_port));
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("video_control_->fast_forward_video (..)");
      return -1;
    }
  TAO_ENDTRY;

  ACE_INET_Addr server_udp_addr (server_port,
                                 address);

  if (ACE_OS::connect (this->video_dgram_.get_handle (),(sockaddr *) server_udp_addr.get_addr (),
                       server_udp_addr.get_size ()) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,"(%P|%t)datagram connect failed %p"),-1);

  // set the pointers to the correct values
  *ctr_fd = *data_fd = this->video_dgram_.get_handle ();
  // set both the control and data socket the same UDP socket.
  //  ACE_DEBUG ((LM_DEBUG,
  //              "Control and data fd = %d\n",
  //              *ctr_fd));
  return 0;
}

// connects and handshakes with the server
int
Command_Handler::connect_to_audio_server (char *address, 
                                          int *ctr_fd, 
                                          int *data_fd, 
                                          int *max_pkt_size)
{

  // set the pointers to the correct values
  *max_pkt_size = -INET_SOCKET_BUFFER_SIZE;

  // construct the server addr
  ACE_INET_Addr server_addr (VCR_TCP_PORT,
                             address);

  this->audio_stream_.close ();
  if (this->audio_connector_.connect (this->audio_stream_,
                                      server_addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) Connection to server failed: %p\n",
                       "connect"),
                      -1);
  // Write the CmdINITaudio to tell the server that this is a audio
  // client.
  int tmp;
  tmp = CmdINITaudio;
  this->audio_stream_.send_n (&tmp, sizeof (tmp));
  int ack;
  this->audio_stream_.recv_n (&ack, sizeof (ack));

  ACE_DEBUG ((LM_DEBUG,"(%P|%t) got ack :%d\n",ack));
  
  // initialize the command handler , ORB
  if (this->resolve_audio_reference () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) command_handler: resolve_audio_reference returned -1"),
                       -1);

  CORBA::UShort server_port;
  ACE_INET_Addr local_addr;
  CORBA::String client_address_string;
  TAO_TRY
    {
      ACE_NEW_RETURN (client_address_string,
                      char [BUFSIZ],
                      -1);
      this->audio_dgram_.close ();
      // Get the local UDP address
      if (this->audio_dgram_.open (ACE_Addr::sap_any) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,"(%P|%t)datagram open failed %p"),-1);

      if (this->audio_dgram_.get_local_addr (local_addr) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,"(%P|%t)datagram get local addr failed %p"),-1);
      // form a string 
      ::sprintf (client_address_string,
                 "%s:%d",
                 local_addr.get_host_name (),
                 local_addr.get_port_number ());
      
      //      ACE_DEBUG ((LM_DEBUG,
      //          "(%P|%t) Client string is %s\n",
      //          client_address_string));
      
      server_port = this->audio_control_->set_peer (client_address_string,
                                                    TAO_TRY_ENV);
      TAO_CHECK_ENV;
      if (server_port == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) set_peer failed\n"),
                          -1);
      ACE_DEBUG ((LM_DEBUG,"(%P|%t) set_peer done:server port =%d\n",server_port));
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("audio_control_->set_peer () (..)");
      return -1;
    }
  TAO_ENDTRY;

  ACE_INET_Addr server_udp_addr (server_port,
                                 address);

  if (ACE_OS::connect (this->audio_dgram_.get_handle (),(sockaddr *) server_udp_addr.get_addr (),
                       server_udp_addr.get_size ()) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,"(%P|%t)datagram connect failed %p"),-1);

  // set the pointers to the correct values
  *ctr_fd = *data_fd = this->audio_dgram_.get_handle ();
  // set both the control and data socket the same UDP socket.
  //  ACE_DEBUG ((LM_DEBUG,
  //          "Control and data fd = %d\n",
  //          *ctr_fd));
  return 0;
}

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
    size = ABgetSamples(rawBuf, maxSize);
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

// ----------------------------------------------------------------------

// Client_Sig_Handler methods
// handles the timeout SIGALRM signal
Client_Sig_Handler::Client_Sig_Handler (Command_Handler *command_handler)
  : command_handler_ (command_handler)
{
}

Client_Sig_Handler::~Client_Sig_Handler (void)
{
  ACE_Reactor::instance ()->remove_handler (this,
                                            ACE_Event_Handler::NULL_MASK);

  ACE_Reactor::instance ()->remove_handler (this->sig_set);
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

  this->sig_set.sig_add (SIGINT);
  this->sig_set.sig_add (SIGQUIT);
  this->sig_set.sig_add (SIGALRM);  
  this->sig_set.sig_add (SIGUSR1);
  this->sig_set.sig_add (SIGUSR2);  

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
  //  ACE_DEBUG ((LM_DEBUG, "(%P|%t) received signal %S\n", signum));

  switch (signum)
    {
    case SIGALRM:
      // Handle the timeout
      this->TimerHandler (signum);
      // %% ??!!!
      break;
    case SIGUSR1:
      ::usr1_handler (signum);
      break;
    case SIGUSR2:
      ::default_usr2_handler (signum);
      break;
    case SIGCHLD:
      //      ACE_DEBUG ((LM_DEBUG, "(%P|%t) received signal %S\n", signum));
      pid = ACE_OS::wait (&status);
      // if (pid == UIpid)
//         {
//           ACE_DEBUG ((LM_DEBUG, "(%P|%t) UI process died, removing signal handlers from the reactor\n", signum));
//           this->command_handler_->close ();
//           ACE_Reactor::instance ()->remove_handler (this->sig_set);
//           ACE_Reactor::instance ()->end_event_loop ();
//         }
      return 0;
    case SIGINT:
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) received signal %S, removing signal handlers from the reactor\n", signum));
      this->command_handler_->close ();
      ACE_Reactor::instance ()->remove_handler (this->sig_set);
      ACE_Reactor::instance ()->end_event_loop ();
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
  if (audioSocket > 0)
    {
      //      ACE_DEBUG ((LM_DEBUG,"(%P|%t) Adjusting audio nextsample value\n"));
      double frames_per_group =
        (double)shared->totalFrames/shared->totalGroups;
      long nextframe = shared->nextGroup * frames_per_group;
      shared->nextSample = (int)((double)shared->audioPara.samplesPerSecond *
                                 ((double)nextframe / shared->pictureRate));
      
    /* audio involved,  TimerProcessing() will adjust the rate automatically */
    }
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
  
  /*
  usleep(200000);
  */
}

void
Client_Sig_Handler::DisplayPicture(void)
{
  int toDisplay = 1;
  int count = timerCount;
  
  if ((shared->cmd != CmdPLAY &&
       shared->cmd != CmdFF &&
       shared->cmd != CmdFB) ||
      lastCount == count)
    return;
  if (timer_on >4)   /* 4 is a magic number */
    {
      this->stop_timer();
      
      // stop both the audio and video.
      this->command_handler_->stop ();
      
      /* tries to rewind and play again */
      if (shared->cmd == CmdPLAY && shared->loopBack)
        loopBack();
      
      return;
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
          goto loop_end;
        }
 
        switch (shared->cmd)
        {
          int position;
        case CmdPLAY:
          position = shared->nextFrame;
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
            perror("CTR failed to allocate space for fr filter");
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
    DisplayPicture();
  }
  //  cerr << "Timerprocessing signal-handler done\n";
}

CORBA::Boolean
Video_Client_StreamEndPoint::handle_preconnect (void)
{
  ACE_DEBUG ((LM_DEBUG,"(%P|%t) handle_preconnect called\n"));
  CORBA::UShort server_port;
  ACE_INET_Addr local_addr;
  CORBA::String client_address_string;
  
  ACE_NEW_RETURN (client_address_string,
                  char [BUFSIZ],
                  -1);
  // Get the local UDP address
  if (this->dgram_.open (ACE_Addr::sap_any) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,"(%P|%t)datagram open failed %p"),-1);

      // set the socket buffer sizes to 64k.
  int sndbufsize = ACE_DEFAULT_MAX_SOCKET_BUFSIZ;
  int rcvbufsize = ACE_DEFAULT_MAX_SOCKET_BUFSIZ;

  if (this->dgram_.set_option (SOL_SOCKET,
                               SO_SNDBUF,
                               (void *) &sndbufsize,
                               sizeof (sndbufsize)) == -1
      && errno != ENOTSUP)
    return -1;
  else if (this->dgram_.set_option (SOL_SOCKET,
                                    SO_RCVBUF,
                                    (void *) &rcvbufsize,
                                    sizeof (rcvbufsize)) == -1
           && errno != ENOTSUP)
    return -1;

  if (this->dgram_.get_local_addr (local_addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,"(%P|%t)datagram get local addr failed %p"),-1);
  // form a string 
  ::sprintf (client_address_string,
             "%s:%d",
             local_addr.get_host_name (),
             local_addr.get_port_number ());
      
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Client string is %s\n",
              client_address_string));
  return 0;
}

CORBA::Boolean
Video_Client_StreamEndPoint::handle_postconnect (void)
{
  ACE_DEBUG ((LM_DEBUG,"(%P|%t) handle_postconnect called \n"));
  return 0;
}

// CORBA::Boolean
// Video_Client_StreamEndPoint::connect (AVStreams::StreamEndPoint_ptr responder, 
//                                       AVStreams::streamQoS &qos_spec, 
//                                       const AVStreams::flowSpec &the_spec,  
//                                       CORBA::Environment &env)
// {
//   ACE_DEBUG ((LM_DEBUG,"(%P|%t) Video_Client_StreamEndPoint::connect () \n"));
//   this->handle_preconnect ();
//   responder->request_connection (this->_this (env),
//                                  0,
//                                  qos_spec,
//                                  the_spec,
//                                  env);
//   TAO_CHECK_ENV_RETURN (env, CORBA::B_FALSE);
//   this->handle_postconnect ();
// }

