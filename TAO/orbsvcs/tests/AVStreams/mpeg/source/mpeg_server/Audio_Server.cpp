// $Id$
#include "Audio_Server.h"

Audio_Global::Audio_Global (void)
  :state (AUDIO_WAITING),
   addSamples (0),
   nextTime (0),
   upp (0),
   delta_sps (0),
   bytes_sent (0),
   start_time (0),
   conn_tag (0),
   serviceSocket (-1),
   audioSocket (-1),
   fd (0),
   totalSamples (0),
   fileSize (0),
   cmd (0),
   live_source (0),
   databuf_size (0),
   cmdsn (0),
   nextsample (0),
   sps (0),
   spslimit (0),
   spp (0),
   pktbuf (0),
   fbpara (0)
{
}

int 
Audio_Global::CmdRead(char *buf, int psize)
{
  int res = wait_read_bytes(serviceSocket, buf, psize);
  if (res == 0) return (1);
  if (res == -1) {
    fprintf(stderr, "AS error on read cmdSocket, size %d", psize);
    perror("");
    return (-1);
  }
  return 0;
}

void
Audio_Global::CmdWrite(char *buf, int size)
{
  int res = wait_write_bytes(serviceSocket, buf, size);
  if (res == -1) {
    if (errno != EPIPE) perror("AS writes to serviceSocket");
    exit(errno != EPIPE);
  }
}

int 
Audio_Global::INITaudio(void)
{
  int result;
  int failureType; /* 0 - can't open file, 1 - can't open live source */
  INITaudioPara para;

  result = CmdRead((char *)&para, sizeof(para));
  if (result != 0)
    return result;
#ifdef NeedByteOrderConversion
  para.sn = ntohl(para.sn);
  para.version = ntohl(para.version);
  para.nameLength = ntohl(para.nameLength);
  para.para.encodeType = ntohl(para.para.encodeType);
  para.para.channels = ntohl(para.para.channels);
  para.para.samplesPerSecond = ntohl(para.para.samplesPerSecond);
  para.para.bytesPerSample = ntohl(para.para.bytesPerSample);
#endif
  if (para.nameLength>0)
    result = CmdRead(audioFile, para.nameLength);
  if (result != 0)
    return result;
  if (Mpeg_Global::session_num > Mpeg_Global::session_limit || para.version != VERSION) {
    char errmsg[128];
    cmd = CmdFAIL;
    CmdWrite((char *)&cmd, 1);
    if (Mpeg_Global::session_num > Mpeg_Global::session_limit) {
      sprintf(errmsg,
	      "Too many sessions being serviced, please try again later.\n");
    }
    else {
      sprintf(errmsg, "Version # not match, AS %d.%02d, Client %d.%02d",
	      VERSION / 100, VERSION % 100,
	      para.version / 100, para.version % 100);
    }
    write_string(serviceSocket, errmsg);
    return(1);
  }
  memcpy(&audioPara, &para.para, sizeof(audioPara));
  /*
  fprintf(stderr, "Client Audio para: encode %d, ch %d, sps %d, bps %d.\n",
	  para.para.encodeType, para.para.channels,
	  para.para.samplesPerSecond, para.para.bytesPerSample);
   */
  audioFile[para.nameLength] = 0;
  {
    int len = strlen(audioFile);
    if (strncasecmp("LiveAudio", audioFile, 9) &&
	strcasecmp(".au", audioFile+len-3)) {
      char errmsg[128];
      cmd = CmdFAIL;
      CmdWrite((char *)&cmd, 1);
      sprintf(errmsg, "%s without suffix .au", audioFile);
      write_string(serviceSocket, errmsg);
      return(1);
    }
  }
  /*
  fprintf(stderr, "Audio file %s got.\n", audioFile);
  */

  if (!strncasecmp("LiveAudio", audioFile, 9)) {
    fd = OpenLiveAudio(&(para.para));
    if (fd == -1) {
      failureType = 1;
      goto failure;
    }
    fileSize =0x7fffffff;
    totalSamples = fileSize / audioPara.bytesPerSample;
    live_source = 1;
  }
  else {
    LeaveLiveAudio();
    fd = open(audioFile, O_RDONLY);
    if (fd == -1)
    {
      fprintf(stderr, "AS error on opening audio file %s", audioFile);
      perror("");
      failureType = 0;
      goto failure;
    }

    /* Try to get audioFile format audioPara here */

    /* figure out totalsamples */
    fileSize = lseek(fd, 0L, SEEK_END);
    lseek(fd, 0L, SEEK_SET);
    totalSamples = fileSize / audioPara.bytesPerSample;
    /*
    fprintf(stderr, "Total Samples=%d in audio file %ss.\n", totalSamples, audioFile);
    */
  }
  {
    INITaudioReply reply;
    
    reply.para.encodeType = htonl(audioPara.encodeType);
    reply.para.channels = htonl(audioPara.channels);
    reply.para.samplesPerSecond = htonl(audioPara.samplesPerSecond);
    reply.para.bytesPerSample = htonl(audioPara.bytesPerSample);
    reply.totalSamples = htonl(totalSamples);

    reply.live = htonl(live_source);
    reply.format = htonl(AUDIO_RAW);
    
    CmdWrite((char *)&cmd, 1);
    CmdWrite((char *)&reply, sizeof(reply));
  }
  return 0;
  
 failure:
  {
    /*
    fprintf(stderr, "AS error: failed initializing audio file.\n");
    */
    cmd = CmdFAIL;
    CmdWrite((char *)&cmd, 1);
    write_string(serviceSocket,
		 failureType == 0 ? "Failed to open audio file for read." :
		 "Failed to connect to live audio source.");
    return(1);
  }
}

