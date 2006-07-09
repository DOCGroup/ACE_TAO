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

  /// Constuctor
  TAO_Notify_SupplierAdmin (void);

  /// Destructor
  virtual ~TAO_Notify_SupplierAdmin ();

  /// Init
  void init (TAO_Notify_EventChannel *ec ACE_ENV_ARG_DECL);

  /// ServantBase refcount methods.
  virtual void _add_ref (ACE_ENV_SINGLE_ARG_DECL);
  virtual void _remove_ref (ACE_ENV_SINGLE_ARG_DECL);


  virtual const char * get_admin_type_name () const;

  virtual TAO_Notify::Topology_Object* load_child (
    const ACE_CString &type,
    CORBA::Long id,
    const TAO_Notify::NVPList& attrs
    ACE_ENV_ARG_DECL);

  TAO_Notify_ProxyConsumer * find_proxy_consumer (
      TAO_Notify::IdVec & id_path,
      size_t position
      ACE_ENV_ARG_DECL);

protected:

    TAO_Notify::Topology_Object *load_proxy (
    CORBA::Long id,
    CosNotifyChannelAdmin::ClientType ctype,
    const TAO_Notify::NVPList& attrs
    ACE_ENV_ARG_DECL);

  /// = NotifyExt::SupplierAdmin methods
  CosNotifyChannelAdmin::ProxyConsumer_ptr
  obtain_notification_push_consumer_with_qos (
      CosNotifyChannelAdmin::ClientType ctype,
      CosNotifyChannelAdmin::ProxyID_out proxy_id,
      const CosNotification::QoSProperties & initial_qos
      ACE_ENV_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException,
                     CosNotifyChannelAdmin::AdminLimitExceeded,
                     CosNotification::UnsupportedQoS));

  /// = CosNotifyChannelAdmin::SupplierAdmin methods
  virtual CosNotifyChannelAdmin::AdminID MyID (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual ::CosNotifyChannelAdmin::EventChannel_ptr MyChannel (
      ACE_ENV_SINGLE_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual ::CosNotifyChannelAdmin::InterFilterGroupOperator MyOperator (
      ACE_ENV_SINGLE_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual ::CosNotifyChannelAdmin::ProxyIDSeq * pull_consumers (
      ACE_ENV_SINGLE_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual ::CosNotifyChannelAdmin::ProxyIDSeq * push_consumers (
      ACE_ENV_SINGLE_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual ::CosNotifyChannelAdmin::ProxyConsumer_ptr get_proxy_consumer (
      CosNotifyChannelAdmin::ProxyID proxy_id
      ACE_ENV_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException,
                     CosNotifyChannelAdmin::ProxyNotFound));

  virtual ::CosNotifyChannelAdmin::ProxyConsumer_ptr
  obtain_notification_pull_consumer (
      CosNotifyChannelAdmin::ClientType ctype,
      CosNotifyChannelAdmin::ProxyID_out proxy_id
      ACE_ENV_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException,
                     CosNotifyChannelAdmin::AdminLimitExceeded));

  virtual ::CosNotifyChannelAdmin::ProxyConsumer_ptr
  obtain_notification_push_consumer (
      CosNotifyChannelAdmin::ClientType ctype,
      CosNotifyChannelAdmin::ProxyID_out proxy_id
      ACE_ENV_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException,
                     CosNotifyChannelAdmin::AdminLimitExceeded));

  virtual void destroy (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual ::CosNotification::QoSProperties* get_qos (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void set_qos (const CosNotification::QoSProperties& qos
                        ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     CosNotification::UnsupportedQoS));

  virtual void validate_qos (
      const CosNotification::QoSProperties & required_qos,
      CosNotification::NamedPropertyRangeSeq_out available_qos
      ACE_ENV_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException,
                     CosNotification::UnsupportedQoS));

  virtual void offer_change (const CosNotification::EventTypeSeq & added,
                             const CosNotification::EventTypeSeq & removed
                             ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     , CosNotifyComm::InvalidEventType
                     ));

  virtual CosNotifyFilter::FilterID add_filter (
      CosNotifyFilter::Filter_ptr new_filter ACE_ENV_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void remove_filter (
      CosNotifyFilter::FilterID filter ACE_ENV_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException,
                     CosNotifyFilter::FilterNotFound));

  virtual ::CosNotifyFilter::Filter_ptr get_filter (
      CosNotifyFilter::FilterID filter
      ACE_ENV_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException,
                     CosNotifyFilter::FilterNotFound));

  virtual ::CosNotifyFilter::FilterIDSeq * get_all_filters (
      ACE_ENV_SINGLE_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void remove_all_filters (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual ::CosEventChannelAdmin::ProxyPushConsumer_ptr obtain_push_consumer (
      ACE_ENV_SINGLE_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual ::CosEventChannelAdmin::ProxyPullConsumer_ptr obtain_pull_consumer (
      ACE_ENV_SINGLE_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

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
