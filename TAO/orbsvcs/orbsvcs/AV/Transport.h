// -*- C++ -*-

//=============================================================================
/**
 *  @file   Transport.h
 *
 *  @author Nagarajan Surendran <naga@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_AV_TRANSPORT_H
#define TAO_AV_TRANSPORT_H
#include /**/ "ace/pre.h"

#include <list>

#include "orbsvcs/AV/FlowSpec_Entry.h"
#include "orbsvcs/AV/AV_Core.h"

#include "orbsvcs/AV/AV_export.h"

#include "ace/Service_Object.h"
#include "ace/Acceptor.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/SOCK_Connector.h"
#include "ace/Connector.h"
#include "ace/Addr.h"
#include "ace/SOCK_Dgram.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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
  /// Creator method, the Transport name can only be set when the
  /// object is created.
  explicit TAO_AV_Transport_Item (ACE_CString const &name);
  TAO_AV_Transport_Item (TAO_AV_Transport_Item const &) = delete;
  TAO_AV_Transport_Item &operator= (TAO_AV_Transport_Item const &) = delete;
  TAO_AV_Transport_Item (TAO_AV_Transport_Item &&) = delete;
  TAO_AV_Transport_Item &operator= (TAO_AV_Transport_Item &&) = delete;

  /// Return a reference to the character representation of the Transport
  /// factories name.
  ACE_CString const &name ();

  /// Return a pointer to the Transport factory.
  TAO_AV_Transport_Factory *factory ();

  /// Set the factory pointer's valus.
  void factory (TAO_AV_Transport_Factory *factory);

private:
  /// Transport factory name.
  ACE_CString name_;

  /// Pointer to factory object.
  TAO_AV_Transport_Factory *factory_ {};
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
  explicit TAO_AV_Flow_Protocol_Item (ACE_CString const &name);
  TAO_AV_Flow_Protocol_Item (TAO_AV_Flow_Protocol_Item const &) = delete;
  TAO_AV_Flow_Protocol_Item &operator= (TAO_AV_Flow_Protocol_Item const &) = delete;
  TAO_AV_Flow_Protocol_Item (TAO_AV_Flow_Protocol_Item &&) = delete;
  TAO_AV_Flow_Protocol_Item &operator= (TAO_AV_Flow_Protocol_Item &&) = delete;

  /// return a reference to the character representation of the Flow_Protocol
  /// factories name.
  ACE_CString const &name ();

  /// return a pointer to the Flow_Protocol factory.
  TAO_AV_Flow_Protocol_Factory *factory ();

  /// set the factory pointer's valus.
  void factory (TAO_AV_Flow_Protocol_Factory *factory);

private:
  /// Flow_Protocol factory name.
  ACE_CString name_;

  /// pointer to factory object.
  TAO_AV_Flow_Protocol_Factory *factory_ {};
};

/**
 * @class TAO_AV_Flow_Handler
 * @brief
 */
class TAO_AV_Export TAO_AV_Flow_Handler
{
public:
  /// Constructor.
  TAO_AV_Flow_Handler ();

  virtual ~TAO_AV_Flow_Handler ();
  TAO_AV_Flow_Handler (TAO_AV_Flow_Handler const &) = delete;
  TAO_AV_Flow_Handler &operator= (TAO_AV_Flow_Handler const &) = delete;
  TAO_AV_Flow_Handler (TAO_AV_Flow_Handler &&) = delete;
  TAO_AV_Flow_Handler &operator= (TAO_AV_Flow_Handler &&) = delete;
  /// Start/stop the flow handler.
  virtual int start (TAO_FlowSpec_Entry::Role role);
  virtual int stop  (TAO_FlowSpec_Entry::Role role);

  /// Schedule timer. Uses the get_timeout method on the callback.
  virtual int schedule_timer ();
  virtual int cancel_timer ();

  /// get the transport.
  TAO_AV_Transport *transport ();

  /// set/get protocol_object.
  TAO_AV_Protocol_Object* protocol_object ();
  void protocol_object (TAO_AV_Protocol_Object *protocol_object);

  /// set the callback.
  void callback (TAO_AV_Callback *callback);

  /// Handle timeout. called from reactor.
  virtual int handle_timeout (ACE_Time_Value const &tv, void const *arg = nullptr);

  /// set the remote address.
  virtual int set_remote_address (ACE_Addr *address);

  /// get the underlying event handler. To be overridden by the derived clases.
  virtual ACE_Event_Handler *event_handler () = 0;

  virtual int change_qos (AVStreams::QoS);

protected:
  TAO_AV_Transport *transport_ {};
  TAO_AV_Callback *callback_ {};
  TAO_AV_Protocol_Object *protocol_object_ {};
  long timer_id_ {-1};
  ACE_Reactor *reactor_ {};
  void *timeout_arg_ {};
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
  TAO_AV_Transport ();

  virtual ~TAO_AV_Transport ();
  TAO_AV_Transport (TAO_AV_Transport const &) = delete;
  TAO_AV_Transport &operator= (TAO_AV_Transport const &) = delete;
  TAO_AV_Transport (TAO_AV_Transport &&) = delete;
  TAO_AV_Transport &operator= (TAO_AV_Transport &&) = delete;

  virtual int open (ACE_Addr *address) = 0;

  virtual int close () = 0;

  /// Write the complete Message_Block chain to the connection.
  virtual int mtu () = 0;
  virtual ACE_Addr *get_peer_addr () = 0;
  virtual ACE_Addr *get_local_addr ();
  virtual ssize_t send (ACE_Message_Block const *mblk,
                        ACE_Time_Value *s = nullptr) = 0;

