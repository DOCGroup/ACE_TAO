/* -*- C++ -*- */
/**
 *  @file SequenceProxyPushConsumer.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_SEQUENCEPROXYPUSHCONSUMER_H
#define TAO_NS_SEQUENCEPROXYPUSHCONSUMER_H
#include "ace/pre.h"

#include "../notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosNotifyChannelAdminS.h"
#include "../ProxyConsumer_T.h"

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

/**
 * @class TAO_NS_SequenceProxyPushConsumer
 *
 * @brief
 *
 */
class TAO_Notify_Export TAO_NS_SequenceProxyPushConsumer : public virtual TAO_NS_ProxyConsumer_T <POA_CosNotifyChannelAdmin::SequenceProxyPushConsumer>
{
  friend class TAO_NS_Builder;
public:
  /// Constuctor
  TAO_NS_SequenceProxyPushConsumer (void);

  /// Destructor
  ~TAO_NS_SequenceProxyPushConsumer ();

  /// TAO_NS_Destroy_Callback methods
  virtual void release (void);

  /// Destroy this object.
  virtual void destroy (ACE_ENV_SINGLE_ARG_DECL);

protected:
  ///= Data Members
  TAO_NS_Property_Time pacing_interval_;

  ///= Protected Methods

  //= interface methods
  virtual CosNotifyChannelAdmin::ProxyType MyType (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual void connect_sequence_push_supplier (CosNotifyComm::SequencePushSupplier_ptr push_supplier
                                               ACE_ENV_ARG_DECL
                                               )
    ACE_THROW_SPEC ((
                     CORBA::SystemException,
                     CosEventChannelAdmin::AlreadyConnected
                     ));

  virtual void push_structured_events (const CosNotification::EventBatch & notifications
                                      ACE_ENV_ARG_DECL
                                      )
    ACE_THROW_SPEC ((
                     CORBA::SystemException,
                     CosEventComm::Disconnected
                     ));

  virtual void disconnect_sequence_push_consumer (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#if defined (__ACE_INLINE__)
#include "SequenceProxyPushConsumer.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_SEQUENCEPROXYPUSHCONSUMER_H */
