// $Id$

#include "Video_Control_i.h"
#include "mpeg_server/Video_Server.h"

Video_Control_i::Video_Control_i ()
{
}

CORBA::Boolean 
Video_Control_i::init_video (const Video_Control::INITvideoPara &para,
                             Video_Control::INITvideoReply_out reply,
                             CORBA::Environment& env)
{
  
  Video_Control_Handler_Instance *vchi;
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t)Video_Control_i::init_video () called \n"));
  Video_Control_Handler *vch ;
  
  if ((vchi = VIDEO_CONTROL_HANDLER_INSTANCE::instance () )!= 0)
    {
      vch = VIDEO_CONTROL_HANDLER_INSTANCE::instance ()->get_video_control_handler () ;
      ACE_DEBUG ((LM_DEBUG,
                  "Video_Control_Handler_instance address %x,%x",
                  vchi,vch));

    }
  else
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t)Video_control_Handler_instance is null \n"),
                      -1);

  if (vch == 0)
    ACE_ERROR_RETURN ((LM_DEBUG,
                       "(%P|%t)Video_Control_Handler_Instance::get_video_Control_handler returned null \n"),
                      -1);
  
  else
    return vch->init_video (para,reply,env);

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
  return 0;
}


CORBA::Boolean 
Video_Control_i::fast_backward (const Video_Control::FFpara &para,
                                CORBA::Environment& env)
{
  return 0;
}


CORBA::Boolean 
Video_Control_i::step (const Video_Control::STEPpara &para,
                       CORBA::Environment& env)
{
  return 0;
}


CORBA::Boolean 
Video_Control_i::play (const Video_Control::PLAYpara &para,
                       CORBA::Long_out vts,
                       CORBA::Environment& env)
{
  Video_Control_Handler_Instance *vchi;
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t)Video_Control_i::play () called \n"));
  Video_Control_Handler *vch ;
  
  if ((vchi = VIDEO_CONTROL_HANDLER_INSTANCE::instance () )!= 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Video_Control_Handler_instance address %x",
                  vchi));
    vch = VIDEO_CONTROL_HANDLER_INSTANCE::instance ()->get_video_control_handler () ;
    }
  else
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t)Video_control_Handler_instance is null \n"),
                      -1);

  if (vch == 0)
    ACE_ERROR_RETURN ((LM_DEBUG,
                       "(%P|%t)Video_Control_Handler_Instance::get_video_Control_handler returned null \n"),
                      -1);
  
  else
    //    vch->play (para,vts,env);
    vch->play (para,vts,env);
  return 0;
}


CORBA::Boolean 
Video_Control_i::position (const Video_Control::POSITIONpara &para,
                           CORBA::Environment& env)
{
  return 0;
}


CORBA::Boolean 
Video_Control_i::speed (const Video_Control::SPEEDpara &para,
                        CORBA::Environment& env)
{
  Video_Control_Handler_Instance *vchi;
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t)Video_Control_i::stop () \n"));
  Video_Control_Handler *vch ;
  
  if ((vchi = VIDEO_CONTROL_HANDLER_INSTANCE::instance () )!= 0)
    {
      vch = VIDEO_CONTROL_HANDLER_INSTANCE::instance ()->get_video_control_handler () ;
      ACE_DEBUG ((LM_DEBUG,
                  "Video_Control_Handler_instance address %x,%x",
                  vchi,vch));

    }
  else
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t)Video_control_Handler_instance is null \n"),
                      -1);

  if (vch == 0)
    ACE_ERROR_RETURN ((LM_DEBUG,
                       "(%P|%t)Video_Control_Handler_Instance::get_video_Control_handler returned null \n"),
                      -1);
  
  else
    return vch->speed (para,env);

  return 0;
}


CORBA::Boolean 
Video_Control_i::stop (CORBA::Long cmdsn,
                       CORBA::Environment& env)
{
  Video_Control_Handler_Instance *vchi;
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t)Video_Control_i::stop () \n"));
  Video_Control_Handler *vch ;
  
  if ((vchi = VIDEO_CONTROL_HANDLER_INSTANCE::instance () )!= 0)
    {
      vch = VIDEO_CONTROL_HANDLER_INSTANCE::instance ()->get_video_control_handler () ;
      ACE_DEBUG ((LM_DEBUG,
                  "Video_Control_Handler_instance address %x,%x",
                  vchi,vch));

    }
  else
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t)Video_control_Handler_instance is null \n"),
                      -1);

  if (vch == 0)
    ACE_ERROR_RETURN ((LM_DEBUG,
                       "(%P|%t)Video_Control_Handler_Instance::get_video_Control_handler returned null \n"),
                      -1);
  
  else
    return vch->stop (cmdsn,env);

  return 0;
}

Video_Control_i::~Video_Control_i ()
{

}
