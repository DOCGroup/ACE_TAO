/* -*- C++ -*- */
/* $Id$ */
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

#if !defined (AV_COMMAND_HANDLER_H)
#define AV_COMMAND_HANDLER_H

#include "ace/Reactor.h"
#include "ace/Event_Handler.h"
#include "ace/ARGV.h"
#include "tao/TAO.h"
#include "mpeg_shared/Video_ControlC.h"
#include "orbsvcs/CosNamingC.h"
#include "ace/SOCK_Dgram.h"
#include "ace/SOCK_Connector.h"
#include "mpeg_shared/Audio_ControlC.h"
#include "orbsvcs/AV/AVStreams_i.h"


class Command_Handler;

class Video_Client_StreamEndPoint
  :public virtual TAO_Client_StreamEndPoint
{
public:
  Video_Client_StreamEndPoint (void);

  Video_Client_StreamEndPoint (Command_Handler *command_handler);
  // constructor

  virtual int handle_open (void);
  // called when streamendpoint is instantiated

  virtual int handle_close (void);
  // called when streamendpoint is being destructed

  virtual CORBA::Boolean handle_preconnect (AVStreams::flowSpec &the_spec);
  // called before connecting

  virtual CORBA::Boolean handle_postconnect (AVStreams::flowSpec &the_spec);
  // called after connecting
  
  virtual int handle_start (const AVStreams::flowSpec &the_spec,  
                             CORBA::Environment &env) ;

  virtual int handle_stop (const AVStreams::flowSpec &the_spec,
                            CORBA::Environment &env) ;
  
  virtual int handle_destroy (const AVStreams::flowSpec &the_spec,  
                               CORBA::Environment &env);
  
  virtual ACE_HANDLE get_handle (void);
  // gets the underlying socket descriptor from the SOCK_Dgram

private:
  ACE_SOCK_Dgram dgram_;
  // The datagram used for streaming.
  Command_Handler *command_handler_;
};

class Audio_Client_StreamEndPoint
  :public virtual TAO_Client_StreamEndPoint
{
public:
  Audio_Client_StreamEndPoint (void);

  Audio_Client_StreamEndPoint (Command_Handler *command_handler);
  // constructor

  virtual int handle_open (void);
  // called when streamendpoint is instantiated

  virtual int handle_close (void);
  // called when streamendpoint is being destructed

  virtual CORBA::Boolean handle_preconnect (AVStreams::flowSpec &the_spec);
  // called before connecting

  virtual CORBA::Boolean handle_postconnect (AVStreams::flowSpec &the_spec);
  // called after connecting
  
  virtual int handle_start (const AVStreams::flowSpec &the_spec,  
                             CORBA::Environment &env) ;

  virtual int handle_stop (const AVStreams::flowSpec &the_spec,
                            CORBA::Environment &env) ;
  
  virtual int handle_destroy (const AVStreams::flowSpec &the_spec,  
                               CORBA::Environment &env);
  
  virtual ACE_HANDLE get_handle (void);
  // gets the underlying socket descriptor from the SOCK_Dgram

private:
  ACE_SOCK_Dgram dgram_;
  // The datagram used for streaming.
  Command_Handler *command_handler_;
};


class Video_Client_VDev
  : public virtual TAO_VDev
{
public:
  Video_Client_VDev (void);
  Video_Client_VDev (Command_Handler *command_handler);

protected:
  CORBA::Boolean set_media_ctrl (CORBA::Object_ptr media_ctrl,
                                 CORBA::Environment& env);

private:
  Video_Control_ptr video_control_;
  // The video controller

  Command_Handler *command_handler_;
  // pointer to the command handler object
};


class Audio_Client_VDev
  : public virtual TAO_VDev
{
public:
  Audio_Client_VDev (void);
  Audio_Client_VDev (Command_Handler *command_handler);

protected:
  CORBA::Boolean set_media_ctrl (CORBA::Object_ptr media_ctrl,
                                 CORBA::Environment& env);

private:
  Audio_Control_ptr audio_control_;
  // The Audio controller

  Command_Handler *command_handler_;
  // pointer to the command handler object
};