/* send a packet of audio samples to audioSocket
   returns: 0 - no more data from audio file: EOF reached;
            1 - More data is available from the audio file */
int
Audio_Global::send_packet(int firstSample, int samples)
{
  //  ACE_DEBUG ((LM_DEBUG,"(%P|%t) send_packet called\n"));
  long offset = firstSample * audioPara.bytesPerSample;
  int size = samples * audioPara.bytesPerSample;
  char * buf = (char *)pktbuf + sizeof(*pktbuf);
  int len;
  int resent = 0;
  int segsize, sentsize;

  if (live_source) {
    len = ReadLiveAudioSamples(buf, samples);
    len *= audioPara.bytesPerSample;
  }
  else {
    lseek(fd, offset, SEEK_SET);
    while ((len = read(fd, buf, size)) == -1) {
      if (errno == EINTR)
	continue;   /* interrupted */
      perror("AS error on read audio file");
      return(-1);
    }
    if (len < audioPara.bytesPerSample) {
      return 0;
    }
  }
  
  samples = len / audioPara.bytesPerSample;
  len = samples * audioPara.bytesPerSample;
  bytes_sent += len;
  pktbuf->firstSample = htonl(firstSample);
  pktbuf->samples = htonl(samples);
  pktbuf->actualSamples = htonl(samples);
  pktbuf->dataBytes = htonl(len);
  if (spslimit < sps) { /* interpolation needed */
    SFprintf(stderr, "AS audio sample interpolation not available yet.\n");
  }
  segsize = sizeof(*pktbuf) + len;
  if (conn_tag != 0) {
    while ((sentsize = write(audioSocket, (char *)pktbuf, segsize)) == -1) {
      if (errno == EINTR) /* interrupted */
	continue;
      if (errno == ENOBUFS) {
	if (resent) {
	  perror("AS Warning, pkt discarded because");
	  break;
	}
	else {
	  resent = 1;
	  usleep(5000);
	  continue;
	}
      }
      if (errno != EPIPE) {
	fprintf(stderr, "AS error on send audio packet %d(%d):",
		firstSample, samples);
	perror("");
      }
      exit((errno != EPIPE));
    }
  }
  else {
    sentsize = wait_write_bytes(audioSocket, (char *)pktbuf, segsize);
    if (sentsize == -1) {
      if (errno != EPIPE) {
	fprintf(stderr, "AS error on send audio packet %d(%d):",
		firstSample, samples);
	perror("");
      }
      exit((errno != EPIPE));
     }
  }
  if (sentsize < segsize) {
    SFprintf(stderr, "AS warning: message size %dB, sent only %dB\n",
	    segsize, sentsize);
  }
  /*
  SFprintf(stderr, "AS sent audio packet %d(%d).\n",
	  firstSample, samples);
  */
  return (len < size ? 0 : 1);
}

/* send a packet of audio samples to audioSocket
   returns: 0 - no more data from audio file: EOF reached;
            1 - More data is available from the audio file */
int
Audio_Global::SendPacket(void)
{
  int moredata;
  pktbuf->cmdsn = htonl(cmdsn);
  pktbuf->resend = htonl(0);
  pktbuf->samplesPerSecond = htonl(sps);
  moredata = send_packet(nextsample, spp);
  if (moredata) {
    nextsample += spp;
  }
  return moredata;
}

void
Audio_Global::ResendPacket(int firstsample, int samples)
{
  pktbuf->cmdsn = htonl(cmdsn);
  pktbuf->resend = htonl(1);
  pktbuf->samplesPerSecond = htonl(sps);
  while (samples > 0) {
    int size = samples < spp ? samples : spp;
    send_packet(firstsample, size);
    firstsample += size;
    samples -= size;
    if (samples > 0) {
      usleep(10000);
    }
  }
}

