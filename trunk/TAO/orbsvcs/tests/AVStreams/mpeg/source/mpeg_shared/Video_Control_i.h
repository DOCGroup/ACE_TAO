/* -*- C++ -*- */
// $Id$

#if !defined (AV_VIDEO_CONTROL_H)
#define AV_VIDEO_CONTROL_H


#include "Video_ControlS.h"

class Video_Control_i
  : public virtual POA_Video_Control
{
public:
  Video_Control_i ();
  virtual CORBA::Boolean init_video (const Video_Control::INITvideoPara &para,
                                     CORBA::Environment &_tao_environment);
  
  virtual CORBA::Boolean stat_stream (CORBA::Char_out ch,
                                      CORBA::Long_out size,
                                      CORBA::Environment &_tao_environment);

  virtual CORBA::Boolean close (CORBA::Environment &_tao_environment);
  
  virtual CORBA::Boolean stat_sent (CORBA::Environment &_tao_environment);

  virtual CORBA::Boolean fast_forward (const Video_Control::FFpara &para,
                                       CORBA::Environment &_tao_environment
                                       );

  virtual CORBA::Boolean fast_backward (const Video_Control::FFpara &para,
                                        CORBA::Environment &_tao_environment);

  virtual CORBA::Boolean step (const Video_Control::STEPpara &para,
                               CORBA::Environment &_tao_environment);
  
  virtual CORBA::Boolean play (const Video_Control::PLAYpara &para,
                               CORBA::Long_out vts,
                               CORBA::Environment &_tao_environment);

  virtual CORBA::Boolean position (const Video_Control::POSITIONpara &para,
                                   CORBA::Environment &_tao_environment);

  virtual CORBA::Boolean speed (const Video_Control::SPEEDpara &para,
                                CORBA::Environment &_tao_environment);

  virtual CORBA::Boolean stop (CORBA::Long cmdsn,
                               CORBA::Environment &_tao_environment);

  virtual ~Video_Control_i (void);
  
};

#endif /* if !defined (AV_VIDEO_CONTROL_H) */
