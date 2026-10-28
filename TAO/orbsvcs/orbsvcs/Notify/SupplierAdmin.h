// -*- C++ -*-

/**
 *  @file SupplierAdmin.h
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 */

#ifndef TAO_Notify_SUPPLIERADMIN_H
#define TAO_Notify_SUPPLIERADMIN_H

#include /**/ "ace/pre.h"

#include "orbsvcs/Notify/notify_serv_export.h"
#include "orbsvcs/Notify/Admin.h"

#include "orbsvcs/NotifyExtS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Notify_ProxyConsumer;

/**
 * @class TAO_Notify_SupplierAdmin
 *
 * @brief Implementation of CosNotifyChannelAdmin::SupplierAdmin
 */
class TAO_Notify_Serv_Export TAO_Notify_SupplierAdmin
  : public virtual POA_NotifyExt::SupplierAdmin,
    public virtual TAO_Notify_Admin
{
public:
  using Ptr = TAO_Notify_Refcountable_Guard_T< TAO_Notify_SupplierAdmin >;

  /// Constructor
  TAO_Notify_SupplierAdmin ();

  /// Destructor
  ~TAO_Notify_SupplierAdmin () override;

  TAO_Notify_SupplierAdmin (TAO_Notify_SupplierAdmin const &) = delete;
  TAO_Notify_SupplierAdmin (TAO_Notify_SupplierAdmin &&) = delete;
  TAO_Notify_SupplierAdmin &operator= (TAO_Notify_SupplierAdmin const &) = delete;
  TAO_Notify_SupplierAdmin &operator= (TAO_Notify_SupplierAdmin &&) = delete;

  /// Init
  void init (TAO_Notify_EventChannel *ec);

  /// ServantBase refcount methods.
  void _add_ref () override;
  void _remove_ref () override;

  char const * get_admin_type_name () const override;

  TAO_Notify::Topology_Object* load_child (
    ACE_CString const &type,
    CORBA::Long id,
    TAO_Notify::NVPList const &attrs) override;

  TAO_Notify_ProxyConsumer * find_proxy_consumer (
      TAO_Notify::IdVec & id_path,
      size_t position);

  /// TAO_Notify_Container_T requires a destroy method
  void destroy () override;

protected:
    TAO_Notify::Topology_Object *load_proxy (
    CORBA::Long id,
    CosNotifyChannelAdmin::ClientType ctype,
    const TAO_Notify::NVPList& attrs);

  /// = NotifyExt::SupplierAdmin methods
  CosNotifyChannelAdmin::ProxyConsumer_ptr
  obtain_notification_push_consumer_with_qos (
      CosNotifyChannelAdmin::ClientType ctype,
      CosNotifyChannelAdmin::ProxyID_out proxy_id,
      CosNotification::QoSProperties const &initial_qos) override;

  /// = CosNotifyChannelAdmin::SupplierAdmin methods
  CosNotifyChannelAdmin::AdminID MyID () override;

  ::CosNotifyChannelAdmin::EventChannel_ptr MyChannel () override;

  ::CosNotifyChannelAdmin::InterFilterGroupOperator MyOperator () override;

  ::CosNotifyChannelAdmin::ProxyIDSeq * pull_consumers () override;

  ::CosNotifyChannelAdmin::ProxyIDSeq * push_consumers () override;

  ::CosNotifyChannelAdmin::ProxyConsumer_ptr get_proxy_consumer (
      CosNotifyChannelAdmin::ProxyID proxy_id) override;

  ::CosNotifyChannelAdmin::ProxyConsumer_ptr
  obtain_notification_pull_consumer (
      CosNotifyChannelAdmin::ClientType ctype,
      CosNotifyChannelAdmin::ProxyID_out proxy_id) override;

  ::CosNotifyChannelAdmin::ProxyConsumer_ptr
  obtain_notification_push_consumer (
      CosNotifyChannelAdmin::ClientType ctype,
      CosNotifyChannelAdmin::ProxyID_out proxy_id) override;

  ::CosNotification::QoSProperties* get_qos () override;

  void set_qos (CosNotification::QoSProperties const &qos) override;

  void validate_qos (
      CosNotification::QoSProperties const &required_qos,
      CosNotification::NamedPropertyRangeSeq_out available_qos) override;

  void offer_change (CosNotification::EventTypeSeq const &added,
                     CosNotification::EventTypeSeq const &removed) override;

  CosNotifyFilter::FilterID add_filter (
      CosNotifyFilter::Filter_ptr new_filter) override;

  void remove_filter (CosNotifyFilter::FilterID filter) override;

  ::CosNotifyFilter::Filter_ptr get_filter (
      CosNotifyFilter::FilterID filter) override;

  ::CosNotifyFilter::FilterIDSeq * get_all_filters () override;

  void remove_all_filters () override;

  ::CosEventChannelAdmin::ProxyPushConsumer_ptr obtain_push_consumer () override;

  ::CosEventChannelAdmin::ProxyPullConsumer_ptr obtain_pull_consumer () override;

private:
  /// Release
  void release () override;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"

#endif /* TAO_Notify_SUPPLIERADMIN_H */
