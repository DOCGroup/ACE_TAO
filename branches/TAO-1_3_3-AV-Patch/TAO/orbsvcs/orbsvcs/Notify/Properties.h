/* -*- C++ -*- */
/**
 *  @file Properties.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_Notify_PROPERTIES_H
#define TAO_Notify_PROPERTIES_H
#include /**/ "ace/pre.h"

#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/TAO_Singleton.h"
#include "tao/ORB.h"
#include "tao/PortableServer/PortableServer.h"
#include "orbsvcs/CosNotificationC.h"

class TAO_Notify_Factory;
class TAO_Notify_Builder;

/**
 * @class TAO_Notify_Properties
 *
 * @brief Global properties that strategize Notify's run-time behaviour.
 *
 */
class TAO_Notify_Export TAO_Notify_Properties
{
  friend class TAO_Singleton<TAO_Notify_Properties, TAO_SYNCH_MUTEX>;

public:
  /// Constuctor
  TAO_Notify_Properties (void);

  /// Destructor
  ~TAO_Notify_Properties ();

  // = Property Accessors
  TAO_Notify_Factory* factory (void);
  void factory (TAO_Notify_Factory* factory);

  TAO_Notify_Builder* builder (void);
  void builder (TAO_Notify_Builder* builder);

  CORBA::ORB_ptr orb (void);
  void orb (CORBA::ORB_ptr orb);

  PortableServer::POA_ptr default_poa (void);
  void default_poa (PortableServer::POA_ptr default_poa);

  CORBA::Boolean asynch_updates (void);
  void asynch_updates (CORBA::Boolean asynch_updates);

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

protected:
  /// Object Factory
  TAO_Notify_Factory* factory_;

  /// Object Builder
  TAO_Notify_Builder* builder_;

  /// ORB
  CORBA::ORB_var orb_;

  // POA
  PortableServer::POA_var default_poa_;

  /// True if send asynch updates.
  CORBA::Boolean asynch_updates_;

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
};

typedef TAO_Singleton<TAO_Notify_Properties, TAO_SYNCH_MUTEX> TAO_Notify_PROPERTIES;

TAO_NOTIFY_SINGLETON_DECLARE (TAO_Singleton, TAO_Notify_Properties, TAO_SYNCH_MUTEX);

#if defined (__ACE_INLINE__)
#include "Properties.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_Notify_PROPERTIES_H */
