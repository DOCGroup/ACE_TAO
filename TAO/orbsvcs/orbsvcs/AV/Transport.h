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
#include "ace/Addr.h"
#include "AVStreams_i.h"

//  Transports

class TAO_ORBSVCS_Export TAO_AV_Transport
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
  virtual int get_peer_addr (ACE_Addr &addr) = 0;
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

  virtual int get_peer_addr (ACE_Addr &addr);

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

class TAO_AV_UDP_Flow_Handler;

class TAO_AV_UDP_Transport
  :public TAO_AV_Transport
{
  // = TITLE
  //     A transport abstraction for udp sockets.
  //
  // = DESCRIPTION
  //     Uses the ACE_SOCK_Dgram to send the data.
public:
  TAO_AV_UDP_Transport (void);

  TAO_AV_UDP_Transport (TAO_AV_UDP_Flow_Handler *handler);

  virtual  ~TAO_AV_UDP_Transport (void);
  virtual int open (ACE_Addr *addr);

  virtual int close (void);

  virtual int mtu (void);

  virtual int get_peer_addr (ACE_Addr &addr);

  virtual int set_remote_address (const ACE_INET_Addr &address);

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
  TAO_AV_UDP_Flow_Handler *handler_;
  ACE_Addr *addr_;
  ACE_INET_Addr peer_addr_;
};

class TAO_Base_StreamEndPoint;
class TAO_AV_Core;
class TAO_FlowSpec_Entry;

// Acceptors
class TAO_AV_Acceptor
{
public:
  TAO_AV_Acceptor (void);
  virtual ~TAO_AV_Acceptor (void);
  virtual int open (TAO_Base_StreamEndPoint *endpoint,
                    TAO_AV_Core *av_core,
                    TAO_FlowSpec_Entry *entry) = 0;
  virtual int open_default (TAO_Base_StreamEndPoint *endpoint,
                            TAO_AV_Core *av_core,
                            TAO_FlowSpec_Entry *entry) = 0;
  virtual const char *flowname ();
  virtual int close (void) = 0;
protected:
  ACE_CString flowname_;
  TAO_AV_Core *av_core_;
  ACE_Addr *address_;
};

class TAO_AV_TCP_Acceptor;

class TAO_AV_TCP_Base_Acceptor
  :public ACE_Acceptor <TAO_AV_TCP_Flow_Handler,ACE_SOCK_ACCEPTOR>
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

class TAO_AV_TCP_Acceptor
  :public TAO_AV_Acceptor
{
public:
  TAO_AV_TCP_Acceptor (void);
  virtual ~TAO_AV_TCP_Acceptor (void);
  virtual int open (TAO_Base_StreamEndPoint *endpoint,
                    TAO_AV_Core *av_core,
                    TAO_FlowSpec_Entry *entry);
  virtual int open_default (TAO_Base_StreamEndPoint *endpoint,
                            TAO_AV_Core *av_core,
                            TAO_FlowSpec_Entry *entry);
  virtual int close (void);
  virtual int make_svc_handler (TAO_AV_TCP_Flow_Handler *&handler);
protected:
  TAO_AV_TCP_Base_Acceptor acceptor_;
  TAO_FlowSpec_Entry *entry_;
  TAO_Base_StreamEndPoint *endpoint_;
};

class TAO_AV_Flow_Handler;

class TAO_AV_UDP_Acceptor;

class TAO_ORBSVCS_Export TAO_AV_Dgram_Acceptor
{
public:
  virtual int open (TAO_AV_UDP_Acceptor *acceptor,
                    ACE_Reactor *reactor,
                    ACE_Addr &address,
                    TAO_AV_UDP_Flow_Handler *&handler);
  virtual int make_svc_handler (TAO_AV_UDP_Flow_Handler *&handler);
  virtual int activate_svc_handler (TAO_AV_UDP_Flow_Handler *handler);
protected:
  ACE_Reactor *reactor_;
  TAO_AV_UDP_Acceptor *acceptor_;
};

class TAO_AV_UDP_Connector;

class TAO_ORBSVCS_Export TAO_AV_Dgram_Connector
{
public:
  virtual int open (TAO_AV_UDP_Connector *connector,
                    ACE_Reactor *reactor);

  virtual int connect (TAO_AV_UDP_Flow_Handler *&handler,
                       const ACE_Addr &remote_addr,
                       ACE_Addr &local_addr);
  virtual int make_svc_handler (TAO_AV_UDP_Flow_Handler *&handler);
  virtual int activate_svc_handler (TAO_AV_UDP_Flow_Handler *handler);
protected:
  ACE_Reactor *reactor_;
  TAO_AV_UDP_Connector *connector_;
};

class TAO_AV_UDP_Acceptor
  :public TAO_AV_Acceptor
{
public:
  TAO_AV_UDP_Acceptor (void);
  virtual ~TAO_AV_UDP_Acceptor (void);
  virtual int open (TAO_Base_StreamEndPoint *endpoint,
                    TAO_AV_Core *av_core,
                    TAO_FlowSpec_Entry *entry);
  virtual int open_default (TAO_Base_StreamEndPoint *endpoint,
                            TAO_AV_Core *av_core,
                            TAO_FlowSpec_Entry *entry);
  virtual int close (void);
  virtual int make_svc_handler (TAO_AV_UDP_Flow_Handler *&handler);
protected:
  TAO_AV_Dgram_Acceptor acceptor_;
  TAO_Base_StreamEndPoint *endpoint_;
  TAO_FlowSpec_Entry *entry_;
};

// Connectors
class TAO_AV_Connector
{
public:
  TAO_AV_Connector (void);
  virtual ~TAO_AV_Connector (void);
  virtual const char *flowname (void);

