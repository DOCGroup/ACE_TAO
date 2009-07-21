/* -*- C++ -*- */
/**
 *  @file ConsumerAdmin.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_Notify_CONSUMERADMIN_H
#define TAO_Notify_CONSUMERADMIN_H

#include /**/ "ace/pre.h"

#include "orbsvcs/Notify/notify_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/NotifyExtS.h"

#include "orbsvcs/Notify/Admin.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Notify_ProxySupplier;

/**
 * @class TAO_Notify_ConsumerAdmin
 *
 * @brief Implementation of CosNotifyChannelAdmin::ConsumerAdmin
 *
 */
class TAO_Notify_Serv_Export TAO_Notify_ConsumerAdmin :
                             public virtual POA_NotifyExt::ConsumerAdmin,
                             public virtual TAO_Notify_Admin
{
public:
  typedef TAO_Notify_Refcountable_Guard_T< TAO_Notify_ConsumerAdmin > Ptr;
  /// Constructor
  TAO_Notify_ConsumerAdmin (void);

  /// Destructor
  virtual ~TAO_Notify_ConsumerAdmin ();

  /// Init
  void init (TAO_Notify_EventChannel *ec);

  /// ServantBase refcount methods.
  virtual void _add_ref (void);
  virtual void _remove_ref (void);

  virtual const char * get_admin_type_name () const;

  virtual TAO_Notify::Topology_Object* load_child (
    const ACE_CString &type,
    CORBA::Long id,
    const TAO_Notify::NVPList& attrs);

  TAO_Notify_ProxySupplier * find_proxy_supplier (
      TAO_Notify::IdVec & id_path,
      size_t position);

  /// TAO_Notify_Container_T requires a destroy method
  virtual void destroy (void);

protected:
  TAO_Notify::Topology_Object *load_proxy (
    CORBA::Long id,
    CosNotifyChannelAdmin::ClientType ctype,
    const TAO_Notify::NVPList& attrs);

  /// = NotifyExt::ConsumerAdmin methods
  virtual CosNotifyChannelAdmin::ProxySupplier_ptr
  obtain_notification_push_supplier_with_qos (CosNotifyChannelAdmin::ClientType ctype,
                                              CosNotifyChannelAdmin::ProxyID_out proxy_id,
                                              const CosNotification::QoSProperties & initial_qos);

  /// = CosNotifyChannelAdmin::ConsumerAdmin methods
  virtual CosNotifyChannelAdmin::AdminID MyID (void);

  virtual ::CosNotifyChannelAdmin::EventChannel_ptr MyChannel (void);

  virtual ::CosNotifyChannelAdmin::InterFilterGroupOperator MyOperator (void);

  virtual ::CosNotifyFilter::MappingFilter_ptr priority_filter (void);

  virtual void priority_filter (CosNotifyFilter::MappingFilter_ptr priority_filter);

  virtual ::CosNotifyFilter::MappingFilter_ptr lifetime_filter (void);

  virtual void lifetime_filter (CosNotifyFilter::MappingFilter_ptr lifetime_filter);

  virtual ::CosNotifyChannelAdmin::ProxyIDSeq * pull_suppliers (void);

  virtual ::CosNotifyChannelAdmin::ProxyIDSeq * push_suppliers (void);

  virtual ::CosNotifyChannelAdmin::ProxySupplier_ptr get_proxy_supplier (CosNotifyChannelAdmin::ProxyID proxy_id);

  virtual ::CosNotifyChannelAdmin::ProxySupplier_ptr
  obtain_notification_pull_supplier (CosNotifyChannelAdmin::ClientType ctype,
                                     CosNotifyChannelAdmin::ProxyID_out proxy_id);

  virtual ::CosNotifyChannelAdmin::ProxySupplier_ptr
  obtain_notification_push_supplier (CosNotifyChannelAdmin::ClientType ctype,
                                     CosNotifyChannelAdmin::ProxyID_out proxy_id);

  virtual ::CosNotification::QoSProperties * get_qos (void);

  virtual void set_qos (const CosNotification::QoSProperties & qos);

  virtual void validate_qos (const CosNotification::QoSProperties & required_qos,
                             CosNotification::NamedPropertyRangeSeq_out available_qos);

  virtual void subscription_change (const CosNotification::EventTypeSeq & added,
                                    const CosNotification::EventTypeSeq & removed);

  virtual CosNotifyFilter::FilterID add_filter (CosNotifyFilter::Filter_ptr new_filter);

  virtual void remove_filter (CosNotifyFilter::FilterID filter);

  virtual ::CosNotifyFilter::Filter_ptr get_filter (CosNotifyFilter::FilterID filter);

  virtual ::CosNotifyFilter::FilterIDSeq * get_all_filters (void);

  virtual void remove_all_filters (void);

  virtual ::CosEventChannelAdmin::ProxyPushSupplier_ptr obtain_push_supplier (void);

  virtual ::CosEventChannelAdmin::ProxyPullSupplier_ptr obtain_pull_supplier (void);

private:

  /// Release this object.
  virtual void release (void);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"

#endif /* TAO_Notify_CONSUMERADMIN_H */
