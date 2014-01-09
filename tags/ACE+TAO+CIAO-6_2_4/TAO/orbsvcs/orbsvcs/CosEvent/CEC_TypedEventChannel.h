// -*- C++ -*-

//=============================================================================
/**
 *  @file   CEC_TypedEventChannel.h
 *
 *  @author Jon Astle (jon@astle45.fsnet.co.uk)
 *
 *  $Id$
 *
 * A new implementation of the COS Typed Event Channel, based on
 * the untyped version by Carlos O'Ryan (coryan@cs.wustl.edu)
 *
 */
//=============================================================================

#ifndef TAO_CEC_TYPEDEVENTCHANNEL_H_
#define TAO_CEC_TYPEDEVENTCHANNEL_H_
#include /**/ "ace/pre.h"

#include "orbsvcs/CosEvent/event_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosEvent/CEC_Defaults.h"

#include "orbsvcs/CosTypedEventChannelAdminS.h"

#include "orbsvcs/CosEvent/CEC_Factory.h"

#include "tao/IFR_Client/IFR_BasicC.h"
#include "tao/AnyTypeCode/NVList.h"

#include "ace/Hash_Map_Manager.h"
#include "ace/Null_Mutex.h"
#include "ace/Truncate.h"
#include "ace/SString.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_CEC_TypedEventChannel_Attributes
 *
 * @brief Defines the construction time attributes for the Typed
 * Event Channel.
 *
 * The typed event channel implementation is controlled by
 * two mechanisms:
 * The CEC_Factory that provides the strategies for the EC
 * implementation.
 * The EC attributes that define constants and values required
 * by the EC construction.
 * This class encapsulates those constants and values, providing
 * an easy mechanism to extend the attributes without requiring
 * changes in the EC constructor.
 */
class TAO_Event_Serv_Export TAO_CEC_TypedEventChannel_Attributes
{
public:
  /**
   * The basic constructor.
   * The attributes listed as arguments are *required* by the EC, and
   * no appropiate defaults are available for them.
   */
  TAO_CEC_TypedEventChannel_Attributes (PortableServer::POA_ptr typed_supplier_poa,
                                        PortableServer::POA_ptr typed_consumer_poa,
                                        CORBA::ORB_ptr orb,
                                        CORBA::Repository_ptr interface_repository);

  // Most fields are public, there is no need to protect them, in fact
  // the user should be able to set any values she wants.

  /// Can consumers or suppliers invoke connect_push_* multiple times?
  int consumer_reconnect;
  int supplier_reconnect;

  /**
   * It not zero the event channel will send disconnect callbacks when
   * a disconnect method is called on a Proxy.  In other words, if a
   * consumer calls disconnect_push_supplier() on its proxy the EC
   * will invoke disconnect_push_consumer() on the consumer.  A
   * similar thing is done for suppliers.
   * It is a matter of debate what the spec requires for the regular
   * event service.
   */
  int disconnect_callbacks;

  /**
   * If not zero the event channel will deactive its Impl and call
   * orb->shutdown(0), when destoy is invoked.
   */
  int destroy_on_shutdown;

private:
  /// Only the EC can read the private fields.
  friend class TAO_CEC_TypedEventChannel;

  /// The POAs
  PortableServer::POA_ptr typed_supplier_poa;
  PortableServer::POA_ptr typed_consumer_poa;

  /// The ORB
  CORBA::ORB_ptr orb;

  /// The Interface Repository
  CORBA::Repository_ptr interface_repository;
};

class TAO_CEC_Operation_Params;

//Class TAO_CEC_TypedEventChannel
class TAO_Event_Serv_Export TAO_CEC_TypedEventChannel : public virtual POA_CosTypedEventChannelAdmin::TypedEventChannel
{
public:
  class ServantBaseHash
  {
  public:
    u_long operator() (PortableServer::ServantBase* const & ptr) const {
      return ACE_Utils::truncate_cast<u_long> ((intptr_t)ptr);
    }
  };

  typedef ACE_Hash_Map_Manager_Ex<PortableServer::ServantBase*,
                                  unsigned int,
                                  ServantBaseHash,
                                  ACE_Equal_To<PortableServer::ServantBase*>,
                                  TAO_SYNCH_MUTEX> ServantRetryMap;

