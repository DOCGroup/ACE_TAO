/* -*- C++ -*- */

//=============================================================================
/**
 *  @file   Transport.h
 *
 *  $Id$
 *
 *  @author Nagarajan Surendran <naga@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_AV_TRANSPORT_H
#define TAO_AV_TRANSPORT_H
#include "ace/pre.h"

#include "ace/Service_Object.h"
#include "ace/Acceptor.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/SOCK_Connector.h"
#include "ace/Connector.h"
#include "ace/Addr.h"
#include "ace/SOCK_Dgram.h"
#include "orbsvcs/AV/AV_export.h"
#include "FlowSpec_Entry.h"
#include "AV_Core.h"

// Forward declarations.
class TAO_AV_Core;
class TAO_AV_Transport_Factory;
class TAO_AV_Protocol_Object;
class TAO_AV_Callback;
class TAO_AV_Transport;

/**
 * @class TAO_AV_Transport_Item
 * @brief
 */
class TAO_AV_Export TAO_AV_Transport_Item
{
public:
  /// creator method, the Transport name can only be set when the
  /// object is created.
  TAO_AV_Transport_Item (const ACE_CString &name);

  /// return a reference to the character representation of the Transport
  /// factories name.
  const ACE_CString &name (void);

  /// return a pointer to the Transport factory.
  TAO_AV_Transport_Factory *factory (void);

  /// set the factory pointer's valus.
  void factory (TAO_AV_Transport_Factory *factory);

private:
  /// Transport factory name.
  ACE_CString name_;

  /// pointer to factory object.
  TAO_AV_Transport_Factory *factory_;
};




class TAO_AV_Flow_Protocol_Factory;

/**
 * @class TAO_AV_Flow_Protocol_Item
 * @brief
 */
class TAO_AV_Export TAO_AV_Flow_Protocol_Item
{
public:
  /// creator method, the Flow_Protocol name can only be set when the
  /// object is created.
  TAO_AV_Flow_Protocol_Item (const ACE_CString &name);

  /// return a reference to the character representation of the Flow_Protocol
  /// factories name.
  const ACE_CString &name (void);

  /// return a pointer to the Flow_Protocol factory.
  TAO_AV_Flow_Protocol_Factory *factory (void);

  /// set the factory pointer's valus.
  void factory (TAO_AV_Flow_Protocol_Factory *factory);

private:
  /// Flow_Protocol factory name.
  ACE_CString name_;

  /// pointer to factory object.
  TAO_AV_Flow_Protocol_Factory *factory_;
};


/**
 * @class TAO_AV_Flow_Handler
 * @brief
 */
class TAO_AV_Export TAO_AV_Flow_Handler
{
public:
  /// Constructor.
  TAO_AV_Flow_Handler (void);

  virtual ~TAO_AV_Flow_Handler (void);
  /// Start/stop the flow handler.
  virtual int start (TAO_FlowSpec_Entry::Role role);
  virtual int stop  (TAO_FlowSpec_Entry::Role role);

  /// Schedule timer. Uses the get_timeout method on the callback.
  virtual int schedule_timer (void);
  virtual int cancel_timer (void);

  /// get the transport.
  TAO_AV_Transport *transport (void);

  /// set/get protocol_object.
  TAO_AV_Protocol_Object* protocol_object (void);
  void protocol_object (TAO_AV_Protocol_Object *protocol_object);

  /// set the callback.
  void callback (TAO_AV_Callback *callback);

  /// Handle timeout. called from reactor.
  virtual int handle_timeout (const ACE_Time_Value &tv, const void *arg = 0);

  /// set the remote address.
  virtual int set_remote_address (ACE_Addr *address);

  /// get the underlying event handler. To be overridden by the derived clases.
  virtual ACE_Event_Handler* event_handler (void) = 0;

  virtual int change_qos (AVStreams::QoS);

protected:
  TAO_AV_Transport *transport_;
  TAO_AV_Callback *callback_;
  TAO_AV_Protocol_Object *protocol_object_;
  long timer_id_;
  ACE_Reactor *reactor_;
  void *timeout_arg_;
};

//  Transports
/**
 * @class TAO_AV_Transport
 * @brief A Base class for the different transport protocols.
 *        All the different transports should derive and implement
 *        the open,close,send and recv methods.
 */
class TAO_AV_Export TAO_AV_Transport
{
public:
  TAO_AV_Transport (void);

  virtual ~TAO_AV_Transport (void);

  virtual int open (ACE_Addr *address) = 0;

  virtual int close (void) = 0;

  /// Write the complete Message_Block chain to the connection.
  virtual int mtu (void) = 0;
  virtual ACE_Addr *get_peer_addr (void) = 0;
  virtual ACE_Addr *get_local_addr (void);
  virtual ssize_t send (const ACE_Message_Block *mblk,
                        ACE_Time_Value *s = 0) = 0;

