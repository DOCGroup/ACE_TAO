/* -*- C++ -*- */
/**
 *  @file Admin.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_Notify_ADMIN_H
#define TAO_Notify_ADMIN_H

#include /**/ "ace/pre.h"

#include "notify_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "FilterAdmin.h"
#include "EventTypeSeq.h"
#include "Topology_Object.h"

class TAO_Notify_Proxy;
class TAO_Notify_EventChannel;
template <class TYPE> class TAO_Notify_Container_T;

/**
 * @class TAO_Notify_Admin
 *
 * @brief Base class for the ConsumerAdmin and SupplierAdmin.
 *
 */

class TAO_Notify_Serv_Export TAO_Notify_Admin : public TAO_Notify::Topology_Parent
{
  friend class TAO_Notify_Builder;
public:
  typedef CosNotifyChannelAdmin::AdminIDSeq SEQ;
  typedef CosNotifyChannelAdmin::AdminIDSeq_var SEQ_VAR;

  /// Constuctor
  TAO_Notify_Admin ();

  /// Destructor
  ~TAO_Notify_Admin ();

  /// Init
  void init (TAO_Notify_EventChannel *ec ACE_ENV_ARG_DECL);

  /// Insert the proxy in the <proxy_container_>.
  void insert (TAO_Notify_Proxy* proxy ACE_ENV_ARG_DECL);

  /// Remove type from container_
  void remove (TAO_Notify_Proxy *proxy ACE_ENV_ARG_DECL);

  /// Access Admin FilterAdmin.
  TAO_Notify_FilterAdmin& filter_admin (void);

  /// Set Filter operator
  void filter_operator (
      CosNotifyChannelAdmin::InterFilterGroupOperator filter_operator
    );

  /// Access Filter operator
  CosNotifyChannelAdmin::InterFilterGroupOperator filter_operator (void);

  /// Obtain the Admin's subscribed types.
  void subscribed_types (TAO_Notify_EventTypeSeq& subscribed_types
                         ACE_ENV_ARG_DECL);

  TAO_Notify_EventChannel * event_channel () const;
  /// Shutdown
  virtual int shutdown (ACE_ENV_SINGLE_ARG_DECL);

  virtual void save_persistent (TAO_Notify::Topology_Saver& saver ACE_ENV_ARG_DECL);
  virtual TAO_Notify::Topology_Object* load_child (const ACE_CString &type,
    CORBA::Long id, const TAO_Notify::NVPList& attrs ACE_ENV_ARG_DECL);
  virtual void reconnect (ACE_ENV_SINGLE_ARG_DECL);

  void set_default (bool is_default);
  bool is_default () const;
  virtual void load_attrs(const TAO_Notify::NVPList& attrs);

protected:
  void save_attrs (TAO_Notify::NVPList& attrs);
  virtual const char * get_admin_type_name (void) const = 0;

protected:
  typedef TAO_Notify_Container_T <TAO_Notify_Proxy>
    TAO_Notify_Proxy_Container;

  /// = Data Members

  /// The EventChannel.
  TAO_Notify_EventChannel *ec_;

  /// The Proxy Container.
  TAO_Notify_Proxy_Container *proxy_container_;

  /// The types that we've subscribed our proxy objects with the event manager.
  TAO_Notify_EventTypeSeq subscribed_types_;

  /// Filter Administration
  TAO_Notify_FilterAdmin filter_admin_;

  /// Filter operator
  CosNotifyChannelAdmin::InterFilterGroupOperator filter_operator_;

  bool is_default_;
};

#if defined (__ACE_INLINE__)
#include "Admin.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_Notify_ADMIN_H */
