// -*- C++ -*-

//=============================================================================
/**
 *  @file   QoS_UDP.h
 *
 *  $Id$
 *
 *  @author Yamuna Krishnamurthy <yamuna@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_AV_QOS_UDP_H
#define TAO_AV_QOS_UDP_H
#include /**/ "ace/pre.h"


#include "ace/config-all.h"

#if defined (ACE_HAS_RAPI) || defined (ACE_HAS_WINSOCK2_GQOS)

#include "orbsvcs/AV/Protocol_Factory.h"

#include "ace/Service_Config.h"
#include "ace/QoS/QoS_Session_Factory.h"
#include "ace/QoS/QoS_Decorator.h"
#include "ace/QoS/SOCK_Dgram_Mcast_QoS.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_AV_Export TAO_AV_UDP_QoS_Factory : public TAO_AV_Transport_Factory
{
public:
  TAO_AV_UDP_QoS_Factory (void);
  virtual ~TAO_AV_UDP_QoS_Factory (void);
  virtual int init (int argc, ACE_TCHAR *argv[]);
  /// Initialization hook.
  virtual int match_protocol (const char *protocol_string);
  virtual TAO_AV_Acceptor *make_acceptor (void);
  virtual TAO_AV_Connector *make_connector (void);
};

class TAO_AV_UDP_QoS_Flow_Handler;

/**
 * @class TAO_AV_UDP_QoS_Transport
 *
 * @brief A transport abstraction for udp sockets.
 *
 * Uses the ACE_SOCK_Dgram to send the data.
 */
class TAO_AV_UDP_QoS_Transport
  :public TAO_AV_Transport
{
public:
  TAO_AV_UDP_QoS_Transport (void);

  TAO_AV_UDP_QoS_Transport (TAO_AV_UDP_QoS_Flow_Handler *handler);

  virtual  ~TAO_AV_UDP_QoS_Transport (void);

  virtual int open (ACE_Addr *addr);

  virtual int close (void);

  virtual int mtu (void);

  virtual ACE_Addr *get_peer_addr (void);

  virtual int set_remote_address (const ACE_INET_Addr &address);

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

  /// Read received data into the iovec buffers.
  virtual ssize_t recv (iovec *iov,
                        int iovcnt,
                        ACE_Time_Value *s = 0);
protected:
  TAO_AV_UDP_QoS_Flow_Handler *handler_;
  ACE_Addr *addr_;
  ACE_INET_Addr peer_addr_;
};

class TAO_AV_UDP_QoS_Flow_Handler
  :public virtual TAO_AV_Flow_Handler,
   public virtual ACE_Event_Handler
{
public:
  /// Constructor.
  /**
   * Destructor
   */
  TAO_AV_UDP_QoS_Flow_Handler (void);
  ~TAO_AV_UDP_QoS_Flow_Handler (void);
  int open (ACE_Addr &address);
  virtual TAO_AV_Transport *transport (void);
  virtual int set_remote_address (ACE_Addr *address);
  virtual ACE_HANDLE get_handle (void) const;
  virtual int handle_input (ACE_HANDLE fd);
  virtual int handle_timeout (const ACE_Time_Value &tv, const void *arg = 0);
  virtual int change_qos (AVStreams::QoS);
  /// Handles a QoS event. Right now, just
  /// prints a message.
  virtual int handle_qos (ACE_HANDLE fd);
  ACE_SOCK_Dgram_Mcast_QoS *get_socket (void);
  virtual ACE_Event_Handler* event_handler (void){ return this; }
  virtual ACE_QoS_Session* qos_session (void);
  virtual void qos_session (ACE_QoS_Session *qos_session);

  int translate (ACE_Flow_Spec *ace_flow_spec,
                 CosPropertyService::Properties &qos_params);
  int translate (CosPropertyService::Properties &qos_params,
                 ACE_Flow_Spec *ace_flow_spec);

  void negotiator (AVStreams::Negotiator_ptr);

  void endpoint (TAO_Base_StreamEndPoint *endpoint);
  TAO_Base_StreamEndPoint* endpoint (void);

  void flowspec_entry (TAO_FlowSpec_Entry *entry);
  TAO_FlowSpec_Entry* flowspec_entry (void);

  void av_core (TAO_AV_Core *avcore);
  TAO_AV_Core* av_core (void);

protected:
  TAO_AV_Core *av_core_;
  ACE_INET_Addr peer_addr_;
  ACE_SOCK_Dgram_Mcast_QoS qos_sock_dgram_;
  ACE_QoS_Session *qos_session_;
  TAO_FlowSpec_Entry *entry_;
  TAO_Base_StreamEndPoint *endpoint_;
  AVStreams::Negotiator_ptr negotiator_;
};

