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

#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/NotifyExtS.h"
#include "Admin.h"

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

/**
 * @class TAO_Notify_ConsumerAdmin
 *
 * @brief Implementation of CosNotifyChannelAdmin::ConsumerAdmin
 *
 */
class TAO_Notify_Export TAO_Notify_ConsumerAdmin : public POA_NotifyExt::ConsumerAdmin
                                             , public virtual TAO_Notify_Admin
{
public:
  /// Constuctor
  TAO_Notify_ConsumerAdmin (void);

  /// Destructor
  ~TAO_Notify_ConsumerAdmin ();

  /// Init
  void init (TAO_Notify_EventChannel *ec ACE_ENV_ARG_DECL);

  /// ServantBase refcount methods.
  virtual void _add_ref (ACE_ENV_SINGLE_ARG_DECL);
  virtual void _remove_ref (ACE_ENV_SINGLE_ARG_DECL);

  /// Release this object.
  virtual void release (void);

protected:

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
  virtual CosNotifyChannelAdmin::AdminID MyID (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual ::CosNotifyChannelAdmin::EventChannel_ptr MyChannel (ACE_ENV_SINGLE_ARG_DECL

  )
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual ::CosNotifyChannelAdmin::InterFilterGroupOperator MyOperator (ACE_ENV_SINGLE_ARG_DECL)
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

  virtual ::CosNotifyFilter::MappingFilter_ptr lifetime_filter (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual void lifetime_filter (CosNotifyFilter::MappingFilter_ptr lifetime_filter
                                ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual ::CosNotifyChannelAdmin::ProxyIDSeq * pull_suppliers (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual ::CosNotifyChannelAdmin::ProxyIDSeq * push_suppliers (ACE_ENV_SINGLE_ARG_DECL)
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

  virtual void destroy (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual ::CosNotification::QoSProperties * get_qos (ACE_ENV_SINGLE_ARG_DECL)
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

  virtual ::CosNotifyFilter::FilterIDSeq * get_all_filters (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual void remove_all_filters (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual ::CosEventChannelAdmin::ProxyPushSupplier_ptr obtain_push_supplier (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual ::CosEventChannelAdmin::ProxyPullSupplier_ptr obtain_pull_supplier (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#if defined (__ACE_INLINE__)
#include "ConsumerAdmin.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_Notify_CONSUMERADMIN_H */