  virtual int open (TAO_Base_StreamEndPoint *endpoint,
                    TAO_AV_Core *av_core) = 0;

  virtual int connect (TAO_FlowSpec_Entry *entry,
                       TAO_AV_Transport *&transport) = 0;

  virtual int close (void) = 0;
protected:
  ACE_CString flowname_;
};

class TAO_AV_TCP_Connector;

class TAO_AV_TCP_Base_Connector
  :public ACE_Connector <TAO_AV_TCP_Flow_Handler,ACE_SOCK_CONNECTOR>
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

class TAO_AV_TCP_Connector
  :public TAO_AV_Connector
{
public:
  TAO_AV_TCP_Connector (void);
  virtual ~TAO_AV_TCP_Connector (void);

  virtual int open (TAO_Base_StreamEndPoint *endpoint,
                    TAO_AV_Core *av_core);

  virtual int connect (TAO_FlowSpec_Entry *entry,
                       TAO_AV_Transport *&transport);
  virtual int close (void);
  virtual int make_svc_handler (TAO_AV_TCP_Flow_Handler *&handler);
protected:
  TAO_AV_Core *av_core_;
  TAO_AV_TCP_Base_Connector connector_;
  TAO_Base_StreamEndPoint *endpoint_;
  TAO_FlowSpec_Entry *entry_;
};

class TAO_AV_UDP_Connector
  :public TAO_AV_Connector
{
public:
  TAO_AV_UDP_Connector (void);
  ~TAO_AV_UDP_Connector (void);
  virtual int open (TAO_Base_StreamEndPoint *endpoint,
                    TAO_AV_Core *av_core);

  virtual int connect (TAO_FlowSpec_Entry *entry,
                       TAO_AV_Transport *&transport);
  virtual int close (void);
  virtual int make_svc_handler (TAO_AV_UDP_Flow_Handler *&handler);
protected:
  TAO_AV_Dgram_Connector connector_;
  TAO_Base_StreamEndPoint *endpoint_;
  TAO_AV_Core *av_core_;
  TAO_FlowSpec_Entry *entry_;
};

// Protocol Factories
class TAO_ORBSVCS_Export TAO_AV_Protocol_Factory
{
public:
  TAO_AV_Protocol_Factory (void);
  virtual ~TAO_AV_Protocol_Factory (void);

  virtual int match_protocol (TAO_AV_Core::Protocol protocol) = 0;
  virtual TAO_AV_Acceptor *make_acceptor (void) = 0;
  virtual TAO_AV_Connector *make_connector (void) = 0;
};

class TAO_AV_TCP_Protocol_Factory
  :public TAO_AV_Protocol_Factory
{
public:
  TAO_AV_TCP_Protocol_Factory (void);
  virtual ~TAO_AV_TCP_Protocol_Factory (void);
  virtual int match_protocol (TAO_AV_Core::Protocol protocol);
  virtual TAO_AV_Acceptor *make_acceptor (void);
  virtual TAO_AV_Connector *make_connector (void);

};

class TAO_AV_UDP_Protocol_Factory
  :public TAO_AV_Protocol_Factory
{
public:
  TAO_AV_UDP_Protocol_Factory (void);
  virtual ~TAO_AV_UDP_Protocol_Factory (void);
  virtual int match_protocol (TAO_AV_Core::Protocol protocol);
  virtual TAO_AV_Acceptor *make_acceptor (void);
  virtual TAO_AV_Connector *make_connector (void);
};

class TAO_AV_Flow_Handler
{
public:
  TAO_AV_Flow_Handler (TAO_AV_Callback *callback);
  virtual int start (void);
  virtual int stop  (void);
  virtual TAO_AV_Transport *transport (void);
  virtual int set_remote_address (ACE_Addr *address);
  //  virtual ACE_Event_Handler* event_handler (void) = 0;
protected:
  TAO_AV_Transport *transport_;
  TAO_AV_Callback *callback_;
};

class TAO_AV_TCP_Flow_Handler
  :public virtual TAO_AV_Flow_Handler,
   public ACE_Svc_Handler <ACE_SOCK_STREAM, ACE_NULL_SYNCH>
{
public:
  TAO_AV_TCP_Flow_Handler (TAO_AV_Callback *callback = 0);
  virtual TAO_AV_Transport *transport (void);
  virtual int handle_input (ACE_HANDLE fd);
  //  virtual ACE_Event_Handler* event_handler (void){ return this; }
protected:
  TAO_AV_Core *av_core_;
};

class TAO_AV_UDP_Flow_Handler
  :public virtual TAO_AV_Flow_Handler,
   public virtual ACE_SOCK_Dgram,
   public virtual ACE_Event_Handler
{
public:
  TAO_AV_UDP_Flow_Handler (TAO_AV_Callback *callback);
  virtual TAO_AV_Transport *transport (void);
  virtual int set_remote_address (ACE_Addr *address);
  virtual ACE_HANDLE get_handle (void) const;
  virtual int handle_input (ACE_HANDLE fd);
protected:
  TAO_AV_Core *av_core_;
  ACE_INET_Addr peer_addr_;
};

typedef ACE_Unbounded_Set<TAO_AV_Connector*> TAO_AV_ConnectorSet;
typedef ACE_Unbounded_Set_Iterator<TAO_AV_Connector*> TAO_AV_ConnectorSetItor;

class TAO_ORBSVCS_Export TAO_AV_Connector_Registry
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

class TAO_ORBSVCS_Export TAO_AV_Acceptor_Registry
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

#endif /* TAO_AV_TRANSPORT_H */
