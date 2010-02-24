// -*- C++ -*-

//=============================================================================
/**
 *  @file   SCTP_SEQ.h
 *
 *  $Id$
 *
 *  @author Yamuna Krishnamurthy <yamuna@oomworks.com>
 */
//=============================================================================

#ifndef TAO_AV_SCTP_SEQ_H
#define TAO_AV_SCTP_SEQ_H
#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if defined (ACE_HAS_SCTP)

#include "ace/Auto_Ptr.h"
#include "ace/Service_Config.h"
#include "orbsvcs/AV/Protocol_Factory.h"
#include "ace/SOCK_SEQPACK_Association.h"
#include "ace/SOCK_SEQPACK_Acceptor.h"
#include "ace/SOCK_SEQPACK_Connector.h"

extern "C" {
#include <netinet/sctp.h>
};

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

typedef ACE_Unbounded_Set <ACE_CString> Interface_Seq;
typedef ACE_Unbounded_Set_Iterator <ACE_CString> Interface_Seq_Itor;

//typedef auto_ptr <Interface_Seq> Interface_Seq_Ptr;
typedef ACE_Hash_Map_Manager <ACE_CString,Interface_Seq,ACE_Null_Mutex>  Secondary_Addr_Map;
typedef ACE_Hash_Map_Entry <ACE_CString,Interface_Seq> Secondary_Addr_Map_Entry;
typedef ACE_Hash_Map_Iterator <ACE_CString,Interface_Seq,ACE_Null_Mutex>  Secondary_Addr_Map_Iterator;

/**
 * @class TAO_AV_SCTP_SEQ_Factory
 */
class TAO_AV_Export TAO_AV_SCTP_SEQ_Factory : public TAO_AV_Transport_Factory
{
public:
  /// Initialization hook.
  TAO_AV_SCTP_SEQ_Factory (void);
  virtual ~TAO_AV_SCTP_SEQ_Factory (void);
  virtual int init (int argc, ACE_TCHAR *argv[]);
  virtual int match_protocol (const char *protocol_string);
  virtual TAO_AV_Acceptor *make_acceptor (void);
  virtual TAO_AV_Connector *make_connector (void);
};

class TAO_AV_SCTP_SEQ_Flow_Handler;

/**
 * @class TAO_AV_SCTP_SEQ_Transport
 * @brief A transport abstraction for udp sockets.
 *        Uses the ACE_SOCK_Dgram to send the data.
 */
class TAO_AV_Export TAO_AV_SCTP_SEQ_Transport
  :public TAO_AV_Transport
{
public:
  TAO_AV_SCTP_SEQ_Transport (void);

  TAO_AV_SCTP_SEQ_Transport (TAO_AV_SCTP_SEQ_Flow_Handler *handler);

  virtual  ~TAO_AV_SCTP_SEQ_Transport (void);
  virtual int open (ACE_Addr *addr);

  virtual int close (void);

  virtual int mtu (void);

  virtual ACE_Addr *get_peer_addr (void);

  /// Write the complete Message_Block chain to the connection.
  virtual ssize_t send (const ACE_Message_Block *mblk,
                        ACE_Time_Value *s = 0);

  /// Write the contents of the buffer of length len to the connection.
  virtual ssize_t send (const char *buf,
                        size_t len,
                        ACE_Time_Value *s = 0);

  /// Write the contents of iovcnt iovec's to the connection.
  virtual ssize_t send (const iovec *iov,
                        int iovcnt,
                        ACE_Time_Value *s = 0);

  /// Read len bytes from into buf.
  virtual ssize_t recv (char *buf,
                        size_t len,
                        ACE_Time_Value *s = 0);

  /// Read len bytes from into buf using flags.
  virtual ssize_t recv (char *buf,
                        size_t len,
                        int flags,
                        ACE_Time_Value *s = 0);

  ///  Read received data into the iovec buffers.
  virtual ssize_t recv (iovec *iov,
                        int iovcnt,
                        ACE_Time_Value *s = 0);

  TAO_AV_SCTP_SEQ_Flow_Handler *handler (void) { return this->handler_; }

protected:
  TAO_AV_SCTP_SEQ_Flow_Handler *handler_;
  ACE_Addr *addr_;
  ACE_INET_Addr peer_addr_;
};