// class Video_Client_MMDevice 
//   :public virtual TAO_MMDevice
// {
// public:
//   Video_Client_MMDevice (TAO_ORB_Manager *orb_manager);
//   // Constructor taking pointer to an ORB Manager

//   AVStreams::VDev_ptr vdev (void);
//   // Return the virtual device pointer

//   Video_Client_StreamEndPoint *streamendpoint (void);
//   // Return the streamendpoint pointer
  
//   virtual AVStreams::StreamEndPoint_A_ptr  create_A (AVStreams::StreamCtrl_ptr the_requester, 
//                                                      AVStreams::VDev_out the_vdev, 
//                                                      AVStreams::streamQoS &the_qos, 
//                                                      CORBA::Boolean_out met_qos, 
//                                                      char *&named_vdev, 
//                                                      const AVStreams::flowSpec &the_spec,  
//                                                      CORBA::Environment &env);
//   // Called by StreamCtrl to create a "A" type streamandpoint and vdev
// private:
//   TAO_ORB_Manager *orb_manager_;
//   // Pointer to the  ORB Manager

//   Video_Client_StreamEndPoint video_streamendpoint_;
//   // The client side of the video stream.
//   TAO_VDev video_vdev_;
//   // Video virtual device
// };

class Video_Endpoint_Reactive_Strategy_A 
  : public TAO_AV_Endpoint_Reactive_Strategy_A<Video_Client_StreamEndPoint,Video_Client_VDev,AV_Null_MediaCtrl> 
{
public:
  Video_Endpoint_Reactive_Strategy_A (TAO_ORB_Manager *orb_manager,
                                      Command_Handler *command_handler);
  // constructor . The orb manager is needed for the TAO_AV_Endpoint_Reactive_Strategy_A.

  virtual int make_vdev (Video_Client_VDev *&vdev);
  // hook to make our Vdev with the pointer to command handler.
  virtual int make_stream_endpoint (Video_Client_StreamEndPoint *& endpoint);
  // hook to make our streamendpoint taking a command handler pointer
private:
  Command_Handler *command_handler_;
  // pointer to command handler object

};

class Audio_Endpoint_Reactive_Strategy_A 
  : public TAO_AV_Endpoint_Reactive_Strategy_A<Audio_Client_StreamEndPoint,Audio_Client_VDev,AV_Null_MediaCtrl> 
{
public:
  Audio_Endpoint_Reactive_Strategy_A (TAO_ORB_Manager *orb_manager,
                                      Command_Handler *command_handler);
  // constructor . The orb manager is needed for the TAO_AV_Endpoint_Reactive_Strategy_A.

  virtual int make_vdev (Audio_Client_VDev *&vdev);
  // hook to make our Vdev with the pointer to command handler.
  virtual int make_stream_endpoint (Audio_Client_StreamEndPoint *& endpoint);
  // hook to make our streamendpoint taking a command handler pointer
private:
  Command_Handler *command_handler_;
  // pointer to command handler object

};
  
