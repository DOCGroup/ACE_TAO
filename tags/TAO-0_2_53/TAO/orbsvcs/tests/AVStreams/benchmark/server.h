/* -*- C++ -*- */
// $Id$


// NOTE: If you encounter trouble resolving the Naming Service, try
// running the Naming Service and the server in the same window. [MJB]


#ifndef TAO_AV_BENCH_SERVER_H
#define TAO_AV_BENCH_SERVER_H


#include "ace/Get_Opt.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Acceptor.h"
#include "ace/Svc_Handler.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/INET_Addr.h"
#include "ace/SOCK_CODgram.h"
#include "ace/Select_Reactor.h"
#include "orbsvcs/orbsvcs/Naming/Naming_Utils.h"
#include "orbsvcs/AV/AVStreams_i.h"
#include "orbsvcs/AV/Endpoint_Strategy.h"
#include "client.h"
#include "child.h"

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
  ~AV_Server_Sig_Handler (void);
  // Destructor

private:
  ACE_HANDLE handle_;
  // dummy handle for the sig handler.
  ACE_Sig_Set sig_set;
};

class Server
{
public:
  Server (void);
  // Default constructor

  int init (int argc,
            char **argv,
            CORBA::Environment& env);

  int run (CORBA::Environment& env);

  ~Server (void);
private:
  TAO_ORB_Manager orb_manager_;
  // the TAO ORB manager.

  TAO_Naming_Client my_name_client_;
  // The TAO Naming server

  //CosNaming::NamingContext_var naming_context_;
  // The root naming context of the naming service

  //  AV_Server_Sig_Handler signal_handler_;

  ACE_Process_Options process_options_;
  // The process options for the process to be spawned by the process strategy

  //  TAO_AV_Endpoint_Process_Strategy_B process_strategy_;
  // The proces strategy for the video.

  TAO_AV_Endpoint_Reactive_Strategy_B <Bench_Server_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl> reactive_strategy_;
  // Reactive strategy

  TAO_MMDevice *mmdevice_;
  // The video server multimedia device
};

#endif /* TAO_AV_BENCH_SERVER_H */