/**
 * @class TAO_AV_SCTP_SEQ_Flow_Handler
 */
class TAO_AV_SCTP_SEQ_Flow_Handler
  :public virtual TAO_AV_Flow_Handler,
   public ACE_Svc_Handler <ACE_SOCK_SEQPACK_ASSOCIATION, ACE_NULL_SYNCH>
{
public:
  TAO_AV_SCTP_SEQ_Flow_Handler (TAO_AV_Callback *callback = 0);
  virtual ~TAO_AV_SCTP_SEQ_Flow_Handler (void);
  virtual TAO_AV_Transport *transport (void);
  virtual int open (void * = 0);
  virtual int handle_input (ACE_HANDLE fd);
  virtual int handle_timeout (const ACE_Time_Value &tv, const void *arg = 0);
  virtual ACE_Event_Handler* event_handler (void){ return this; }
  /// Change the QoS
  virtual int change_qos (AVStreams::QoS);

protected:
  TAO_AV_Core *av_core_;
};

class TAO_AV_SCTP_SEQ_Acceptor;

/**
 * @class TAO_AV_SCTP_SEQ_Base_Acceptor
 */
class TAO_AV_SCTP_SEQ_Base_Acceptor  :public ACE_Acceptor <TAO_AV_SCTP_SEQ_Flow_Handler,ACE_SOCK_SEQPACK_ACCEPTOR>
{
 public:
  virtual int acceptor_open (TAO_AV_SCTP_SEQ_Acceptor *acceptor,
                             ACE_Reactor *reactor,
                             const ACE_INET_Addr &local_addr,
                             TAO_FlowSpec_Entry *entry);
  virtual int make_svc_handler (TAO_AV_SCTP_SEQ_Flow_Handler *& handler);
 protected:
  TAO_AV_SCTP_SEQ_Acceptor *acceptor_;
  ACE_Reactor *reactor_;
  TAO_FlowSpec_Entry *entry_;
};

/**
 * @class TAO_AV_SCTP_SEQ_Acceptor
 * @brief
 */
class TAO_AV_Export TAO_AV_SCTP_SEQ_Acceptor
   :public TAO_AV_Acceptor
{
public:
  TAO_AV_SCTP_SEQ_Acceptor (void);
  virtual ~TAO_AV_SCTP_SEQ_Acceptor (void);
  virtual int open (TAO_Base_StreamEndPoint *endpoint,
                    TAO_AV_Core *av_core,
                    TAO_FlowSpec_Entry *entry,
                    TAO_AV_Flow_Protocol_Factory *factory,
                    TAO_AV_Core::Flow_Component flow_comp =
                    TAO_AV_Core::TAO_AV_DATA);

  virtual int open_default (TAO_Base_StreamEndPoint *endpoint,
                            TAO_AV_Core *av_core,
                            TAO_FlowSpec_Entry *entry,
                            TAO_AV_Flow_Protocol_Factory *factory,
                            TAO_AV_Core::Flow_Component flow_comp =
                            TAO_AV_Core::TAO_AV_DATA);

  virtual int close (void);
  virtual int make_svc_handler (TAO_AV_SCTP_SEQ_Flow_Handler *&handler);

protected:
  ACE_INET_Addr *address_;
  TAO_AV_SCTP_SEQ_Base_Acceptor acceptor_;
  TAO_Base_StreamEndPoint *endpoint_;
  TAO_FlowSpec_Entry *entry_;
  TAO_AV_Flow_Protocol_Factory *flow_protocol_factory_;
};

class TAO_AV_SCTP_SEQ_Connector;

/**
 * @class TAO_AV_Base_Connector
 */
