/* -*- C++ -*- */

// $Id$
// ============================================================================
//
// = LIBRARY
//   ORBSVCS AVStreams
//
// = FILENAME
//   Transport.h
//
// = AUTHOR
//    Nagarajan Surendran <naga@cs.wustl.edu>
//
//
// ============================================================================

#if !defined TAO_AV_TRANSPORT_H
#define TAO_AV_TRANSPORT_H

#include "ace/Acceptor.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Connector.h"
#include "ace/SOCK_Connector.h"
#include "ace/Addr.h"
#include "ace/SOCK_Dgram.h"
#include "orbsvcs/AV/AV_export.h"
#include "AV_Core.h"
#include "FlowSpec_Entry.h"

// Forward declarations.
class TAO_AV_Protocol_Object;
class TAO_AV_Callback;
class TAO_AV_Transport;

class TAO_AV_Flow_Handler
{
public:
  TAO_AV_Flow_Handler (void);
  // Constructor.

  virtual int start (TAO_FlowSpec_Entry::Role role);
  virtual int stop  (TAO_FlowSpec_Entry::Role role);
  // Start/stop the flow handler.

  virtual int schedule_timer (void);
  // Schedule timer. Uses the get_timeout method on the callback.

  TAO_AV_Transport *transport (void);
  // get the transport.

  TAO_AV_Protocol_Object* protocol_object (void);
  void protocol_object (TAO_AV_Protocol_Object *protocol_object);
  // set/get protocol_object.

  void callback (TAO_AV_Callback *callback);
  // set the callback.

  int handle_timeout (const ACE_Time_Value &tv, const void *arg = 0);
  // Handle timeout. called from reactor.

  virtual int set_remote_address (ACE_Addr *address);
  // set the remote address.

  virtual ACE_Event_Handler* event_handler (void) = 0;
  // get the underlying event handler. To be overridden by the derived clases.

protected:
  TAO_AV_Transport *transport_;
  TAO_AV_Callback *callback_;
  TAO_AV_Protocol_Object *protocol_object_;
  long timer_id_;
  ACE_Reactor *reactor_;
  void *timeout_arg_;
};

//  Transports
class TAO_AV_Export TAO_AV_Transport
{
  // TITLE
  //   = A Base class for the different transport protocols.
  //
  // DESCRIPTION
  //   = All the different transports should derive and implement
  //     the open,close,send and recv methods.
public:
  TAO_AV_Transport (void);

  virtual ~TAO_AV_Transport (void);

  virtual int open (ACE_Addr *address) = 0;

  virtual int close (void) = 0;

  virtual int mtu (void) = 0;
  virtual ACE_Addr *get_peer_addr (void) = 0;
  virtual ACE_Addr *get_local_addr (void);
  virtual ssize_t send (const ACE_Message_Block *mblk,
                        ACE_Time_Value *s = 0) = 0;
  // Write the complete Message_Block chain to the connection.

  virtual ssize_t send (const char *buf,
                        size_t len,
                        ACE_Time_Value *s = 0) = 0;
  // Write the contents of the buffer of length len to the connection.

  virtual ssize_t send (const iovec *iov,
                        int iovcnt,
                        ACE_Time_Value *s = 0) = 0;
  // Write the contents of iovcnt iovec's to the connection.

  virtual ssize_t recv (char *buf,
                        size_t len,
                        ACE_Time_Value *s = 0) = 0;
  // Read len bytes from into buf.

  virtual ssize_t recv (char *buf,
                        size_t len,
                        int flags,
                        ACE_Time_Value *s = 0) = 0;
  // Read len bytes from into buf using flags.

  virtual ssize_t recv (iovec *iov,
                        int iovcnt,
                        ACE_Time_Value *s = 0) = 0;
  //  Read received data into the iovec buffers.

};

class TAO_Base_StreamEndPoint;
class TAO_AV_Core;
class TAO_FlowSpec_Entry;

class TAO_AV_Acceptor
{
public:
  TAO_AV_Acceptor (void);
  virtual ~TAO_AV_Acceptor (void);
  virtual int open (TAO_Base_StreamEndPoint *endpoint,
                    TAO_AV_Core *av_core,
                    TAO_FlowSpec_Entry *entry,
                    TAO_AV_Flow_Protocol_Factory *factory) = 0;

  virtual int open_default (TAO_Base_StreamEndPoint *endpoint,
                            TAO_AV_Core *av_core,
                            TAO_FlowSpec_Entry *entry,
                            TAO_AV_Flow_Protocol_Factory *factory) = 0;

  const char *flowname ();
  virtual int close (void) = 0;
protected:
  ACE_CString flowname_;
  TAO_AV_Core *av_core_;
  ACE_Addr *address_;
};

class TAO_AV_Connector
{
public:
  TAO_AV_Connector (void);
  virtual ~TAO_AV_Connector (void);
  const char *flowname (void);

  virtual int open (TAO_Base_StreamEndPoint *endpoint,
                    TAO_AV_Core *av_core,
                    TAO_AV_Flow_Protocol_Factory *factory) = 0;

  virtual int connect (TAO_FlowSpec_Entry *entry,
                       TAO_AV_Transport *&transport) = 0;

  virtual int close (void) = 0;
protected:
  ACE_CString flowname_;
};


typedef ACE_Unbounded_Set<TAO_AV_Connector*> TAO_AV_ConnectorSet;
typedef ACE_Unbounded_Set_Iterator<TAO_AV_Connector*> TAO_AV_ConnectorSetItor;

class TAO_AV_Export TAO_AV_Connector_Registry
{
public:
  TAO_AV_Connector_Registry (void);
  ~TAO_AV_Connector_Registry (void);
  int open (TAO_Base_StreamEndPoint *endpoint,
            TAO_AV_Core *av_core,
            TAO_AV_FlowSpecSet &flow_spec_set);
  //  int connect (TAO_FlowSpec_Entry *flowspec,
  //               TAO_AV_Transport *&transport);
  int close_all (void);
  //  TAO_AV_Connector *get_connector (TAO_AV_Core::Protocol protocol);
  // Return the connector bridges
  TAO_AV_ConnectorSetItor begin (void);
  TAO_AV_ConnectorSetItor end (void);
protected:
  TAO_AV_ConnectorSet connectors_;
};

typedef ACE_Unbounded_Set<TAO_AV_Acceptor*>
        TAO_AV_AcceptorSet;
typedef ACE_Unbounded_Set_Iterator<TAO_AV_Acceptor*>
        TAO_AV_AcceptorSetItor;

class TAO_AV_Export TAO_AV_Acceptor_Registry
{
public:
  TAO_AV_Acceptor_Registry (void);
  ~TAO_AV_Acceptor_Registry (void);
  int open (TAO_Base_StreamEndPoint *endpoint,
            TAO_AV_Core *av_core,
            TAO_AV_FlowSpecSet &flow_spec_set);
  int close_all (void);
  TAO_AV_AcceptorSetItor begin (void);
  TAO_AV_AcceptorSetItor end (void);
protected:
  int open_default (TAO_Base_StreamEndPoint *endpoint,
                    TAO_AV_Core *av_core,
                    TAO_FlowSpec_Entry *entry);
  TAO_AV_AcceptorSet acceptors_;
};

#if defined (__ACE_INLINE__)
#include "Transport.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_AV_TRANSPORT_H */
