/* -*- C++ -*- */
//$Id$

#ifndef _AUDIO_SERVER_H
#define _AUDIO_SERVER_H

#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include "include/common.h"
#include "mpeg_shared/fileio.h"
#include "mpeg_shared/routine.h"
#include "mpeg_shared/com.h"
#include "server_proto.h"

#include "ace/SOCK_CODgram.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/SOCK_Stream.h"
#include "tao/TAO.h"
#include "mpeg_shared/Audio_ControlS.h"
#include "orbsvcs/CosNamingC.h"
#include "Globals.h"

class Audio_Sig_Handler
  : public virtual ACE_Event_Handler
{
  // = TITLE
  //    Defines the signal handler for the audio process.
public:
  Audio_Sig_Handler (Audio_Global *audio_global);
  // Constructor.

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
  Audio_Global *audio_global_;
};

class Audio_Data_Handler : public virtual ACE_Event_Handler
{
public:
  Audio_Data_Handler (ACE_HANDLE data_fd,
                      Audio_Global *audio_global);

  // Constructor

  virtual int handle_input (ACE_HANDLE fd = ACE_INVALID_HANDLE);
  // Called when data shows up.

  virtual ACE_HANDLE get_handle (void) const;
  // Get the handle used by this event handler

private:
  ACE_HANDLE data_fd_;
  Audio_Global *audio_global_;
};

// The stream endpoint
class Audio_Server_StreamEndPoint :
  public virtual TAO_Server_StreamEndPoint
{
public:
  virtual int handle_open (void) ;
  // called when streamendpoint is instantiated

  virtual int handle_close (void) ;
  // called when streamendpoint is being destructed

  virtual int handle_stop (const AVStreams::flowSpec &the_spec,
                            CORBA::Environment &env) ;
  // Application needs to define this

  virtual int handle_start (const AVStreams::flowSpec &the_spec,
                             CORBA::Environment &env) ;
  // Application needs to define this


  virtual int handle_destroy (const AVStreams::flowSpec &the_spec,
                               CORBA::Environment &env) ;
  // Application needs to define this

  virtual CORBA::Boolean handle_connection_requested (AVStreams::flowSpec &the_spec,
                                                      CORBA::Environment &env) ;

private:
  ACE_SOCK_CODgram dgram_;
};

#endif /*_AUDIO_SERVER_H */
