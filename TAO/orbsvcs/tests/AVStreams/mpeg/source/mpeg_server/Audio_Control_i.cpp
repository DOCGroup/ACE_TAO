// $Id$

#include "Audio_Control_State.h"
#include "Audio_Control_i.h"

ACE_RCSID(mpeg_server, Audio_Control_i, "$Id$")

Audio_Control_i::Audio_Control_i (void)
  :audio_global_ (AUDIO_GLOBAL::instance ()),
   reactor_ (TAO_ORB_Core_instance ()->reactor ())
{
}

int
Audio_Control_i::create_handlers (void)
{
  ACE_NEW_RETURN (this->data_handler_,
                  Audio_Data_Handler (this->audio_global_->audioSocket,
                                      this->audio_global_),
                  -1);

  ACE_NEW_RETURN (this->sig_handler_, 
                  Audio_Sig_Handler (this->audio_global_),
                  -1);
  return 0;
}

CORBA::Boolean 
Audio_Control_i::init_audio (const Audio_Control::INITaudioPara & para,
                             Audio_Control::INITaudioReply_out reply,
                             CORBA::Environment &env)
{
  int result;
  int failureType; /* 0 - can't open file, 1 - can't open live source */

  for (int i=0 ; i< para.audiofile.length (); i++)
    this->audio_global_->audioFile[i] = para.audiofile [i];
  this->audio_global_->audioFile [i] = 0;

  if (Mpeg_Global::session_num > Mpeg_Global::session_limit || para.version != VERSION) 
    return 0; // failure

  memcpy(&(this->audio_global_->audioPara), &para.para, sizeof(this->audio_global_->audioPara));
  /*
  fprintf(stderr, "Client Audio para: encode %d, ch %d, sps %d, bps %d.\n",
	  para.para.encodeType, para.para.channels,
	  para.para.samplesPerSecond, para.para.bytesPerSample);
   */
  {
    int len = strlen(this->audio_global_->audioFile);
    if (strncasecmp("LiveAudio", this->audio_global_->audioFile, 9) &&
	strcasecmp(".au", this->audio_global_->audioFile+len-3))
      {
//         ACE_ERROR_RETURN ((LM_ERROR,
//                            "(%p|%t) Audio_Control_i::init_audio () failed\n"),
//                           0);
        cerr << "init_audio failed in line " << __LINE__ << endl;
        return 0;

      }
  }
  
  ACE_DEBUG ((LM_DEBUG, "(%P|%t)Audio file %s got.\n", this->audio_global_->audioFile));
  

  if (!strncasecmp("LiveAudio", this->audio_global_->audioFile, 9))
    {
      // May need to be uncommented for live audio.
      //      this->audio_global_->fd = OpenLiveAudio(&(para.para));
      if (this->audio_global_->fd == -1) {
        failureType = 1;
        goto failure;
      }
      this->audio_global_->fileSize =0x7fffffff;
      this->audio_global_->totalSamples = this->audio_global_->fileSize / this->audio_global_->audioPara.bytesPerSample;
      this->audio_global_->live_source = 1;
    }
  else
    {
      LeaveLiveAudio();
      this->audio_global_->fd = open(this->audio_global_->audioFile, O_RDONLY);
      if (this->audio_global_->fd == -1)
        {
          ACE_ERROR ((LM_ERROR, "AS error on opening audio file %s,%p", this->audio_global_->audioFile));
          failureType = 0;
          goto failure;
        }

      /* Try to get this->audio_global_->audioFile format this->audio_global_->audioPara here */
      
      /* figure out this->audio_global_->totalSamples */
      this->audio_global_->fileSize = lseek(this->audio_global_->fd, 0L, SEEK_END);
      lseek(this->audio_global_->fd, 0L, SEEK_SET);
      this->audio_global_->totalSamples = this->audio_global_->fileSize / this->audio_global_->audioPara.bytesPerSample;
      
      fprintf(stderr, "Total Samples=%d in audio file %ss.\n", this->audio_global_->totalSamples, this->audio_global_->audioFile);
      
    }

  reply.para.encodeType = this->audio_global_->audioPara.encodeType;
  reply.para.channels = this->audio_global_->audioPara.channels;
  reply.para.samplesPerSecond = this->audio_global_->audioPara.samplesPerSecond;
  reply.para.bytesPerSample = this->audio_global_->audioPara.bytesPerSample;
  reply.totalSamples = this->audio_global_->totalSamples;
  
  reply.live = this->audio_global_->live_source;
  reply.format = AUDIO_RAW;
  
  return 1;
 failure:
  {
   
    fprintf(stderr, "AS error: failed initializing audio file.\n");
    
    return 0;
  }

}                 
        

