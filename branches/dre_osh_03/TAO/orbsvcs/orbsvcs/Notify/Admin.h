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

#ifndef TAO_NS_ADMIN_H
#define TAO_NS_ADMIN_H
#include "ace/pre.h"

#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "FilterAdmin.h"
#include "EventTypeSeq.h"
#include "Object.h"

class TAO_NS_Proxy;
class TAO_NS_EventChannel;
template <class TYPE> class TAO_NS_Container_T;

/**
 * @class TAO_NS_Admin
 *
 * @brief Base class for the ConsumerAdmin and SupplierAdmin.
 *
 */

class TAO_Notify_Export TAO_NS_Admin : public virtual TAO_NS_Object
{
  friend class TAO_NS_Builder;
public:
  typedef CosNotifyChannelAdmin::AdminIDSeq SEQ;
  typedef CosNotifyChannelAdmin::AdminIDSeq_var SEQ_VAR;

  /// Constuctor
  TAO_NS_Admin (void);

  /// Destructor
  ~TAO_NS_Admin ();

  /// Init
  void init (TAO_NS_EventChannel *ec ACE_ENV_ARG_DECL);

  /// Insert the proxy in the <proxy_container_>.
  void insert (TAO_NS_Proxy* proxy ACE_ENV_ARG_DECL);

  /// Remove type from container_
  void remove (TAO_NS_Proxy *proxy ACE_ENV_ARG_DECL);

  /// Access Admin FilterAdmin.
  TAO_NS_FilterAdmin& filter_admin (void);

  /// Set Filter operator
  void filter_operator (CosNotifyChannelAdmin::InterFilterGroupOperator filter_operator);

  /// Access Filter operator
  CosNotifyChannelAdmin::InterFilterGroupOperator filter_operator (void);

  /// Obtain the Admin's subscribed types.
  void subscribed_types (TAO_NS_EventTypeSeq& subscribed_types ACE_ENV_ARG_DECL);

  /// Shutdown
  virtual int shutdown (ACE_ENV_SINGLE_ARG_DECL);

protected:
  typedef TAO_NS_Container_T <TAO_NS_Proxy> TAO_NS_Proxy_Container;

  /// = Data Members

  /// The EventChannel.
  TAO_NS_EventChannel *ec_;

  /// The Proxy Container.
  TAO_NS_Proxy_Container *proxy_container_;

  /// The types that we've subscribed our proxy objects with the event manager.
  TAO_NS_EventTypeSeq subscribed_types_;

  /// Filter Administration
  TAO_NS_FilterAdmin filter_admin_;

  /// Filter operator
  CosNotifyChannelAdmin::InterFilterGroupOperator filter_operator_;
};

#if defined (__ACE_INLINE__)
#include "Admin.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_ADMIN_H */
