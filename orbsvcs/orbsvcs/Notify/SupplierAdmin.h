// -*- C++ -*-

/**
 *  @file SupplierAdmin.h
 *
 *  $Id$
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
 *
 */
class TAO_Notify_Serv_Export TAO_Notify_SupplierAdmin
  : public virtual POA_NotifyExt::SupplierAdmin,
    public virtual TAO_Notify_Admin
{
public:
  typedef TAO_Notify_Refcountable_Guard_T< TAO_Notify_SupplierAdmin > Ptr;

  /// Constructor
  TAO_Notify_SupplierAdmin (void);

  /// Destructor
  virtual ~TAO_Notify_SupplierAdmin ();

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

  TAO_Notify_ProxyConsumer * find_proxy_consumer (
      TAO_Notify::IdVec & id_path,
      size_t position);

  /// TAO_Notify_Container_T requires a destroy method
  virtual void destroy (void);

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
      const CosNotification::QoSProperties & initial_qos);

  /// = CosNotifyChannelAdmin::SupplierAdmin methods
  virtual CosNotifyChannelAdmin::AdminID MyID (void);

  virtual ::CosNotifyChannelAdmin::EventChannel_ptr MyChannel (void);

  virtual ::CosNotifyChannelAdmin::InterFilterGroupOperator MyOperator (void);

  virtual ::CosNotifyChannelAdmin::ProxyIDSeq * pull_consumers (void);

  virtual ::CosNotifyChannelAdmin::ProxyIDSeq * push_consumers (void);

  virtual ::CosNotifyChannelAdmin::ProxyConsumer_ptr get_proxy_consumer (
      CosNotifyChannelAdmin::ProxyID proxy_id);

  virtual ::CosNotifyChannelAdmin::ProxyConsumer_ptr
  obtain_notification_pull_consumer (
      CosNotifyChannelAdmin::ClientType ctype,
      CosNotifyChannelAdmin::ProxyID_out proxy_id);

  virtual ::CosNotifyChannelAdmin::ProxyConsumer_ptr
  obtain_notification_push_consumer (
      CosNotifyChannelAdmin::ClientType ctype,
      CosNotifyChannelAdmin::ProxyID_out proxy_id);

  virtual ::CosNotification::QoSProperties* get_qos (void);

  virtual void set_qos (const CosNotification::QoSProperties& qos);

  virtual void validate_qos (
      const CosNotification::QoSProperties & required_qos,
      CosNotification::NamedPropertyRangeSeq_out available_qos);

  virtual void offer_change (const CosNotification::EventTypeSeq & added,
                             const CosNotification::EventTypeSeq & removed);

  virtual CosNotifyFilter::FilterID add_filter (
      CosNotifyFilter::Filter_ptr new_filter);

  virtual void remove_filter (CosNotifyFilter::FilterID filter);

  virtual ::CosNotifyFilter::Filter_ptr get_filter (
      CosNotifyFilter::FilterID filter);

  virtual ::CosNotifyFilter::FilterIDSeq * get_all_filters (void);

  virtual void remove_all_filters (void);

  virtual ::CosEventChannelAdmin::ProxyPushConsumer_ptr obtain_push_consumer ();

  virtual ::CosEventChannelAdmin::ProxyPullConsumer_ptr obtain_pull_consumer ();

private:
  /// Release
  virtual void release (void);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"

#endif /* TAO_Notify_SUPPLIERADMIN_H */