#if 0
int
Audio_Global::PLAYaudio(void)
{
  int hasdata = 1;
  int addSamples;
  int packets = 0;
  unsigned nextTime;
  int upp;  /* micro-seconds per packet */
  int delta_sps = 0;  /* compensation for sps from feedback msgs */
  int nfds = (serviceSocket > audioSocket ? serviceSocket : audioSocket) + 1;
  int result;
  /*
  fprintf(stderr, "PLAY . . .\n");
  */
  {
    PLAYaudioPara para;
    result = CmdRead((char *)&para, sizeof(para));
    if (result != 0)
      return result;
#ifdef NeedByteOrderConversion
    para.sn = ntohl(para.sn);
    para.nextSample = ntohl(para.nextSample);
    para.samplesPerSecond = ntohl(para.samplesPerSecond);
    para.samplesPerPacket = ntohl(para.samplesPerPacket);
    para.ABsamples = ntohl(para.ABsamples);
    para.spslimit = ntohl(para.spslimit);
#endif
    nextsample = para.nextSample;
    cmdsn = para.sn;
    sps = para.samplesPerSecond;
    spslimit = para.spslimit;
    spp = para.samplesPerPacket;
    addSamples = para.ABsamples / 2;
    if (spp * audioPara.bytesPerSample > databuf_size) {
      spp = databuf_size / audioPara.bytesPerSample;
    }
    /*
    SFprintf(stderr, "AS got CmdPLAY: sps %d\n", sps);
    */
  }
  /*
  fprintf(stderr, "AS: nextSampe=%d for PLAY.\n", para.nextSample);
  */

  upp = (int)(1000000.0 / ((double)sps / (double)spp));
  nextTime = get_usec();
  
  CmdWrite((char *)&nextTime, sizeof(int));

  if (live_source) {
    StartPlayLiveAudio();
  }
  
  for (;;)
  {
    struct fd_set read_mask, write_mask;
    struct timeval tval;
    unsigned curTime = get_usec();
    
    if (hasdata) {
      if (addSamples < - spp) {  /* slow down by not sending packets */
	nextTime += upp;
	addSamples += spp;
      }
      else {
	int need_sleep = 0;
	while (nextTime <= curTime && hasdata) {
	  if (need_sleep) usleep(5000);
	  hasdata = SendPacket();
	  need_sleep = 1;
	  packets ++;
	  nextTime += upp;
	  if (addSamples > 0 && packets % SPEEDUP_SCALE == 0) {
	    addSamples -= spp;
	    usleep(5000);
	    hasdata = SendPacket();
	    packets ++;
	  }
	}
      }
    }
    curTime = nextTime - curTime;
    if (curTime > 5000000) curTime = 5000000; /* limit on 5 second weit time
						 in case error happens */
    tval.tv_sec = curTime / 1000000;
    tval.tv_usec = curTime % 1000000;
    FD_ZERO(&read_mask);
    FD_SET(serviceSocket, &read_mask);
    FD_SET(audioSocket, &read_mask);
#ifdef _HPUX_SOURCE
    if (select(nfds, (int *)&read_mask, NULL, NULL, hasdata ? &tval : NULL) == -1)
#else
    if (select(nfds, &read_mask, NULL, NULL, hasdata ? &tval : NULL) == -1)
#endif
    {
      if (errno == EINTR)
        continue;
      perror("AS error on select reading or writing");
      return(-1);
    }
    if (FD_ISSET(serviceSocket, &read_mask)){  /* STOP, SPEED, or CLOSE*/
      unsigned char tmp;
      result = CmdRead((char *)&tmp, 1);
      if (result != 0)
        return result;
      switch (tmp)
      {
      case CmdSPEED:
	{
	  SPEEDaudioPara para;
	  result = CmdRead((char *)&para, sizeof(para));
          if (result != 0)
            return result;
#ifdef NeedByteOrderConversion
	  para.sn = ntohl(para.sn);
	  para.samplesPerSecond = ntohl(para.samplesPerSecond);
	  para.samplesPerPacket = ntohl(para.samplesPerPacket);
	  para.spslimit = ntohl(para.spslimit);
#endif
	  sps  = para.samplesPerSecond;
	  spslimit = para.spslimit;
	  spp = para.samplesPerPacket;
	  if (spp * audioPara.bytesPerSample > databuf_size) {
	    spp = databuf_size / audioPara.bytesPerSample;
	  }
	  delta_sps = 0;  /* reset compensation value */
	  upp = (int)(1000000.0 / ((double)sps / (double)spp));
	  /*
	  SFprintf(stderr, "AS got CmdSPEED: sps %d\n", sps);
	  */
	}
	break;
      case CmdSTOP:
	{
	  int val;
	  cmd = tmp;
	  /*
	     fprintf(stderr, "AS: CmdSTOP. . .\n");
	     */
	  result = CmdRead((char *)&val, sizeof(int));
          if (result != 0)
            return result;
	  /*
	  CmdWrite(AUDIO_STOP_PATTERN, strlen(AUDIO_STOP_PATTERN));
	  */
	  if (live_source) {
	    StopPlayLiveAudio();
	  }
	  return 0;  /* return from PLAYaudio() */
	}
      case CmdCLOSE:
	if (live_source) {
	  StopPlayLiveAudio();
	}
	return(1);  /* The whole AS session terminates */
      default:
	if (live_source) {
	  StopPlayLiveAudio();
	}
	fprintf(stderr, "AS error: cmd=%d while expects STOP/SPEED/CLOSE.\n", tmp);
	return(-1);
      }
    }
    
    if (FD_ISSET(audioSocket, &read_mask)){  /* Feedback packet */
      int bytes, len;
      for (;;) {
	if (conn_tag >= 0) {
	  len = wait_read_bytes(audioSocket, (char *)fbpara, sizeof(*fbpara));
	  if (len == 0) return(1); /* connection broken */
	  else if (len < 0) { /* unexpected error */
	    perror("AS read1 FB");
	    return(-1);
	  }
	}
	else { /* discard mode packet stream, read the whole packet */
	  len = read(audioSocket, (char *)fbpara,  FBBUF_SIZE);
	}
	if (len == -1) {
	  if (errno == EINTR) continue; /* interrupt */
	  else {
	    if (errno != EPIPE && errno != ECONNRESET) perror("AS failed to read() fbmsg header");
	    break;
	  }
	}
	break;
      }
      if (len < sizeof(*fbpara)) {
	if (len > 0) fprintf(stderr,
			 "AS warn read() len %dB < sizeof(*fbpara) %dB\n",
			 len, sizeof(*fbpara));
	continue;
      }
#ifdef NeedByteOrderConversion
      fbpara->type = ntohl(fbpara->type);
#endif
      bytes = (fbpara->type > 0) ?
              sizeof(APdescriptor) * (fbpara->type - 1) :
              0;
      if (bytes > 0) {
	if (conn_tag >= 0) { /* not discard mode packet stream,
				read the rest of packet */
	  len = wait_read_bytes(audioSocket,
				((char *)fbpara) + sizeof(*fbpara),
				bytes);
	  if (len == 0) return(1); /* connection broken */
	  else if (len < 0) { /* unexpected error */
	    perror("AS read2 FB");
	    return(-1);
	  }
	  len += sizeof(*fbpara);
	}
      }
      bytes += sizeof(*fbpara);
      if (len < bytes) {
	if (len > 0) fprintf(stderr,
			     "AS only read partial FBpacket, %dB out of %dB.\n",
			     len, bytes);
	continue;
      }
      if (live_source) {  /* ignore all feedback messags for live source */
	continue;
      }
      
#ifdef NeedByteOrderConversion
      fbpara->cmdsn = ntohl(fbpara->cmdsn);
#endif
      if (len != sizeof(*fbpara) +
	  (fbpara->type ? (fbpara->type -1) * sizeof(APdescriptor) : 0)) {
	/* unknown message, discard */
	SFprintf(stderr, "AS Unkown fb msg: len = %d, type = %d\n",
		len, fbpara->type);
	continue;
      }
      if (fbpara->cmdsn != cmdsn) {  /* discard the outdated message */
	continue;
      }
#ifdef NeedByteOrderConversion
      {
	int i, * ptr = (int *)fbpara + 2;
	for (i = 0; i < (len >> 2) - 2; i++) *ptr = ntohl(*ptr);
      }
#endif
      if (fbpara->type == 0) { /* feedback message */
	/*
	SFprintf(stderr, "AS got fbmsg: addsamples %d, addsps %d\n",
		fbpara->data.fb.addSamples, fbpara->data.fb.addsps);
	*/
	addSamples += fbpara->data.fb.addSamples;
	if (fbpara->data.fb.addsps) {
	  delta_sps += fbpara->data.fb.addsps;
	  upp = (int)(1000000.0 / ((double)(sps + delta_sps) / (double)spp));
	}
      }
      else { /* resend requests */
	APdescriptor * req = &(fbpara->data.ap);
	int i;
	/*
	SFprintf(stderr, "AS got %d resend reqs\n", fbpara->type);
	*/
	for (i = 0; i < fbpara->type; i ++) {
	  ResendPacket(req->firstSample, req->samples);
	  req ++;
	}
      }
    }
  }
}
#endif