CORBA::Boolean 
Audio_Control_i::play (const Audio_Control::PLAYPara & para,
                       CORBA::Long_out ats,
                       CORBA::Environment &env)

{
  return this->state_->play (para,ats);
}

CORBA::Boolean 
Audio_Control_i::speed (const Audio_Control::SPEEDPara & para,
                        CORBA::Environment &env)

{
  return this->state_->speed (para);
}

CORBA::Boolean 
Audio_Control_i::stop (CORBA::Long cmdsn,
                       CORBA::Environment &env)
{
  return this->state_->stop (cmdsn);
}
 
CORBA::Boolean
Audio_Control_i::set_peer (char *&peer,
                           CORBA::Environment &env)
{
  ACE_INET_Addr client_data_addr (peer);
  // Data (UDP) Address of the client.
  
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) set_peer called: %s,%s,%d\n",
              peer,
              client_data_addr.get_host_addr (),
              client_data_addr.get_port_number ()));
  
  
  if (this->dgram_.open (client_data_addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, 
                       "(%P|%t) UDP open failed: %p\n"),
                      -1);

  ACE_INET_Addr server_data_addr;
  // Data (UDP) Address of this server.
  
  if (this->dgram_.get_local_addr 
      (server_data_addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, 
                       "(%P|%t) UDP get_local_addr failed: %p\n"),
                      -1);

  ACE_DEBUG ((LM_DEBUG, 
              "(%P|%t) Audio_Server: My UDP port number is %d\n",
              server_data_addr.get_port_number ()));

  this->audio_global_->audioSocket = this->dgram_.get_handle ();
  this->create_handlers () ; // very important.
  if (this->register_handlers () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) Audio_Control_i::set_peer: register_handlers failed\n"),
                      -1);

  ACE_DEBUG ((LM_DEBUG,"(%P|%t) set_peer: server port = %d\n",server_data_addr.get_port_number ()));
  ACE_NEW_RETURN (peer,
                  char [BUFSIZ],
                  0);
  server_data_addr.set (server_data_addr.get_port_number (),
                        server_data_addr.get_host_name ());
  server_data_addr.addr_to_string (peer,
                                   BUFSIZ);


  return 1;
}

void
Audio_Control_i::close (CORBA::Environment &env)
{
  this->state_->close ();
  return;
}

int
Audio_Control_i::register_handlers (void)
{
  int result;

  // change the state of audio control to be waiting state
  this->change_state (AUDIO_CONTROL_WAITING_STATE::instance ());

  // Register the event handlers with the Reactor
  // first the data handler, i.e. UDP
  result = this->reactor_->register_handler (this->data_handler_, 
                                             ACE_Event_Handler::READ_MASK);
  if (result < 0)
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%P|%t) register_handler for data_handler failed\n"),
                        -1);

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) registered this->audio_global_->fd for data handler = (%d)\n",
              this->data_handler_->get_handle ()));
  
  // finally, the signal handler, for periodic transmission
  // of packets
  result = this->sig_handler_->register_handler ();
  
  if (result < 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) register_handler for sig_handler"
                       "failed!\n"),
                      -1);

  return 0;
}

void
Audio_Control_i::change_state (Audio_Control_State *state)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Audio_Control_i::Changing to state %d\n",
              state->get_state ()));
  this->state_ = state;
}

// Returns the current state object .
Audio_Control_State *
Audio_Control_i::get_state (void)
{
  return this->state_;
}

Audio_Control_i::~Audio_Control_i ()
{
  delete this->data_handler_;
  delete this->sig_handler_;
}
