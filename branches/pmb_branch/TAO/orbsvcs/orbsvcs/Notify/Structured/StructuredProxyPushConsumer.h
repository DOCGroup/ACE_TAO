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
#include "../ProxyConsumer_T.h"
#include "../Destroy_Callback.h"

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

/**
 * @class TAO_NS_StructuredProxyPushConsumer
 *
 * @brief CosNotifyChannelAdmin::StructuredProxyPushConsumer implementation.
 *
 */
class TAO_Notify_Export TAO_NS_StructuredProxyPushConsumer : public virtual TAO_NS_ProxyConsumer_T <POA_CosNotifyChannelAdmin::StructuredProxyPushConsumer>, public TAO_NS_Destroy_Callback
{
  friend class TAO_NS_Builder;

public:
  /// Constuctor
  TAO_NS_StructuredProxyPushConsumer (void);

  /// Destructor
  ~TAO_NS_StructuredProxyPushConsumer ();

  /// TAO_NS_Destroy_Callback methods
  virtual void release (void);

  /// Destroy this object.
  virtual void destroy (ACE_ENV_SINGLE_ARG_DECL);

protected:

  // = interface methods
  virtual CosNotifyChannelAdmin::ProxyType MyType (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual void connect_structured_push_supplier (
    CosNotifyComm::StructuredPushSupplier_ptr push_supplier
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosEventChannelAdmin::AlreadyConnected
  ));

virtual void push_structured_event (
    const CosNotification::StructuredEvent & notification
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosEventComm::Disconnected
  ));

  virtual void disconnect_structured_push_consumer (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#if defined (__ACE_INLINE__)
#include "StructuredProxyPushConsumer.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_STRUCTUREDPROXYPUSHCONSUMER_H */
