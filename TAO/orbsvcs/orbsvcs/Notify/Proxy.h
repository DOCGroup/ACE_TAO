/* -*- C++ -*- */
/**
 *  @file Proxy.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_PROXY_H
#define TAO_NS_PROXY_H
#include "ace/pre.h"

#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Object_T.h"
#include "EventTypeSeq.h"
#include "FilterAdmin.h"

class TAO_NS_Admin;
class TAO_NS_Peer;

/**
 * @class TAO_NS_Proxy
 *
 * @brief Base class proxy for all proxys in NS.
 *
 */
class TAO_Notify_Export TAO_NS_Proxy : public virtual TAO_NS_Object_T <TAO_NS_Proxy, TAO_NS_Admin>
{
  friend class TAO_NS_Peer;

public:
  /// Constuctor
  TAO_NS_Proxy (void);

  /// Destructor
  ~TAO_NS_Proxy ();

  /// Subscribed types.
  TAO_NS_EventTypeSeq& subscribed_types (void);

  /// Check if this event passes the admin and proxy filters.
  CORBA::Boolean check_filters (TAO_NS_Event_var &event ACE_ENV_ARG_DECL);

  /// Subscription type added
  void type_added (const TAO_NS_EventType& added);

  /// Subscription type removed
  void type_removed (const TAO_NS_EventType& removed);

  /// Have updates been turned off.
  CORBA::Boolean updates_off (void);

  /// Destroy this object.
  virtual void destroy (ACE_ENV_SINGLE_ARG_DECL) = 0;

  /// Access our Peer.
  virtual TAO_NS_Peer* peer (void) = 0;

  /// Obtain Types.
  virtual CosNotification::EventTypeSeq* obtain_types (CosNotifyChannelAdmin::ObtainInfoMode mode ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  /// Notification of subscriptions/offers set at the admin.
  virtual void admin_subscription (const CosNotification::EventTypeSeq & added,
                                   const CosNotification::EventTypeSeq & removed
                                   ACE_ENV_ARG_DECL) = 0;


  /// Override, TAO_NS_Object::qos_changed
  virtual void qos_changed (void);

protected:
  typedef TAO_NS_Object_T <TAO_NS_Proxy, TAO_NS_Admin> inherited;

  /// Filter Administration
  TAO_NS_FilterAdmin filter_admin_;

  /// Types added.
  TAO_NS_EventTypeSeq added_;

  /// Types removed.
  TAO_NS_EventTypeSeq removed_;

  /// The types that we're subscribed with the event manager.
  TAO_NS_EventTypeSeq subscribed_types_;

  /// True if updates have been turned off.
  CORBA::Boolean updates_off_;
};

#if defined (__ACE_INLINE__)
#include "Proxy.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_PROXY_H */
