/* -*- C++ -*- */

// $Id$

// ============================================================================
//
// = LIBRARY
//   ORBSVCS AVStreams
//
// = FILENAME
//   MCast.h
//
// = AUTHOR
//    Nagarajan Surendran <naga@cs.wustl.edu>
//
//
// ============================================================================


#ifndef TAO_AV_MCAST_H
#define TAO_AV_MCAST_H

#include "Transport.h"
#include "ace/INET_Addr.h"

class TAO_AV_UDP_MCast_Flow_Handler;

class TAO_AV_UDP_MCast_Transport
  :public TAO_AV_Transport
{
  // = TITLE
  //     A transport abstraction for socket streams.
  //
  // = DESCRIPTION
  //     Uses the ACE_SOCK_Stream to send the data.
public:
  TAO_AV_UDP_MCast_Transport (void);

  TAO_AV_UDP_MCast_Transport (TAO_AV_UDP_MCast_Flow_Handler *handler);

  virtual ~TAO_AV_UDP_MCast_Transport (void);

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
  TAO_AV_UDP_MCast_Flow_Handler *handler_;
  ACE_INET_Addr peer_addr_;
};

class TAO_AV_UDP_MCast_Acceptor
  :public TAO_AV_Acceptor
{
public:
  TAO_AV_UDP_MCast_Acceptor (void);
  virtual ~TAO_AV_UDP_MCast_Acceptor (void);
  virtual int open (TAO_Base_StreamEndPoint *endpoint,
                    TAO_AV_Core *av_core,
                    TAO_FlowSpec_Entry *entry);
  virtual int open_default (TAO_Base_StreamEndPoint *endpoint,
                            TAO_AV_Core *av_core,
                            TAO_FlowSpec_Entry *entry);
  virtual int close (void);
  virtual int make_svc_handler (TAO_AV_UDP_MCast_Flow_Handler *&udp_handler);
  virtual int activate_svc_handler (TAO_AV_UDP_MCast_Flow_Handler *handler);
protected:
  TAO_FlowSpec_Entry *entry_;
  TAO_Base_StreamEndPoint *endpoint_;
  TAO_AV_Core *av_core_;
};

class TAO_AV_UDP_MCast_Connector
  :public TAO_AV_Connector
{
public:
  TAO_AV_UDP_MCast_Connector (void);
  virtual ~TAO_AV_UDP_MCast_Connector (void);
  virtual int open (TAO_Base_StreamEndPoint *endpoint,
                    TAO_AV_Core *av_core);

  virtual int connect (TAO_FlowSpec_Entry *entry,
                       TAO_AV_Transport *&transport);

  virtual int close (void);
  virtual int make_svc_handler (TAO_AV_UDP_MCast_Flow_Handler *&udp_handler);
  virtual int activate_svc_handler (TAO_AV_UDP_MCast_Flow_Handler *handler);
protected:
  ACE_CString flowname_;
  TAO_Base_StreamEndPoint *endpoint_;
  TAO_AV_Core *av_core_;
  TAO_FlowSpec_Entry *entry_;
};

class TAO_AV_UDP_MCast_Flow_Handler
  :public virtual TAO_AV_Flow_Handler,
   public virtual ACE_Event_Handler,
   public virtual ACE_SOCK_Dgram_Mcast
{
public:
  TAO_AV_UDP_MCast_Flow_Handler (TAO_AV_Callback *callback);
  virtual ACE_HANDLE get_handle (void) const;
  virtual int handle_input (ACE_HANDLE fd);
protected:
  ACE_INET_Addr peer_addr_;
};

class TAO_AV_UDP_MCast_Protocol_Factory
  :public TAO_AV_Protocol_Factory
{
public:
  TAO_AV_UDP_MCast_Protocol_Factory (void);
  virtual ~TAO_AV_UDP_MCast_Protocol_Factory (void);
  virtual int match_protocol (TAO_AV_Core::Protocol protocol);
  virtual TAO_AV_Acceptor *make_acceptor (void);
  virtual TAO_AV_Connector *make_connector (void);
};

#endif /* TAO_AV_MCAST_H */
