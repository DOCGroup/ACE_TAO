/* -*- C++ -*- */

/* $Id$  */


#if defined (MPEG_VIDEO_SERVER_H)
#define MPEG_VIDEO_SERVER_H

#include "ace/Event_Handler.h"

class Video_Server
{
public:
  Video_Server (int control_fd,
                int data_fd,
                int rttag,
                int max_pkt_size);
  int run (void);

protected:

  int position (void);
  int step_video (void);
  int fast_forward (void);
  int fast_backward (void);
  int play (void);
  int close (void);
  int stat_stream (void);
  int stat_sent (void);

};


class Video_Data_Handler 
  : public virtual Event_Handler
{

public:
  Video_Data_Handler (int video_data_fd);
  // Construct this handler with a data fd

  virtual int handle_input (ACE_HANDLE fd = ACE_INVALID_HANDLE);
  // Called when input events occur (e.g., connection or data).
private:
  int data_handle_;

};

class Video_Control_Handler 
  : public virtual Event_Handler
{

public:
  Video_Control_Handler (int video_control_fd);
  // Construct this handler with a data fd

  virtual int handle_input (ACE_HANDLE fd = ACE_INVALID_HANDLE);
  // Called when input events occur (e.g., connection or data).

private:
  int control_handle_;

};







#endif
