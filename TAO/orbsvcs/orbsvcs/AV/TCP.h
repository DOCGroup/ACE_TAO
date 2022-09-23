// -*- C++ -*-

//=============================================================================
/**
 *  @file   TCP.h
 *
 *  @author Nagarajan Surendran <naga@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_AV_TCP_H
#define TAO_AV_TCP_H
#include /**/ "ace/pre.h"

#include "orbsvcs/AV/Protocol_Factory.h"
#include "orbsvcs/AV/FlowSpec_Entry.h"

#include "ace/Service_Config.h"
#include "ace/Svc_Handler.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_AV_TCP_Factory
 * @brief
 */
class TAO_AV_Export TAO_AV_TCP_Factory : public TAO_AV_Transport_Factory
{
public:
  /// Initialization hook.
  TAO_AV_TCP_Factory ();
  ~TAO_AV_TCP_Factory () override;
  int init (int argc, ACE_TCHAR *argv[]) override;
  int match_protocol (const char *protocol_string) override;
  TAO_AV_Acceptor *make_acceptor () override;
  TAO_AV_Connector *make_connector () override;
};

class TAO_AV_TCP_Flow_Handler;

/**
 * @class TAO_AV_TCP_Transport
 * @brief A transport abstraction for socket streams.
 *        Uses the ACE_SOCK_Stream to send the data.
 */
class TAO_AV_TCP_Transport
  : public TAO_AV_Transport
{
public:
  TAO_AV_TCP_Transport ();

  TAO_AV_TCP_Transport (TAO_AV_TCP_Flow_Handler *handler);

  ~TAO_AV_TCP_Transport () override;

  int open (ACE_Addr *address) override;

  int close () override;

  int mtu () override;

  ACE_Addr *get_peer_addr () override;

  /// Write the complete Message_Block chain to the connection.
  ssize_t send (const ACE_Message_Block *mblk,
                        ACE_Time_Value *s = 0) override;

  /// Write the contents of the buffer of length len to the connection.
  ssize_t send (const char *buf,
                        size_t len,
                        ACE_Time_Value *s = 0) override;

  /// Write the contents of iovcnt iovec's to the connection.
  ssize_t send (const iovec *iov,
                        int iovcnt,
                        ACE_Time_Value *s = 0) override;

  /// Read len bytes from into buf.
  ssize_t recv (char *buf,
                        size_t len,
                        ACE_Time_Value *s = 0) override;

  /// Read len bytes from into buf using flags.
  ssize_t recv (char *buf,
                        size_t len,
                        int flags,
                        ACE_Time_Value *s = 0) override;

  ///  Read received data into the iovec buffers.
  ssize_t recv (iovec *iov,
                        int iovcnt,
                        ACE_Time_Value *s = 0) override;
protected:
  TAO_AV_TCP_Flow_Handler *handler_;
};

/**
 * @class TAO_AV_TCP_Flow_Handler
 * @brief
 */
class TAO_AV_TCP_Flow_Handler
  :public virtual TAO_AV_Flow_Handler,
   public virtual ACE_Svc_Handler <ACE_SOCK_STREAM, ACE_NULL_SYNCH>
{
public:
  TAO_AV_TCP_Flow_Handler (TAO_AV_Callback *callback = 0);
  ~TAO_AV_TCP_Flow_Handler () override;
  virtual TAO_AV_Transport *transport ();
  int open (void * = 0) override;
  int handle_input (ACE_HANDLE fd) override;
  int handle_timeout (const ACE_Time_Value &tv, const void *arg = 0) override;
  ACE_Event_Handler* event_handler () override{ return this; }
protected:
  TAO_AV_Core *av_core_;
};

class TAO_AV_TCP_Acceptor;

/**
 * @class TAO_AV_TCP_Base_Acceptor
 * @brief
 */
class TAO_AV_TCP_Base_Acceptor : public ACE_Acceptor <TAO_AV_TCP_Flow_Handler,ACE_SOCK_ACCEPTOR>
{
public:
  virtual int acceptor_open (TAO_AV_TCP_Acceptor *acceptor,
                    ACE_Reactor *reactor,
                    const ACE_INET_Addr &local_addr,
                    TAO_FlowSpec_Entry *entry);
  int make_svc_handler (TAO_AV_TCP_Flow_Handler *& handler) override;
protected:
  TAO_AV_TCP_Acceptor *acceptor_;
  ACE_Reactor *reactor_;
  TAO_FlowSpec_Entry *entry_;
};

/**
 * @class TAO_AV_TCP_Acceptor
 * @brief
 */
class TAO_AV_TCP_Acceptor : public TAO_AV_Acceptor
{
public:
  TAO_AV_TCP_Acceptor ();
  ~TAO_AV_TCP_Acceptor () override;

  int open (TAO_Base_StreamEndPoint *endpoint,
                    TAO_AV_Core *av_core,
                    TAO_FlowSpec_Entry *entry,
                    TAO_AV_Flow_Protocol_Factory *factory,
                    TAO_AV_Core::Flow_Component flow_component =
                        TAO_AV_Core::TAO_AV_DATA) override;