  /// Write the contents of the buffer of length len to the connection.
  virtual ssize_t send (char const *buf,
                        size_t len,
                        ACE_Time_Value *s = nullptr) = 0;

  /// Write the contents of iovcnt iovec's to the connection.
  virtual ssize_t send (iovec const *iov,
                        int iovcnt,
                        ACE_Time_Value *s = nullptr) = 0;

  /// Read len bytes from into buf.
  virtual ssize_t recv (char *buf,
                        size_t len,
                        ACE_Time_Value *s = nullptr) = 0;

  /// Read len bytes from into buf using flags.
  virtual ssize_t recv (char *buf,
                        size_t len,
                        int flags,
                        ACE_Time_Value *s = nullptr) = 0;

  ///  Read received data into the iovec buffers.
  virtual ssize_t recv (iovec *iov,
                        int iovcnt,
                        ACE_Time_Value *s = nullptr) = 0;
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
  TAO_AV_Acceptor ();
  virtual ~TAO_AV_Acceptor ();
  TAO_AV_Acceptor (TAO_AV_Acceptor const &) = delete;
  TAO_AV_Acceptor &operator= (TAO_AV_Acceptor const &) = delete;
  TAO_AV_Acceptor (TAO_AV_Acceptor &&) = delete;
  TAO_AV_Acceptor &operator= (TAO_AV_Acceptor &&) = delete;
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

  char const *flowname ();
  virtual int close () = 0;
protected:
  ACE_CString flowname_;
  TAO_AV_Core *av_core_ {};
  ACE_Addr *address_ {};
};

/**
 * @class TAO_AV_Connector
 * @brief
 */
class TAO_AV_Export TAO_AV_Connector
{
public:
  TAO_AV_Connector ();
  virtual ~TAO_AV_Connector ();
  TAO_AV_Connector (TAO_AV_Connector const &) = delete;
  TAO_AV_Connector &operator= (TAO_AV_Connector const &) = delete;
  TAO_AV_Connector (TAO_AV_Connector &&) = delete;
  TAO_AV_Connector &operator= (TAO_AV_Connector &&) = delete;
  char const *flowname ();

  virtual int open (TAO_Base_StreamEndPoint *endpoint,
                    TAO_AV_Core *av_core,
                    TAO_AV_Flow_Protocol_Factory *factory) = 0;

  virtual int connect (TAO_FlowSpec_Entry *entry,
                       TAO_AV_Transport *&transport,
                       TAO_AV_Core::Flow_Component flow_component) = 0;

  virtual int close () = 0;
protected:
  ACE_CString flowname_;
};

using TAO_AV_ConnectorSet = std::list<TAO_AV_Connector *>;
using TAO_AV_ConnectorSetItor = TAO_AV_ConnectorSet::iterator;

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
  TAO_AV_Connector_Registry ();
  ~TAO_AV_Connector_Registry ();
  TAO_AV_Connector_Registry (TAO_AV_Connector_Registry const &) = delete;
  TAO_AV_Connector_Registry &operator= (TAO_AV_Connector_Registry const &) = delete;
  TAO_AV_Connector_Registry (TAO_AV_Connector_Registry &&) = delete;
  TAO_AV_Connector_Registry &operator= (TAO_AV_Connector_Registry &&) = delete;
  int open (TAO_Base_StreamEndPoint *endpoint,
            TAO_AV_Core *av_core,
            TAO_AV_FlowSpecSet &flow_spec_set);
  int close (TAO_AV_Connector *connector);
  int close_all ();
  TAO_AV_ConnectorSetItor begin ();
  TAO_AV_ConnectorSetItor end ();
protected:
  TAO_AV_ConnectorSet connectors_;
};

using TAO_AV_AcceptorSet = std::list<TAO_AV_Acceptor *>;
using TAO_AV_AcceptorSetItor = TAO_AV_AcceptorSet::iterator;

/**
 * @class TAO_AV_Acceptor_Registry
 * @brief
 */
class TAO_AV_Export TAO_AV_Acceptor_Registry
{
public:
  TAO_AV_Acceptor_Registry ();
  ~TAO_AV_Acceptor_Registry ();
  TAO_AV_Acceptor_Registry (TAO_AV_Acceptor_Registry const &) = delete;
  TAO_AV_Acceptor_Registry &operator= (TAO_AV_Acceptor_Registry const &) = delete;
  TAO_AV_Acceptor_Registry (TAO_AV_Acceptor_Registry &&) = delete;
  TAO_AV_Acceptor_Registry &operator= (TAO_AV_Acceptor_Registry &&) = delete;
  int open (TAO_Base_StreamEndPoint *endpoint,
            TAO_AV_Core *av_core,
            TAO_AV_FlowSpecSet &flow_spec_set);
  int close (TAO_AV_Acceptor *acceptor);
  int close_all ();
  TAO_AV_AcceptorSetItor begin ();
  TAO_AV_AcceptorSetItor end ();
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
  TAO_AV_Transport_Factory ();
  ~TAO_AV_Transport_Factory () override;
  TAO_AV_Transport_Factory (TAO_AV_Transport_Factory const &) = delete;
  TAO_AV_Transport_Factory &operator= (TAO_AV_Transport_Factory const &) = delete;
  TAO_AV_Transport_Factory (TAO_AV_Transport_Factory &&) = delete;
  TAO_AV_Transport_Factory &operator= (TAO_AV_Transport_Factory &&) = delete;
  virtual int init (int argc, ACE_TCHAR *argv[]);
  virtual int match_protocol (char const *protocol_string);
  virtual TAO_AV_Acceptor *make_acceptor ();
  virtual TAO_AV_Connector *make_connector ();
  int ref_count {};
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "orbsvcs/AV/Transport.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_AV_TRANSPORT_H */
