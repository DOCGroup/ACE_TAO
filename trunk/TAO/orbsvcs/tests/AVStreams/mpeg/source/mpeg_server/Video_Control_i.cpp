// $Id$

#include "Video_Control_i.h"
#include "mpeg_server/Video_Server.h"

Video_Control_i::Video_Control_i ()
  : reactor_ (TAO_ORB_Core_instance ()->reactor ())
{
  
}

int
Video_Control_i::create_handlers (void)
{
  ACE_NEW_RETURN (this->data_handler_,
                  Video_Data_Handler,
                  -1);

  ACE_NEW_RETURN (this->sig_handler_, 
                  Video_Sig_Handler,
                  -1);
  return 0;
}
CORBA::Boolean 
Video_Control_i::init_video (const Video_Control::INITvideoPara &para,
                             Video_Control::INITvideoReply_out reply,
                             CORBA::Environment& env)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Video_Control_i::init_video () called\n"));
  
  CORBA::Boolean result = this->state_->init_video (para,
                                                    reply);

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Video_Control_State::init_video returned %d\n", result));
  return result;

}


CORBA::Boolean 
Video_Control_i::stat_stream (CORBA::Char_out ch,
                              CORBA::Long_out size,
                              CORBA::Environment& env)
{
  return 0;
}


CORBA::Boolean 
Video_Control_i::close (CORBA::Environment& env)
{
  ACE_DEBUG ((LM_DEBUG, 
              "(%P|%t) Video_Control_i::close:"
              "shutting down the ORB\n"));

  TAO_ORB_Core_instance ()-> orb ()-> shutdown ();
  return CORBA::B_TRUE;
}


CORBA::Boolean 
Video_Control_i::stat_sent (CORBA::Environment& env)
{
  return 0;
}


CORBA::Boolean 
Video_Control_i::fast_forward (const Video_Control::FFpara &para,
                               CORBA::Environment& env)
{
  return this->state_->fast_forward (para);
}


CORBA::Boolean 
Video_Control_i::fast_backward (const Video_Control::FFpara &para,
                                CORBA::Environment& env)
{
  return this->state_->fast_backward (para);
}


CORBA::Boolean 
Video_Control_i::step (const Video_Control::STEPpara &para,
                       CORBA::Environment& env)
{
  return this->state_->step (para);
}


// this gets called by the client, when it wants
// to start playback
CORBA::Boolean 
Video_Control_i::play (const Video_Control::PLAYpara &para,
                       CORBA::Long_out vts,
                       CORBA::Environment& env)
{
  return this->state_->play (para,
                             vts);
}


CORBA::Boolean 
Video_Control_i::position (const Video_Control::POSITIONpara &para,
                           CORBA::Environment& env)
{
  return this->state_->position (para);
}


CORBA::Boolean 
Video_Control_i::speed (const Video_Control::SPEEDpara &para,
                        CORBA::Environment& env)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Video_Control_i::speed () called\n"));
  return this->state_->speed (para);
}


CORBA::Boolean 
Video_Control_i::stop (CORBA::Long cmdsn,
                       CORBA::Environment& env)
{
  return this->state_->stop (cmdsn);
}

CORBA::UShort
Video_Control_i::set_peer (const char *peer,
                           CORBA::Environment &_tao_environment)
{
   ACE_INET_Addr client_data_addr (peer);
  // Data (UDP) Address of the client.

   ACE_DEBUG ((LM_DEBUG,
               "(%P|%t) set_peer called: %s,%s,%d\n",
               peer,
               client_data_addr.get_host_addr (),
               client_data_addr.get_port_number ()));

 
  if (VIDEO_SINGLETON::instance ()->dgram.open (client_data_addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, 
                       "(%P|%t) UDP open failed: %p\n"),
                      -1);

  ACE_INET_Addr server_data_addr;
  // Data (UDP) Address of this server.
  
  if (VIDEO_SINGLETON::instance ()-> dgram.get_local_addr 
      (server_data_addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, 
                       "(%P|%t) UDP get_local_addr failed: %p\n"),
                      -1);

  ACE_DEBUG ((LM_DEBUG, 
              "(%P|%t) Video_Server: My UDP port number is %d\n",
              server_data_addr.get_port_number ()));

  if (this->register_handlers () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) Video_Control_i::set_peer: register_handlers failed\n"),
                      -1);

  VIDEO_SINGLETON::instance ()->videoSocket = VIDEO_SINGLETON::instance ()->dgram.get_handle ();

  ACE_DEBUG ((LM_DEBUG,"(%P|%t) set_peer: server port = %d\n",server_data_addr.get_port_number ()));
  return server_data_addr.get_port_number ();

}

int
Video_Control_i::register_handlers (void)
{
  int result;

  // Register the event handlers with the Reactor

  // first the data handler, i.e. UDP
  result = this->reactor_->register_handler (this->data_handler_, 
                                             ACE_Event_Handler::READ_MASK);
  if (result < 0)
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%P|%t) register_handler for data_handler failed\n"),
                        -1);

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) registered fd for data handler = (%d)\n",
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

// Changes the state of the video control handler from the current
// state to the state represented by the argument.
void
Video_Control_i::change_state (Video_Control_State *state)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Video_Control_i::Changing to state %d\n",
              state->get_state ()));
  this->state_ = state;
}

// Returns the current state object .
Video_Control_State *
Video_Control_i::get_state (void)
{
  return this->state_;
}


Video_Control_i::~Video_Control_i ()
{
  delete this->data_handler_;
  delete this->sig_handler_;
}
