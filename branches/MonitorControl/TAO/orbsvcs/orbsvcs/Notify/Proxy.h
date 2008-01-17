// -*- C++ -*-

/**
 *  @file Proxy.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 */

#ifndef TAO_Notify_PROXY_H
#define TAO_Notify_PROXY_H

#include /**/ "ace/pre.h"

#include "orbsvcs/Notify/notify_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Notify/Topology_Object.h"
#include "orbsvcs/Notify/EventTypeSeq.h"
#include "orbsvcs/Notify/FilterAdmin.h"
#include "orbsvcs/Notify/Admin.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Notify_Admin;
class TAO_Notify_Peer;

/**
 * @class TAO_Notify_Proxy
 *
 * @brief Base class proxy for all proxys in NS.
 *
 */
class TAO_Notify_Serv_Export TAO_Notify_Proxy
  : public TAO_Notify::Topology_Parent
{
  friend class TAO_Notify_Peer;

public:
  typedef TAO_Notify_Refcountable_Guard_T<TAO_Notify_Proxy> Ptr;
  typedef CosNotifyChannelAdmin::ProxyIDSeq SEQ;
  typedef CosNotifyChannelAdmin::ProxyIDSeq_var SEQ_VAR;

  /// Constuctor
  TAO_Notify_Proxy (void);

  /// Destructor
  virtual ~TAO_Notify_Proxy ();

  /// Activate
  virtual CORBA::Object_ptr activate (PortableServer::Servant servant);

  /// Activate with a given ID
  virtual CORBA::Object_ptr activate (
      PortableServer::Servant servant,
      CORBA::Long id);

  /// Deactivate
  void deactivate (void);

  /// Obtain the Proxy's subscribed types.
  void subscribed_types (TAO_Notify_EventTypeSeq& subscribed_types);

  /// Check if this event passes the admin and proxy filters.
  CORBA::Boolean check_filters (
      const TAO_Notify_Event* event,
      TAO_Notify_FilterAdmin& parent_filter_admin,
      CosNotifyChannelAdmin::InterFilterGroupOperator filter_operator
    );

  /// Inform this proxy that the following types are being advertised.
  void types_changed (const TAO_Notify_EventTypeSeq& added,
                      const TAO_Notify_EventTypeSeq& removed);

  /// Have updates been turned off.
  CORBA::Boolean updates_off (void);

  /// Destroy this object.
  virtual void destroy (void) = 0;

  /// Access our Peer.
  virtual TAO_Notify_Peer* peer (void) = 0;

  /// Implement the Obtain Types.
  virtual CosNotification::EventTypeSeq* obtain_types (
      CosNotifyChannelAdmin::ObtainInfoMode mode,
      const TAO_Notify_EventTypeSeq& types
    );

  /// Notification of subscriptions/offers set at the admin.
  virtual void admin_types_changed (const CosNotification::EventTypeSeq & added,
                                    const CosNotification::EventTypeSeq & removed) = 0;


  /// Override, TAO_Notify_Object::qos_changed
  virtual void qos_changed (const TAO_Notify_QoSProperties& qos_properties);

  // TAO_Notify::Topology_Object

  virtual void save_persistent (TAO_Notify::Topology_Saver& saver);
  virtual void save_attrs(TAO_Notify::NVPList& attrs);
  virtual const char * get_proxy_type_name (void) const = 0;

  virtual TAO_Notify::Topology_Object* load_child (const ACE_CString &type, CORBA::Long id,
    const TAO_Notify::NVPList& attrs);

protected:

  /// Filter Administration
  TAO_Notify_FilterAdmin filter_admin_;

  /// The types that we're subscribed with the event manager.
  TAO_Notify_EventTypeSeq subscribed_types_;

  /// True if updates have been turned off.
  CORBA::Boolean updates_off_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "orbsvcs/Notify/Proxy.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_Notify_PROXY_H */
