// -*- C++ -*-

/**
 *  @file Object.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_Notify_OBJECT_H
#define TAO_Notify_OBJECT_H

#include /**/ "ace/pre.h"

#include "orbsvcs/Notify/notify_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Notify/QoSProperties.h"
#include "orbsvcs/Notify/AdminProperties.h"
#include "orbsvcs/Notify/Worker_Task.h"
#include "orbsvcs/Notify/Refcountable.h"
#include "orbsvcs/Notify/Name_Value_Pair.h"
#include "orbsvcs/Notify/Event_Manager.h"

#include "orbsvcs/NotifyExtC.h"

#include "tao/PortableServer/Servant_Base.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Notify_POA_Helper;
class TAO_Notify_Timer;
class TAO_Notify_RT_Builder;

/**
 * @class TAO_Notify_Object
 *
 * @brief Base Object for RT_Notify's CORBA Objects.
 *
 */
class TAO_Notify_Serv_Export TAO_Notify_Object : public TAO_Notify_Refcountable
{
  friend class TAO_Notify_Builder;
  friend class TAO_Notify_RT_Builder;

public:
  /// Id for Objects.
  typedef CORBA::Long ID;

  /// Destructor
  virtual ~TAO_Notify_Object (void);

  /// This Object's ID
  ID id (void) const;

  /// Activate
  virtual CORBA::Object_ptr activate (PortableServer::Servant servant);

  /// Activate with existing id
  virtual CORBA::Object_ptr activate (
      PortableServer::Servant servant,
      CORBA::Long id);

  /// Deactivate
  void deactivate (void);

  /// Have we been shutdown. returns true if shutdown.
  bool has_shutdown (void);

  void execute_task (TAO_Notify_Method_Request& method_request);

  /// Get CORBA Ref.
  CORBA::Object_ptr ref (void);

  /// Set the QoS Properties.
  virtual void set_qos (const CosNotification::QoSProperties & qos);

  /// Get the QoS Properties.
  CosNotification::QoSProperties* get_qos (void);

  bool find_qos_property_value (
    const char * name,
    CosNotification::PropertyValue & value)const;

  /// Obtain the Timer manager associated with this object.
  virtual TAO_Notify_Timer* timer (void);

  /// Accessor for the Event Manager
  TAO_Notify_Event_Manager& event_manager (void);

  /// Shutdown. Returns 1 if the shutdown was already run once before.
  virtual int shutdown (void);

  /// Load our attributes. Each derived type should call the superclass
  /// load first before loading its own attributes.
  virtual void load_attrs(const TAO_Notify::NVPList& attrs);

  /// Allow access to the underlying worker task.
  TAO_Notify_Worker_Task* get_worker_task (void);

protected:
  /// Constructor
  TAO_Notify_Object (void);

  /// Init this object with data from <rhs>.
  void initialize (TAO_Notify_Object* parent);

  /// Uses the poas from the supplied object
  void inherit_poas (TAO_Notify_Object& parent);

  /// Adopts the supplied poa as all are poas
  void adopt_poa (TAO_Notify_POA_Helper* single);

  /// Changes the primary poa to the current proxy poa
  void set_primary_as_proxy_poa();

  /// Accessor for the proxy_poa_
  TAO_Notify_POA_Helper* proxy_poa (void);

  /// Accessor for the object_poa_
  TAO_Notify_POA_Helper* object_poa (void);

  /// Get the POA assigned to us.
  TAO_Notify_POA_Helper* poa (void);

  // Sets the admin properties
  void set_event_manager(TAO_Notify_Event_Manager* event_manager);

  // Sets the admin properties
  void set_admin_properties(TAO_Notify_AdminProperties* admin_properties);

  /// Accessor for the Admin Properties
  TAO_Notify_AdminProperties& admin_properties (void);

  /// Notification that can be overridden by subclasses to be informed that
  /// <qos_properties_> have been modified.
  virtual void qos_changed (const TAO_Notify_QoSProperties& qos_properties);

  /// Called by derived types to save their attributes. Each
  /// derived type should call its superclass version before
  /// saving its own attrs.
  virtual void save_attrs(TAO_Notify::NVPList& attrs);

  ///= Protected data members.

  /// QoS Properties.
  TAO_Notify_QoSProperties qos_properties_;

private:
  /// Set Worker Task. This object assume ownership of the set object.
  void set_worker_task (TAO_Notify_Worker_Task* worker_task);

  /// Setting the proxy_poa_ gives ownership to this class.
  void set_proxy_poa (TAO_Notify_POA_Helper* proxy_poa);

  /// Setting the object_poa_ gives ownership to this class.
  void set_object_poa (TAO_Notify_POA_Helper* object_poa);

  /// Setting the object_poa_ gives ownership to this class.
  void set_poa (TAO_Notify_POA_Helper* object_poa);

  /// Shutdown the current worker task and delete it if we own it.
  void shutdown_worker_task (void);

  /// Shutdown the current proxy poa.
  void destroy_proxy_poa (void);

  /// Shutdown the current object poa.
  void destroy_object_poa (void);

  /// Shutdown the current poa.
  void destroy_poa (void);

protected:

  /// The mutex to serialize access to state variables.
  TAO_SYNCH_MUTEX lock_;

private:

  /// The POA in which the object is activated.
  TAO_Notify_POA_Helper* poa_;

  /// The POA in which the proxys are activated.
  TAO_Notify_POA_Helper* proxy_poa_;
  bool own_proxy_poa_;

  /// The POA in which the object's children are activated.
  TAO_Notify_POA_Helper* object_poa_;
  bool own_object_poa_;

  /// Id assigned to this object
  ID id_;

  /// The event manager.
  /// TAO_Notify_Event_Manager inl includes Object.h
  TAO_Notify_Refcountable_Guard_T< TAO_Notify_Event_Manager > event_manager_;

  /// Admin Properties.
  TAO_Notify_AdminProperties::Ptr admin_properties_;

  /// Worker Task.
  TAO_Notify_Worker_Task::Ptr worker_task_;
  bool own_worker_task_;

  /// Are we shutdown (i,e. scheduled for destroy).
  bool shutdown_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "orbsvcs/Notify/Object.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_Notify_OBJECT_H */
