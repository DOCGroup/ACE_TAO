// $Id$

#include "Audio_Server.h"
#include "Audio_Control_i.h"
#include "Audio_Control_State.h"


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
  if (TAO_ORB_Core_instance ()->reactor ()->register_handler 
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
  if (TAO_ORB_Core_instance ()->reactor ()->register_handler (sig_set, 
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
  //  ACE_DEBUG ((LM_DEBUG, "(%P|%t) received signal %S\n", signum));

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
  ACE_DEBUG ((LM_DEBUG,"(%P|%t) Audio_Data_Handler::handle_input ()\n"));
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
  ACE_DEBUG ((LM_DEBUG,"(%P|%t) Audio_Control_Handler::handle_input ()\n"));
  int result;
  switch (this->audio_global_->state)
    {
    case Audio_Global::AUDIO_WAITING:
      result = this->audio_global_->CmdRead((char *)&(this->audio_global_->cmd), 1);
      if (result != 0)
        return -1;
      ACE_DEBUG ((LM_DEBUG,"(%P|%t)command %d received",this->audio_global_->cmd));
      switch (this->audio_global_->cmd)
        {
        case CmdPLAY:
          this->audio_global_->state = Audio_Global::AUDIO_PLAY;
          result = this->audio_global_->play_audio();
          if (result != 0)
            return -1;
          break;
        case CmdCLOSE:
          ACE_DEBUG ((LM_DEBUG,"(%P|%t) A session closed\n"));
          TAO_ORB_Core_instance ()->reactor ()->end_event_loop ();
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
        return -1;
      switch (tmp)
      {
      case CmdSPEED:
	{
	  SPEEDaudioPara para;
	  result = this->audio_global_->CmdRead((char *)&para, sizeof(para));
          if (result != 0)
            return -1;
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
            return -1;
	  /*
	  CmdWrite(AUDIO_STOP_PATTERN, strlen(AUDIO_STOP_PATTERN));
	  */
	  if (this->audio_global_->live_source) {
	    StopPlayLiveAudio();
	  }
          this->audio_global_->state = Audio_Global::AUDIO_WAITING;
          break;
	}
      case CmdCLOSE:
	if (this->audio_global_->live_source) {
	  StopPlayLiveAudio();
	}
        // Make sure you remove the timer handler if you end the
        // event loop.

        TAO_ORB_Core_instance ()->reactor ()->end_event_loop ();
	return(1);  /* The whole AS session terminates */
      default:
	if (this->audio_global_->live_source) {
	  StopPlayLiveAudio();
	}
	fprintf(stderr, "AS error: cmd=%d while expects STOP/SPEED/CLOSE.\n", tmp);
	return -1;
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

#if 0
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
#endif

int
Audio_Server::init (int argc,
                    char **argv,
                    int rttag,
                    int max_pkt_size,
                    CORBA::Environment &env)
{
  int result;

  if (this->initialize_orb (argc,
                            argv,
                            env) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) Audio_Server: orb initialization failed!"),
                      -1);
  
  this->audio_global_ = AUDIO_GLOBAL::instance ();
  this->audio_global_->conn_tag = max_pkt_size;
  if (max_pkt_size < 0) max_pkt_size = -max_pkt_size;
  else if (max_pkt_size == 0) max_pkt_size = 1024 * 1024;
  
  this->audio_global_->start_time = time(NULL);
  
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
Audio_Server::initialize_orb (int argc,
                              char **argv,
                              CORBA::Environment &env)
{
  //  ACE_DEBUG ((LM_DEBUG, "(%P|%t) %s:%d\n", __FILE__, __LINE__));
  int result;

  // Initialize the orb_manager
  this->orb_manager_.init_child_poa (argc,
                                     argv,
                                     "child_poa",
                                     env);
  TAO_CHECK_ENV_RETURN (env,
                        -1);


  // %% hack to make the ORB manager pick its own port
  // we create an Ace_Time_Value of time out zero,
  // and then call a dummy orb run
  ACE_Time_Value tv (0);
  if (this->orb_manager_.orb ()-> run (tv) == -1)
    ACE_ERROR_RETURN ( (LM_ERROR,
                        "(%P|%t) ORB_run %p\n",
                        "run"),
                       -1);

  //  ACE_DEBUG ((LM_DEBUG, "(%P|%t) %s:%d\n", __FILE__, __LINE__));

  this->orb_manager_.activate_under_child_poa ("Audio_Control",
                                               AUDIO_CONTROL_I::instance (),
                                               env);
  TAO_CHECK_ENV_RETURN (env,-1);
  //  ACE_DEBUG ((LM_DEBUG, "(%P|%t) %s:%d\n", __FILE__, __LINE__));
  CORBA::Object_var naming_obj =
    this->orb_manager_.orb ()->resolve_initial_references ("NameService");
  if (CORBA::is_nil (naming_obj.in ()))
    ACE_ERROR_RETURN ((LM_ERROR,
                       " (%P|%t) Unable to resolve the Name Service.\n"),
                      -1);
  //  ACE_DEBUG ((LM_DEBUG, "(%P|%t) %s:%d\n", __FILE__, __LINE__));
  CosNaming::NamingContext_var naming_context =
    CosNaming::NamingContext::_narrow (naming_obj.in (),
                                       env);
  TAO_CHECK_ENV_RETURN (env,
                        -1);
  
  //  ACE_DEBUG ((LM_DEBUG, "(%P|%t) %s:%d\n", __FILE__, __LINE__));
  // Create a name for the audio control object
  CosNaming::Name audio_control_name (1);
  audio_control_name.length (1);
  audio_control_name [0].id = CORBA::string_dup ("Audio_Control");
  
  // Register the audio control object with the naming server.
  naming_context->bind (audio_control_name,
                        AUDIO_CONTROL_I::instance ()->_this (env),
                        env);

  if (env.exception () != 0)
    {
      env.clear ();
      naming_context->rebind (audio_control_name,
                              AUDIO_CONTROL_I::instance ()->_this (env),
                              env);
      TAO_CHECK_ENV_RETURN (env,
                            -1);
    }

  //  ACE_DEBUG ((LM_DEBUG, "(%P|%t) %s:%d\n", __FILE__, __LINE__));
  AUDIO_CONTROL_I::instance ()->change_state (AUDIO_CONTROL_WAITING_STATE::instance ());
  //  ACE_DEBUG ((LM_DEBUG, "(%P|%t) %s:%d\n", __FILE__, __LINE__));
  return 0;
}

#ifdef o
int
Audio_Server::run (void)
{
  ACE_DEBUG ((LM_DEBUG,"%s,%d\n",__FILE__,__LINE__));
  // Register the handlers with the TAO reactor.

  if (TAO_ORB_Core_instance ()->reactor ()->register_handler (this->control_handler_,
                                                              ACE_Event_Handler::READ_MASK) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%P|%t)Audio_Server::run () failed to register control handler\n"),
                        -1);
    }
  ACE_DEBUG ((LM_DEBUG,"%s,%d\n",__FILE__,__LINE__));
  if (TAO_ORB_Core_instance ()->reactor ()->register_handler (this->data_handler_,
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
  if (TAO_ORB_Core_instance ()->reactor ()->handler (this->audio_global_->serviceSocket,
                                                     ACE_Event_Handler::READ_MASK) == -1)
  
    ACE_ERROR_RETURN ((LM_ERROR,"(%P|%t)handler for serviceSocket not registered\n"),-1);
  else
    ACE_DEBUG ((LM_DEBUG,"Control handler registered\n"));
  // Set the global state of the Audio_Server to be in WAITING state.
  ACE_DEBUG ((LM_DEBUG,"%s,%d\n",__FILE__,__LINE__));
  this->audio_global_->state = Audio_Global::AUDIO_WAITING;

  // run the reactor event loop.
  TAO_ORB_Core_instance ()->reactor ()->run_event_loop ();

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

#endif

int
Audio_Server::run (CORBA::Environment &env)
{
  return this->orb_manager_.run (env);
}