class TAO_AV_UDP_QoS_Acceptor
  :public TAO_AV_Acceptor
{
public:
  TAO_AV_UDP_QoS_Acceptor (void);
  virtual ~TAO_AV_UDP_QoS_Acceptor (void);
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

  virtual int open_i (ACE_INET_Addr *address);

  virtual int close (void);

  virtual int activate_svc_handler (TAO_AV_UDP_QoS_Flow_Handler *handler);


protected:
  TAO_Base_StreamEndPoint *endpoint_;
  TAO_FlowSpec_Entry *entry_;
  TAO_AV_Flow_Protocol_Factory *flow_protocol_factory_;
  ACE_QoS_Session *qos_session_;
  ACE_QoS_Manager qos_manager_;
  ACE_INET_Addr qos_acceptor_addr_;
};

class TAO_AV_UDP_QoS_Connector
  :public TAO_AV_Connector
{
public:
  TAO_AV_UDP_QoS_Connector (void);
  ~TAO_AV_UDP_QoS_Connector (void);
  virtual int open (TAO_Base_StreamEndPoint *endpoint,
                    TAO_AV_Core *av_core,
                    TAO_AV_Flow_Protocol_Factory *factory);

  virtual int connect (TAO_FlowSpec_Entry *entry,
                       TAO_AV_Transport *&transport,
                       TAO_AV_Core::Flow_Component flow_comp =
                           TAO_AV_Core::TAO_AV_DATA);
  virtual int activate_svc_handler (TAO_AV_UDP_QoS_Flow_Handler *handler);
  virtual int close (void);

  int translate (CosPropertyService::Properties &qos_params,
                 ACE_Flow_Spec *ace_flow_spec);

protected:
  TAO_Base_StreamEndPoint *endpoint_;
  TAO_AV_Core *av_core_;
  TAO_FlowSpec_Entry *entry_;
  TAO_AV_Flow_Protocol_Factory *flow_protocol_factory_;
  ACE_QoS_Session *qos_session_;
  ACE_QoS_Manager qos_manager_;
};

// class TAO_AV_Export TAO_AV_UDP_Object  : public TAO_AV_Protocol_Object
// {
// public:
//   TAO_AV_UDP_Object (TAO_AV_Callback *callback,
//                      TAO_AV_Transport *transport = 0);

//   virtual ~TAO_AV_UDP_Object (void);
//   // Dtor

//   virtual int handle_input (void);

//   virtual int send_frame (ACE_Message_Block *frame,
//                           TAO_AV_frame_info *frame_info = 0);
//   // send a data frame.

//   virtual int send_frame (const iovec *iov,
//                           int iovcnt,
//                           TAO_AV_frame_info *frame_info = 0);

//   virtual int destroy (void);
//   // end the stream.

// private:
//   ACE_Message_Block frame_;
//   // Pre-allocated memory to receive the data...
// };

class TAO_AV_UDP_QoS_Flow_Factory : public TAO_AV_Flow_Protocol_Factory
{
public:
  TAO_AV_UDP_QoS_Flow_Factory (void);
  virtual ~TAO_AV_UDP_QoS_Flow_Factory (void);
  virtual int init (int argc, ACE_TCHAR *argv[]);
  /// Initialization hook.
  virtual int match_protocol (const char *flow_string);
  TAO_AV_Protocol_Object* make_protocol_object (TAO_FlowSpec_Entry *entry,
                                                TAO_Base_StreamEndPoint *endpoint,
                                                TAO_AV_Flow_Handler *handler,
                                                TAO_AV_Transport *transport);
};

/// Helper class to create qos sessions,
/// activate qos handlers and set qos
/// (For separation of concerns)
class TAO_AV_UDP_QoS_Session_Helper
{
public:
  TAO_AV_UDP_QoS_Session_Helper (void);
  ~TAO_AV_UDP_QoS_Session_Helper (void);

  /// Open a QoS Session with the specified address
  ACE_QoS_Session* open_qos_session (TAO_AV_UDP_QoS_Flow_Handler *handler,
                                     ACE_INET_Addr &addr);

  /// Activate the QoS handler to receive QoS events
  int activate_qos_handler (ACE_QoS_Session *qos_session,
                            TAO_AV_UDP_QoS_Flow_Handler *handler);

  /// Set the required QoS for the session
  int set_qos (ACE_Flow_Spec& ace_flow_spec,
               TAO_AV_UDP_QoS_Flow_Handler *handler);
};

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_STATIC_SVC_DECLARE (TAO_AV_UDP_QoS_Flow_Factory)
ACE_FACTORY_DECLARE (TAO_AV, TAO_AV_UDP_QoS_Flow_Factory)

ACE_STATIC_SVC_DECLARE (TAO_AV_UDP_QoS_Factory)
ACE_FACTORY_DECLARE (TAO_AV, TAO_AV_UDP_QoS_Factory)


#if defined(__ACE_INLINE__)
#include "orbsvcs/AV/QoS_UDP.inl"
#endif /* __ACE_INLINE__ */

#endif /* ACE_HAS_RAPI || ACE_HAS_WINSOCK2_GQOS */

#include /**/ "ace/post.h"
#endif /* TAO_AV_QOS_UDP_H */
