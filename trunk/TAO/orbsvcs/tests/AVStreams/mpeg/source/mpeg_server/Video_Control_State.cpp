// $Id$

#include "Video_Control_State.h"
#include "Video_Server.h"

ACE_RCSID(mpeg_server, Video_Control_State, "$Id$")

Video_Control_State::Video_Control_State ()
  : vci_ (VIDEO_CONTROL_I::instance ())
{
}

Video_Control_State::Video_States
Video_Control_State::get_state (void)
{
  return this->state_;
}

CORBA::Boolean 
Video_Control_State::init_video (const Video_Control::INITvideoPara &init_para,
                                 Video_Control::INITvideoReply_out reply)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Video_Control_State::init_video called\n"));
  int failureType = 0;
  // This is independent of the state and hence we implement it here
  
  // video file string
  for (int i=0;i<init_para.videofile.length (); i++)
    VIDEO_SINGLETON::instance ()->videoFile[i] = init_para.videofile[i];
  
  VIDEO_SINGLETON::instance ()->videoFile [init_para.videofile.length ()] = 0;

  if (Mpeg_Global::session_num > Mpeg_Global::session_limit || init_para.version != VERSION) {
    return 0;
    // ~~ We should be throwing exceptions here.
  }
  VIDEO_SINGLETON::instance ()->cmdsn = init_para.sn;
  /*
    fprintf (stderr, "MPEG file %s got.\n", VIDEO_SINGLETON::instance ()->videoFile);
    */
  if (!strncasecmp ("LiveVideo", VIDEO_SINGLETON::instance ()->videoFile, 9)) {
    if (OpenLiveVideo (&VIDEO_SINGLETON::instance ()->video_format, &VIDEO_SINGLETON::instance ()->horizontalSize,
                       &VIDEO_SINGLETON::instance ()->verticalSize, &VIDEO_SINGLETON::instance ()->averageFrameSize,
                       &VIDEO_SINGLETON::instance ()->fps, &VIDEO_SINGLETON::instance ()->pelAspectRatio) == -1) {
      failureType = 100;
      return 0;
    }
    if (VIDEO_SINGLETON::instance ()->video_format == VIDEO_MPEG2) {
      failureType = 101;
      return 0;
    }
    VIDEO_SINGLETON::instance ()->live_source = 1;

    VIDEO_SINGLETON::instance ()->fileSize =0x7fffffff;
    VIDEO_SINGLETON::instance ()->maxS = VIDEO_SINGLETON::instance ()->maxG = VIDEO_SINGLETON::instance ()->maxI = VIDEO_SINGLETON::instance ()->maxP = VIDEO_SINGLETON::instance ()->maxB = VIDEO_SINGLETON::instance ()->minS = VIDEO_SINGLETON::instance ()->minG = VIDEO_SINGLETON::instance ()->minI = VIDEO_SINGLETON::instance ()->minP = VIDEO_SINGLETON::instance ()->minB = 1;
    VIDEO_SINGLETON::instance ()->numS = VIDEO_SINGLETON::instance ()->numG = VIDEO_SINGLETON::instance ()->numF = VIDEO_SINGLETON::instance ()->numI = 0x7fffffff;
    VIDEO_SINGLETON::instance ()->numP = VIDEO_SINGLETON::instance ()->numB = 0;
    VIDEO_SINGLETON::instance ()->vbvBufferSize = 1;
    VIDEO_SINGLETON::instance ()->firstGopFrames = 1;
    VIDEO_SINGLETON::instance ()->patternSize = 1;
    VIDEO_SINGLETON::instance ()->pattern[0] = 'I';
    VIDEO_SINGLETON::instance ()->pattern[1] = 0;
    VIDEO_SINGLETON::instance ()->packetBufSize = VIDEO_SINGLETON::instance ()->verticalSize * VIDEO_SINGLETON::instance ()->horizontalSize * 3;
    VIDEO_SINGLETON::instance ()->packet = (VideoPacket *)malloc (sizeof (VideoMessage) + sizeof (VideoPacket) +
                                          VIDEO_SINGLETON::instance ()->packetBufSize);
    if (VIDEO_SINGLETON::instance ()->packet == NULL)	
      {
        perror ("Error: VS error on malloc VIDEO_SINGLETON::instance ()->packet buffer");
        exit (1);
      }
    VIDEO_SINGLETON::instance ()->packet = (VideoPacket *) ((char *)VIDEO_SINGLETON::instance ()->packet + sizeof (VideoMessage));

  }
  else {
    static double pictureRateTable[] = {23.976, 24, 25, 29.97, 30, 50, 59.94, 60};

    VIDEO_SINGLETON::instance ()->video_format = VIDEO_MPEG1;
    failureType = VIDEO_SINGLETON::instance ()->init_MPEG1_video_file ();
    if (failureType) 
      return 0;
    VIDEO_SINGLETON::instance ()->fps = pictureRateTable[VIDEO_SINGLETON::instance ()->pictureRate - 1];
  }

  // Set the Reply parameter values
  ACE_NEW_RETURN (reply,
                  Video_Control::INITvideoReply,
                  0);
 
  reply->totalHeaders = VIDEO_SINGLETON::instance ()->numS;
  reply->totalGroups = VIDEO_SINGLETON::instance ()->numG;
  reply->totalFrames = VIDEO_SINGLETON::instance ()->numF;
  reply->sizeIFrame = VIDEO_SINGLETON::instance ()->maxI;
  reply->sizePFrame = VIDEO_SINGLETON::instance ()->maxP;
  reply->sizeBFrame = VIDEO_SINGLETON::instance ()->maxB;
  reply->sizeSystemHeader = VIDEO_SINGLETON::instance ()->maxS;
  reply->sizeGop = VIDEO_SINGLETON::instance ()->maxG;
  reply->averageFrameSize = VIDEO_SINGLETON::instance ()->averageFrameSize;
  reply->verticalSize = VIDEO_SINGLETON::instance ()->verticalSize;
  reply->horizontalSize = VIDEO_SINGLETON::instance ()->horizontalSize;
  reply->pelAspectRatio = VIDEO_SINGLETON::instance ()->pelAspectRatio;
  reply->pictureRate1000 = (int) (VIDEO_SINGLETON::instance ()->fps * 1000);
  reply->vbvBufferSize = VIDEO_SINGLETON::instance ()->vbvBufferSize;
  reply->firstGopFrames = VIDEO_SINGLETON::instance ()->firstGopFrames;

  reply->pattern.length (VIDEO_SINGLETON::instance ()->patternSize);

  // copy the sequence of char..
  for ( i = 0; i < VIDEO_SINGLETON::instance ()->patternSize; i++)
    reply->pattern [i] = VIDEO_SINGLETON::instance ()->pattern [i];
  
  reply->live = VIDEO_SINGLETON::instance ()->live_source;
  reply->format = VIDEO_SINGLETON::instance ()->video_format;
  
    /* write the first SH, GOP and IFrame to VIDEO_SINGLETON::instance ()->serviceSocket (TCP),
       using code for SendPacket () */
  {
   
    //    ~~ He sends the first frame thru the connected TCP socket,
    // we replace it to send thru the UDP data socket instead!!
    if (VIDEO_SINGLETON::instance ()->live_source) StartPlayLiveVideo ();
      
    if (VIDEO_SINGLETON::instance ()->live_source) {
      int frame = 0;
      VIDEO_SINGLETON::instance ()->SendPicture (&frame);
    }
    else if (VIDEO_SINGLETON::instance ()->video_format == VIDEO_MPEG1) {
      VIDEO_SINGLETON::instance ()->SendPacket (1, 0, 0, 0, 1); //
      // make the first_time flag = 1, so that the packet gets sent
      // correctly using UDP
    }
    else {
      fprintf (stderr, "VS: VIDEO_SINGLETON::instance ()->video_format %d not supported.\n",
               VIDEO_SINGLETON::instance ()->video_format);
    }
    if (VIDEO_SINGLETON::instance ()->live_source) StopPlayLiveVideo ();
  }
  return 1;
}


