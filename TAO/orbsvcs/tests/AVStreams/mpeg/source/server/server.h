/* -*- c++ -*- */
/* $Id$ */

// ============================================================================
//
// = LIBRARY
//    server
// 
// = FILENAME
//    server.h
//
// = DESCRIPTION
//   @@ Please add a synopsis of this file.
//
// = AUTHORS
//    Sumedh Mungee (sumedh@cs.wustl.edu)
//    Nagarajan Surendran (naga@cs.wustl.edu)
// 
// ============================================================================

#if !defined (TAO_AV_SERVER_H)
#define TAO_AV_SERVER_H

#include "ace/Get_Opt.h"
#include "ace/Acceptor.h"
#include "ace/Svc_Handler.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/INET_Addr.h"
#include "ace/SOCK_CODgram.h"
#include "ace/Select_Reactor.h"

#include "include/common.h"         
#include "mpeg_server/server_proto.h"   
#include "mpeg_shared/fileio.h"         
#include "mpeg_shared/routine.h"        
#include "mpeg_shared/com.h"            
#include "mpeg_server/Video_Control_i.h"
#include "orbsvcs/orbsvcs/Naming/Naming_Utils.h"

#if defined (NATIVE_ATM)
#include "atmcom.h"
#endif /* NATIVE_ATM */

#include "mpeg_server/Video_Server.h"
#include "mpeg_server/Audio_Server.h"

// Forward declaration.
class AV_Svc_Handler;

// @@ We should probably try to replace the ACE_Acceptor with the
// ACE_Strategy_Acceptor using the ACE_Process_Strategy... 
//   : public virtual ACE_Acceptor <AV_Svc_Handler, ACE_SOCK_ACCEPTOR>
// {
//   // = TITLE
//   //   This defines a AV_Acceptor which is an Acceptor and
//   //   overrides the make_svc_handler method of the Acceptor.
//   //
//   // = DESCRIPTION
//   //   This class overrides the Acceptor's make_svc_handler so that a
//   //   AV_Svc_Handler can be created with a non-default constructor.
// public:  
//   virtual int make_svc_handler (AV_Svc_Handler *&sh);
//   // Create a new <AV_Svc_Handler> passing 'this' to the service
//   // handler.
// };

class AV_Svc_Handler 
  : public virtual ACE_Svc_Handler <ACE_SOCK_STREAM, ACE_NULL_SYNCH>
{
  // = TITLE
  //   This class defines the service handler for a new connection to
  //   the AV_Server.
  //
  // = DESCRIPTION
  //   This calls the handle_connection method for a new connection
  //   which demuxes the connection to a video or audio server
  //   depending on the connection request.
public:
  // = Initialization method.
  AV_Svc_Handler (ACE_Thread_Manager *t = 0);

  virtual int open (void *);
  // Perform the work of the SVC_HANDLER. Called by the acceptor
  // when a new connection shows up
  
  virtual int handle_connection (ACE_HANDLE = ACE_INVALID_HANDLE);
  // Handle one client connection.

  virtual int svc (void);
  // Thread method

  virtual int close (u_long);
  // Called if ACE_Svc_Handler is closed down unexpectedly.

  int handle_timeout (const ACE_Time_Value &,
                                  const void *arg);
  // handle the timeout 

private:

  ACE_SOCK_CODgram dgram_;
  // the UDP data socket

  ACE_INET_Addr server_data_addr_;
  // Data (UDP) Address of this server.
  
  ACE_INET_Addr client_data_addr_;
  // Data (UDP) Address of the client.

  Video_Server *vs_;
  // @@ need a similar component for audio!
  
  Audio_Server *as_;
  // the audio server.
};

class AV_Server_Sig_Handler 
  : public virtual ACE_Event_Handler
{
public:
  AV_Server_Sig_Handler (void);

  virtual ACE_HANDLE get_handle (void) const;

  int register_handler (void);
  // this will register this sig_handler
  // with the reactor for SIGCHLD,SIGTERM,SIGINT

  virtual int shutdown (ACE_HANDLE, 
                        ACE_Reactor_Mask);

  virtual int handle_input (ACE_HANDLE);
  // handle input on the dummy handle.

  virtual int handle_signal (ACE_HANDLE signum,
                             siginfo_t * = 0,
                             ucontext_t* = 0);
  // handles the SIGCHLD,SIGTERM,SIGINT for the parent process i.e
  // the main thread..

  void int_handler (int sig);
  // Signal handler function for SIGTERM,SIGBUS,SIGINT

  void clear_child (int sig);
  // Signal handler function for SIGCHLD

  ~AV_Server_Sig_Handler (void);
  // Destructor

private:
  ACE_HANDLE handle_;
  // dummy handle for the sig handler.
  ACE_Sig_Set sig_set;
};

typedef ACE_Acceptor <AV_Svc_Handler, ACE_SOCK_ACCEPTOR> AV_ACCEPTOR;

class AV_Server
{
  // = TITLE
  //   Defines a class that abstracts the functionality of a 
  //   video and audio server.
  //
  // = DESCRIPTION
  //   Using the class is as simple as calling init () first and then
  //   run. It uses an acceptor with the default ACE_Reactor::instance ().
public:
  AV_Server (void);
  // constructor

  int init (int argc,
            char **argv,
            CORBA::Environment& env);
  // Initialize the AV_Server

  int run (CORBA::Environment& env);
  // Run the AV_Server

  AV_ACCEPTOR *acceptor (void);
  // Returns the acceptor.

  static void on_exit_routine (void);
  // Routine called when this process exits.

  static pid_t current_pid_;
  // %% the pid the server is currently waiting on

  static int done_;
  // %% the done flag, set by the signal handler

  ~AV_Server (void);
  // Destructor
  //private:
private:
  // @@ Why are some of these data members pointers and others
  // objects?  Shouldn't we be consistent here?

  // TAO_Naming_Server naming_server_;
  // the TAO naming server

  TAO_ORB_Manager orb_manager_;
  // the TAO ORB manager.

  // %% need to comment!!
  //  typedef ACE_Strategy_Acceptor <AV_Svc_Handler, ACE_SOCK_ACCEPTOR> AV_ACCEPTOR;
  //  typedef ACE_Thread_Strategy <AV_Svc_Handler> AV_THREAD_STRATEGY;

  //  AV_THREAD_STRATEGY thread_strategy_;
  // the strategy
  
  AV_ACCEPTOR acceptor_;
  // the acceptor

  AV_Server_Sig_Handler signal_handler_;
  // Signal handler for SIGCHLD,SIGINT,SIGTERM,SIGBUS

  ACE_INET_Addr server_control_addr_;
  // Control (TCP) Address of this server.

  int parse_args (int argcs,
                  char **argv);
  // Parse the arguments.
};

typedef ACE_Singleton<AV_Server,ACE_Null_Mutex> AV_SERVER;

#endif /* TAO_AV_SERVER_H */
