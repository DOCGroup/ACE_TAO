/* -*- C++ -*- */
// $Id$

// NOTE: If you encounter trouble resolving the Naming Service, try
// running the Naming Service and the server in the same window. [MJB]

#ifndef TAO_AV_BENCH_CLIENT_H
#define TAO_AV_BENCH_CLIENT_H

#include "ace/Reactor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Event_Handler.h"
#include "ace/ARGV.h"
#include "ace/Get_Opt.h"
#include "tao/TAO.h"
#include "orbsvcs/CosNamingC.h"
#include "ace/SOCK_Dgram.h"
#include "orbsvcs/AV/AVStreams_i.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Synch.h"
#include "ace/High_Res_Timer.h"
#include "orbsvcs/Naming/Naming_Utils.h"

#define TCP_PORT 5000

class Client_StreamEndPoint
  : public  TAO_Client_StreamEndPoint
{
  // = TITLE
  //    Defines a client StreamEndpoint class
  // = DESCRIPTION
  //    Makes use of a connected dgram to receive data from the server
  //    side stream endpoint.
public:
  Client_StreamEndPoint (void);

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

};

class ttcp_Client_StreamEndPoint;

class ttcp_Acceptor
  :public virtual ACE_Acceptor <ttcp_Client_StreamEndPoint,ACE_SOCK_ACCEPTOR>
{
public:
  ttcp_Acceptor (ttcp_Client_StreamEndPoint *endpoint);

  virtual int make_svc_handler (ttcp_Client_StreamEndPoint *&sh);
private:
  ttcp_Client_StreamEndPoint *endpoint_;
};

class Client;

class ttcp_Client_StreamEndPoint
  :public Client_StreamEndPoint,
   public virtual ACE_Svc_Handler <ACE_SOCK_STREAM, ACE_NULL_SYNCH>
{
public:
  ttcp_Client_StreamEndPoint (Client *client =0);
  // constructor
  virtual CORBA::Boolean handle_preconnect (AVStreams::flowSpec &the_spec);
  // called before connecting

  virtual CORBA::Boolean handle_postconnect (AVStreams::flowSpec &the_spec);
  // called after connecting

  virtual int open (void *);
  // called when server connects to us.
private:
  ttcp_Acceptor acceptor_;
  // The Acceptor.

  Client *client_;
};
class ttcp_Endpoint_Reactive_Strategy_A
  : public TAO_AV_Endpoint_Reactive_Strategy_A<ttcp_Client_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl>
{
public:
  ttcp_Endpoint_Reactive_Strategy_A (TAO_ORB_Manager *orb_manager,
                                     Client *client_ptr);
  // constructor . The orb manager is needed for the TAO_AV_Endpoint_Reactive_Strategy_A.

  virtual int make_stream_endpoint (ttcp_Client_StreamEndPoint *& endpoint);
  // hook to make our streamendpoint taking a Client pointer
private:
  Client *client_;
  // pointer to command handler object
};


class Globals
{
  // = TITLE
  //     Globals class to be used as a singleton.
  //
  // = DESCRIPTION
  //     This is used both by the client.
public:
  Globals (void);
  // constructor.

  int parse_args (int argc,char **argv);
  //parse the arguments.

  ACE_Barrier *barrier_;
  //  barrier for the client threads.

  int ready_;
  // ready flag used by the high priority thread to wake up the low
  // priority threads after it's parsed the arguments.

  ACE_SYNCH_MUTEX ready_mtx_;
  // mutex for the condition variable.

  ACE_Condition<ACE_SYNCH_MUTEX> ready_cond_;
  // condition variable for the low priority threads to wait
  //until the high priority thread is done with the arguments parsing.


  int block_size_;
  // size of the block to be sent in Kilobytes.

  int number_;
  // number of times to send the block

  char *hostname_;
  // hostname to bind to.

  u_short port_;
  //  port number to bind to.

  enum strategy {TTCP_REACTIVE=0,DGRAM_REACTIVE =1};
  strategy strategy_;
  // strategy to be used for MMDevice.

  int thread_count_;
  // No. of threads.
};

typedef ACE_Singleton <Globals,ACE_Thread_Mutex> GLOBALS;

class Client : public ACE_Task<ACE_SYNCH>
// one of these per client thread
{
public:
  Client (int argc, char **argv, int task_id);

  virtual int svc (void);

  void set_stream (ACE_SOCK_Stream & stream);

private:
  int bind_to_server (void);

  int establish_stream (void);

  TAO_ORB_Manager orb_manager_;

  TAO_Naming_Client my_name_client_;

  ttcp_Endpoint_Reactive_Strategy_A ttcp_reactive_strategy_;

  TAO_AV_Endpoint_Reactive_Strategy_A<Client_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl> reactive_strategy_;

  AVStreams::MMDevice_var server_mmdevice_;

  TAO_MMDevice *client_mmdevice_;

  TAO_StreamCtrl streamctrl_;
  // Video stream controller

  int argc_;
  char **argv_;

  int task_id_;
  // id of this task.

  ACE_SOCK_Stream stream_;
};

#endif /* TAO_AV_BENCH_CLIENT_H */