// our version of play audio.
int
Audio_Global::play_audio(void)
{
  int result;
 
  ACE_DEBUG ((LM_DEBUG,"(%P|%t) play_audio () called \n"));
 
  {
    PLAYaudioPara para;
    result = CmdRead((char *)&para, sizeof(para));
    if (result != 0)
      return result;
#ifdef NeedByteOrderConversion
    para.sn = ntohl(para.sn);
    para.nextSample = ntohl(para.nextSample);
    para.samplesPerSecond = ntohl(para.samplesPerSecond);
    para.samplesPerPacket = ntohl(para.samplesPerPacket);
    para.ABsamples = ntohl(para.ABsamples);
    para.spslimit = ntohl(para.spslimit);
#endif
    nextsample = para.nextSample;
    cmdsn = para.sn;
    sps = para.samplesPerSecond;
    spslimit = para.spslimit;
    spp = para.samplesPerPacket;
    addSamples = para.ABsamples / 2;
    if (spp * audioPara.bytesPerSample > databuf_size) {
      spp = databuf_size / audioPara.bytesPerSample;
    }
    /*
      SFprintf(stderr, "AS got CmdPLAY: sps %d\n", sps);
    */
  }
  /*
  fprintf(stderr, "AS: nextSampe=%d for PLAY.\n", para.nextSample);
  */

  upp = (int)(1000000.0 / ((double)sps / (double)spp));
  nextTime = get_usec();
  
  CmdWrite((char *)&nextTime, sizeof(int));
  if (live_source) {
    StartPlayLiveAudio();
  }
  this->send_audio ();
}

int
Audio_Global::send_audio (void)
{
  static int packets = 0;
  static int hasdata = 1;
  static struct timeval tval;
  unsigned curTime = get_usec();
    
  if (hasdata) {
    if (addSamples < - spp) {  /* slow down by not sending packets */
      /*      ACE_DEBUG ((LM_DEBUG,"(%P|%t) slow down by not sending\n")); */
      nextTime += upp;
      addSamples += spp;
    }
    else {
      /*      ACE_DEBUG ((LM_DEBUG,"(%P|%t) sending."
                  "nexttime = %d, curTime = %d, hasdata = %d\n",
                  nextTime, curTime, hasdata)); */
      int need_sleep = 0;
      while ( (nextTime <= curTime) && (hasdata)) {
        if (need_sleep) usleep(5000);
        hasdata = SendPacket();
        need_sleep = 1;
        packets ++;
        nextTime += upp;
        if (addSamples > 0 && packets % SPEEDUP_SCALE == 0) {
          addSamples -= spp;
          usleep(5000);
          hasdata = SendPacket();
          packets ++;
        }
      }
    }
  }
  curTime = nextTime - curTime;
  if (curTime > 5000000) curTime = 5000000; /* limit on 5 second weit time
                                               in case error happens */
  tval.tv_sec = curTime / 1000000;
  tval.tv_usec = curTime % 1000000;
  
  if (hasdata)
    {
      // schedule a sigalrm to simulate select timeout.
      ACE_Time_Value tv (tval);
      ACE_OS::ualarm (tv,0);
    }
  return 0;
}