class Command_Handler 
  : public virtual ACE_Event_Handler
{
  // = TITLE
  //   Defines the event handler class for the client commands
  //
  // = DESCRIPTION
  //   This class makes use of a UNIX socket, over which
  //   it will receive commands from the GUI

public:

  Command_Handler (ACE_HANDLE command_handle);
  // Construct this handler with a control (UNIX) handle

  ~Command_Handler (void);
  // Destructor

  int init (int argc,char **argv);
  // initialize the ORB

  int run (void);
  // Run the ORB event loop

  void set_video_data_handle (ACE_HANDLE data_fd);
  // sets the data handle (UDP) of the command handler

  void set_video_control (Video_Control_ptr video_control);
  // called to set the video control object pointer of the comand handler.

  void set_audio_data_handle (ACE_HANDLE data_fd);
  // sets the data handle (UDP) of the command handler

  void set_audio_control (Audio_Control_ptr video_control);
  // called to set the video control object pointer of the comand handler.

  int get_video_control (void);
  // Gets the video control reference thru the property service from
  // the video server virtual device

  int resolve_video_reference (void);
  // Resolve the video control reference.

  int resolve_audio_reference (void);
  // Resolve the audio control reference.

  virtual int handle_input (ACE_HANDLE fd = ACE_INVALID_HANDLE);
  // Called when input events occur (e.g., connection or data).

  virtual ACE_HANDLE get_handle (void) const;
  // Returns the handle used by the event_handler.
  
  int init_av (void);
  
  int init_video_channel (char *phostname,char *videofile);

  int init_audio_channel (char *phostname,char *videofile);

  int stat_stream (CORBA::Char_out ch,
                              CORBA::Long_out size) ;
  
  int  close (void) ;
  
  int  stat_sent (void) ;
  
  int  fast_forward (void);
  
  int fast_backward (void);
  
  int step (void);
  
  int play (int flag,
                       CORBA::Environment& env);
                      
  int position_action (int operation_tag);

  int position (void);
  
  int position_release (void);

  int volume (void);

  int balance (void);

  int speed (void);
  
  int stop (void);

  int stop_playing (void);

  int connect_to_video_server (char *address,
                               int *ctr_fd,
                               int *data_fd,
                               int *max_pkt_size);

  int connect_to_audio_server (char *address,
                               int *ctr_fd,
                               int *data_fd,
                               int *max_pkt_size);


private:
  ACE_SOCK_Dgram video_dgram_;
  // UDP socket on which to send/recv data
  
  ACE_SOCK_Dgram audio_dgram_;
  // Audio UDP socket

  ACE_HANDLE video_data_handle_;
  // UDP socket for video

  ACE_HANDLE audio_data_handle_;
  // UDP socket for audio

  ACE_HANDLE command_handle_;
  // The fd for the UNIX command socket

  TAO_ORB_Manager orb_manager_;
  // the ORB manager

  Video_Control_ptr video_control_;
  // Video Control CORBA object

  AVStreams::MMDevice_var video_server_mmdevice_;
  // The video server multimedia device

  Video_Endpoint_Reactive_Strategy_A video_reactive_strategy_;
  // Strategy for creating stream endpoints

  TAO_MMDevice video_client_mmdevice_;
  // The video client multimedia device

  TAO_StreamCtrl video_streamctrl_;
  // Video stream controller 

  Audio_Control_ptr audio_control_;
  // Audio Control CORBA object

  AVStreams::MMDevice_var audio_server_mmdevice_;
  // The audio server multimedia device

  Audio_Endpoint_Reactive_Strategy_A audio_reactive_strategy_;
  // Strategy for creating stream endpoints

  TAO_MMDevice audio_client_mmdevice_;
  // The audio client multimedia device

  TAO_StreamCtrl audio_streamctrl_;
  // audio stream controller
};


class Client_Sig_Handler 
  : public virtual ACE_Event_Handler
  // = TITLE
  //   Defines the signal handler class for the client timer processing
  //
  // = DESCRIPTION
  //     %%
{
public:
  Client_Sig_Handler (Command_Handler *command_handler);
  // We need the command handler to call close ()

  ~Client_Sig_Handler (void);
  // Destructor

  virtual ACE_HANDLE get_handle (void) const;

  int register_handler (void);
  // this will register this sig_handler
  // with the reactor for SIGCHLD,SIGTERM,SIGINT

  virtual int shutdown (ACE_HANDLE, 
                        ACE_Reactor_Mask);
  // handles the reactor shutdown

  virtual int handle_input (ACE_HANDLE);
  // handle input on the dummy handle.

  virtual int handle_signal (ACE_HANDLE signum,
                             siginfo_t * = 0,
                             ucontext_t* = 0);
  // handles SIGALRM

  void TimerHandler (int signum);

  void stop_timer (void);

  void DisplayPicture (void);
  // display the picture.

  void TimerProcessing (void);

  void PlayAudioOnly (void);

private:
  ACE_HANDLE handle_;
  // dummy handle for the sig handler.

  ACE_Sig_Set sig_set;
  // the signal set

  Command_Handler *command_handler_;
  // We need the command handler to call close ()

};

#endif /* AV_COMMAND_HANDLER_H */
