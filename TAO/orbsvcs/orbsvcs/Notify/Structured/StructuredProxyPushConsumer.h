/* -*- C++ -*- */
/**
 *  @file StructuredProxyPushConsumer.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_STRUCTUREDPROXYPUSHCONSUMER_H
#define TAO_NS_STRUCTUREDPROXYPUSHCONSUMER_H
#include "ace/pre.h"

#include "../notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosNotifyChannelAdminS.h"
#include "../ProxyConsumer.h"
#include "../Destroy_Callback.h"

/**
 * @class TAO_NS_StructuredProxyPushConsumer
 *
 * @brief CosNotifyChannelAdmin::StructuredProxyPushConsumer implementation.
 *
 */
class TAO_Notify_Export TAO_NS_StructuredProxyPushConsumer : public virtual POA_CosNotifyChannelAdmin::StructuredProxyPushConsumer, public virtual TAO_NS_ProxyConsumer, public TAO_NS_Destroy_Callback
{
  friend class TAO_NS_Builder;

public:
  /// Constuctor
  TAO_NS_StructuredProxyPushConsumer (void);

  /// Destructor
  ~TAO_NS_StructuredProxyPushConsumer ();  

  /// Implements TAO_NS_Object::servant method.
  virtual PortableServer::Servant servant (void);

  /// ServantBase refcount methods.
  virtual void _add_ref (ACE_ENV_SINGLE_ARG_DECL);
  virtual void _remove_ref (ACE_ENV_SINGLE_ARG_DECL);

  /// TAO_NS_Destroy_Callback methods
  virtual void release (void);

protected:
  /// = Servant methods
  virtual void connect_structured_push_supplier (
                                                 CosNotifyComm::StructuredPushSupplier_ptr push_supplier
                                                 )
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     , CosEventChannelAdmin::AlreadyConnected
                     ));

  virtual ::CosNotifyChannelAdmin::ProxyType MyType (
    
  )
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual ::CosNotifyChannelAdmin::SupplierAdmin_ptr MyAdmin (
    
  )
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual ::CosNotification::EventTypeSeq * obtain_subscription_types (
                                                                       CosNotifyChannelAdmin::ObtainInfoMode mode
                                                                       )
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual void validate_event_qos (
                                   const CosNotification::QoSProperties & required_qos,
                                   CosNotification::NamedPropertyRangeSeq_out available_qos
                                   )
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     , CosNotification::UnsupportedQoS
                     ));

  virtual void push_structured_event (
                                      const CosNotification::StructuredEvent & notification
                                      ACE_ENV_ARG_DECL
                                      )
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     , CosEventComm::Disconnected
                     ));

  virtual void disconnect_structured_push_consumer (
    
  )
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual ::CosNotification::QoSProperties * get_qos (
    
  )
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual void set_qos (
                        const CosNotification::QoSProperties & qos
                        )
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     , CosNotification::UnsupportedQoS
                     ));

  virtual void validate_qos (
                             const CosNotification::QoSProperties & required_qos,
                             CosNotification::NamedPropertyRangeSeq_out available_qos
                             )
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     , CosNotification::UnsupportedQoS
                     ));

  virtual CosNotifyFilter::FilterID add_filter (
                                                CosNotifyFilter::Filter_ptr new_filter
                                                )
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual void remove_filter (
                              CosNotifyFilter::FilterID filter
                              )
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     , CosNotifyFilter::FilterNotFound
                     ));

  virtual ::CosNotifyFilter::Filter_ptr get_filter (
                                                    CosNotifyFilter::FilterID filter
                                                    )
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     , CosNotifyFilter::FilterNotFound
                     ));

  virtual ::CosNotifyFilter::FilterIDSeq * get_all_filters (
    
  )
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual void remove_all_filters (
    
  )
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual void offer_change (
                             const CosNotification::EventTypeSeq & added,
                             const CosNotification::EventTypeSeq & removed
                             )
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     , CosNotifyComm::InvalidEventType
                     ));
};

#if defined (__ACE_INLINE__)
#include "StructuredProxyPushConsumer.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_STRUCTUREDPROXYPUSHCONSUMER_H */
