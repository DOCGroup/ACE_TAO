/* -*- C++ -*- */
/**
 *  @file ProxyPushConsumer.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_PROXYPUSHCONSUMER_H
#define TAO_NS_PROXYPUSHCONSUMER_H
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
 * @class TAO_NS_ProxyPushConsumer
 *
 * @brief
 *
 */
class TAO_Notify_Export TAO_NS_ProxyPushConsumer : public virtual TAO_NS_ProxyConsumer_T <POA_CosNotifyChannelAdmin::ProxyPushConsumer>
{
 friend class TAO_NS_Builder;
public:
  /// Constuctor
  TAO_NS_ProxyPushConsumer (void);

  /// Destructor
  ~TAO_NS_ProxyPushConsumer ();

  /// Release
  virtual void release (void);

  /// Destroy this object.
  virtual void destroy (ACE_ENV_SINGLE_ARG_DECL);

protected:
  ///= CosNotifyChannelAdmin::ProxyPushConsumer methods

  virtual CosNotifyChannelAdmin::ProxyType MyType (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual void push (const CORBA::Any & data ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosEventComm::Disconnected
                   ));

  virtual void connect_any_push_supplier (CosEventComm::PushSupplier_ptr push_supplier ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException,
                     CosEventChannelAdmin::AlreadyConnected
                     ));

  virtual void disconnect_push_consumer (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

private:
  // Overloaded TAO_NS_ProxyConsumer::push to get around Borland compiler warnings.
  virtual void push (TAO_NS_Event_var &event);
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#if defined (__ACE_INLINE__)
#include "ProxyPushConsumer.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_PROXYPUSHCONSUMER_H */
