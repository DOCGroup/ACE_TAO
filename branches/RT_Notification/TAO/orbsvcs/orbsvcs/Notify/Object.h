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

#include "tao/PortableServer/Servant_Base.h"
#include "orbsvcs/NotifyExtC.h"
#include "QoSProperties.h"
#include "AdminProperties.h"
#include "Refcountable.h"

class TAO_NS_POA_Helper;
class TAO_NS_Worker_Task;
class TAO_NS_Event_Manager;
class TAO_NS_Timer;

/**
 * @class TAO_NS_Object
 *
 * @brief Base Object for RT_Notify's CORBA Objects.
 *
 */
class TAO_Notify_Export TAO_NS_Object : public TAO_NS_Refcountable
{
  friend class TAO_NS_Builder;

public:
  /// Id for Objects.
  typedef CORBA::Long ID;

  /// Constuctor
  TAO_NS_Object (void);

  /// Destructor
  virtual ~TAO_NS_Object (void);

  /// This Object's ID
  ID id (void);

  /// Activate
  virtual CORBA::Object_ptr activate (PortableServer::Servant servant ACE_ENV_ARG_DECL);

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
  void proxy_poa_own (TAO_NS_POA_Helper* proxy_poa);

  /// Accessor for the proxy_poa_
  TAO_NS_POA_Helper* proxy_poa (void);

  /// Setting the object_poa_ gives ownership to this class.
  void object_poa_own (TAO_NS_POA_Helper* object_poa);

  /// Accessor for the object_poa_
  TAO_NS_POA_Helper* object_poa (void);

  /// Set Worker Task. This object assume ownership of the set object.
  void worker_task_own (TAO_NS_Worker_Task* worker_task);

  /// Set Worker Task. Does not assume ownership.
  void worker_task (TAO_NS_Worker_Task* worker_task);

  /// Set the QoS Properties.
  virtual void set_qos (const CosNotification::QoSProperties & qos ACE_ENV_ARG_DECL);

  /// Get the QoS Properties.
  CosNotification::QoSProperties* get_qos (ACE_ENV_SINGLE_ARG_DECL);

  /// Obtain the Timer manager associated with this object.
  virtual TAO_NS_Timer* timer (void);

  /// shutdown. Returns 1 ifif the shutdown was already run once before.
  virtual int shutdown (ACE_ENV_SINGLE_ARG_DECL);

  /// Accessor for the Event Manager
  TAO_NS_Event_Manager* event_manager (void);

protected:
  /// Init this object with data from <rhs>.
  void init (TAO_NS_Object* parent);

  /// Shutdown the current worker task and delete it if we own it.
  void shutdown_worker_task (void);

  /// Shutdown the current proxy poa.
  void shutdown_proxy_poa (void);

  /// Shutdown the current object poa.
  void shutdown_object_poa (void);

  /// Notification that can be overridden by subclasses to be informed that <qos_properties_> have been modified.
  virtual void qos_changed (const TAO_NS_QoSProperties& qos_properties);

  ///= Protected data members.

  /// The event manager.
  TAO_NS_Event_Manager* event_manager_;

  /// Admin Properties.
  TAO_NS_AdminProperties_var admin_properties_;

  /// QoS Properties.
  TAO_NS_QoSProperties qos_properties_;

  /// Id assigned to this object
  ID id_;

  /// The POA in which the object is activated.
  TAO_NS_POA_Helper* poa_;

  /// The POA in which the proxys are activated.
  TAO_NS_POA_Helper* proxy_poa_;

  /// Flag that indicates if we own <proxy_poa_>
  int own_proxy_poa_;

  /// The POA in which the object's children are activated.
  TAO_NS_POA_Helper* object_poa_;

  /// Flag that indicates if we own <object_poa_>
  int own_object_poa_;

  /// Worker Task.
  TAO_NS_Worker_Task* worker_task_;

  /// Ownership flag
  int own_worker_task_;

  /// Are we shutdown (i,e. scheduled for destroy).
  int shutdown_;
};

#if defined (__ACE_INLINE__)
#include "Object.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_OBJECT_H */
