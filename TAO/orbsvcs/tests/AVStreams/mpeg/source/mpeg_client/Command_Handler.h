// $Id$

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


// ============================================================================
//
// = LIBRARY
//    mpeg_client
// 
// = FILENAME
//    Command_Handler.h
//
// = DESCRIPTION
//     Defines the client side components of the A/V framework like
//     video stream endpoints ,video multi media device, video virtual device.
//
// = AUTHORS
//    Sumedh Mungee (sumedh@cs.wustl.edu)
//    Nagarajan Surendran (naga@cs.wustl.edu)
// 
// ============================================================================


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
#include "ace/High_Res_Timer.h"
#include "ace/Acceptor.h"



class Command_Handler;

class Gui_Acceptor
:public ACE_Acceptor <Command_Handler, ACE_SOCK_ACCEPTOR> 
{
public:
  Gui_Acceptor (Command_Handler *);

  virtual int make_svc_handler (Command_Handler *&sh);
private:
  Command_Handler *command_handler_;
};

class Video_Client_StreamEndPoint
  :public virtual TAO_Client_StreamEndPoint
{
  // = TITLE
  //    Defines a video client StreamEndpoint class
  // = DESCRIPTION
  //    Makes use of a connected dgram to receive data from the server
  //    side stream endpoint.
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
  // = TITLE
  //    Defines a audio client stream endpoint
  // = DESCRIPTION
  //    Uses a connected dgram to receive data from the audio server.
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
  // = TITLE
  //    Defines a class that is a client side Virtual device for video.
  // = Description
  //    Needs a pointer to the command handler object so that it can
  //    set the video control pointer of the command handler.
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
  // = TITLE
  //    Defines a client side audio virtual device
  // = DESCRIPTION
  //    Takes a pointer to a command handler object so that it can set
  //    the audio control object pointer of the command handler.
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

class Video_Endpoint_Reactive_Strategy_A 
  : public TAO_AV_Endpoint_Reactive_Strategy_A<Video_Client_StreamEndPoint,Video_Client_VDev,AV_Null_MediaCtrl> 
{
  // = TITLE
  //    Implementation of the Reactive Strategy for the client
  //    video MMDevice.
  // = DESCRIPTION
  //    Takes a command handler object pointer. It overrides the
  //    make_vdev and make_stream_endpoint hooks to create the video
  //    vdev and video streamendpoint with command handler object pointers.
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
  // = TITLE
  //    Implementation of the Reactive Strategy for the client audio
  //     MMDevice
  // = DESCRIPTION
  //    Takes a command handler object pointer. It overrides the
  //    make_vdev and make_stream_endpoint hooks to create the audio
  //    vdev and audio streamendpoint with command handler object pointers.
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
  : public virtual ACE_Event_Handler,
    public virtual ACE_Svc_Handler <ACE_SOCK_STREAM, ACE_NULL_SYNCH>
{
  // = TITLE
  //   Defines the event handler class for the client commands
  //
  // = DESCRIPTION
  //   This class makes use of a UNIX socket, over which
  //   it will receive commands from the GUI

public:

  Command_Handler (void);
  // Default constructor

  Command_Handler (ACE_HANDLE command_handle);
  // Construct this handler with a control (UNIX) handle

  ~Command_Handler (void);
  // Destructor

  int init (int argc,char **argv);
  // initialize the ORB

  int run (void);
  // Run the ORB event loop

  virtual int open (void *);
  // Perform the work of the SVC_HANDLER. Called by the acceptor
  // when a new connection shows up


  virtual int close (u_long);
  // Called if ACE_Svc_Handler is closed down unexpectedly.

  int handle_timeout (const ACE_Time_Value &,
                                  const void *arg);
  // handle the timeout 


  void stop_timer (void);
  // stop the internal timer

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
  // Initialize both the audio and video
  
  int init_java_av (ACE_HANDLE fd);
  // initialize both the audio and video reading the iors from java GUI thru a socket.

  int init_video_channel (char *phostname,char *videofile);
  // Initializes the video channel by bind the client and server video
  // mmdevices together and gets the video control object.

  int init_audio_channel (char *phostname,char *videofile);
  // Initializes the audio channel by bind the client and server audio
  // mmdevices together and gets the video control object.

  int stat_stream (CORBA::Char_out ch,CORBA::Long_out size) ;
  // statistics of this stream.  

  int  close (void) ;
  // close the audio and video connections

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
  // resolves the server video mmdevice and binds the local and server
  // mmdevices together.

  int connect_to_audio_server (char *address,
                               int *ctr_fd,
                               int *data_fd,
                               int *max_pkt_size);
  // resolves the server video mmdevice and binds the local and server
  // mmdevices together.

private:
  int busy_;
  // flag to indicate the state of the command handler

  char *audio_mmdevice_ior_;

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

  ACE_High_Res_Timer timer_;
  // timer to record the time taken for the play sequence.

  Gui_Acceptor acceptor_;
};


class Client_Sig_Handler 
  : public virtual ACE_Event_Handler
  // = TITLE
  //    Defines the signal handler class for the client timer processing
  //
  // = DESCRIPTION
  //    Checks if the video is ready and refreshes the display and
  //    also plays the audio packets in the timer processing.
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
