// $Id$

#include "Audio_Server.h"
#include "Audio_Control_i.h"
#include "Audio_Control_State.h"

ACE_RCSID(mpeg_server, Audio_Server, "$Id$")

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
  //  ACE_DEBUG ((LM_DEBUG,"(%P|%t) Audio_Data_Handler::handle_input ()\n"));
  int bytes, len;
  for (;;) {
    if (this->audio_global_->conn_tag >= 0) {
      len = wait_read_bytes(this->audio_global_->audioSocket, (char *)this->audio_global_->fbpara, sizeof(*(this->audio_global_->fbpara)));
      if (len == 0) return(1); /* connection broken */
      else if (len < 0) { /* unexpected error */
       ACE_OS::perror ("AS read1 FB");
        return(-1);
      }
    }
    else { /* discard mode packet stream, read the whole packet */
      len = ::read(this->audio_global_->audioSocket, (char *)this->audio_global_->fbpara,  FBBUF_SIZE);
    }
    if (len == -1) {
      if (errno == EINTR) continue; /* interrupt */
      else {
        if (errno != EPIPE && errno != ECONNRESET)ACE_OS::perror ("AS failed to ACE_OS::read () fbmsg header");
        break;
      }
    }
    break;
  }
  //~~ check for end of file.
  if (len == 0)
    {
      ACE_DEBUG ((LM_DEBUG,"(%P|%t)End of file while reading feedback packedt\n"));
      TAO_ORB_Core_instance ()->reactor ()->end_event_loop ();
      return 0;
    }

  if (len < sizeof(*this->audio_global_->fbpara)) {
    if (len > 0) fprintf(stderr,
			 "AS warn ACE_OS::read () len %dB < sizeof(*this->audio_global_->fbpara) %dB\n",
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
       ACE_OS::perror ("AS read2 FB");
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

// Audio_Server_StreamEndPoint methods.

int
Audio_Server_StreamEndPoint::handle_open (void) 
{
  return 0;
}

int
Audio_Server_StreamEndPoint::handle_close (void) 
{
  // called when streamendpoint is being destructed
  return 0;
}

int
Audio_Server_StreamEndPoint::handle_stop (const AVStreams::flowSpec &the_spec,
                                          CORBA::Environment &env) 
{
  return 0;
}
  
int
Audio_Server_StreamEndPoint::handle_start (const AVStreams::flowSpec &the_spec,  
                                           CORBA::Environment &env) 
{
 return 0;
}
  
int
Audio_Server_StreamEndPoint::handle_destroy (const AVStreams::flowSpec &the_spec,  
                                             CORBA::Environment &env) 
{
  return 0;
}

CORBA::Boolean 
Audio_Server_StreamEndPoint::handle_connection_requested (AVStreams::flowSpec &the_spec,  
                                                          CORBA::Environment &env) 
{
  //  ACE_DEBUG ((LM_DEBUG,"(%P|%t) Audio_Server_StreamEndPoint::handle_connection_requested:() %s \n",
  //              the_spec[0]));

  char *server_string;

  server_string = CORBA::string_dup ((const char *) the_spec [0]);
  CORBA::Boolean result;
  result = AUDIO_CONTROL_I::instance ()->set_peer (server_string,env);
  // Get media control from my vdev and call set_peer on that.
  
  the_spec.length (1);
  the_spec [0]=server_string;

  return result;
}

