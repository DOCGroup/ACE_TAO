/* -*- C++ -*- */
// $Id$

#ifndef AV_VIDEO_CONTROL_H
#define AV_VIDEO_CONTROL_H

#include "mpeg_shared/Video_ControlS.h"
#include "Video_Server.h"

class Video_Data_Handler;
class Video_Sig_Handler;
class Video_Control_State;

class Video_Control_i
  : public virtual POA_Video_Control
{
public:
  Video_Control_i ();
  // constructor

  int create_handlers (void);
  // creates the data and signal handlers

  virtual CORBA::Boolean init_video (const Video_Control::INITvideoPara &para,
                                     Video_Control::INITvideoReply_out reply,
                                     CORBA::Environment &_tao_environment);

  virtual CORBA::Boolean stat_stream (CORBA::Char_out ch,
                                      CORBA::Long_out size,
                                      CORBA::Environment &_tao_environment);

  virtual void close (CORBA::Environment &_tao_environment);

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

  virtual CORBA::Boolean set_peer (char * &peer,
                                   CORBA::Environment &_tao_environment);
  // called by the client to inform us about it's ip and
  // udp address.

  void change_state (Video_Control_State *state);
  // Used to change the state

  Video_Control_State *get_state (void);
  // Accessor for the state_

  virtual ~Video_Control_i (void);
  // Destructor

protected:
  int register_handlers (void);
  // called by set_peer to install the data_handler and
  // the sig_handler

  ACE_Reactor *reactor_;
  // The Reactor

  Video_Control_State *state_;
  // State pattern - pointer to abstract State object

  Video_Data_Handler *data_handler_;
  // Data Socket Event Handler

  Video_Sig_Handler *sig_handler_;
  // signal handler for SIGALRM to periodically send the video frames
  // to the client

};

// Video_Control_i instance singleton.
// %%
typedef ACE_Singleton <Video_Control_i, ACE_Null_Mutex> VIDEO_CONTROL_I;

#endif /* if !defined (AV_VIDEO_CONTROL_H) */
