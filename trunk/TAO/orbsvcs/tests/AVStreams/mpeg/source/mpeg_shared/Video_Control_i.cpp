// $Id$

#include "Video_Control_i.h"

Video_Control_i::Video_Control_i (void)
{

}

CORBA::Boolean 
init_video (const Video_Control::INITvideoPara &para,
            CORBA::Environment)
{
  return 0;
}


CORBA::Boolean 
stat_stream (CORBA::Char_out ch,
             CORBA::Long_out size,
             CORBA::Environment)
{
  return 0;
}


CORBA::Boolean 
close (CORBA::Environment)
{
  return 0;
}


CORBA::Boolean 
stat_sent (CORBA::Environment)
{
  return 0;
}


CORBA::Boolean 
fast_forward (const Video_Control::FFpara &para,
              CORBA::Environment)
{
  return 0;
}


CORBA::Boolean 
fast_backward (const Video_Control::FFpara &para,
               CORBA::Environment)
{
  return 0;
}


CORBA::Boolean 
step (const Video_Control::STEPpara &para,
      CORBA::Environment)
{
  return 0;
}


CORBA::Boolean 
play (const Video_Control::PLAYpara &para,
      CORBA::Long_out vts,
      CORBA::Environment)
{
  return 0;
}


CORBA::Boolean 
position (const Video_Control::POSITIONpara &para,
          CORBA::Environment)
{
  return 0;
}


CORBA::Boolean 
speed (const Video_Control::SPEEDpara &para,
       CORBA::Environment)
{
  return 0;
}


CORBA::Boolean 
stop (CORBA::Long cmdsn,
      CORBA::Environment)
{
  return 0;
}
