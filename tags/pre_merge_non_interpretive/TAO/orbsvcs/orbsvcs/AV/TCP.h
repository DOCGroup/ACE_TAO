/* -*- C++ -*- */

// $Id$
// ============================================================================
//
// = LIBRARY
//   ORBSVCS AVStreams
//
// = FILENAME
//   TCP.h
//
// = AUTHOR
//    Nagarajan Surendran <naga@cs.wustl.edu>
//
//
// ============================================================================

#ifndef TAO_AV_TCP_H
#include "ace/pre.h"
#define TAO_AV_TCP_H

#include "ace/OS.h"
#include "Protocol_Factory.h"
#include "FlowSpec_Entry.h"

class TAO_AV_Export TAO_AV_TCP_Factory : public TAO_AV_Transport_Factory
{
public:
  TAO_AV_TCP_Factory (void);
  virtual ~TAO_AV_TCP_Factory (void);
  virtual int init (int argc, char *argv[]);
  // Initialization hook.
  virtual int match_protocol (const char *protocol_string);
  virtual TAO_AV_Acceptor *make_acceptor (void);
  virtual TAO_AV_Connector *make_connector (void);
};

class TAO_AV_TCP_Flow_Handler;

class TAO_AV_TCP_Transport
  :public TAO_AV_Transport
{
  // = TITLE
  //     A transport abstraction for socket streams.
  //
  // = DESCRIPTION
  //     Uses the ACE_SOCK_Stream to send the data.
public:
  TAO_AV_TCP_Transport (void);

  TAO_AV_TCP_Transport (TAO_AV_TCP_Flow_Handler *handler);

  virtual ~TAO_AV_TCP_Transport (void);

  virtual int open (ACE_Addr *address);

  virtual int close (void);

  virtual int mtu (void);

  virtual ACE_Addr *get_peer_addr (void);

  virtual ssize_t send (const ACE_Message_Block *mblk,
                        ACE_Time_Value *s = 0);
  // Write the complete Message_Block chain to the connection.

  virtual ssize_t send (const char *buf,
                        size_t len,
                        ACE_Time_Value *s = 0);
  // Write the contents of the buffer of length len to the connection.

  virtual ssize_t send (const iovec *iov,
                        int iovcnt,
                        ACE_Time_Value *s = 0);
  // Write the contents of iovcnt iovec's to the connection.

  virtual ssize_t recv (char *buf,
                        size_t len,
                        ACE_Time_Value *s = 0);
  // Read len bytes from into buf.

  virtual ssize_t recv (char *buf,
                        size_t len,
                        int flags,
                        ACE_Time_Value *s = 0);
  // Read len bytes from into buf using flags.

  virtual ssize_t recv (iovec *iov,
                        int iovcnt,
                        ACE_Time_Value *s = 0);
  //  Read received data into the iovec buffers.
protected:
  TAO_AV_TCP_Flow_Handler *handler_;
};

class TAO_AV_TCP_Flow_Handler
  :public virtual TAO_AV_Flow_Handler,
   public ACE_Svc_Handler <ACE_SOCK_STREAM, ACE_NULL_SYNCH>
{
public:
  TAO_AV_TCP_Flow_Handler (TAO_AV_Callback *callback = 0);
  virtual TAO_AV_Transport *transport (void);
  virtual int open (void * = 0);
  virtual int handle_input (ACE_HANDLE fd);
  virtual int handle_timeout (const ACE_Time_Value &tv, const void *arg = 0);
  virtual ACE_Event_Handler* event_handler (void){ return this; }
protected:
  TAO_AV_Core *av_core_;
};

class TAO_AV_TCP_Acceptor;

class TAO_AV_TCP_Base_Acceptor  :public ACE_Acceptor <TAO_AV_TCP_Flow_Handler,ACE_SOCK_ACCEPTOR>
{
public:
  virtual int open (TAO_AV_TCP_Acceptor *acceptor,
                    ACE_Reactor *reactor,
                    const ACE_INET_Addr &local_addr,
                    TAO_FlowSpec_Entry *entry);
  virtual int make_svc_handler (TAO_AV_TCP_Flow_Handler *& handler);
protected:
  TAO_AV_TCP_Acceptor *acceptor_;
  ACE_Reactor *reactor_;
  TAO_FlowSpec_Entry *entry_;
};

class TAO_AV_TCP_Acceptor  :public TAO_AV_Acceptor
{
public:
  TAO_AV_TCP_Acceptor (void);
  virtual ~TAO_AV_TCP_Acceptor (void);

