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
class TAO_Notify_Export TAO_NS_Proxy : public TAO_NS_Object_T <TAO_NS_Proxy, TAO_NS_Admin>
{
public:
  /// Constuctor
  TAO_NS_Proxy (void);

  /// Destructor
  ~TAO_NS_Proxy ();

  /// Init method.
  // virtual void init (ACE_ENV_SINGLE_ARG_DECL);

  /// Subscribed types.
  TAO_NS_EventTypeSeq& subscribed_types (void);

  /// Check if this event passes the admin and proxy filters.
  CORBA::Boolean check_filters (TAO_NS_Event_var &event ACE_ENV_ARG_DECL);

  /// Destroy this object.
  virtual void destroy (ACE_ENV_SINGLE_ARG_DECL) = 0;

  /// Access our Peer.
  virtual TAO_NS_Peer* peer (void) = 0;

  /// Notification of subscriptions/offers set at the admin.
  virtual void admin_subscription (const CosNotification::EventTypeSeq & added,
                                   const CosNotification::EventTypeSeq & removed
                                   ACE_ENV_ARG_DECL) = 0;
protected:
  typedef TAO_NS_Object_T <TAO_NS_Proxy, TAO_NS_Admin> inherited;

  /// Filter Administration
  TAO_NS_FilterAdmin filter_admin_;

  /// The types that we're subscribed with the event manager.
  TAO_NS_EventTypeSeq subscribed_types_;
};

#if defined (__ACE_INLINE__)
#include "Proxy.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_PROXY_H */
