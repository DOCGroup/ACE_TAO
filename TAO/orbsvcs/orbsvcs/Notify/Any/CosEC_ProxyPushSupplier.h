/* -*- C++ -*- */
/**
 *  @file CosEC_ProxyPushSupplier.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_COSEC_PROXYPUSHSUPPLIER_H
#define TAO_NS_COSEC_PROXYPUSHSUPPLIER_H
#include "ace/pre.h"

#include "../notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosEventChannelAdminS.h"
#include "../ProxySupplier_T.h"

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

#if defined ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION_EXPORT
template class TAO_Notify_Export
TAO_NS_ProxySupplier_T<POA_CosEventChannelAdmin::ProxyPushSupplier>;
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION_EXPORT */

/**
 * @class TAO_NS_CosEC_ProxyPushSupplier
 *
 * @brief
 *
 */
class TAO_Notify_Export TAO_NS_CosEC_ProxyPushSupplier : public virtual TAO_NS_ProxySupplier_T <POA_CosEventChannelAdmin::ProxyPushSupplier>
{
  friend class TAO_NS_Builder;
public:
  /// Constuctor
  TAO_NS_CosEC_ProxyPushSupplier (void);

  /// Destructor
  ~TAO_NS_CosEC_ProxyPushSupplier ();

  /// Destroy this object.
  virtual void destroy (ACE_ENV_SINGLE_ARG_DECL);

  /// Release
  virtual void release (void);

  // = Interface methods
  virtual void connect_push_consumer (
    CosEventComm::PushConsumer_ptr push_consumer
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosEventChannelAdmin::AlreadyConnected,
    CosEventChannelAdmin::TypeError
  ));

  virtual void disconnect_push_supplier (
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
#include "CosEC_ProxyPushSupplier.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_COSEC_PROXYPUSHSUPPLIER_H */
