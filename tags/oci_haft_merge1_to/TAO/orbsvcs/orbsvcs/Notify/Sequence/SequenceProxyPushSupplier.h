/* -*- C++ -*- */
/**
 *  @file SequenceProxyPushSupplier.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_Notify_SEQUENCEPROXYPUSHSUPPLIER_H
#define TAO_Notify_SEQUENCEPROXYPUSHSUPPLIER_H
#include "ace/pre.h"

#include "../notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosNotifyChannelAdminS.h"
#include "../ProxySupplier_T.h"

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

#if defined ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION_EXPORT
template class TAO_Notify_Export
TAO_Notify_ProxySupplier_T<POA_CosNotifyChannelAdmin::SequenceProxyPushSupplier>;
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION_EXPORT */

/**
 * @class TAO_Notify_SequenceProxyPushSupplier
 *
 * @brief Implements the CosNotifyChannelAdmin::SequenceProxyPushSupplier methods.
 *
 *
 */
class TAO_Notify_Export TAO_Notify_SequenceProxyPushSupplier : public virtual TAO_Notify_ProxySupplier_T <POA_CosNotifyChannelAdmin::SequenceProxyPushSupplier>
{
  friend class TAO_Notify_Builder;

public:
  /// Constuctor
  TAO_Notify_SequenceProxyPushSupplier (void);

  /// Destructor
  ~TAO_Notify_SequenceProxyPushSupplier ();

  /// Destroy this object.
  virtual void destroy (ACE_ENV_SINGLE_ARG_DECL);

  /// TAO_Notify_Destroy_Callback methods
  virtual void release (void);

  /// = Servant methods
  virtual CosNotifyChannelAdmin::ProxyType MyType (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual void connect_sequence_push_consumer (
    CosNotifyComm::SequencePushConsumer_ptr push_consumer
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosEventChannelAdmin::AlreadyConnected,
    CosEventChannelAdmin::TypeError
  ));

  virtual void disconnect_sequence_push_supplier (
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
#include "SequenceProxyPushSupplier.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_Notify_SEQUENCEPROXYPUSHSUPPLIER_H */
