// $Id$

#ifndef TAO_AV_BENCH_CHILD_H
#define TAO_AV_BENCH_CHILD_H

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
#include "ace/SOCK_Connector.h"
#include "client.h"


class ttcp_Stream_Handler : public virtual ACE_Event_Handler
{
  // = TITLE
  //   Defines the event handler class for the Video Control.
  //
  // = DESCRIPTION
  //   This class makes use of a TCP socket.It contains a pointer to
  //   the current state which is implemented using the state pattern.
public:

  ttcp_Stream_Handler (int ttcp_Stream_fd);
  // Construct this handler with a control (TCP) fd
  // %% use sock stream instead of fd

  virtual int handle_input (ACE_HANDLE fd = ACE_INVALID_HANDLE);
  // Called when input events occur (e.g., connection or data).

  virtual ACE_HANDLE get_handle (void) const;
  // Returns the handle used by the event_handler.
private:
  ACE_HANDLE control_handle_;
};

class Bench_Server_StreamEndPoint
  :public  TAO_Server_StreamEndPoint
{
public:
  Bench_Server_StreamEndPoint (void);

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
  ACE_SOCK_Connector connector_;
  ACE_SOCK_Stream tcp_stream_;
  ttcp_Stream_Handler *stream_handler_;
};


#endif /* TAO_AV_BENCH_CHILD_H */