class TAO_AV_SCTP_SEQ_Base_Connector  : public ACE_Connector <TAO_AV_SCTP_SEQ_Flow_Handler,ACE_SOCK_SEQPACK_CONNECTOR>
{
public:
  // To avoid warnings of open and connect hiding the base class functions these have to renamed.
  int connector_open (TAO_AV_SCTP_SEQ_Connector *connector,
                      ACE_Reactor *reactor);
  int connector_connect (TAO_AV_SCTP_SEQ_Flow_Handler *&handler,
                         const ACE_Multihomed_INET_Addr &remote_addr,
                         const ACE_Multihomed_INET_Addr &local_addr);
  virtual int make_svc_handler (TAO_AV_SCTP_SEQ_Flow_Handler *& handler);
protected:
  TAO_AV_SCTP_SEQ_Connector *connector_;
  ACE_Reactor *reactor_;
};

/**
 * @class TAO_AV_SCTP_SEQ_Connector
 * @brief
 */
class TAO_AV_SCTP_SEQ_Connector  : public TAO_AV_Connector
{
public:
  TAO_AV_SCTP_SEQ_Connector (void);
  virtual ~TAO_AV_SCTP_SEQ_Connector (void);

  virtual int open (TAO_Base_StreamEndPoint *endpoint,
                    TAO_AV_Core *av_core,
                    TAO_AV_Flow_Protocol_Factory *factory);

  virtual int connect (TAO_FlowSpec_Entry *entry,
                       TAO_AV_Transport *&transport,
                       TAO_AV_Core::Flow_Component flow_comp =
                       TAO_AV_Core::TAO_AV_DATA);
  virtual int close (void);
  virtual int make_svc_handler (TAO_AV_SCTP_SEQ_Flow_Handler *&handler);
protected:
  TAO_AV_Core *av_core_;
  TAO_AV_SCTP_SEQ_Base_Connector connector_;
  TAO_Base_StreamEndPoint *endpoint_;
  TAO_FlowSpec_Entry *entry_;
  TAO_AV_Flow_Protocol_Factory *flow_protocol_factory_;
};

/**
 * @class TAO_AV_SCTP_SEQ_Object
 * @brief TAO_AV_Protocol_Object for the User Datagram Protocol (SCTP_SEQ)
 */
class TAO_AV_Export TAO_AV_SCTP_SEQ_Object  : public TAO_AV_Protocol_Object
{
public:
  TAO_AV_SCTP_SEQ_Object (TAO_AV_Callback *callback,
                     TAO_AV_Transport *transport = 0);

  /// Dtor
  virtual ~TAO_AV_SCTP_SEQ_Object (void);

  virtual int handle_input (void);

  /// send a data frame.
  virtual int send_frame (ACE_Message_Block *frame,
                          TAO_AV_frame_info *frame_info = 0);

  virtual int send_frame (const iovec *iov,
                          int iovcnt,
                          TAO_AV_frame_info *frame_info = 0);

  virtual int send_frame (const char*buf,
                          size_t len);

  /// end the stream.
  virtual int destroy (void);

private:
  /// Pre-allocated memory to receive the data...
  ACE_Message_Block frame_;
};

/**
 * @class TAO_AV_SCTP_SEQ_Flow_Factory
 * @brief
 */
class TAO_AV_Export TAO_AV_SCTP_SEQ_Flow_Factory : public TAO_AV_Flow_Protocol_Factory
{
public:
  /// Initialization hook.
  TAO_AV_SCTP_SEQ_Flow_Factory (void);
  virtual ~TAO_AV_SCTP_SEQ_Flow_Factory (void);
  virtual int init (int argc, ACE_TCHAR *argv[]);
  virtual int match_protocol (const char *flow_string);
  TAO_AV_Protocol_Object* make_protocol_object (TAO_FlowSpec_Entry *entry,
                                                TAO_Base_StreamEndPoint *endpoint,
                                                TAO_AV_Flow_Handler *handler,
                                                TAO_AV_Transport *transport);
};

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_STATIC_SVC_DECLARE (TAO_AV_SCTP_SEQ_Flow_Factory)
ACE_FACTORY_DECLARE (TAO_AV, TAO_AV_SCTP_SEQ_Flow_Factory)

ACE_STATIC_SVC_DECLARE (TAO_AV_SCTP_SEQ_Factory)
ACE_FACTORY_DECLARE (TAO_AV, TAO_AV_SCTP_SEQ_Factory)


#endif // ACE_HAS_SCTP

#include /**/ "ace/post.h"

#endif /* TAO_AV_SCTP_SEQ_H */