void 
Audio_Global::on_exit_routine(void)
{
  struct sockaddr_in peeraddr_in;
  int size = sizeof(peeraddr_in);
  
  /*
  fprintf(stderr, "An AS session terminated\n");
  */
  if (getpeername(serviceSocket,
		  (struct sockaddr *)&peeraddr_in, &size) == 0 &&
      peeraddr_in.sin_family == AF_INET) {
    if (strncmp(inet_ntoa(peeraddr_in.sin_addr), "129.95.50", 9)) {
      struct hostent *hp;
      time_t val = time(NULL);
      char * buf = ctime(&start_time);

      hp = gethostbyaddr((char *)&(peeraddr_in.sin_addr), 4, AF_INET);
      buf[strlen(buf)-1] = 0;
      printf("%s: %s %3dm%02ds %dB %s\n",
	     buf,
	     hp == NULL ? inet_ntoa(peeraddr_in.sin_addr) : hp->h_name,
	     (val - start_time) / 60, (val - start_time) % 60,
	     bytes_sent, audioFile);
    }
  }
  ComCloseConn(serviceSocket);
  ComCloseConn(audioSocket);
}
//----------------------------------------
// Audio_Sig_Handler methods.

Audio_Sig_Handler::Audio_Sig_Handler (Audio_Global *audio_global)
  :audio_global_ (audio_global)
{
}

int
Audio_Sig_Handler::register_handler (void)
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

  //  sig_set.sig_add (SIGINT);
  // sig_set.sig_add (SIGQUIT);
  sig_set.sig_add (SIGALRM);  

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
Audio_Sig_Handler::get_handle (void) const
{
  return this->handle_;
}

int 
Audio_Sig_Handler::handle_input (ACE_HANDLE)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) handling asynchonrous input...\n"));
  return 0;
}

int 
Audio_Sig_Handler::shutdown (ACE_HANDLE, ACE_Reactor_Mask)
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
Audio_Sig_Handler::handle_signal (int signum, siginfo_t *, ucontext_t *)
{
  //  ACE_DEBUG ((LM_DEBUG, "(%t) received signal %S\n", signum));

  switch (signum)
    {
    case SIGALRM:
      if (this->audio_global_->state == Audio_Global::AUDIO_PLAY)
        this->audio_global_->send_audio ();// honor the signal only if you're in play state.
      break;
    default: 
      ACE_DEBUG ((LM_DEBUG, 
		  "(%t) %S: not handled, returning to program\n", 
                  signum));
      break;
    }
  //  ACE_DEBUG ((LM_DEBUG,"returning from handle_signal"));
  return 0;
}

//----------------------------------------------
// Audio_Data_Handler methods

Audio_Data_Handler::Audio_Data_Handler (ACE_HANDLE data_fd,
                                        Audio_Global *audio_global)
  :data_fd_ (data_fd),
   audio_global_ (audio_global)
{
}

ACE_HANDLE
Audio_Data_Handler::get_handle (void) const
{
  return this->data_fd_;
}

int
Audio_Data_Handler::handle_input (ACE_HANDLE fd)
{
  ACE_DEBUG ((LM_DEBUG,"(%P|%t)Audio_Data_Handler::handle_input ()\n"));
  int bytes, len;
  for (;;) {
    if (this->audio_global_->conn_tag >= 0) {
      len = wait_read_bytes(this->audio_global_->audioSocket, (char *)this->audio_global_->fbpara, sizeof(*(this->audio_global_->fbpara)));
      if (len == 0) return(1); /* connection broken */
      else if (len < 0) { /* unexpected error */
        perror("AS read1 FB");
        return(-1);
      }
    }
    else { /* discard mode packet stream, read the whole packet */
      len = ::read(this->audio_global_->audioSocket, (char *)this->audio_global_->fbpara,  FBBUF_SIZE);
    }
    if (len == -1) {
      if (errno == EINTR) continue; /* interrupt */
      else {
        if (errno != EPIPE && errno != ECONNRESET) perror("AS failed to read() fbmsg header");
        break;
      }
    }
    break;
  }
  if (len < sizeof(*this->audio_global_->fbpara)) {
    if (len > 0) fprintf(stderr,
			 "AS warn read() len %dB < sizeof(*this->audio_global_->fbpara) %dB\n",
			 len, sizeof(*this->audio_global_->fbpara));
    // continue;
    // simulate the continue ??
    this->audio_global_->send_audio ();
  }
#ifdef NeedByteOrderConversion
  this->audio_global_->fbpara->type = ntohl(this->audio_global_->fbpara->type);
#endif
  bytes = (this->audio_global_->fbpara->type > 0) ?
    sizeof(APdescriptor) * (this->audio_global_->fbpara->type - 1) :
    0;
  if (bytes > 0) {
    if (this->audio_global_->conn_tag >= 0) { /* not discard mode packet stream,
                            read the rest of packet */
      len = wait_read_bytes(this->audio_global_->audioSocket,
                            ((char *)this->audio_global_->fbpara) + sizeof(*this->audio_global_->fbpara),
                            bytes);
      if (len == 0) return(1); /* connection broken */
      else if (len < 0) { /* unexpected error */
        perror("AS read2 FB");
        return(-1);
      }
      len += sizeof(*this->audio_global_->fbpara);
    }
  }
  bytes += sizeof(*this->audio_global_->fbpara);
  if (len < bytes) {
    if (len > 0) fprintf(stderr,
                         "AS only read partial FBpacket, %dB out of %dB.\n",
                         len, bytes);
    //    continue;
    this->audio_global_->send_audio ();
  }
  if (this->audio_global_->live_source) {  /* ignore all feedback messags for live source */
    //    continue;
    this->audio_global_->send_audio ();
  }
      
#ifdef NeedByteOrderConversion
  this->audio_global_->fbpara->cmdsn = ntohl(this->audio_global_->fbpara->cmdsn);
#endif
  if (len != sizeof(*this->audio_global_->fbpara) +
      (this->audio_global_->fbpara->type ? (this->audio_global_->fbpara->type -1) * sizeof(APdescriptor) : 0)) {
    /* unknown message, discard */
    SFprintf(stderr, "AS Unkown fb msg: len = %d, type = %d\n",
             len, this->audio_global_->fbpara->type);
    //    continue;
    this->audio_global_->send_audio ();
  }
  if (this->audio_global_->fbpara->cmdsn != this->audio_global_->cmdsn) {  /* discard the outdated message */
    //    continue;
    this->audio_global_->send_audio ();
  }
#ifdef NeedByteOrderConversion
  {
    int i, * ptr = (int *)this->audio_global_->fbpara + 2;
    for (i = 0; i < (len >> 2) - 2; i++) *ptr = ntohl(*ptr);
  }
#endif
  if (this->audio_global_->fbpara->type == 0) { /* feedback message */
    /*
      SFprintf(stderr, "AS got fbmsg: addsamples %d, addsps %d\n",
      this->audio_global_->fbpara->data.fb.addSamples, this->audio_global_->fbpara->data.fb.addsps);
    */
    this->audio_global_->addSamples += this->audio_global_->fbpara->data.fb.addSamples;
    if (this->audio_global_->fbpara->data.fb.addsps) {
      this->audio_global_->delta_sps += this->audio_global_->fbpara->data.fb.addsps;
      this->audio_global_->upp = (int)(1000000.0 / ((double)(this->audio_global_->sps + this->audio_global_->delta_sps) / (double)this->audio_global_->spp));
    }
  }
  else { /* resend requests */
    APdescriptor * req = &(this->audio_global_->fbpara->data.ap);
    int i;
    /*
      SFprintf(stderr, "AS got %d resend reqs\n", this->audio_global_->fbpara->type);
    */
    for (i = 0; i < this->audio_global_->fbpara->type; i ++) {
      this->audio_global_->ResendPacket(req->firstSample, req->samples);
      req ++;
    }
  } 
  // send a audio frame.??
  this->audio_global_->send_audio ();
  return 0;
}

