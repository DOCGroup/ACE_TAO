/* -*- C++ -*- */
/**
 *  @file SupplierAdmin.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_SUPPLIERADMIN_H
#define TAO_NS_SUPPLIERADMIN_H
#include "ace/pre.h"

#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosNotifyChannelAdminS.h"
#include "Admin.h"
#include "Destroy_Callback.h"

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

/**
 * @class TAO_NS_SupplierAdmin
 *
 * @brief Implementation of CosNotifyChannelAdmin::SupplierAdmin
 *
 */
class TAO_Notify_Export TAO_NS_SupplierAdmin : public virtual POA_CosNotifyChannelAdmin::SupplierAdmin, public virtual TAO_NS_Admin, public TAO_NS_Destroy_Callback
{
  friend class TAO_NS_Builder;
public:
  /// Constuctor
  TAO_NS_SupplierAdmin (void);

  /// Destructor
  ~TAO_NS_SupplierAdmin ();

  /// Return servant
  virtual PortableServer::Servant servant (void);

  /// ServantBase refcount methods.
  virtual void _add_ref (ACE_ENV_SINGLE_ARG_DECL);
  virtual void _remove_ref (ACE_ENV_SINGLE_ARG_DECL);

  /// TAO_NS_Destroy_Callback methods
  virtual void release (void);

protected:
  /// = CosNotifyChannelAdmin::SupplierAdmin methods
  virtual CosNotifyChannelAdmin::AdminID MyID (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual ::CosNotifyChannelAdmin::EventChannel_ptr MyChannel (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual ::CosNotifyChannelAdmin::InterFilterGroupOperator MyOperator (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual ::CosNotifyChannelAdmin::ProxyIDSeq * pull_consumers (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual ::CosNotifyChannelAdmin::ProxyIDSeq * push_consumers (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual ::CosNotifyChannelAdmin::ProxyConsumer_ptr get_proxy_consumer (CosNotifyChannelAdmin::ProxyID proxy_id
                                                                         ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     , CosNotifyChannelAdmin::ProxyNotFound
                     ));

  virtual ::CosNotifyChannelAdmin::ProxyConsumer_ptr obtain_notification_pull_consumer (CosNotifyChannelAdmin::ClientType ctype,
                                                                                        CosNotifyChannelAdmin::ProxyID_out proxy_id
                                                                                        ACE_ENV_ARG_DECL
                                                                                        )
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     , CosNotifyChannelAdmin::AdminLimitExceeded
                     ));

  virtual ::CosNotifyChannelAdmin::ProxyConsumer_ptr obtain_notification_push_consumer (CosNotifyChannelAdmin::ClientType ctype,
                                                                                        CosNotifyChannelAdmin::ProxyID_out proxy_id
                                                                                        ACE_ENV_ARG_DECL
                                                                                        )
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     , CosNotifyChannelAdmin::AdminLimitExceeded
                     ));

  virtual void destroy (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual ::CosNotification::QoSProperties* get_qos (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual void set_qos (const CosNotification::QoSProperties& qos ACE_ENV_ARG_DECL)
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

  virtual void offer_change (const CosNotification::EventTypeSeq & added,
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

  virtual ::CosEventChannelAdmin::ProxyPushConsumer_ptr obtain_push_consumer (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual ::CosEventChannelAdmin::ProxyPullConsumer_ptr obtain_pull_consumer (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#if defined (__ACE_INLINE__)
#include "SupplierAdmin.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_SUPPLIERADMIN_H */
