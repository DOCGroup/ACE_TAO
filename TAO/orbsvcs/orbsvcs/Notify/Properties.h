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

#ifndef TAO_NS_PROPERTIES_H
#define TAO_NS_PROPERTIES_H
#include "ace/pre.h"

#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Singleton.h"
#include "tao/ORB.h"
#include "tao/PortableServer/PortableServer.h"
#include "orbsvcs/CosNotificationC.h"

class TAO_NS_Factory;
class TAO_NS_Builder;

/**
 * @class TAO_NS_Properties
 *
 * @brief Global properties that strategize RT Notify's run-time behaviour.
 *
 */
class TAO_Notify_Export TAO_NS_Properties
{
  friend class ACE_Singleton<TAO_NS_Properties, TAO_SYNCH_MUTEX>;

public:
  /// Constuctor
  TAO_NS_Properties (void);

  /// Destructor
  ~TAO_NS_Properties ();

  // = Property Accessors
  TAO_NS_Factory* factory (void);
  void factory (TAO_NS_Factory* factory);

  TAO_NS_Builder* builder (void);
  void builder (TAO_NS_Builder* builder);

  CORBA::ORB_ptr orb (void);
  void orb (CORBA::ORB_ptr orb);

  PortableServer::POA_ptr default_poa (void);
  void default_poa (PortableServer::POA_ptr default_poa);

  long scope_policy (void);
  void scope_policy (long scope_policy);

  long sched_policy (void);
  void sched_policy (long sched_policy);

  CORBA::Boolean updates (void);
  void updates (CORBA::Boolean updates);

  ACE_Time_Value& update_period (void);
  void update_period (ACE_Time_Value& time_value);

  // The QoS Property that must be applied to each newly created Event Channel
  const CosNotification::QoSProperties& default_event_channel_qos_properties (void);

  // Set the default EC QoS Property.
  void default_event_channel_qos_properties (const CosNotification::QoSProperties &ec_qos);

protected:
  /// Object Factory
  TAO_NS_Factory* factory_;

  /// Object Builder
  TAO_NS_Builder* builder_;

  /// ORB
  CORBA::ORB_var orb_;

  // POA
  PortableServer::POA_var default_poa_;

  /// Sched policy
  long thr_sched_policy_;

  /// Scope policy
  long thr_scope_policy_;

  /// True if updates are sent.
  CORBA::Boolean updates_;

  /// The Update period for updates.
  ACE_Time_Value update_period_;

  /// The default EC QoS Properties.
  CosNotification::QoSProperties ec_qos_;
};

typedef ACE_Singleton<TAO_NS_Properties, TAO_SYNCH_MUTEX> TAO_NS_PROPERTIES;

TAO_NOTIFY_SINGLETON_DECLARE (ACE_Singleton, TAO_NS_Properties, TAO_SYNCH_MUTEX);

#if defined (__ACE_INLINE__)
#include "Properties.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_PROPERTIES_H */