// ---------------------------------------------
// Audio_Control_Handler methods

Audio_Control_Handler::Audio_Control_Handler (ACE_HANDLE control_fd,
                                              Audio_Global *audio_global)
  :control_fd_ (control_fd),
   audio_global_ (audio_global)
{
}

ACE_HANDLE
Audio_Control_Handler::get_handle (void) const
{
  return this->audio_global_->serviceSocket;
}

int
Audio_Control_Handler::handle_input (ACE_HANDLE fd)
{
  ACE_DEBUG ((LM_DEBUG,"(%P|%t)Audio_Control_Handler::handle_input ()\n"));
  int result;
  switch (this->audio_global_->state)
    {
    case Audio_Global::AUDIO_WAITING:
      result = this->audio_global_->CmdRead((char *)&(this->audio_global_->cmd), 1);
      if (result != 0)
        return result;
      ACE_DEBUG ((LM_DEBUG,"(%P|%t)command %d received",this->audio_global_->cmd));
      switch (this->audio_global_->cmd)
        {
        case CmdPLAY:
          this->audio_global_->state = Audio_Global::AUDIO_PLAY;
          result = this->audio_global_->play_audio();
          if (result != 0)
            return result;
          break;
        case CmdCLOSE:
          ACE_DEBUG ((LM_DEBUG,"(%P|%t) A session closed\n"));
          ACE_Reactor::instance ()->end_event_loop ();
          return(0);
          break;
        default:
          fprintf(stderr, "audio channel command %d not recoganizeable\n", 
                  this->audio_global_->cmd);
          break;
        }
      break;
    case Audio_Global::AUDIO_PLAY:
      unsigned char tmp;
      result = this->audio_global_->CmdRead((char *)&tmp, 1);
      if (result != 0)
        return result;
      switch (tmp)
      {
      case CmdSPEED:
	{
	  SPEEDaudioPara para;
	  result = this->audio_global_->CmdRead((char *)&para, sizeof(para));
          if (result != 0)
            return result;
#ifdef NeedByteOrderConversion
	  para.sn = ntohl(para.sn);
	  para.samplesPerSecond = ntohl(para.samplesPerSecond);
	  para.samplesPerPacket = ntohl(para.samplesPerPacket);
	  para.spslimit = ntohl(para.spslimit);
#endif
	  this->audio_global_->sps  = para.samplesPerSecond;
	  this->audio_global_->spslimit = para.spslimit;
	  this->audio_global_->spp = para.samplesPerPacket;
	  if (this->audio_global_->spp * this->audio_global_->audioPara.bytesPerSample > this->audio_global_->databuf_size) {
	    this->audio_global_->spp = this->audio_global_->databuf_size / this->audio_global_->audioPara.bytesPerSample;
	  }
	  this->audio_global_->delta_sps = 0;  /* reset compensation value */
	  this->audio_global_->upp = (int)(1000000.0 / ((double)(this->audio_global_->sps) / (double)(this->audio_global_->spp)));
	  /*
	  SFprintf(stderr, "AS got CmdSPEED: sps %d\n", sps);
	  */
	}
	break;
      case CmdSTOP:
	{
	  int val;
	  this->audio_global_->cmd = tmp;
	  
          fprintf(stderr, "AS: CmdSTOP. . .\n");
          
	  result = this->audio_global_->CmdRead((char *)&val, sizeof(int));
          if (result != 0)
            return result;
	  /*
	  CmdWrite(AUDIO_STOP_PATTERN, strlen(AUDIO_STOP_PATTERN));
	  */
	  if (this->audio_global_->live_source) {
	    StopPlayLiveAudio();
	  }
          this->audio_global_->state = Audio_Global::AUDIO_WAITING;
	  return 0;
	}
      case CmdCLOSE:
	if (this->audio_global_->live_source) {
	  StopPlayLiveAudio();
	}
        // Make sure you remove the timer handler if you end the
        // event loop.

        ACE_Reactor::instance ()->end_event_loop ();
	return(1);  /* The whole AS session terminates */
      default:
	if (this->audio_global_->live_source) {
	  StopPlayLiveAudio();
	}
	fprintf(stderr, "AS error: cmd=%d while expects STOP/SPEED/CLOSE.\n", tmp);
	return(-1);
      }
      this->audio_global_->send_audio ();
      break;
    default:
      ACE_DEBUG ((LM_DEBUG,"(%P|%t)Audio_Control_Handler::handle_input () invalid state\n"));
      break;
    }
  return 0;
}


