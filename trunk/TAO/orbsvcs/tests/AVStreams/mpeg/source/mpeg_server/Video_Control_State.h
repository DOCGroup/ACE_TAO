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

#if !defined (MPEG_VIDEO_CONTROL_STATE_H)
#define MPEG_VIDEO_CONTROL_STATE_H

#include "ace/Singleton.h"

class Video_Control_Handler;

class Video_Control_State
{
public:
  Video_Control_State ();
  // constructor

  enum Video_States 
  {
    WAITING,
    VIDEO_PLAY = 1,
    VIDEO_FAST_FORWARD = 2,
    VIDEO_FAST_BACKWARD = 3
  };

  // Indicates the state of the Video Server process ie. play, fast forward play
  virtual int handle_input (ACE_HANDLE h = 0) = 0;
  // state pattern - this is handled differently
  // depending on the state
 
  Video_States get_state (void);
  // returns the current state

  void set_state (Video_States state);
  // sets the state

 
protected:
  
  Video_States state_;
  
  Video_Control_Handler *vch_;

};

class Video_Control_Waiting_State
  : public virtual Video_Control_State
{
public:
  virtual int handle_input (ACE_HANDLE h = 0);

};

class Video_Control_Play_State
  : public virtual Video_Control_State
{
public:
  virtual int handle_input (ACE_HANDLE h = 0);

};

class Video_Control_Fast_Forward_State
  : public virtual Video_Control_State
{
public:
  virtual int handle_input (ACE_HANDLE h = 0);

};

class Video_Control_Fast_Backward_State
  : public virtual Video_Control_State
{
public:
  virtual int handle_input (ACE_HANDLE h = 0);

};

typedef ACE_TSS_Singleton <Video_Control_Waiting_State,       ACE_SYNCH_MUTEX> VIDEO_CONTROL_WAITING_STATE;     
typedef ACE_TSS_Singleton <Video_Control_Play_State,          ACE_SYNCH_MUTEX> VIDEO_CONTROL_PLAY_STATE;        
typedef ACE_TSS_Singleton <Video_Control_Fast_Forward_State,  ACE_SYNCH_MUTEX> VIDEO_CONTROL_FAST_FORWARD_STATE;
typedef ACE_TSS_Singleton <Video_Control_Fast_Backward_State, ACE_SYNCH_MUTEX> VIDEO_CONTROL_FAST_BACKWARD_STATE;


#endif // MPEG_VIDEO_SERVER_STATE_H
