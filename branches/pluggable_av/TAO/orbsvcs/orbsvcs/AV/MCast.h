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

  virtual ACE_Addr *get_peer_addr (void);
  virtual ACE_Addr *get_local_addr (void);
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
  ACE_INET_Addr local_addr_;
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
  virtual int open_i (ACE_Reactor *reactor,
                      ACE_INET_Addr *&address,
                      TAO_AV_UDP_MCast_Flow_Handler *&handler);

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
  TAO_AV_UDP_MCast_Flow_Handler *handler_;
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

  int connect_i (ACE_Reactor *reactor,
                 ACE_INET_Addr *&address,
                 TAO_AV_UDP_MCast_Flow_Handler *&handler);

  virtual int close (void);
  virtual int make_svc_handler (TAO_AV_UDP_MCast_Flow_Handler *&udp_handler);
  virtual int activate_svc_handler (TAO_AV_UDP_MCast_Flow_Handler *handler);
protected:
  ACE_CString flowname_;
  TAO_Base_StreamEndPoint *endpoint_;
  TAO_AV_Core *av_core_;
  TAO_FlowSpec_Entry *entry_;
  TAO_AV_UDP_MCast_Flow_Handler *handler_;
};

class TAO_AV_UDP_MCast_Flow_Handler
  :public virtual TAO_AV_Flow_Handler,
   public virtual ACE_Event_Handler
{
public:
  TAO_AV_UDP_MCast_Flow_Handler (TAO_AV_Callback *callback);
  // Ctor
  ~TAO_AV_UDP_MCast_Flow_Handler (void);
  // Dtor
  virtual ACE_HANDLE get_handle (void) const;
  virtual int handle_input (ACE_HANDLE fd);
  ACE_SOCK_Dgram_Mcast *get_mcast_socket (void) const;
protected:
  ACE_INET_Addr peer_addr_;
  ACE_SOCK_Dgram_Mcast *dgram_mcast_;

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

#if defined(__ACE_INLINE__)
#include "MCast.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_AV_MCAST_H */