//-------------------------------------------------
// Audio Server methods.

Audio_Server::Audio_Server (void)
  :audio_global_ (0)
{
}

Audio_Server::Audio_Server (int argc,
                            char **argv,
                            //               ACE_SOCK_Stream& control,
                            int rttag,
                            int max_pkt_size)
  :audio_global_ (0)
{
  this->init (argc,
              argv,
              //              control,
              rttag,
              max_pkt_size);
}

int
Audio_Server::init (int argc,
                    char **argv,
                    //                    ACE_SOCK_Stream& control,
                    int rttag,
                    int max_pkt_size)
{
  int result;
  int junk;
  u_short port;
  ACE_UINT32 ip;

  TAO_TRY
    {
      this->orb_manager_.init (argc,argv,TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Audio_Server::init ()");
      return -1;
    }
  TAO_ENDTRY;

  // Allocate memory for the audio globals.

  ACE_NEW_RETURN (this->audio_global_,
                  Audio_Global,
                  -1);

  if (this->parse_args (argc,
                        argv) == -1)
    return -1;


  control.set_handle (this->audio_global_->serviceSocket);

  ACE_INET_Addr server_data_addr;
  ACE_INET_Addr client_data_addr;

  // do the handshaking.
  // Client is sending us JUNK
  ACE_DEBUG ((LM_DEBUG,"%s,%d\n",__FILE__,__LINE__));
  control.recv_n (&junk, sizeof junk);
  ACE_DEBUG ((LM_DEBUG,"%s,%d,received %d\n",__FILE__,__LINE__,junk));  
  // Client is sending us it's port number
  control.recv_n (&port, sizeof port);
  ACE_DEBUG ((LM_DEBUG,"%s,%d,received %u\n",__FILE__,__LINE__,port));
  // Client is sending us it's IP addr
  control.recv_n (&ip, sizeof ip);
  ACE_DEBUG ((LM_DEBUG,"%s,%d,received %d\n",__FILE__,__LINE__,ip));

  client_data_addr.set (port,
                        ip,
                        0);
  ACE_DEBUG ((LM_DEBUG,"%s,%d\n",__FILE__,__LINE__));
  ACE_DEBUG ((LM_DEBUG, 
              "(%P|%t) Client IP == %s, "
              "Client Port == %d\n",
              client_data_addr.get_host_name (),
              client_data_addr.get_port_number ()));

  server_data_addr.set ((unsigned short)0);
  ACE_DEBUG ((LM_DEBUG,"%s,%d\n",__FILE__,__LINE__));
  // "Connect" our dgram to the client endpoint.
  if (this->dgram_.open (client_data_addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, 
                       "(%P|%t) UDP open failed: %p\n"),
                      -1);
    ACE_DEBUG ((LM_DEBUG,"%s,%d\n",__FILE__,__LINE__));
  if (this->dgram_.get_local_addr (server_data_addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, 
                       "(%P|%t) UDP get_local_addr failed: %p\n",
                       "address:"),
                      -1);
  ACE_DEBUG ((LM_DEBUG,"%s,%d\n",__FILE__,__LINE__));
  port = server_data_addr.get_port_number ();
  ACE_DEBUG ((LM_DEBUG,"%s,%d\n",__FILE__,__LINE__));
  // %% we need to fix this ?
  // XXX this is a hack to get my IP address set correctly! By default,
  // get_ip_address is returning 0.0.0.0, even after calling
  // get_local_addr () !!
  server_data_addr.set (port,
                        server_data_addr.get_host_name ());
  ACE_DEBUG ((LM_DEBUG,"%s,%d\n",__FILE__,__LINE__));

  ACE_DEBUG ((LM_DEBUG, 
              "(%P|%t) UDP IP address is %s, and the port number is %d\n",
              server_data_addr.get_host_addr (),
              server_data_addr.get_port_number ()));

  ip = server_data_addr.get_ip_address ();

  port = htons (port);
  ip = htonl (ip);
  // Client wants us to send the port number first
  control.send_n (&port,
                  (int) sizeof (u_short));
  ACE_DEBUG ((LM_DEBUG,"%s,%d\n",__FILE__,__LINE__));
  // Client wants us to send it the IP address
  control.send_n (&ip,
                  (int) sizeof (ACE_UINT32));
  ACE_DEBUG ((LM_DEBUG,"%s,%d\n",__FILE__,__LINE__));
//   // Client is sending us a command
//   u_char cmd;
//   if (control.recv_n (&cmd,
//                       1) == -1)
//     ACE_ERROR_RETURN ((LM_ERROR,
//                        "%P|%t, Command recieve failed: %p"),
//                       -1);

  // end of handshaking.



  ACE_DEBUG ((LM_DEBUG,"%s,%d\n",__FILE__,__LINE__));
  this->audio_global_->serviceSocket = control.get_handle ();
  ACE_DEBUG ((LM_DEBUG,"(%P|%t)serviceSocket = %d",this->audio_global_->serviceSocket));
  this->audio_global_->audioSocket = this->dgram_.get_handle ();
  this->audio_global_->conn_tag = max_pkt_size;

  // Create the handlers 
  ACE_NEW_RETURN (this->control_handler_,
                  Audio_Control_Handler (this->audio_global_->serviceSocket,
                                         this->audio_global_),
                  -1);
  
  ACE_NEW_RETURN (this->data_handler_,
                  Audio_Data_Handler (this->dgram_.get_handle (),
                                      this->audio_global_),
                  -1);

  ACE_NEW_RETURN (this->sig_handler_,
                  Audio_Sig_Handler (this->audio_global_),
                  -1);


  if (max_pkt_size < 0) max_pkt_size = -max_pkt_size;
  else if (max_pkt_size == 0) max_pkt_size = 1024 * 1024;
  
  this->audio_global_->start_time = time(NULL);
  
  //  atexit(on_exit_routine);

  result = this->audio_global_->INITaudio ();
  ACE_DEBUG ((LM_DEBUG,"%s,%d\n",__FILE__,__LINE__));
  if (result != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) Audio_Server: "),
                      result);

  this->audio_global_->fbpara = (AudioFeedBackPara *)malloc(FBBUF_SIZE);
  if (this->audio_global_->fbpara == NULL) {
    perror("AS failed to allocate mem for fbpara");
    return (-1);
  }

  this->audio_global_->databuf_size = max_pkt_size - sizeof(AudioPacket);
  if (this->audio_global_->databuf_size > DATABUF_SIZE)
    this->audio_global_->databuf_size = DATABUF_SIZE;
  
  this->audio_global_->pktbuf = (AudioPacket *)malloc(sizeof(AudioPacket) + 
                                                     this->audio_global_->databuf_size);
  if (this->audio_global_->pktbuf == NULL) {
    perror("AS failed to allocate mem for pktbuf");
    return(-1);
  }
  ACE_DEBUG ((LM_DEBUG,"%s,%d\n",__FILE__,__LINE__));
  return 0;
}