  /// Write the contents of the buffer of length len to the connection.
  virtual ssize_t send (const char *buf,
                        size_t len,
                        ACE_Time_Value *s = 0) = 0;

  /// Write the contents of iovcnt iovec's to the connection.
  virtual ssize_t send (const iovec *iov,
                        int iovcnt,
                        ACE_Time_Value *s = 0) = 0;

  /// Read len bytes from into buf.
  virtual ssize_t recv (char *buf,
                        size_t len,
                        ACE_Time_Value *s = 0) = 0;

  /// Read len bytes from into buf using flags.
  virtual ssize_t recv (char *buf,
                        size_t len,
                        int flags,
                        ACE_Time_Value *s = 0) = 0;

  ///  Read received data into the iovec buffers.
  virtual ssize_t recv (iovec *iov,
                        int iovcnt,
                        ACE_Time_Value *s = 0) = 0;

};

class TAO_Base_StreamEndPoint;
class TAO_AV_Core;
class TAO_FlowSpec_Entry;

/**
 * @class TAO_AV_Acceptor
 * @brief
 */
class TAO_AV_Export TAO_AV_Acceptor
{
public:
  TAO_AV_Acceptor (void);
  virtual ~TAO_AV_Acceptor (void);
  virtual int open (TAO_Base_StreamEndPoint *endpoint,
                    TAO_AV_Core *av_core,
                    TAO_FlowSpec_Entry *entry,
                    TAO_AV_Flow_Protocol_Factory *factory,
                    TAO_AV_Core::Flow_Component flow_comp) = 0;

  virtual int open_default (TAO_Base_StreamEndPoint *endpoint,
                            TAO_AV_Core *av_core,
                            TAO_FlowSpec_Entry *entry,
                            TAO_AV_Flow_Protocol_Factory *factory,
                            TAO_AV_Core::Flow_Component flow_comp) = 0;

  const char *flowname ();
  virtual int close (void) = 0;
protected:
  ACE_CString flowname_;
  TAO_AV_Core *av_core_;
  ACE_Addr *address_;
};

/**
 * @class TAO_AV_Connector
 * @brief
 */
class TAO_AV_Export TAO_AV_Connector
{
public:
  TAO_AV_Connector (void);
  virtual ~TAO_AV_Connector (void);
  const char *flowname (void);

  virtual int open (TAO_Base_StreamEndPoint *endpoint,
                    TAO_AV_Core *av_core,
                    TAO_AV_Flow_Protocol_Factory *factory) = 0;

  virtual int connect (TAO_FlowSpec_Entry *entry,
                       TAO_AV_Transport *&transport,
                       TAO_AV_Core::Flow_Component flow_component) = 0;

  virtual int close (void) = 0;
protected:
  ACE_CString flowname_;
};


typedef ACE_Unbounded_Set<TAO_AV_Connector*> TAO_AV_ConnectorSet;
typedef ACE_Unbounded_Set_Iterator<TAO_AV_Connector*> TAO_AV_ConnectorSetItor;

/**
 * @class TAO_AV_Connector_Registry
 * @brief
 */
class TAO_AV_Export TAO_AV_Connector_Registry
{
public:
  /**
   *  int connect (TAO_FlowSpec_Entry *flowspec,
   *               TAO_AV_Transport *&transport);
   *  TAO_AV_Connector *get_connector (TAO_AV_Core::Protocol protocol);
   * Return the connector bridges
   */
  TAO_AV_Connector_Registry (void);
  ~TAO_AV_Connector_Registry (void);
  int open (TAO_Base_StreamEndPoint *endpoint,
            TAO_AV_Core *av_core,
            TAO_AV_FlowSpecSet &flow_spec_set);
  int close_all (void);
  TAO_AV_ConnectorSetItor begin (void);
  TAO_AV_ConnectorSetItor end (void);
protected:
  TAO_AV_ConnectorSet connectors_;
};

typedef ACE_Unbounded_Set<TAO_AV_Acceptor*> TAO_AV_AcceptorSet;
typedef ACE_Unbounded_Set_Iterator<TAO_AV_Acceptor*> TAO_AV_AcceptorSetItor;

/**
 * @class TAO_AV_Acceptor_Registry
 * @brief
 */
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

/**
 * @class TAO_AV_Transport_Factory
 * @brief
 */
class TAO_AV_Export TAO_AV_Transport_Factory : public ACE_Service_Object
{
public:
  /// Initialization hook.
  TAO_AV_Transport_Factory (void);
  virtual ~TAO_AV_Transport_Factory (void);
  virtual int init (int argc, char *argv[]);
  virtual int match_protocol (const char *protocol_string);
  virtual TAO_AV_Acceptor *make_acceptor (void);
  virtual TAO_AV_Connector *make_connector (void);
  int ref_count;
};


#if defined (__ACE_INLINE__)
#include "Transport.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_AV_TRANSPORT_H */
