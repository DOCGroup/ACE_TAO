// $Id$

#include "Video_Control_i.h"
#include "mpeg_server/Video_Server.h"

Video_Control_i::Video_Control_i ()
{
}

CORBA::Boolean 
Video_Control_i::init_video (const Video_Control::INITvideoPara &para,
                             CORBA::Environment&)
{
  return 0;
}


CORBA::Boolean 
Video_Control_i::stat_stream (CORBA::Char_out ch,
                              CORBA::Long_out size,
                              CORBA::Environment&)
{
  return 0;
}


CORBA::Boolean 
Video_Control_i::close (CORBA::Environment&)
{
  return 0;
}


CORBA::Boolean 
Video_Control_i::stat_sent (CORBA::Environment&)
{
  return 0;
}


CORBA::Boolean 
Video_Control_i::fast_forward (const Video_Control::FFpara &para,
                               CORBA::Environment&)
{
  return 0;
}


CORBA::Boolean 
Video_Control_i::fast_backward (const Video_Control::FFpara &para,
                                CORBA::Environment&)
{
  return 0;
}


CORBA::Boolean 
Video_Control_i::step (const Video_Control::STEPpara &para,
                       CORBA::Environment&)
{
  return 0;
}


CORBA::Boolean 
Video_Control_i::play (const Video_Control::PLAYpara &para,
                       CORBA::Long_out vts,
                       CORBA::Environment& env)
{
  ACE_DEBUG ((LM_DEBUG,
              "Video_Control_i::play () called \n"));
  VIDEO_CONTROL_HANDLER_INSTANCE::instance ()->get_video_control_handler ()->play (para,
                                                                                   vts,
                                                                                   env);

  return 0;
}


CORBA::Boolean 
Video_Control_i::position (const Video_Control::POSITIONpara &para,
                           CORBA::Environment&)
{
  return 0;
}


CORBA::Boolean 
Video_Control_i::speed (const Video_Control::SPEEDpara &para,
                        CORBA::Environment&)
{
  return 0;
}


CORBA::Boolean 
Video_Control_i::stop (CORBA::Long cmdsn,
                       CORBA::Environment&)
{
  return 0;
}

Video_Control_i::~Video_Control_i ()
{

}
