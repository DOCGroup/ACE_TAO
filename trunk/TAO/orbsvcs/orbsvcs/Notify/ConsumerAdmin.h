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
class TAO_Notify_Serv_Export TAO_Notify_ConsumerAdmin : public POA_NotifyExt::ConsumerAdmin
                                             , public virtual TAO_Notify_Admin
{
public:
  typedef TAO_Notify_Refcountable_Guard_T< TAO_Notify_ConsumerAdmin > Ptr;
  /// Constuctor
  TAO_Notify_ConsumerAdmin (void);

  /// Destructor
  virtual ~TAO_Notify_ConsumerAdmin ();

  /// Init
  void init (TAO_Notify_EventChannel *ec ACE_ENV_ARG_DECL);

  /// ServantBase refcount methods.
  virtual void _add_ref (void);
  virtual void _remove_ref (void);


  virtual const char * get_admin_type_name () const;

  virtual TAO_Notify::Topology_Object* load_child (
    const ACE_CString &type,
    CORBA::Long id,
    const TAO_Notify::NVPList& attrs
    ACE_ENV_ARG_DECL);

  TAO_Notify_ProxySupplier * find_proxy_supplier (
      TAO_Notify::IdVec & id_path,
      size_t position
      ACE_ENV_ARG_DECL);

protected:
  TAO_Notify::Topology_Object *load_proxy (
    CORBA::Long id,
    CosNotifyChannelAdmin::ClientType ctype,
    const TAO_Notify::NVPList& attrs
    ACE_ENV_ARG_DECL);

  /// = NotifyExt::ConsumerAdmin methods
  virtual CosNotifyChannelAdmin::ProxySupplier_ptr
  obtain_notification_push_supplier_with_qos (CosNotifyChannelAdmin::ClientType ctype,
                                              CosNotifyChannelAdmin::ProxyID_out proxy_id,
                                              const CosNotification::QoSProperties & initial_qos
                                              ACE_ENV_ARG_DECL
                                              )
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     , CosNotifyChannelAdmin::AdminLimitExceeded
                     , CosNotification::UnsupportedQoS
                     ));

  /// = CosNotifyChannelAdmin::ConsumerAdmin methods
  virtual CosNotifyChannelAdmin::AdminID MyID (void)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual ::CosNotifyChannelAdmin::EventChannel_ptr MyChannel (ACE_ENV_SINGLE_ARG_DECL

  )
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual ::CosNotifyChannelAdmin::InterFilterGroupOperator MyOperator (void)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual ::CosNotifyFilter::MappingFilter_ptr priority_filter (ACE_ENV_SINGLE_ARG_DECL

  )
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual void priority_filter (CosNotifyFilter::MappingFilter_ptr priority_filter ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual ::CosNotifyFilter::MappingFilter_ptr lifetime_filter (void)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual void lifetime_filter (CosNotifyFilter::MappingFilter_ptr lifetime_filter
                                ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual ::CosNotifyChannelAdmin::ProxyIDSeq * pull_suppliers (void)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual ::CosNotifyChannelAdmin::ProxyIDSeq * push_suppliers (void)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual ::CosNotifyChannelAdmin::ProxySupplier_ptr get_proxy_supplier (CosNotifyChannelAdmin::ProxyID proxy_id
                                                                         ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     , CosNotifyChannelAdmin::ProxyNotFound
                     ));

  virtual ::CosNotifyChannelAdmin::ProxySupplier_ptr
  obtain_notification_pull_supplier (CosNotifyChannelAdmin::ClientType ctype,
                                     CosNotifyChannelAdmin::ProxyID_out proxy_id
                                     ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     , CosNotifyChannelAdmin::AdminLimitExceeded
                     ));

  virtual ::CosNotifyChannelAdmin::ProxySupplier_ptr
  obtain_notification_push_supplier (CosNotifyChannelAdmin::ClientType ctype,
                                     CosNotifyChannelAdmin::ProxyID_out proxy_id
                                     ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     , CosNotifyChannelAdmin::AdminLimitExceeded
                     ));

  virtual void destroy (void)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual ::CosNotification::QoSProperties * get_qos (void)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual void set_qos (const CosNotification::QoSProperties & qos
                        ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     , CosNotification::UnsupportedQoS
                     ));

  virtual void validate_qos (const CosNotification::QoSProperties & required_qos,
                             CosNotification::NamedPropertyRangeSeq_out available_qos
                             ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     , CosNotification::UnsupportedQoS
                     ));

  virtual void subscription_change (const CosNotification::EventTypeSeq & added,
                                    const CosNotification::EventTypeSeq & removed
                                    ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     , CosNotifyComm::InvalidEventType
                     ));

  virtual CosNotifyFilter::FilterID add_filter (CosNotifyFilter::Filter_ptr new_filter ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual void remove_filter (CosNotifyFilter::FilterID filter ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     , CosNotifyFilter::FilterNotFound
                     ));

  virtual ::CosNotifyFilter::Filter_ptr get_filter (CosNotifyFilter::FilterID filter ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     , CosNotifyFilter::FilterNotFound
                     ));

  virtual ::CosNotifyFilter::FilterIDSeq * get_all_filters (void)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual void remove_all_filters (void)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual ::CosEventChannelAdmin::ProxyPushSupplier_ptr obtain_push_supplier (void)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual ::CosEventChannelAdmin::ProxyPullSupplier_ptr obtain_pull_supplier (void)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

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