CORBA::Boolean 
Video_Control_State::stat_stream (CORBA::Char_out ch,
                              CORBA::Long_out size)
                              
{
  return 0;
}


void
Video_Control_State::close (void)
{
  return;
}


CORBA::Boolean 
Video_Control_State::stat_sent (void)
{
  return 0;
}


CORBA::Boolean 
Video_Control_State::fast_forward (const Video_Control::FFpara &para)
                               
{
  return 0;
}


CORBA::Boolean 
Video_Control_State::fast_backward (const Video_Control::FFpara &para)
                                
{
  return 0;
}


CORBA::Boolean 
Video_Control_State::step (const Video_Control::STEPpara &para)
                       
{
  return 0;
}


CORBA::Boolean 
Video_Control_State::play (const Video_Control::PLAYpara &para,
                       CORBA::Long_out vts)
                       
{
  return 0;
}


CORBA::Boolean 
Video_Control_State::position (const Video_Control::POSITIONpara &para)
                           
{
  return 0;
}


CORBA::Boolean 
Video_Control_State::speed (const Video_Control::SPEEDpara &para)
                        
{
  return 0;
}


CORBA::Boolean 
Video_Control_State::stop (CORBA::Long cmdsn)
                       
{
  return 0;
}

// ----------------------------------------------------------------------

