// -*- C++ -*-

/**
 *  @file Properties.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 */

#ifndef TAO_Notify_PROPERTIES_H
#define TAO_Notify_PROPERTIES_H

#include /**/ "ace/pre.h"

#include "orbsvcs/Notify/notify_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosNotificationC.h"
#include "orbsvcs/CosNotifyChannelAdminC.h"

#include "tao/PortableServer/PortableServer.h"

#include "tao/TAO_Singleton.h"
#include "tao/ORB.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Notify_Factory;
class TAO_Notify_Builder;

/**
 * @class TAO_Notify_Properties
 *
 * @brief Global properties that strategize Notify's run-time behaviour.
 *
 */
class TAO_Notify_Serv_Export TAO_Notify_Properties
{
public:
  /// Constructor
  TAO_Notify_Properties (void);

  /// Destructor
  ~TAO_Notify_Properties ();

  /// Return a singleton instance of this class.
  static TAO_Notify_Properties * instance (void);

  static void close (void);

  // = Property Accessors
  TAO_Notify_Factory* factory (void);
  void factory (TAO_Notify_Factory* factory);

  TAO_Notify_Builder* builder (void);
  void builder (TAO_Notify_Builder* builder);

  CORBA::ORB_ptr orb (void);
  void orb (CORBA::ORB_ptr orb);
  CORBA::ORB_ptr dispatching_orb (void);
  void dispatching_orb (CORBA::ORB_ptr dispatching_orb);

  PortableServer::POA_ptr default_poa (void);
  void default_poa (PortableServer::POA_ptr default_poa);

  CORBA::Boolean asynch_updates (void);
  void asynch_updates (CORBA::Boolean asynch_updates);

  bool allow_reconnect (void);
  void allow_reconnect (bool b);
  bool validate_client (void);
  void validate_client (bool b);
  ACE_Time_Value validate_client_delay (void);
  void validate_client_delay (ACE_Time_Value b);
  ACE_Time_Value validate_client_interval (void);
  void validate_client_interval (ACE_Time_Value b);

  // Turn on/off update messages.
  CORBA::Boolean updates (void);
  void updates (CORBA::Boolean updates);
  bool separate_dispatching_orb (void);
  void separate_dispatching_orb (bool b);

  // The QoS Property that must be applied to each newly created Event Channel
  const CosNotification::QoSProperties& default_event_channel_qos_properties (void);

  // Set the default EC QoS Property.
  void default_event_channel_qos_properties (const CosNotification::QoSProperties &ec_qos);

  // The QoS Property that must be applied to each newly created Supplier Admin
  const CosNotification::QoSProperties& default_supplier_admin_qos_properties (void);

  // Set the default SA QoS Property.
  void default_supplier_admin_qos_properties (const CosNotification::QoSProperties &sa_qos);

  // The QoS Property that must be applied to each newly created Consumer Admin
  const CosNotification::QoSProperties& default_consumer_admin_qos_properties (void);

  // Set the default CA QoS Property.
  void default_consumer_admin_qos_properties (const CosNotification::QoSProperties &ca_qos);

  // The QoS Property that must be applied to each newly created Proxy Supplier
  const CosNotification::QoSProperties& default_proxy_supplier_qos_properties (void);

  // Set the default PS QoS Property.
  void default_proxy_supplier_qos_properties (const CosNotification::QoSProperties &ps_qos);

  // The QoS Property that must be applied to each newly created Proxy Consumer
  const CosNotification::QoSProperties& default_proxy_consumer_qos_properties (void);

  // Set the default PC QoS Property.
  void default_proxy_consumer_qos_properties (const CosNotification::QoSProperties &pc_qos);

  CosNotifyChannelAdmin::InterFilterGroupOperator defaultConsumerAdminFilterOp (void);
  void defaultConsumerAdminFilterOp (CosNotifyChannelAdmin::InterFilterGroupOperator op);

  CosNotifyChannelAdmin::InterFilterGroupOperator defaultSupplierAdminFilterOp (void);
  void defaultSupplierAdminFilterOp (CosNotifyChannelAdmin::InterFilterGroupOperator op);

protected:
  /// Object Factory
  TAO_Notify_Factory* factory_;

  /// Object Builder
  TAO_Notify_Builder* builder_;

  /// ORB
  CORBA::ORB_var orb_;

  /// dispatching orb
  CORBA::ORB_var dispatching_orb_;

  /// POA
  PortableServer::POA_var default_poa_;

  /// True if send asynch updates.
  CORBA::Boolean asynch_updates_;

  /// True if clients can reconnect to proxies.
  bool allow_reconnect_;
  bool validate_client_;
  ACE_Time_Value validate_client_delay_;
  ACE_Time_Value validate_client_interval_;

  /// True is separate dispatching orb
  bool separate_dispatching_orb_;

  /// True if updates are enabled (default).
  CORBA::Boolean updates_;

  /// The Update period for updates.
  ACE_Time_Value update_period_;

  /// The default EC QoS Properties.
  CosNotification::QoSProperties ec_qos_;

  /// The default SA QoS Properties.
  CosNotification::QoSProperties sa_qos_;

  /// The default CA QoS Properties.
  CosNotification::QoSProperties ca_qos_;

  /// The default PS QoS Properties.
  CosNotification::QoSProperties ps_qos_;

  /// The default PC QoS Properties.
  CosNotification::QoSProperties pc_qos_;

  /// The default consumer admin filter operator.
  CosNotifyChannelAdmin::InterFilterGroupOperator defaultConsumerAdminFilterOp_;

  /// The default supplier admin filter operator.
  CosNotifyChannelAdmin::InterFilterGroupOperator defaultSupplierAdminFilterOp_;
};

/**
 * @todo Remove this legacy TAO_Notify_Properties typedef.
 */
typedef TAO_Notify_Properties TAO_Notify_PROPERTIES;

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "orbsvcs/Notify/Properties.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_Notify_PROPERTIES_H */
