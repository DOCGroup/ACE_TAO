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

#include "Container_T.h"
#include "FilterAdmin.h"
#include "EventTypeSeq.h"

class TAO_NS_Proxy;
class TAO_NS_EventChannel;

/**
 * @class TAO_NS_Admin
 *
 * @brief
 *
 */
class TAO_Notify_Export TAO_NS_Admin : public virtual TAO_NS_Container_T <TAO_NS_Proxy, TAO_NS_Admin, TAO_NS_EventChannel>
{
  typedef ACE_Unbounded_Set <TAO_NS_Object_Id> TAO_NS_Object_Id_Seq;
public:
  enum ADMIN_TYPE
    {
      CONSUMER_ADMIN,
      SUPPLIER_ADMIN
    };

  /// Constuctor
  TAO_NS_Admin (ADMIN_TYPE type);

  /// Destructor
  ~TAO_NS_Admin ();

  /// Insert object to this container.
  virtual void insert (TAO_NS_Proxy* proxy ACE_ENV_ARG_DECL);

  /// Remove type from container_
  virtual void remove (TAO_NS_Proxy* proxy ACE_ENV_ARG_DECL);

  /// Return type.
  ADMIN_TYPE type (void);

  /// Access Admin FilterAdmin.
  TAO_NS_FilterAdmin& filter_admin (void);

  /// Set Filter operator
  void filter_operator (CosNotifyChannelAdmin::InterFilterGroupOperator filter_operator);

  /// Access Filter operator
  CosNotifyChannelAdmin::InterFilterGroupOperator filter_operator (void);

  /// Obtain the Admin's subscribed types.
  void subscribed_types (TAO_NS_EventTypeSeq& subscribed_types ACE_ENV_ARG_DECL);

protected:
  typedef TAO_NS_Container_T <TAO_NS_Proxy, TAO_NS_Admin, TAO_NS_EventChannel> inherited;

  /// = Data Members
  /// List of proxy ID's
  TAO_NS_Object_Id_Seq proxy_id_list_;

  /// Type of Admin
  ADMIN_TYPE type_;

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
