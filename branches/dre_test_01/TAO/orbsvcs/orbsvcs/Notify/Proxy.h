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

#include "Object.h"
#include "EventTypeSeq.h"
#include "FilterAdmin.h"
#include "Admin.h"

class TAO_NS_Admin;
class TAO_NS_Peer;

/**
 * @class TAO_NS_Proxy
 *
 * @brief Base class proxy for all proxys in NS.
 *
 */
class TAO_Notify_Export TAO_NS_Proxy : public virtual TAO_NS_Object
{
  friend class TAO_NS_Peer;

public:
  typedef CosNotifyChannelAdmin::ProxyIDSeq SEQ;
  typedef CosNotifyChannelAdmin::ProxyIDSeq_var SEQ_VAR;

  /// Constuctor
  TAO_NS_Proxy (void);

  /// Destructor
  ~TAO_NS_Proxy ();

  /// Activate
  virtual CORBA::Object_ptr activate (PortableServer::Servant servant ACE_ENV_ARG_DECL);

  /// Deactivate
  void deactivate (ACE_ENV_SINGLE_ARG_DECL);

  /// Obtain the Proxy's subscribed types.
  void subscribed_types (TAO_NS_EventTypeSeq& subscribed_types ACE_ENV_ARG_DECL);

  /// Check if this event passes the admin and proxy filters.
  CORBA::Boolean check_filters (const TAO_NS_Event_var &event
                                , TAO_NS_FilterAdmin& parent_filter_admin
                                , CosNotifyChannelAdmin::InterFilterGroupOperator filter_operator
                                ACE_ENV_ARG_DECL);

  /// Inform this proxy that the following types are being advertised.
  void types_changed (const TAO_NS_EventTypeSeq& added, const TAO_NS_EventTypeSeq& removed ACE_ENV_ARG_DECL);

  /// Have updates been turned off.
  CORBA::Boolean updates_off (void);

  /// Destroy this object.
  virtual void destroy (ACE_ENV_SINGLE_ARG_DECL) = 0;

  /// Access our Peer.
  virtual TAO_NS_Peer* peer (void) = 0;

  /// Implement the Obtain Types.
  virtual CosNotification::EventTypeSeq* obtain_types (CosNotifyChannelAdmin::ObtainInfoMode mode, const TAO_NS_EventTypeSeq& types ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  /// Notification of subscriptions/offers set at the admin.
  virtual void admin_types_changed (const CosNotification::EventTypeSeq & added,
                                    const CosNotification::EventTypeSeq & removed
                                    ACE_ENV_ARG_DECL) = 0;


  /// Override, TAO_NS_Object::qos_changed
  virtual void qos_changed (const TAO_NS_QoSProperties& qos_properties);

protected:

  /// Filter Administration
  TAO_NS_FilterAdmin filter_admin_;

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
