// $Id$

#include "Audio_Control_State.h"
#include "Audio_Control_i.h"

ACE_RCSID(mpeg_server, Audio_Control_State, "$Id$")

Audio_Control_State::Audio_Control_State (void)
  :audio_global_ (AUDIO_GLOBAL::instance ()),
   audio_control_i_ (AUDIO_CONTROL_I::instance ())
{
}

Audio_Control_State::Audio_States
Audio_Control_State::get_state (void)
{
  return this->state_;
}

void
Audio_Control_State::set_state (Audio_States state)
{
  this->state_ = state;
}

CORBA::Boolean
Audio_Control_State::play (const Audio_Control::PLAYPara & para,
                               CORBA::Long_out ats) 
{
  return 0;
}
  
CORBA::Boolean 
Audio_Control_State::speed (const Audio_Control::SPEEDPara & para) 
{
  return 0;
}

CORBA::Boolean
Audio_Control_State::stop (CORBA::Long cmdsn) 
{
  ACE_DEBUG ((LM_DEBUG,"(%P|%t) Audio_Control_State::stop ()\n"));
  return 1; 
}

void
Audio_Control_State::close (void) 
{
  return;
}

Audio_Control_Waiting_State::Audio_Control_Waiting_State (void)
{
  this->state_ = AUDIO_WAITING;
  this->audio_global_->state = Audio_Global::AUDIO_WAITING;
}

CORBA::Boolean
Audio_Control_Waiting_State::play (const Audio_Control::PLAYPara & para,
                                   CORBA::Long_out ats)
{
  int result;

  ACE_DEBUG ((LM_DEBUG,"(%P|%t) Audio_Control_Waiting_State::play ()\n"));
  this->audio_global_->cmd = CmdPLAY;
  
  //  ACE_DEBUG ((LM_DEBUG,"(%P|%t) play_audio () called \n"));
 
  this->audio_global_->nextsample = para.nextSample;
  this->audio_global_->cmdsn = para.sn;
  this->audio_global_->sps = para.samplesPerSecond;
  this->audio_global_->spslimit = para.spslimit;
  this->audio_global_->spp = para.samplesPerPacket;
  this->audio_global_->addSamples = para.ABsamples / 2;
  if (this->audio_global_->spp * this->audio_global_->audioPara.bytesPerSample > this->audio_global_->databuf_size) {
    this->audio_global_->spp = this->audio_global_->databuf_size / this->audio_global_->audioPara.bytesPerSample;
  }
  /*
      SFprintf(stderr, "AS got CmdPLAY: sps %d\n", sps);
    */
  
  fprintf(stderr, "AS: nextSample = %d for PLAY.\n", para.nextSample);


  this->audio_global_->upp = (int)(1000000.0 / ((double)(this->audio_global_->sps) / (double)(this->audio_global_->spp)));
  this->audio_global_->nextTime = get_usec();
  
  ats = this->audio_global_->nextTime; // out parameter.
  if (this->audio_global_->live_source) {
    StartPlayLiveAudio();
  }

  this->audio_global_->hasdata = 1;
  this->audio_global_->packets = 0;

  this->audio_global_->send_audio ();

  this->audio_global_->state = Audio_Global::AUDIO_PLAY;
  this->audio_control_i_->change_state (AUDIO_CONTROL_PLAY_STATE::instance ());
  return 1;
}

void
Audio_Control_Waiting_State::close (void)
{
  ACE_DEBUG ((LM_DEBUG,"(%P|%t) Audio_Control_Waiting_State::close ()\n"));
  this->audio_global_->cmd = CmdCLOSE;
  ACE_DEBUG ((LM_DEBUG,"(%P|%t) A session closed\n"));
  TAO_ORB_Core_instance ()->orb ()->shutdown ();
  return ;
}

Audio_Control_Play_State::Audio_Control_Play_State (void)
{
  this->state_ = AUDIO_PLAY;
}

CORBA::Boolean
Audio_Control_Play_State::speed (const Audio_Control::SPEEDPara & para)
{
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
  ACE_DEBUG ((LM_DEBUG,"(%P|%t) Audio_Control_Play_State::speed ()\n"));
  return 1;
}

CORBA::Boolean
Audio_Control_Play_State::stop (CORBA::Long cmdsn)
{
  ACE_DEBUG ((LM_DEBUG,"(%P|%t) Audio_Control_Play_State::stop ()\n"));
  if (this->audio_global_->live_source) {
    StopPlayLiveAudio();
  }
  this->audio_global_->state = Audio_Global::AUDIO_WAITING;
  this->audio_control_i_->change_state (AUDIO_CONTROL_WAITING_STATE::instance ());
  return 1;
}

void
Audio_Control_Play_State::close (void)
{
  ACE_DEBUG ((LM_DEBUG,"(%P|%t) Audio_Control_Play_State::close ()\n"));
  if (this->audio_global_->live_source) {
    StopPlayLiveAudio();
  }
  // shutdown the ORB
  TAO_ORB_Core_instance ()->orb ()->shutdown ();
  return;
}
