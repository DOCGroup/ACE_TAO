/* -*- c++ -*- */

#if !defined (_MPEG_SERVER_H)
#define      _MPEG_SERVER_H

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <ctype.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <time.h>
#include <sys/time.h>
#include "../include/common.h"
#include "../mpeg_server/proto.h"
#include "fileio.h"
#include "routine.h"
#include "com.h"
#include "ace/Get_Opt.h"

#include "ace/Acceptor.h"
#include "ace/Svc_Handler.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/INET_Addr.h"
#include "ace/SOCK_CODgram.h"

#ifdef NATIVE_ATM
#include "atmcom.h"
#endif

class Mpeg_Svc_Handler 
  : public virtual ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
    
{
public:
  // = Initialization method.
  Mpeg_Svc_Handler (ACE_Reactor * = 0);

  virtual int open (void *);
  // Perform the work of the SVC_HANDLER.
  
  virtual int handle_one_client (ACE_HANDLE = ACE_INVALID_HANDLE);
  // Handle one client connection.

  virtual int svc (void);
  // Thread method

  virtual int close (u_long);
  // Called if ACE_Svc_Handler is closed down unexpectedly.

  virtual int handle_timeout (const ACE_Time_Value &, const void *arg);
  // Handles acceptor timeouts.
private:

  ACE_SOCK_CODgram dgram_;
  // the UDP data socket

  ACE_INET_Addr server_data_addr_;
  // Data (UDP) Address of this server.
  
  ACE_INET_Addr client_data_addr_;
  // Data (UDP) Address of the client.

};

class Mpeg_Server
{
  // =TITLE
  //   Defines a class that abstracts the functionality of a mpeg
  //   video and audio server.
public:
  Mpeg_Server ();
  int init (int argc,
            char **argv);
  // Initialize the mpeg_server

  int run ();
  // Run the Mpeg_Server

  static  void int_handler (int sig);
  static  void on_exit_routine (void);
  static  void clear_child (int sig);
  // various signal handlers
  // we need them to be static 
  static void init_static (void);

  ~Mpeg_Server ();
private:

  ACE_Acceptor <Mpeg_Svc_Handler, ACE_SOCK_ACCEPTOR> acceptor_;
  // the acceptor

  ACE_INET_Addr server_control_addr_;
  // Control (TCP) Address of this server.

  int parse_args (int argcs,
                  char **argv);
  // parse the arguments

  int set_signals (void);

  // data members
  int inet_port_;

  char *unix_port_;
  char* atm_port_;

  struct FdTable
  {
    int fd;   /* -1 -- free item, >=0 -- fd value */
    int type;
    unsigned stime;  /* setup time, in seconds */
    int state;
  } * fdTable_;

  int size_;
};

#endif // _MPEG_SERVER_H