  int open_default (TAO_Base_StreamEndPoint *endpoint,
                            TAO_AV_Core *av_core,
                            TAO_FlowSpec_Entry *entry,
                            TAO_AV_Flow_Protocol_Factory *factory,
                            TAO_AV_Core::Flow_Component flow_component =
                                TAO_AV_Core::TAO_AV_DATA) override;

  int close () override;
  virtual int make_svc_handler (TAO_AV_TCP_Flow_Handler *&handler);
protected:
  TAO_AV_TCP_Base_Acceptor acceptor_;
  TAO_FlowSpec_Entry *entry_;
  TAO_Base_StreamEndPoint *endpoint_;
  TAO_AV_Flow_Protocol_Factory *flow_protocol_factory_;
};

class TAO_AV_TCP_Connector;

/**
 * @class TAO_AV_Base_Connector
 * @brief
 */
class TAO_AV_TCP_Base_Connector : public ACE_Connector <TAO_AV_TCP_Flow_Handler,ACE_SOCK_CONNECTOR>
{
public:
  // To avoid warnings of open and connect hiding the base class functions these have to renamed.
  int connector_open (TAO_AV_TCP_Connector *connector,
                      ACE_Reactor *reactor);
  int connector_connect (TAO_AV_TCP_Flow_Handler *&handler,
                         const ACE_INET_Addr &remote_addr);
  int make_svc_handler (TAO_AV_TCP_Flow_Handler *& handler) override;
protected:
  TAO_AV_TCP_Connector *connector_;
  ACE_Reactor *reactor_;
};

/**
 * @class TAO_AV_TCP_Connector
 * @brief
 */
class TAO_AV_TCP_Connector : public TAO_AV_Connector
{
public:
  TAO_AV_TCP_Connector ();
  ~TAO_AV_TCP_Connector () override;

  int open (TAO_Base_StreamEndPoint *endpoint,
                    TAO_AV_Core *av_core,
                    TAO_AV_Flow_Protocol_Factory *factory) override;

  int connect (TAO_FlowSpec_Entry *entry,
                       TAO_AV_Transport *&transport,
                       TAO_AV_Core::Flow_Component flow_comp =
                           TAO_AV_Core::TAO_AV_DATA) override;
  int close () override;
  virtual int make_svc_handler (TAO_AV_TCP_Flow_Handler *&handler);
protected:
  TAO_AV_Core *av_core_;
  TAO_AV_TCP_Base_Connector connector_;
  TAO_Base_StreamEndPoint *endpoint_;
  TAO_FlowSpec_Entry *entry_;
  TAO_AV_Flow_Protocol_Factory *flow_protocol_factory_;
};

/**
 * @class TAO_AV_TCP_Object
 * @brief TAO_AV_Protocol_Object for Transmission Control Protocol (TCP)
 */
class TAO_AV_Export TAO_AV_TCP_Object : public TAO_AV_Protocol_Object
{
public:
  TAO_AV_TCP_Object (TAO_AV_Callback *callback,
                     TAO_AV_Transport *transport = 0);

  /// Dtor
  ~TAO_AV_TCP_Object () override;

  int handle_input () override;

  /// send a data frame.
  int send_frame (ACE_Message_Block *frame,
                          TAO_AV_frame_info *frame_info = 0) override;

  int send_frame (const iovec *iov,
                          int iovcnt,
                          TAO_AV_frame_info *frame_info = 0) override;

  int send_frame (const char*buf,
                          size_t len) override;

  /// end the stream.
  int destroy () override;

public:
  /// Pre-allocated memory to receive the data...
  ACE_Message_Block frame_;
};

/**
 * @class TAO_AV_TCP_Flow_Factory
 * @brief
 */
class TAO_AV_TCP_Flow_Factory : public TAO_AV_Flow_Protocol_Factory
{
public:
  /// Initialization hook.
  TAO_AV_TCP_Flow_Factory ();
  ~TAO_AV_TCP_Flow_Factory () override;
  int init (int argc, ACE_TCHAR *argv[]) override;
  int match_protocol (const char *flow_string) override;
  TAO_AV_Protocol_Object* make_protocol_object (TAO_FlowSpec_Entry *entry,
                                                TAO_Base_StreamEndPoint *endpoint,
                                                TAO_AV_Flow_Handler *handler,
                                                TAO_AV_Transport *transport) override;
};

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_STATIC_SVC_DECLARE (TAO_AV_TCP_Flow_Factory)
ACE_FACTORY_DECLARE (TAO_AV, TAO_AV_TCP_Flow_Factory)

ACE_STATIC_SVC_DECLARE (TAO_AV_TCP_Factory)
ACE_FACTORY_DECLARE (TAO_AV, TAO_AV_TCP_Factory)

#include /**/ "ace/post.h"
#endif /* TAO_AV_TCP_H */