int 
Audio_Server::set_peer (void)
{
  return 0;
}

int
Audio_Server::parse_args (int argc,
                          char **argv)
{
  ACE_Get_Opt get_opts (argc,argv,"f:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'f':
        ACE_DEBUG ((LM_DEBUG,"Received control fd %d",ACE_OS::atoi (get_opts.optarg)));
        this->audio_global_->serviceSocket = ACE_OS::atoi (get_opts.optarg);
        break;
      case '?':
        ACE_ERROR_RETURN ((LM_ERROR,"(%P|%t)Unknown option passed to Audio_Server\n"),-1);
        break;
      }
  return 0;
}

int
Audio_Server::run (void)
{
  ACE_DEBUG ((LM_DEBUG,"%s,%d\n",__FILE__,__LINE__));
  // Register the handlers with the TAO reactor.

  if (ACE_Reactor::instance ()->register_handler (this->control_handler_,
                                                              ACE_Event_Handler::READ_MASK) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%P|%t)Audio_Server::run () failed to register control handler\n"),
                        -1);
    }
  ACE_DEBUG ((LM_DEBUG,"%s,%d\n",__FILE__,__LINE__));
  if (ACE_Reactor::instance ()->register_handler (this->data_handler_,
                                                              ACE_Event_Handler::READ_MASK) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%P|%t)Audio_Server::run () failed to register data handler\n"),
                        -1);
    }
  ACE_DEBUG ((LM_DEBUG,"%s,%d\n",__FILE__,__LINE__));
  if (this->sig_handler_->register_handler () == -1)
    {
      
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%P|%t)Audio_Server::run () failed to register data handler\n"),
                        -1);
    }
  if (ACE_Reactor::instance ()->handler (this->audio_global_->serviceSocket,
                                                     ACE_Event_Handler::READ_MASK) == -1)
  
    ACE_ERROR_RETURN ((LM_ERROR,"(%P|%t)handler for serviceSocket not registered\n"),-1);
  else
    ACE_DEBUG ((LM_DEBUG,"Control handler registered\n"));
  // Set the global state of the Audio_Server to be in WAITING state.
  ACE_DEBUG ((LM_DEBUG,"%s,%d\n",__FILE__,__LINE__));
  this->audio_global_->state = Audio_Global::AUDIO_WAITING;

  // run the reactor event loop.
  ACE_Reactor::instance ()->run_event_loop ();

  ACE_DEBUG ((LM_DEBUG,"(%P|%t) Audio_Server::run,reactor came out of event loop \n"));

  return 0;

#if 0
  // unused code.
  int result;
  for (;;)
    {
      /*
        fprintf(stderr, "AS: waiting for a new command...\n");
      */
      result = this->audio_global_->CmdRead((char *)&(this->audio_global_->cmd), 1);
      if (result != 0)
        return result;
      switch (this->audio_global_->cmd)
        {
        case CmdPLAY:
          result = this->audio_global_->PLAYaudio();
          if (result != 0)
            return result;
          break;
        case CmdCLOSE:
          /*
            fprintf(stderr, "a session closed.\n");
          */
          return(0);
          break;
        default:
          fprintf(stderr, "audio channel command %d not recoganizeable\n", 
                  this->audio_global_->cmd);
          break;
        }
    }
#endif
}
