/* -*- C++ -*- */

/* $Id$  */


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


// Function Prototypes

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
int play_send (void);

class Video_Time_Handler : public virtual ACE_Event_Handler
{
public:
  virtual int handle_timeout (const ACE_Time_Value &tv,
                              const void *arg);
};


class Video_Data_Handler 
  : public virtual ACE_Event_Handler
{

public:
  Video_Data_Handler (int video_data_fd);
  // Construct this handler with a data fd

  virtual int handle_input (ACE_HANDLE fd = ACE_INVALID_HANDLE);
  // Called when input events occur (e.g., connection or data).

  virtual ACE_HANDLE get_handle (void) const;
  // Get the handle used by this event handler

private:
  ACE_HANDLE data_handle_;

};

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

private:

  ACE_HANDLE control_handle_;

};

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
  int init (int control_fd,
            int data_fd,
            int rttag,
            int max_pkt_size);
  // initialize the Video Server.
  int run (void);
static  int read_cmd (void);
  // Read a command and demux it to various functions.
  static int SendPacket (int shtag,int gop,int frame,int timeToUse);
  static int CmdRead(char *buf, int psize);
  static void CmdWrite(char *buf, int size);
  static void on_exit_routine(void);
  static PLAYpara para;

protected:
static  int position (void);
static  int step_video (void);
static  int fast_forward (void);
static  int fast_backward (void);
static  int play (void);
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

};

#endif // MPEG_VIDEO_SERVER_H
