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

#if !defined (MPEG_VIDEO_SERVER_H)
#define MPEG_VIDEO_SERVER_H

#include "ace/Reactor.h"
#include "ace/Event_Handler.h"

#include "tao/TAO.h"
#include "include/common.h"
#include "mpeg_shared/fileio.h"
#include "mpeg_shared/com.h"   
#include "mpeg_shared/routine.h"
#include "mpeg_shared/sendpt.h"
#include "mpeg_shared/Video_ControlS.h"
#include "mpeg_server/server_proto.h"
#include "mpeg_server/Video_Control_State.h"
#include "mpeg_server/Globals.h"

class Video_Control_Handler : public virtual ACE_Event_Handler
{
  // = TITLE
  //   Defines the event handler class for the Video Control.
  //
  // = DESCRIPTION
  //   This class makes use of a TCP socket.It contains a pointer to
  //   the current state which is implemented using the state pattern.
public:

  Video_Control_Handler (int video_control_fd);
  // Construct this handler with a control (TCP) fd
  // %% use sock stream instead of fd

  virtual int handle_input (ACE_HANDLE fd = ACE_INVALID_HANDLE);
  // Called when input events occur (e.g., connection or data).

  virtual ACE_HANDLE get_handle (void) const;
  // Returns the handle used by the event_handler.
  
  Video_Control_State *get_state (void);
  // Accessor for the state_ 

  void change_state (Video_Control_State *state);
  // Used to change the state

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

private:
  Video_Control_State *state_;
  // State pattern - pointer to abstract State object

  ACE_HANDLE control_handle_;
};

class Video_Control_Handler_Instance
{
  // = TITLE
  //   Defines a Video_Control_Handler_Instance class which is to be
  //   used as a singleton to give access to a Video_Control_Handler
  //   instance to the state pattern classes.
  //
  // = DESCRIPTION
  //   The Video_Control_Handler has to be set using the
  //   set_video_control_handler method.
public:
  // @@ Please add comments to these methods.
  Video_Control_Handler_Instance (void);
  
  void set_video_control_handler (Video_Control_Handler *h);
  
  Video_Control_Handler *get_video_control_handler (void);

private:
  Video_Control_Handler *video_control_handler_;
};

// Video_Control_Handler instance singleton.
//@@ Does this really need to be a thread-specific singleton?
typedef ACE_TSS_Singleton <Video_Control_Handler_Instance,
                           ACE_SYNCH_MUTEX>
        VIDEO_CONTROL_HANDLER_INSTANCE; 

class Video_Sig_Handler : public virtual ACE_Event_Handler
{
  // = TITLE
  //   Defines a video signal handler class which registers itself with the
  //   default ACE_Reactor::instance () . Handles the
  //   SIGALRM signal.
  //
  // = DESCRIPTION
  //   This class contains a pointer to a Video_Control_Handler
  //   instance and decides the signal action depending on its state.
  //   An object of this class is used to periodically send the video
  //   frames to the client using the Video_Timer_Global class.
public:
  Video_Sig_Handler (Video_Control_Handler *vch);

  virtual ACE_HANDLE get_handle (void) const;

  int register_handler (void);
  // this will register this sig_handler
  // with the reactor for SIGALRM

  virtual int shutdown (ACE_HANDLE, 
                        ACE_Reactor_Mask);

  virtual int handle_input (ACE_HANDLE);

  virtual int handle_signal (ACE_HANDLE signum,
                             siginfo_t * = 0,
                             ucontext_t* = 0);
private:
  ACE_HANDLE handle_;
  // my handle

  Video_Control_Handler *vch_;
  // Pointer to the control handler, for accessing
  // the current state of the server.
};

class Video_Data_Handler : public virtual ACE_Event_Handler
{
  // = TITLE
  //   Defines a event handler for video data using a datagram i.e UDP
  //   socket.
  //
  // = DESCRIPTION
  //   This takes a pointer to a Video_Control_Handler instance and
  //   reacts differently to the events based on the
  //   video_control_handler's state.
public:
  Video_Data_Handler (int video_data_fd,
                      Video_Control_Handler *vch);
  // Construct this handler with a data fd

  virtual int handle_input (ACE_HANDLE fd = ACE_INVALID_HANDLE);
  // Called when input events occur (e.g., connection or data).

  virtual ACE_HANDLE get_handle (void) const;
  // Get the handle used by this event handler

private:
  ACE_HANDLE data_handle_;
  // my handle

  Video_Control_Handler *vch_;
  // Pointer to the control handler, for accessing
  // the current state of the server.
};

class Video_Server
{
  // = TITLE
  //   Defines a class that abstracts the functionality of a
  //   video_server.
  //
  // = DESCRIPTION
  //   This registers 3 event handlers with the <ACE_Reactor::instance>,
  //   namely a control,data and signal handlers.
public:
  Video_Server (void);
  // Default constructor

  Video_Server (int control_fd,
                int data_fd,
                int rttag,
                int max_pkt_size);
  // constructor taking the handles

  ~Video_Server ();
  // Destructor

  int init (int control_fd,
            int data_fd,
            int rttag,
            int max_pkt_size);
  // initialize the Video Server.

  int register_handlers (void);
  // register the handlers with the reactor
  // and set the control_handler to the WAITING state

  static void on_exit_routine(void);

private:

  ACE_Reactor* reactor_;
  // alias Reactor ,points to ACE_Reactor::instance ()

  Video_Data_Handler* data_handler_;
  // Data Socket Event Handler

  Video_Control_Handler* control_handler_;
  // Control Socket Event Handler

  Video_Sig_Handler *sig_handler_;
  // signal handler for SIGALRM to periodically send the video frames
  // to the client
};

#endif /* MPEG_VIDEO_SERVER_H */