Video_Control_Waiting_State::Video_Control_Waiting_State (void)
{
  //%%
  this->state_ = VIDEO_WAITING;
}

CORBA::Boolean 
Video_Control_Waiting_State::stat_stream (CORBA::Char_out ch,
                              CORBA::Long_out size)
                              
{
  return 0;
}

void
Video_Control_Waiting_State::close (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "Video_Control_Waiting_State::close \n"));
  VIDEO_SINGLETON::instance ()->normalExit = 1;
  TAO_ORB_Core_instance ()->reactor ()->end_event_loop ();
  return ;
}


CORBA::Boolean 
Video_Control_Waiting_State::stat_sent (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "Video_Control_Waiting_State::stat_sent \n"));
  return 0;
}

// We are in the waiting state - and the client sent us
// the command fast-forward. 
CORBA::Boolean 
Video_Control_Waiting_State::fast_forward (const Video_Control::FFpara &para)
                               
{
  // Many guys in legacy code depend on this variable.
  VIDEO_SINGLETON::instance ()-> cmd = CmdFF;
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Video_Control_Waiting_State::fast_forward () called\n"));
  VIDEO_SINGLETON::instance ()->init_fast_play (para);
  this->vci_->change_state (VIDEO_CONTROL_FAST_FORWARD_STATE::instance ());
  return 1;
}


// We are in the waiting state - and the client sent us
// the command fast-backward. 
CORBA::Boolean 
Video_Control_Waiting_State::fast_backward (const Video_Control::FFpara &para)
                                
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Video_Control_Waiting_State::fast_backward () called\n"));
  // Many guys in legacy code depend on this variable.
  VIDEO_SINGLETON::instance ()-> cmd = CmdFB;
  VIDEO_SINGLETON::instance ()->init_fast_play (para);
  this->vci_->change_state (VIDEO_CONTROL_FAST_BACKWARD_STATE::instance ());
  return 1;
}


CORBA::Boolean 
Video_Control_Waiting_State::step (const Video_Control::STEPpara &para)
                       
{
  Video_Control::STEPpara step_para = para;
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Video_Control_Waiting_State::step () called\n"));

  int group;
  int tag = 0;
  int result;

  VIDEO_SINGLETON::instance ()->cmdsn = step_para.sn;

  if (!VIDEO_SINGLETON::instance ()-> live_source) {
    if (step_para.nextFrame >= VIDEO_SINGLETON::instance ()->numF) /* send SEQ_END */
      {
        tag = 1;
        step_para.nextFrame --;
      }
    /*
      fprintf (stderr, "STEP . . .frame-%d\n", step_para.this->nextFrame);
    */
    CheckFrameRange (step_para.nextFrame);
    group = VIDEO_SINGLETON::instance ()->FrameToGroup (&step_para.nextFrame);
    if (VIDEO_SINGLETON::instance ()-> precmd != CmdSTEP && !tag ) {
      result = VIDEO_SINGLETON::instance ()->SendReferences (group, step_para.nextFrame);
      if (result < 0 )
        return 0;
    }
  }
  if (VIDEO_SINGLETON::instance ()->live_source) 
    StartPlayLiveVideo ();
 
  if (VIDEO_SINGLETON::instance ()->live_source) {
    VIDEO_SINGLETON::instance ()->SendPicture (&step_para.nextFrame);
  }
  else if (VIDEO_SINGLETON::instance ()->video_format == VIDEO_MPEG1) {
    VIDEO_SINGLETON::instance ()->SendPacket (VIDEO_SINGLETON::instance ()->numS>1, group, tag ? VIDEO_SINGLETON::instance ()->numF : step_para.nextFrame, 0);
  }
  else {
    fprintf (stderr, "VS: wierd1\n");
  }
 
  if (VIDEO_SINGLETON::instance ()->live_source) StopPlayLiveVideo ();
  return 1;
}


CORBA::Boolean 
Video_Control_Waiting_State::play (const Video_Control::PLAYpara &para,
                                   CORBA::Long_out vts)
                       
{
  // Many guys in legacy code depend on this variable.
  VIDEO_SINGLETON::instance ()-> cmd = CmdPLAY;
  //ACE_DEBUG ((LM_DEBUG,
  //            "(%P|%t)Video_Control_Waiting_State::play () called \n"));
  VIDEO_SINGLETON::instance ()->init_play (para,
                                           vts);
  //  cerr << "vts is " << vts << endl;
  this->vci_->change_state (VIDEO_CONTROL_PLAY_STATE::instance ());
  return 1;
}


