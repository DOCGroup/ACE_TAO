/* -*- C++ -*- */

/* $Id$  */

/* Copyright (c) 1995 Oregon Graduate Institute of Science and Technology
 * P.O.Box 91000-1000, Portland, OR 97291, USA;
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of O.G.I. not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission.  O.G.I. makes no representations about the
 * suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 * O.G.I. DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
 * ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
 * O.G.I. BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY
 * DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN
 * AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Author: Shanwei Cen
 *         Department of Computer Science and Engineering
 *         email: scen@cse.ogi.edu
 */

#ifndef MPEG_VIDEO_CONTROL_STATE_H
#define MPEG_VIDEO_CONTROL_STATE_H

#include "ace/Singleton.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "mpeg_shared/Video_ControlS.h"
#include "mpeg_server/Video_Control_i.h"

class Video_Control_i;

class Video_Control_State
{
  // = TITLE
  //   Defines an abstract class that is used to implement the state
  //   pattern for the video control.
  //
  // = DESCRIPTION
  //   The various states of the video control are defined by the
  //   Video_States enum. They can be implemented by subclassing from
  //   this class and overriding the handle_input method.
public:
  Video_Control_State (void);
  // constructor

  enum Video_States
  {
    VIDEO_WAITING,
    VIDEO_PLAY = 1,
    VIDEO_FAST_FORWARD = 2,
    VIDEO_FAST_BACKWARD = 3
  };

  Video_States get_state (void);
  // returns the current state

  void set_state (Video_States state);
  // sets the state

  virtual CORBA::Boolean init_video (const Video_Control::INITvideoPara &para,
                                     Video_Control::INITvideoReply_out reply) ;

  virtual CORBA::Boolean stat_stream (CORBA::Char_out ch,
                                      CORBA::Long_out size);

  virtual void  close (void);

  virtual CORBA::Boolean stat_sent (void);

  virtual CORBA::Boolean fast_forward (const Video_Control::FFpara &para) ;

  virtual CORBA::Boolean fast_backward (const Video_Control::FFpara &para) ;

  virtual CORBA::Boolean step (const Video_Control::STEPpara &para) ;

  virtual CORBA::Boolean play (const Video_Control::PLAYpara &para,
                               CORBA::Long_out vts) ;

  virtual CORBA::Boolean position (const Video_Control::POSITIONpara &para) ;

  virtual CORBA::Boolean speed (const Video_Control::SPEEDpara &para) ;

  virtual CORBA::Boolean stop (CORBA::Long cmdsn) ;

protected:
  Video_States state_;
  // State of this object

  Video_Control_i *vci_;

};

class Video_Control_Waiting_State : public virtual Video_Control_State
{
  // = TITLE
  //   Defines a class that implements the waiting state of the video
  //   control state pattern.
public:
  Video_Control_Waiting_State (void);
  // Default constructor, sets the state to WAITING

  virtual CORBA::Boolean stat_stream (CORBA::Char_out ch,
                                      CORBA::Long_out size);

  virtual void close (void);

  virtual CORBA::Boolean stat_sent (void);

  virtual CORBA::Boolean fast_forward (const Video_Control::FFpara &para);

  virtual CORBA::Boolean fast_backward (const Video_Control::FFpara &para);

  virtual CORBA::Boolean step (const Video_Control::STEPpara &para);


  virtual CORBA::Boolean play (const Video_Control::PLAYpara &para,
                               CORBA::Long_out vts);

  virtual CORBA::Boolean position (const Video_Control::POSITIONpara &para);

  virtual CORBA::Boolean speed (const Video_Control::SPEEDpara &para);

  virtual CORBA::Boolean stop (CORBA::Long cmdsn);
};

class Video_Control_Play_State : public virtual Video_Control_State
{
  // = TITLE
  //   Defines a class that implements the playing state of the video
  //   control state pattern.
public:
  Video_Control_Play_State (void);
  // Default constructor, sets the state to VIDEO_PLAY

  virtual CORBA::Boolean stop (CORBA::Long cmdsn);

  virtual CORBA::Boolean speed (const Video_Control::SPEEDpara &para);
};

class Video_Control_Fast_Forward_State : public virtual Video_Control_State
{
  // = TITLE
  //   Defines a class that implements the fast_forward state of the video
  //   control state pattern.
public:
  Video_Control_Fast_Forward_State (void);
  // Default constructor, sets the state to VIDEO_FAST_FORWARD

  void close (void);

  CORBA::Boolean stop (CORBA::Long cmdsn);
};

class Video_Control_Fast_Backward_State : public virtual Video_Control_State
{
  // = TITLE
  //   Defines a class that implements the fast_backward state of the video
  //   control state pattern.
public:
  Video_Control_Fast_Backward_State (void);
  // Default constructor, sets the state to VIDEO_FAST_BACKWARD

  void close (void);

  CORBA::Boolean stop (CORBA::Long cmdsn);
};

typedef ACE_Singleton <Video_Control_Waiting_State, ACE_SYNCH_MUTEX>
        VIDEO_CONTROL_WAITING_STATE;
typedef ACE_Singleton <Video_Control_Play_State, ACE_SYNCH_MUTEX>
        VIDEO_CONTROL_PLAY_STATE;
typedef ACE_Singleton <Video_Control_Fast_Forward_State, ACE_SYNCH_MUTEX>
        VIDEO_CONTROL_FAST_FORWARD_STATE;
typedef ACE_Singleton <Video_Control_Fast_Backward_State, ACE_SYNCH_MUTEX>
        VIDEO_CONTROL_FAST_BACKWARD_STATE;

#endif /* MPEG_VIDEO_SERVER_STATE_H */
