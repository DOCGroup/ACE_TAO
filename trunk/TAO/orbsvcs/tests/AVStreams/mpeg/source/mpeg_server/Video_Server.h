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
#include "mpeg_server/Video_Control_i.h"

class Video_Control_i;

class Video_Sig_Handler 
  : public virtual ACE_Event_Handler
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
  Video_Sig_Handler ();

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

  Video_Control_i *vci_;
  // Pointer to the Video_Control_i for accessing
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
  Video_Data_Handler ();
  // Constructor

  virtual int handle_input (ACE_HANDLE fd = ACE_INVALID_HANDLE);
  // Called when data shows up.

  virtual ACE_HANDLE get_handle (void) const;
  // Get the handle used by this event handler

private:
  Video_Control_i *vci_;
  
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

  ~Video_Server ();
  // Destructor

  int init (int argc,
            char **argv,
            CORBA::Environment &env);
  // initialize the Video Server.


  int run (CORBA::Environment &env);
  // Runs the ORB event loop       

  static void on_exit_routine(void);

private:
  
  int initialize_orb (int argc,
                      char **argv,
                      CORBA::Environment &env);
  // Initialize the orb using the orb_manager_
  // Also registers the video_control_ object in the poa
  // and in the naming service
  TAO_ORB_Manager orb_manager_;
  // the TAO ORB manager.

};

#endif /* MPEG_VIDEO_SERVER_H */