CORBA::Boolean 
Video_Control_Waiting_State::position (const Video_Control::POSITIONpara &pos_para)
                           
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Video_Control_Waiting_State::position () called \n"));

  if (VIDEO_SINGLETON::instance ()->live_source) 
    return 1;

  CheckGroupRange (pos_para.nextGroup);
  VIDEO_SINGLETON::instance ()-> cmd = CmdPOSITION;
  VIDEO_SINGLETON::instance ()-> cmdsn = pos_para.sn;

  int result = VIDEO_SINGLETON::instance ()->SendPacket 
    (VIDEO_SINGLETON::instance () ->numS > 1 
     || pos_para.nextGroup == 0, 
     pos_para.nextGroup, 0, 0);
  
  return 1;
}


CORBA::Boolean 
Video_Control_Waiting_State::speed (const Video_Control::SPEEDpara &para)
                        
{
  return 0;
}


CORBA::Boolean 
Video_Control_Waiting_State::stop (CORBA::Long cmdsn)
                       
{
  ACE_DEBUG ((LM_DEBUG,
              "Video_Control_Waiting_State::stop ()\n"));
  VIDEO_SINGLETON::instance ()->cmd = CmdSTOP;
  VIDEO_SINGLETON::instance ()->cmdsn = cmdsn;
  Video_Timer_Global::StopTimer();
  this->vci_->change_state (VIDEO_CONTROL_WAITING_STATE::instance ());
  return 1;
}

// Video_Control_Play_State methods

Video_Control_Play_State::Video_Control_Play_State (void)
{
  this->state_ = VIDEO_PLAY;
}

CORBA::Boolean
Video_Control_Play_State::stop (CORBA::Long cmdsn)
{
  ACE_DEBUG ((LM_DEBUG,
              "Video_Control_Play_State::stop ()\n"));
  VIDEO_SINGLETON::instance ()->cmd = CmdSTOP;
  VIDEO_SINGLETON::instance ()->cmdsn = cmdsn;
  Video_Timer_Global::StopTimer();
  this->vci_->change_state (VIDEO_CONTROL_WAITING_STATE::instance ());
  return 1;
}


CORBA::Boolean
Video_Control_Play_State::speed (const Video_Control::SPEEDpara &para)
{
  ACE_DEBUG ((LM_DEBUG,
              "Video_Control_Play_State::speed ()\n"));
  VIDEO_SINGLETON::instance ()->frameRateLimit = para.frameRateLimit1000 / 1000.0;
  VIDEO_SINGLETON::instance ()->sendPatternGops = para.sendPatternGops;
  VIDEO_SINGLETON::instance ()->currentUPF = para.usecPerFrame;
  VIDEO_SINGLETON::instance ()->addedUPF = 0;

  for (int i=0; i<para.sendPattern.length (); i++)
    VIDEO_SINGLETON::instance ()->sendPattern [i] = para.sendPattern [i];
  //  memcpy(VIDEO_SINGLETON::instance ()->sendPattern, para.sendPattern, PATTERN_SIZE);

  Video_Timer_Global::TimerSpeed ();
  VIDEO_SINGLETON::instance ()->play_send ();
  return 1;
}


Video_Control_Fast_Forward_State::Video_Control_Fast_Forward_State (void)
{
  this->state_ = VIDEO_FAST_FORWARD;
}

CORBA::Boolean
Video_Control_Fast_Forward_State::stop (CORBA::Long cmdsn)
{
  ACE_DEBUG ((LM_DEBUG,
              "Video_Control_Fast_Forward_State::stop ()\n"));
  VIDEO_SINGLETON::instance ()->cmd = CmdSTOP;
  VIDEO_SINGLETON::instance ()->cmdsn = cmdsn;
  Video_Timer_Global::StopTimer();
  this->vci_->change_state (VIDEO_CONTROL_WAITING_STATE::instance ());
  return 1;
}

void
Video_Control_Fast_Forward_State::close (void)
{
  return;
}

// ----------------------------------------------------------------------

Video_Control_Fast_Backward_State::Video_Control_Fast_Backward_State (void)
{
  this->state_ = VIDEO_FAST_BACKWARD;
}

CORBA::Boolean
Video_Control_Fast_Backward_State::stop (CORBA::Long cmdsn)
{
  ACE_DEBUG ((LM_DEBUG,
              "Video_Control_Fast_Backward_State::stop ()\n"));
  VIDEO_SINGLETON::instance ()->cmd = CmdSTOP;
  VIDEO_SINGLETON::instance ()->cmdsn = cmdsn;
  Video_Timer_Global::StopTimer();
  this->vci_->change_state (VIDEO_CONTROL_WAITING_STATE::instance ());
  return 1;
}

void
Video_Control_Fast_Backward_State::close (void)
{
  return;
}
