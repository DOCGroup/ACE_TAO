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

#ifndef TAO_Notify_SEQUENCEPROXYPUSHCONSUMER_H
#define TAO_Notify_SEQUENCEPROXYPUSHCONSUMER_H

#include /**/ "ace/pre.h"

#include "../notify_serv_export.h"

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
 * @class TAO_Notify_SequenceProxyPushConsumer
 *
 * @brief
 *
 */
class TAO_Notify_Serv_Export TAO_Notify_SequenceProxyPushConsumer
  : public virtual TAO_Notify_ProxyConsumer_T <POA_CosNotifyChannelAdmin::SequenceProxyPushConsumer>
{
  typedef TAO_Notify_ProxyConsumer_T <POA_CosNotifyChannelAdmin::SequenceProxyPushConsumer> SuperClass;
  friend class TAO_Notify_Builder;
public:
  /// Constuctor
  TAO_Notify_SequenceProxyPushConsumer (void);

  /// Destructor
  ~TAO_Notify_SequenceProxyPushConsumer ();

  /// TAO_Notify_Destroy_Callback methods
  virtual void release (void);

  virtual const char * get_proxy_type_name (void) const;

  virtual void load_attrs (const TAO_Notify::NVPList& attrs);

  // override and hide derived init methods and
  // explicitly direct the init call down the correct
  // leg of the d**n diamond.
  virtual void init (TAO_Notify_SupplierAdmin *supplier_admin);

protected:
  ///= Data Members
  TAO_Notify_Property_Time pacing_interval_;

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

#include /**/ "ace/post.h"

#endif /* TAO_Notify_SEQUENCEPROXYPUSHCONSUMER_H */