  virtual int open (TAO_Base_StreamEndPoint *endpoint,
                    TAO_AV_Core *av_core,
                    TAO_FlowSpec_Entry *entry,
                    TAO_AV_Flow_Protocol_Factory *factory);

  virtual int open_default (TAO_Base_StreamEndPoint *endpoint,
                            TAO_AV_Core *av_core,
                            TAO_FlowSpec_Entry *entry,
                            TAO_AV_Flow_Protocol_Factory *factory);

  virtual int close (void);
  virtual int make_svc_handler (TAO_AV_TCP_Flow_Handler *&handler);
protected:
  TAO_AV_TCP_Base_Acceptor acceptor_;
  TAO_FlowSpec_Entry *entry_;
  TAO_Base_StreamEndPoint *endpoint_;
  TAO_AV_Flow_Protocol_Factory *flow_protocol_factory_;
};

class TAO_AV_TCP_Connector;

class TAO_AV_TCP_Base_Connector  : public ACE_Connector <TAO_AV_TCP_Flow_Handler,ACE_SOCK_CONNECTOR>
{
public:
  // To avoid warnings of open and connect hiding the base class functions these have to renamed.
  int connector_open (TAO_AV_TCP_Connector *connector,
                      ACE_Reactor *reactor);
  int connector_connect (TAO_AV_TCP_Flow_Handler *&handler,
                         const ACE_INET_Addr &remote_addr);
  virtual int make_svc_handler (TAO_AV_TCP_Flow_Handler *& handler);
protected:
  TAO_AV_TCP_Connector *connector_;
  ACE_Reactor *reactor_;
};

class TAO_AV_TCP_Connector  : public TAO_AV_Connector
{
public:
  TAO_AV_TCP_Connector (void);
  virtual ~TAO_AV_TCP_Connector (void);

  virtual int open (TAO_Base_StreamEndPoint *endpoint,
                    TAO_AV_Core *av_core,
                    TAO_AV_Flow_Protocol_Factory *factory);

  virtual int connect (TAO_FlowSpec_Entry *entry,
                       TAO_AV_Transport *&transport);
  virtual int close (void);
  virtual int make_svc_handler (TAO_AV_TCP_Flow_Handler *&handler);
protected:
  TAO_AV_Core *av_core_;
  TAO_AV_TCP_Base_Connector connector_;
  TAO_Base_StreamEndPoint *endpoint_;
  TAO_FlowSpec_Entry *entry_;
  TAO_AV_Flow_Protocol_Factory *flow_protocol_factory_;
};

class TAO_AV_Export TAO_AV_TCP_Object : public TAO_AV_Protocol_Object
{
public:
  TAO_AV_TCP_Object (TAO_AV_Callback *callback,
                     TAO_AV_Transport *transport = 0);

  virtual ~TAO_AV_TCP_Object (void);
  // Dtor

  virtual int handle_input (void);

  virtual int send_frame (ACE_Message_Block *frame,
                          TAO_AV_frame_info *frame_info = 0);
  // send a data frame.

  virtual int send_frame (const iovec *iov,
                          int iovcnt,
                          TAO_AV_frame_info *frame_info = 0);

  virtual int destroy (void);
  // end the stream.

private:
  ACE_Message_Block frame_;
  // Pre-allocated memory to receive the data...
};

class TAO_AV_TCP_Flow_Factory : public TAO_AV_Flow_Protocol_Factory
{
public:
  TAO_AV_TCP_Flow_Factory (void);
  virtual ~TAO_AV_TCP_Flow_Factory (void);
  virtual int init (int argc, char *argv[]);
  // Initialization hook.
  virtual int match_protocol (const char *flow_string);
  TAO_AV_Protocol_Object* make_protocol_object (TAO_FlowSpec_Entry *entry,
                                                TAO_Base_StreamEndPoint *endpoint,
                                                TAO_AV_Flow_Handler *handler,
                                                TAO_AV_Transport *transport);
};

ACE_STATIC_SVC_DECLARE (TAO_AV_TCP_Flow_Factory)
ACE_FACTORY_DECLARE (TAO_AV, TAO_AV_TCP_Flow_Factory)

ACE_STATIC_SVC_DECLARE (TAO_AV_TCP_Factory)
ACE_FACTORY_DECLARE (TAO_AV, TAO_AV_TCP_Factory)

#include "ace/post.h"
#endif /* TAO_AV_TCP_H */