  /**
   * Constructor
   * If <own_factory> is not 0 it assumes ownership of the factory.
   * If the factory is <nil> it uses the Service_Configurator to load
   * the Factory, if not found it uses TAO_CEC_Default_Resource_Factory
   */
  TAO_CEC_TypedEventChannel (const TAO_CEC_TypedEventChannel_Attributes& attributes,
                             TAO_CEC_Factory* factory = 0,
                             int own_factory = 0);

  /// Destructor
  virtual ~TAO_CEC_TypedEventChannel (void);

  /// Start the internal threads (if any), etc.
  /// After this call the EC can be used.
  virtual void activate (void);

  /// Shutdown any internal threads, cleanup all the internal
  /// structures, flush all the messages, etc.
  virtual void shutdown (void);

  /// Access the dispatching module....
  TAO_CEC_Dispatching* dispatching (void) const;

  /// Access the consumer admin implementation.
  TAO_CEC_TypedConsumerAdmin* typed_consumer_admin (void) const;

  /// Access the supplier admin implementation.
  TAO_CEC_TypedSupplierAdmin* typed_supplier_admin (void) const;

  /// Access the consumer control strategy.
  TAO_CEC_ConsumerControl* consumer_control (void) const;

  /// Access the supplier control strategy.
  TAO_CEC_SupplierControl* supplier_control (void) const;

  // = The factory methods, they delegate on the CEC_Factory.
  /// Create and destroy a ProxyPushSupplier
  void create_proxy (TAO_CEC_ProxyPushSupplier*&);
  void destroy_proxy (TAO_CEC_ProxyPushSupplier*);

  /// Create and destroy a TypedProxyPushConsumer
  void create_proxy (TAO_CEC_TypedProxyPushConsumer*&);
  void destroy_proxy (TAO_CEC_TypedProxyPushConsumer*);

  /// Create and destroy a the collections used to store
  /// Proxy*Suppliers
  void create_proxy_collection (TAO_CEC_ProxyPushSupplier_Collection*&);
  void destroy_proxy_collection (TAO_CEC_ProxyPushSupplier_Collection*);

  /// Create and destroy a the collections used to store
  /// Proxy*Consumers
  void create_proxy_collection (TAO_CEC_TypedProxyPushConsumer_Collection*&);
  void destroy_proxy_collection (TAO_CEC_TypedProxyPushConsumer_Collection*);

  /// Access the supplier and consumer POAs from the factory.
  PortableServer::POA_ptr typed_supplier_poa (void);
  PortableServer::POA_ptr typed_consumer_poa (void);

  /// Locking strategies for the ProxyPushConsumer and
  /// ProxyPushSupplier objects
  ACE_Lock* create_consumer_lock (void);
  void destroy_consumer_lock (ACE_Lock*);
  ACE_Lock* create_supplier_lock (void);
  void destroy_supplier_lock (ACE_Lock*);

  /// Used to inform the EC that a Consumer has connected or
  /// disconnected from it.
  virtual void connected (TAO_CEC_TypedProxyPushConsumer*);
  virtual void reconnected (TAO_CEC_TypedProxyPushConsumer*);
  virtual void disconnected (TAO_CEC_TypedProxyPushConsumer*);

  /// Used to inform the EC that a Supplier has connected or
  /// disconnected from it.
  virtual void connected (TAO_CEC_ProxyPushSupplier*);
  virtual void reconnected (TAO_CEC_ProxyPushSupplier*);
  virtual void disconnected (TAO_CEC_ProxyPushSupplier*);

  /// Can the consumers reconnect to the EC?
  int consumer_reconnect (void) const;

  /// Can the suppliers reconnect to the EC?
  int supplier_reconnect (void) const;

  /// Should we send callback disconnect messages when a proxy is
  /// disconnected by the client
  int disconnect_callbacks (void) const;

  // Hash map which will operate as a IFR cache for the Supported Interface's operations and parameters
  typedef ACE_Hash_Map_Manager_Ex<const char *, TAO_CEC_Operation_Params *, ACE_Hash<const char *>, ACE_Equal_To<const char *>, ACE_Null_Mutex> InterfaceDescription;
  typedef InterfaceDescription::iterator Iterator;

  /// Finds a operation/parameter from the IFR cache
  TAO_CEC_Operation_Params * find_from_ifr_cache (const char *operation);

  /// Function allows consumer admin to register the uses interface
  int consumer_register_uses_interace (const char *uses_interface);

  /// Function allows supplier admin to register the supported interface
  int supplier_register_supported_interface (const char *supported_interface);

  /// Function to return the supported_interface_
  const char * supported_interface (void) const;

