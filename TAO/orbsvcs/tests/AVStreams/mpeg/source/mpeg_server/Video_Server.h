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
#include "Globals.h"
#include "../mpeg_shared/fileio.h"
#include "../mpeg_shared/com.h"
#include "../include/common.h"
#include "../mpeg_shared/routine.h"
#include "../mpeg_shared/sendpt.h"
#include "proto.h"
#include "Video_Control_State.h"

// Function Prototypes
// %% put them in some class maybe ?

int FBread (char *buf, int size);
int INITvideo (void);
int PLAYliveVideo(PLAYpara * para);
void ComputeFirstSendPattern(float limit);
int FrameToGroup (int * frame);
int SendReferences(int group, int frame);
int send_to_network(int timeToUse);
void StartPlayLiveVideo (void);
void GetFeedBack (void);
int SendPicture (int *frame);
int play_send (int debug=0);
int fast_play_send (void);

class Video_Control_Handler 
  : public virtual ACE_Event_Handler
{

public:

  Video_Control_Handler (int video_control_fd);
  // Construct this handler with a data fd

  virtual int handle_input (ACE_HANDLE fd = ACE_INVALID_HANDLE);
  // Called when input events occur (e.g., connection or data).

  virtual ACE_HANDLE get_handle (void) const;
  // Returns the handle used by the event_handler.
  
  Video_Control_State *get_state (void);
  // Accessor for the state_ 

  void change_state (Video_Control_State *state);
  // Used to change the state

private:
  Video_Control_State *state_;
  // State pattern - pointer to abstract State object

  ACE_HANDLE control_handle_;

};

class Video_Control_Handler_Instance
{
public:
  Video_Control_Handler_Instance (void);
  
  void set_video_control_handler (Video_Control_Handler *h);
  
  Video_Control_Handler *get_video_control_handler (void);
private:
  Video_Control_Handler *video_control_handler_;
};

typedef ACE_TSS_Singleton <Video_Control_Handler_Instance, ACE_SYNCH_MUTEX> VIDEO_CONTROL_HANDLER_INSTANCE; 

class Video_Sig_Handler 
  : public virtual ACE_Event_Handler
{
public:
  Video_Sig_Handler (Video_Control_Handler *vch);

  virtual ACE_HANDLE get_handle (void) const;

  int register_handler (void);
  // this will register this sig_handler
  // with the reactor

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


class Video_Data_Handler 
  : public virtual ACE_Event_Handler
{

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



// %% this class needs a dtor which deletes the
// 3 handlers - data, control, and sig
class Video_Server
{
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

  int run (void);
  // register the handlers with the reactor
  // and set the control_handler to the WAITING state

  static int SendPacket (int shtag,int gop,int frame,int timeToUse);
  static int CmdRead(char *buf, int psize);
  static void CmdWrite(char *buf, int size);
  static void on_exit_routine(void);
  static PLAYpara para;

  static  int position (void);
  static  int step_video (void);
  static  int fast_forward (void);
  static  int fast_backward (void);
  static  int init_play (void);
  //static   int close (void);
  static  int stat_stream (void);
  static  int stat_sent (void);
  static  int fast_video_play (void);

private:

  ACE_Reactor* reactor_;
  // Reactor ,points to ACE_Reactor::instance ()

  Video_Data_Handler* data_handler_;
  // Data Socket Event Handler

  Video_Control_Handler* control_handler_;
  // Control Socket Event Handler

  Video_Sig_Handler *sig_handler_;
  // signal handler for SIGALRM to periodically send the video frames
  // to the client

};

#endif // MPEG_VIDEO_SERVER_H
