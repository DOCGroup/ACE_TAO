/* -*- C++ -*- */
/**
 *  @file Object.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_OBJECT_H
#define TAO_NS_OBJECT_H
#include "ace/pre.h"

#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/corba.h"
#include "tao/PortableServer/Servant_Base.h"
#include "orbsvcs/NotifyExtC.h"
#include "Types.h"
#include "QoSProperties.h"
#include "AdminProperties.h"

class TAO_NS_POA_Helper;
class TAO_NS_Worker_Task;
class TAO_NS_Event_Manager;
class TAO_NS_Timer;

/**
 * @class TAO_NS_Object_Id
 *
 * @brief Id for Objects.
 *
 */
typedef CORBA::Long TAO_NS_Object_Id;

/**
 * @class TAO_NS_Object
 *
 * @brief Base Object for RT_Notify's CORBA Objects.
 *
 */
class TAO_Notify_Export TAO_NS_Object
{
  friend class TAO_NS_Builder;

public:
  /// Constuctor
  TAO_NS_Object (void);

  /// Destructor
  virtual ~TAO_NS_Object (void);

  /// Init.
  void init (TAO_NS_POA_Helper* poa, TAO_NS_POA_Helper* proxy_poa, TAO_NS_Worker_Task* worker_task);

  /// This Object's ID
  CORBA::Long id (void);

  /// Activate
  CORBA::Object_ptr activate (ACE_ENV_SINGLE_ARG_DECL);

  /// Deactivate
  void deactivate (ACE_ENV_SINGLE_ARG_DECL);

  /// Have we been shutdown. returns 1 if shutdown.
  int has_shutdown (void);

  /// Get CORBA Ref.
  CORBA::Object_ptr ref (ACE_ENV_SINGLE_ARG_DECL);

  /// Get Worker Task.
  TAO_NS_Worker_Task* worker_task (void);

  /// Get the POA assigned to us.
  TAO_NS_POA_Helper* poa (void);

  /// Setting the proxy_poa_ gives ownership to this class.
  void proxy_poa (TAO_NS_POA_Helper* proxy_poa);

  /// Accessor for the proxy_poa_
  TAO_NS_POA_Helper* proxy_poa (void);

  /// Set Worker Task. This object assume ownership of the set object.
  void worker_task_own (TAO_NS_Worker_Task* worker_task);

  /// Set Worker Task. Does not assume ownership.
  void worker_task (TAO_NS_Worker_Task* worker_task);

  /// Get the QoS Properties.
  CosNotification::QoSProperties* get_qos (ACE_ENV_SINGLE_ARG_DECL);

  /// shutdown
  virtual void shutdown (ACE_ENV_SINGLE_ARG_DECL);

  /// Set the QoS Properties.
  virtual void set_qos (const CosNotification::QoSProperties & qos ACE_ENV_ARG_DECL);

  /// Obtain the Timer manager associated with this object.
  virtual TAO_NS_Timer* timer (void);

protected:
  /// Shutdown the current worker task and delete it if we own it.
  void shutdown_worker_task (void);

  /// Shutdown the current proxy poa.
  void shutdown_proxy_poa (void);

  /// Notification that can be overridden by subclasses to be informed that <qos_properties_> have been modified.
  virtual void qos_changed (const TAO_NS_QoSProperties& qos_properties);

  /// Servant which we use. This must be implemented by subclasses.
  virtual PortableServer::Servant servant (void) = 0;

  /// Apply Reactive concurrency.
  virtual void apply_reactive_concurrency (ACE_ENV_SINGLE_ARG_DECL);

  /// Apply Thread Pools.
  virtual void apply_thread_pool_concurrency (const NotifyExt::ThreadPoolParams& tp_params ACE_ENV_ARG_DECL);

  /// Apply Thread Pools with Lanes.
  virtual void apply_lane_concurrency (const NotifyExt::ThreadPoolLanesParams& tpl_params ACE_ENV_ARG_DECL);

  ///= Protected data members.

  /// The event manager.
  TAO_NS_Event_Manager* event_manager_;

  /// Admin Properties.
  TAO_NS_AdminProperties_var admin_properties_;

  /// QoS Properties.
  TAO_NS_QoSProperties qos_properties_;

  /// Id assigned to this object
  TAO_NS_Object_Id id_;

  /// The POA in which the object is activated.
  TAO_NS_POA_Helper* poa_;

  /// Worker Task.
  TAO_NS_Worker_Task* worker_task_;

  /// Ownership flag
  int own_worker_task_;

  /// The POA in which the proxys are activated.
  TAO_NS_POA_Helper* proxy_poa_;

  /// Flag that indicates if we own <proxy_poa_>
  int own_proxy_poa_;

  /// Are we shutdown (i,e. scheduled for destroy).
  int shutdown_;
};

#if defined (__ACE_INLINE__)
#include "Object.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_OBJECT_H */