  /// Function to return the base_interfaces_
  CORBA::RepositoryId base_interfaces (CORBA::ULong index) const;

  /// Function to return the number of base_interfaces_
  CORBA::ULong number_of_base_interfaces (void) const;

  /// Function populates the NVList from the provide param information
  virtual void create_operation_list (TAO_CEC_Operation_Params *oper_params,
                                      CORBA::NVList_out new_list);

  /// Function creates an empty NVList
  virtual void create_list (CORBA::Long count,
                            CORBA::NVList_out new_list);

  // = The CosTypedEventChannelAdmin::TypedEventChannel methods...
  virtual ::CosTypedEventChannelAdmin::TypedConsumerAdmin_ptr
    for_consumers (void);

  virtual ::CosTypedEventChannelAdmin::TypedSupplierAdmin_ptr
    for_suppliers (void);

  virtual void destroy (void);

  ServantRetryMap& get_servant_retry_map (void);

  /// Forwarded to the factory
  CORBA::Policy_ptr
  create_roundtrip_timeout_policy (const ACE_Time_Value &timeout);

protected:
  /// Function caches the full interface description from the IFR
  int cache_interface_description (const char *interface_);

  /// Insert a operation/parameter into the IFR cache
  int insert_into_ifr_cache (const char *operation, TAO_CEC_Operation_Params *parameters);

  /// Function clears the IFR cache
  int clear_ifr_cache (void);

private:
  /// The POAs used to activate "supplier-side" and "consumer-side"
  /// objects.
  PortableServer::POA_var typed_supplier_poa_;
  PortableServer::POA_var typed_consumer_poa_;

  /// The ORB
  CORBA::ORB_var orb_;

  /// Storage of the IFR reference
  CORBA::Repository_var interface_repository_;

  /**
   * This is the abstract factory that creates all the objects that
   * compose an event channel, the event channel simply acts as a
   * Mediator among them.
   */
  TAO_CEC_Factory *factory_;

  /// Flag that indicates if we own the factory.
  int own_factory_;

  /// The dispatching "module"
  TAO_CEC_Dispatching *dispatching_;

  /// The ConsumerAdmin implementation
  TAO_CEC_TypedConsumerAdmin *typed_consumer_admin_;

  /// The SupplierAdmin implementation
  TAO_CEC_TypedSupplierAdmin *typed_supplier_admin_;

  /// Consumer reconnection flags
  int consumer_reconnect_;

  /// Supplier reconnection flags
  int supplier_reconnect_;

  /// If not zero we send callbacks when a proxy is disconnected
  int disconnect_callbacks_;

  /// If not zero the event channel is destroyed on shutdown
  int destroy_on_shutdown_;

  /// Set if the event channel has been destroyed
  int destroyed_;

  /// Strategies to disconnect misbehaving or destroyed consumers and
  /// suppliers
  TAO_CEC_ConsumerControl *consumer_control_;
  ServantRetryMap retry_map_;

  TAO_CEC_SupplierControl *supplier_control_;

  /// The uses_interface_ for the TypedConsumerAdmin.
  ACE_CString uses_interface_;

  /// The supported_interface_ for the TypedSupplierAdmin.
  ACE_CString supported_interface_;

  /// The IFR cache for the interface description
  InterfaceDescription interface_description_;

  /// The supported_interface_ base interfaces
  CORBA::RepositoryIdSeq base_interfaces_;
};


class TAO_Event_Serv_Export TAO_CEC_Param
{
public:
  /// Constructor
  TAO_CEC_Param (void);

  /// Destructor
  ~TAO_CEC_Param (void);

private:
  /// Only the TypedEventChannel can read the private fields.
  friend class TAO_CEC_TypedEventChannel;

  CORBA::String_var name_;
  CORBA::TypeCode_var type_;
  CORBA::Flags direction_;
};


class TAO_Event_Serv_Export TAO_CEC_Operation_Params
{
  /// constructor
  TAO_CEC_Operation_Params (CORBA::ULong num_params);
  /// destructor
  ~TAO_CEC_Operation_Params (void);

private:
  /// Only the TypedEventChannel can read the private fields.
  friend class TAO_CEC_TypedEventChannel;

  CORBA::ULong num_params_;
  TAO_CEC_Param * parameters_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "orbsvcs/CosEvent/CEC_TypedEventChannel.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_CEC_TYPEDEVENTCHANNEL_H_ */
