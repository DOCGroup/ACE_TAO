// $Id$

#include "Video_Control_i.h"
#include "mpeg_server/Video_Server.h"

Video_Control_i::Video_Control_i ()
{
}

Video_Control_Handler *
Video_Control_i::get_video_control_handler ()
{

  Video_Control_Handler_Instance *vchi;
  Video_Control_Handler *vch ;
   
  if ((vchi = VIDEO_CONTROL_HANDLER_INSTANCE::instance () )!= 0)
    {
      vch = VIDEO_CONTROL_HANDLER_INSTANCE::instance ()->get_video_control_handler () ;
      ACE_DEBUG ((LM_DEBUG,
                  "Video_Control_Handler_instance address %x,%x\n",
                  vchi,
                  vch));
      
      
    }
  else
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t)Video_control_Handler_instance is null \n"),
                      0);
  
  if (vch == 0)
    ACE_ERROR_RETURN ((LM_DEBUG,
                       "(%P|%t)Video_Control_Handler_Instance::get_video_Control_handler returned null \n"),
                      0);
  
  return vch;
}

CORBA::Boolean 
Video_Control_i::init_video (const Video_Control::INITvideoPara &para,
                             Video_Control::INITvideoReply_out reply,
                             CORBA::Environment& env)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Video_Control_i::init_video () called\n"));
  
  CORBA::Boolean result = this->get_video_control_handler ()->init_video (para,
                                                  reply,
                                                  env);
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
  return 0;
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
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Video_Control_i::fast_forward () called\n"));
  return this->get_video_control_handler ()->fast_forward (para,
                                                           env);
}


CORBA::Boolean 
Video_Control_i::fast_backward (const Video_Control::FFpara &para,
                                CORBA::Environment& env)
{
  return this->get_video_control_handler ()->fast_backward (para,
                                                            env);
}


CORBA::Boolean 
Video_Control_i::step (const Video_Control::STEPpara &para,
                       CORBA::Environment& env)
{
  return 0;
}


// this gets called by the client, when it wants
// to start playback
CORBA::Boolean 
Video_Control_i::play (const Video_Control::PLAYpara &para,
                       CORBA::Long_out vts,
                       CORBA::Environment& env)
{
  return this->get_video_control_handler ()->play (para,
                                                   vts,
                                                   env);
}


CORBA::Boolean 
Video_Control_i::position (const Video_Control::POSITIONpara &para,
                           CORBA::Environment& env)
{
  return this->get_video_control_handler ()->position (para,
                                                       env);
}


CORBA::Boolean 
Video_Control_i::speed (const Video_Control::SPEEDpara &para,
                        CORBA::Environment& env)
{
  return this->get_video_control_handler ()->speed (para,
                            env);
}


CORBA::Boolean 
Video_Control_i::stop (CORBA::Long cmdsn,
                       CORBA::Environment& env)
{
  return this->get_video_control_handler ()->stop (cmdsn,
                           env);
}

Video_Control_i::~Video_Control_i ()
{

}
